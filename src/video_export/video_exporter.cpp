#include "video_exporter.h"
#include "video_renderer.h"
#include <note_naga_engine/note_naga_engine.h>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <fstream>
#include <QProcess>
#include <QFileInfo>

// Konstruktor a doExport() zůstávají beze změny...
VideoExporter::VideoExporter(NoteNagaMidiSeq *sequence, QString outputPath,
                             QSize resolution, int fps, NoteNagaEngine *m_engine, double secondsVisible, QObject *parent)
    : QObject(parent), m_sequence(sequence), m_outputPath(outputPath), m_resolution(resolution), m_fps(fps), m_engine(m_engine), m_secondsVisible(secondsVisible) {}

void VideoExporter::doExport()
{
    QString tempAudioPath = m_outputPath + ".tmp.wav";
    QString tempVideoPath = m_outputPath + ".tmp.mp4";

    emit progressUpdated(0, tr("Phase 1/3: Rendering audio..."));
    if (!exportAudio(tempAudioPath))
    {
        emit error(tr("Failed to render audio."));
        return;
    }
    emit progressUpdated(0, tr("Phase 2/3: Rendering video..."));
    if (!exportVideo(tempVideoPath))
    {
        emit error(tr("Failed to render video."));
        QFile::remove(tempAudioPath);
        return;
    }
    emit progressUpdated(0, tr("Phase 3/3: Muxing files..."));
    if (!combineAudioVideo(tempVideoPath, tempAudioPath, m_outputPath))
    {
        emit error(tr("Failed to combine video and audio. Is FFmpeg installed and in the system PATH?"));
    }
    QFile::remove(tempAudioPath);
    QFile::remove(tempVideoPath);
    emit finished();
}


// --- ZDE JE OPRAVENÁ METODA EXPORTAUDIO ---
bool VideoExporter::exportAudio(const QString &outputPath)
{
    const int sampleRate = 44100;
    const int numChannels = 2;
    const double totalDuration = nn_ticks_to_seconds(m_engine->getProject()->getActiveSequence()->getMaxTick(), m_engine->getProject()->getPPQ(), m_engine->getProject()->getTempo()) + 1.0;
    const int totalSamples = static_cast<int>(totalDuration * sampleRate);
    const int bufferSizeSamples = 512; 

    std::vector<float> audioBuffer(totalSamples * numChannels);
    std::vector<float> chunkBuffer(bufferSizeSamples * numChannels);

    NoteNagaProject *project = m_engine->getProject();
    NoteNagaMidiSeq *activeSequence = project->getActiveSequence();
    NoteNagaMixer *mixer = m_engine->getMixer();
    NoteNagaDSPEngine *dspEngine = m_engine->getDSPEngine();
    
    // Získáme všechny syntezátory z enginu
    auto synthesizers = m_engine->getSynthesizers();

    // Reset stavu před začátkem
    mixer->stopAllNotes(); 
    project->setCurrentTick(0);
    int last_tick = 0;
    int samplesRendered = 0;

    while (samplesRendered < totalSamples)
    {
        int samplesToRender = std::min(bufferSizeSamples, totalSamples - samplesRendered);
        double currentTime = (double)(samplesRendered + samplesToRender) / sampleRate;
        int end_tick = nn_seconds_to_ticks(currentTime, project->getPPQ(), project->getTempo());

        for (auto *track : activeSequence->getTracks())
        {
            if (track->isMuted() || (activeSequence->getSoloTrack() && activeSequence->getSoloTrack() != track)) continue;

            for (const auto &note : track->getNotes())
            {
                if (!note.start.has_value() || !note.length.has_value()) continue;
                if (last_tick < note.start.value() && note.start.value() <= end_tick)
                {
                    mixer->playNote(note);
                }
                int note_end = note.start.value() + note.length.value();
                if (last_tick < note_end && note_end <= end_tick)
                {
                    mixer->stopNote(note);
                }
            }
        }
        
        // ======================== KLÍČOVÁ OPRAVA ZDE ========================
        // 1. Zprávy z not se přesunou z bufferu mixéru do fronty mixéru.
        mixer->flushNotes();
        
        // 2. Ručně zpracujeme frontu mixéru. Tím se zprávy rozešlou do front syntezátorů.
        mixer->processQueue();

        // 3. Ručně zpracujeme fronty VŠECH syntezátorů. Tím si aktualizují svůj vnitřní stav (které noty hrají).
        for (auto* synth : synthesizers) {
            synth->processQueue();
        }
        // ====================================================================

        // Až TEĎ, když jsou všechny komponenty ve správném stavu, renderujeme audio.
        dspEngine->render(chunkBuffer.data(), samplesToRender);

        memcpy(audioBuffer.data() + (samplesRendered * numChannels), chunkBuffer.data(), samplesToRender * numChannels * sizeof(float));

        samplesRendered += samplesToRender;
        last_tick = end_tick;

        emit progressUpdated((int)((double)samplesRendered / totalSamples * 100), tr("Rendering audio..."));
    }

    std::ofstream file(outputPath.toStdString(), std::ios::binary);
    if (!file.is_open()) return false;

    writeWavHeader(file, sampleRate, totalSamples);

    std::vector<int16_t> intBuffer(audioBuffer.size());
    for (size_t i = 0; i < audioBuffer.size(); ++i)
    {
        intBuffer[i] = static_cast<int16_t>(std::clamp(audioBuffer[i], -1.0f, 1.0f) * 32767.0f);
    }
    file.write(reinterpret_cast<const char *>(intBuffer.data()), intBuffer.size() * sizeof(int16_t));

    return true;
}

// Ostatní metody (exportVideo, combineAudioVideo, writeWavHeader) zůstávají beze změny...
bool VideoExporter::exportVideo(const QString &outputPath)
{
    VideoRenderer renderer(m_engine->getProject()->getActiveSequence());
    renderer.setSecondsVisible(m_secondsVisible);
    cv::VideoWriter videoWriter(outputPath.toStdString(), cv::VideoWriter::fourcc('m', 'p', '4', 'v'), m_fps, cv::Size(m_resolution.width(), m_resolution.height()));

    if (!videoWriter.isOpened())
        return false;

    double totalDuration = nn_ticks_to_seconds(m_engine->getProject()->getActiveSequence()->getMaxTick(), m_engine->getProject()->getPPQ(), m_engine->getProject()->getTempo()) + 1.0;
    int totalFrames = static_cast<int>(totalDuration * m_fps);

    for (int i = 0; i < totalFrames; ++i)
    {
        double currentTime = static_cast<double>(i) / m_fps;
        QImage frame = renderer.renderFrame(currentTime, m_resolution);
        cv::Mat cvFrame(frame.height(), frame.width(), CV_8UC4, (void *)frame.constBits(), frame.bytesPerLine());
        cv::Mat cvFrameBGR;
        cv::cvtColor(cvFrame, cvFrameBGR, cv::COLOR_RGBA2BGR);
        videoWriter.write(cvFrameBGR);
        emit progressUpdated((int)((double)i / totalFrames * 100), tr("Rendering video..."));
    }
    videoWriter.release();
    return true;
}

bool VideoExporter::combineAudioVideo(const QString &videoPath, const QString &audioPath, const QString &finalPath)
{
    QProcess ffmpeg;
    QStringList arguments;
    arguments << "-y" << "-i" << videoPath << "-i" << audioPath << "-c:v" << "copy" << "-c:a" << "aac" << "-b:a" << "192k" << "-shortest" << finalPath;

    ffmpeg.start("ffmpeg", arguments);
    if (!ffmpeg.waitForStarted())
    {
        return false;
    }
    ffmpeg.waitForFinished(-1); // Čekej neomezeně
    return ffmpeg.exitCode() == 0;
}

void VideoExporter::writeWavHeader(std::ofstream &file, int sampleRate, int numSamples)
{
    int numChannels = 2;
    int bitsPerSample = 16;
    int byteRate = sampleRate * numChannels * bitsPerSample / 8;
    int blockAlign = numChannels * bitsPerSample / 8;
    int subchunk2Size = numSamples * numChannels * bitsPerSample / 8;
    int chunkSize = 36 + subchunk2Size;

    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char *>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    int subchunk1Size = 16;
    file.write(reinterpret_cast<const char *>(&subchunk1Size), 4);
    short audioFormat = 1; // PCM
    file.write(reinterpret_cast<const char *>(&audioFormat), 2);
    file.write(reinterpret_cast<const char *>(&numChannels), 2);
    file.write(reinterpret_cast<const char *>(&sampleRate), 4);
    file.write(reinterpret_cast<const char *>(&byteRate), 4);
    file.write(reinterpret_cast<const char *>(&blockAlign), 2);
    file.write(reinterpret_cast<const char *>(&bitsPerSample), 2);
    file.write("data", 4);
    file.write(reinterpret_cast<const char *>(&subchunk2Size), 4);
}
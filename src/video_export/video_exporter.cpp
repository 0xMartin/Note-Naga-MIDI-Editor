#include "video_exporter.h"

#include "video_renderer.h"
#include <note_naga_engine/note_naga_engine.h>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <fstream>
#include <QProcess>
#include <QFileInfo>
#include <QtConcurrent>
#include <QFuture>
#include <numeric>

/**************************************************************************************/
// Util class to manage manual mode for engine components (beze změny)
/**************************************************************************************/
class ManualModeGuard {
public:
    ManualModeGuard(NoteNagaEngine* engine) : m_engine(engine) {
        if (m_engine) {
            m_engine->getMixer()->enterManualMode();
            for (auto* synth : m_engine->getSynthesizers()) {
                synth->enterManualMode();
            }
            m_engine->getAudioWorker()->mute();
        }
    }
    ~ManualModeGuard() {
        if (m_engine) {
            m_engine->getMixer()->exitManualMode();
            for (auto* synth : m_engine->getSynthesizers()) {
                synth->exitManualMode();
            }
            m_engine->getAudioWorker()->unmute();
        }
    }
private:
    NoteNagaEngine* m_engine;
};

/**************************************************************************************/
// VideoExporter implementation
/**************************************************************************************/

VideoExporter::VideoExporter(NoteNagaMidiSeq *sequence, QString outputPath,
                             QSize resolution, int fps, NoteNagaEngine *engine, double secondsVisible, QObject *parent)
    : QObject(parent), m_sequence(sequence), m_outputPath(outputPath), m_resolution(resolution), m_fps(fps), m_engine(engine), m_secondsVisible(secondsVisible)
{
    // Propojíme signál 'finished' z OBOU sledovačů na JEDEN slot 'onTaskFinished'
    connect(&m_audioWatcher, &QFutureWatcher<bool>::finished, this, &VideoExporter::onTaskFinished);
    connect(&m_videoWatcher, &QFutureWatcher<bool>::finished, this, &VideoExporter::onTaskFinished);
}

VideoExporter::~VideoExporter()
{
    cleanup();
}

void VideoExporter::doExport()
{
    m_tempAudioPath = m_outputPath + ".tmp.wav";
    m_tempVideoPath = m_outputPath + ".tmp.mp4";

    emit progressUpdated(0, "Fáze 1/2: Renderování audia a videa...");

    // Vynulujeme počítadlo
    m_finishedTaskCount = 0;

    // Spustíme úlohy pomocí lambda funkcí - je to robustnější pro kompilátor
    QFuture<bool> audioFuture = QtConcurrent::run([this]() {
        return this->exportAudio(m_tempAudioPath);
    });

    QFuture<bool> videoFuture = QtConcurrent::run([this]() {
        return this->exportVideo(m_tempVideoPath);
    });

    // Nastavíme sledovačům, které úlohy mají sledovat
    m_audioWatcher.setFuture(audioFuture);
    m_videoWatcher.setFuture(videoFuture);
}

void VideoExporter::onTaskFinished()
{
    // Atomicky zvýšíme počítadlo a získáme novou hodnotu
    if (m_finishedTaskCount.fetchAndAddOrdered(1) + 1 != 2) {
        // Pokud je toto první dokončená úloha, jen skončíme.
        // Slot se zavolá znovu pro druhou úlohu.
        return;
    }

    // Pokud se dostaneme sem, obě úlohy jsou dokončené.
    // Získáme jejich výsledky.
    bool audioSuccess = m_audioWatcher.future().result();
    bool videoSuccess = m_videoWatcher.future().result();

    if (!audioSuccess) {
        emit error("Nepodařilo se vyrenderovat zvuk.");
        cleanup();
        emit finished();
        return;
    }

    if (!videoSuccess) {
        emit error("Nepodařilo se vyrenderovat video.");
        cleanup();
        emit finished();
        return;
    }

    // Fáze 2: Spojení audia a videa
    emit progressUpdated(100, "Fáze 2/2: Spojování souborů (muxing)...");
    if (!combineAudioVideo(m_tempVideoPath, m_tempAudioPath, m_outputPath))
    {
        emit error("Nepodařilo se spojit video a zvuk. Je FFmpeg nainstalován a v systémové cestě (PATH)?");
    }

    cleanup();
    emit finished();
}

void VideoExporter::cleanup()
{
    if (!m_tempAudioPath.isEmpty()) QFile::remove(m_tempAudioPath);
    if (!m_tempVideoPath.isEmpty()) QFile::remove(m_tempVideoPath);
}

//
// Zbytek souboru .cpp (metody exportAudio, exportVideo, atd.)
// zůstává NAPROSTO STEJNÝ. Zkopírujte je z vaší poslední verze.
// Pro jistotu je zde uvádím znovu.
//

bool VideoExporter::exportAudio(const QString &outputPath)
{
    ManualModeGuard manualMode(this->m_engine);

    const int sampleRate = 44100;
    const int numChannels = 2;
    const double totalDuration = nn_ticks_to_seconds(m_engine->getProject()->getActiveSequence()->getMaxTick(), m_engine->getProject()->getPPQ(), m_engine->getProject()->getTempo()) + 2.0;
    const int totalSamples = static_cast<int>(totalDuration * sampleRate);

    std::vector<float> audioBuffer(totalSamples * numChannels, 0.0f);

    NoteNagaProject *project = m_engine->getProject();
    NoteNagaMidiSeq *activeSequence = project->getActiveSequence();
    NoteNagaMixer *mixer = m_engine->getMixer();
    NoteNagaDSPEngine *dspEngine = m_engine->getDSPEngine();
    auto synthesizers = m_engine->getSynthesizers();

    struct MidiEvent {
        int tick;
        NN_Note_t note;
        bool isNoteOn;
    };

    std::vector<MidiEvent> allEvents;
    for (auto *track : activeSequence->getTracks()) {
        if (track->isMuted() || (activeSequence->getSoloTrack() && activeSequence->getSoloTrack() != track)) continue;
        for (const auto &note : track->getNotes()) {
            if (note.start.has_value() && note.length.has_value()) {
                allEvents.push_back({note.start.value(), note, true});
                allEvents.push_back({note.start.value() + note.length.value(), note, false});
            }
        }
    }

    std::sort(allEvents.begin(), allEvents.end(), [](const MidiEvent &a, const MidiEvent &b) {
        return a.tick < b.tick;
    });

    mixer->stopAllNotes();
    int last_tick = 0;
    int totalSamplesRendered = 0;

    for (const auto& event : allEvents) {
        int ticksToProcess = event.tick - last_tick;
        if (ticksToProcess > 0) {
            double durationToRender = nn_ticks_to_seconds(ticksToProcess, project->getPPQ(), project->getTempo());
            int samplesToRender = static_cast<int>(durationToRender * sampleRate);

            if (totalSamplesRendered + samplesToRender > totalSamples) {
                samplesToRender = totalSamples - totalSamplesRendered;
            }

            if (samplesToRender > 0) {
                dspEngine->render(audioBuffer.data() + totalSamplesRendered * numChannels, samplesToRender, false);
                totalSamplesRendered += samplesToRender;
            }
        }

        if (event.isNoteOn) {
            mixer->playNote(event.note);
        } else {
            mixer->stopNote(event.note);
        }

        mixer->flushNotes();
        mixer->processQueue();
        for (auto* synth : synthesizers) {
            synth->processQueue();
        }

        last_tick = event.tick;
        emit progressUpdated((int)((double)totalSamplesRendered / totalSamples * 50), tr("Renderování zvuku..."));
    }

    int remainingSamples = totalSamples - totalSamplesRendered;
    if (remainingSamples > 0) {
        dspEngine->render(audioBuffer.data() + totalSamplesRendered * numChannels, remainingSamples, false);
    }

    std::ofstream file(outputPath.toStdString(), std::ios::binary);
    if (!file.is_open()) return false;

    writeWavHeader(file, sampleRate, totalSamples);

    std::vector<int16_t> intBuffer(audioBuffer.size());
    for (size_t i = 0; i < audioBuffer.size(); ++i) {
        intBuffer[i] = static_cast<int16_t>(std::clamp(audioBuffer[i], -1.0f, 1.0f) * 32767.0f);
    }
    file.write(reinterpret_cast<const char *>(intBuffer.data()), intBuffer.size() * sizeof(int16_t));

    return true;
}

bool VideoExporter::exportVideo(const QString &outputPath)
{
    cv::VideoWriter videoWriter(outputPath.toStdString(), cv::VideoWriter::fourcc('m', 'p', '4', 'v'), m_fps, cv::Size(m_resolution.width(), m_resolution.height()));
    if (!videoWriter.isOpened())
        return false;

    double totalDuration = nn_ticks_to_seconds(m_engine->getProject()->getActiveSequence()->getMaxTick(), m_engine->getProject()->getPPQ(), m_engine->getProject()->getTempo()) + 1.0;
    const int totalFrames = static_cast<int>(totalDuration * m_fps);

    QVector<int> frameNumbers(totalFrames);
    std::iota(frameNumbers.begin(), frameNumbers.end(), 0);

    QAtomicInt framesDone(0);

    std::function<cv::Mat(int)> renderLambda =
        [this, totalFrames, &framesDone](int i) -> cv::Mat {

        VideoRenderer renderer(m_engine->getProject()->getActiveSequence());
        renderer.setSecondsVisible(m_secondsVisible);

        double currentTime = static_cast<double>(i) / m_fps;
        QImage frame = renderer.renderFrame(currentTime, m_resolution);

        cv::Mat cvFrame(frame.height(), frame.width(), CV_8UC4, (void*)frame.constBits(), frame.bytesPerLine());
        cv::Mat cvFrameBGR;
        cv::cvtColor(cvFrame, cvFrameBGR, cv::COLOR_RGBA2BGR);

        int doneCount = framesDone.fetchAndAddOrdered(1) + 1;
        emit progressUpdated(50 + (int)((double)doneCount / totalFrames * 50), "Renderování videa...");

        return cvFrameBGR;
    };

    QFuture<cv::Mat> renderFuture = QtConcurrent::mapped(frameNumbers.begin(), frameNumbers.end(), renderLambda);
    renderFuture.waitForFinished();

    QList<cv::Mat> renderedFrames = renderFuture.results();
    for(const auto& frame : renderedFrames) {
        if (!frame.empty()) {
             videoWriter.write(frame);
        }
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
    ffmpeg.waitForFinished(-1);
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
    short audioFormat = 1;
    file.write(reinterpret_cast<const char *>(&audioFormat), 2);
    file.write(reinterpret_cast<const char *>(&numChannels), 2);
    file.write(reinterpret_cast<const char *>(&sampleRate), 4);
    file.write(reinterpret_cast<const char *>(&byteRate), 4);
    file.write(reinterpret_cast<const char *>(&blockAlign), 2);
    file.write(reinterpret_cast<const char *>(&bitsPerSample), 2);
    file.write("data", 4);
    file.write(reinterpret_cast<const char *>(&subchunk2Size), 4);
}
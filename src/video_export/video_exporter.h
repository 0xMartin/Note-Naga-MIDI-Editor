#pragma once

#include <QObject>
#include <QSize>
#include <QString>
#include <note_naga_engine/core/types.h>
#include <note_naga_engine/note_naga_engine.h>

class VideoExporter : public QObject
{
    Q_OBJECT

public:
    explicit VideoExporter(NoteNagaMidiSeq *sequence, QString outputPath,
                           QSize resolution, int fps, NoteNagaEngine *m_engine,
                           double secondsVisible, 
                           QObject *parent = nullptr);

public slots:
    void doExport();

signals:
    void progressUpdated(int percentage, const QString &status);
    void finished();
    void error(const QString &errorMessage);

private:
    NoteNagaEngine *m_engine;
    NoteNagaMidiSeq *m_sequence;
    QString m_outputPath;
    QSize m_resolution;
    int m_fps;
    double m_secondsVisible;

    bool exportAudio(const QString &outputPath);
    bool exportVideo(const QString &outputPath);
    bool combineAudioVideo(const QString &videoPath, const QString &audioPath, const QString &finalPath);

    void writeWavHeader(std::ofstream &file, int sampleRate, int numSamples);
};
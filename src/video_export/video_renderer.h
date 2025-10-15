#pragma once

#include <QImage>
#include <QColor>
#include <QPixmap>
#include <map>
#include <vector>
#include <note_naga_engine/core/types.h>

class VideoRenderer {
public:
    VideoRenderer(NoteNagaMidiSeq* sequence);
    QImage renderFrame(double currentTime, const QSize& size);
    
    // Metoda pro nastavení vertikálního měřítka z UI
    void setSecondsVisible(double seconds) { m_secondsVisible = seconds; }

private:
    // Struktura pro uchování informací o notě pro renderování
    struct NoteInfo {
        int note_val;
        double start_time;
        double end_time;
        QColor color;
    };
    // Struktura pro uchování informací o klávese
    struct KeyInfo {
        QRectF rect;
        bool is_white;
    };
    // Struktura pro částice
    struct Particle {
        QPointF pos;
        QPointF vel;
        qreal lifetime;
        qreal initial_lifetime;
        QColor color;
    };

    void prepareNoteData();
    void prepareKeyboardLayout(const QSize& size);
    // Metody pro efekty
    void updateAndDrawParticles(double deltaTime, QPainter& painter);
    void spawnParticles(const NoteInfo& note);

    NoteNagaMidiSeq* m_sequence;
    std::vector<NoteInfo> m_notes;
    std::map<int, KeyInfo> m_keyboardLayout;
    QSize m_lastLayoutSize;
    
    // Proměnné pro efekty a nastavení
    double m_secondsVisible = 5.0;
    QPixmap m_particlePixmap;
    std::vector<Particle> m_particles;
    std::map<int, bool> m_activeNotesState; // Pro detekci stisknutí nových not
    double m_lastFrameTime = -1.0;
};
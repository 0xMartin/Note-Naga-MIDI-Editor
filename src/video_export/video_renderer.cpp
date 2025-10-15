#include "video_renderer.h"
#include <QPainter>
#include <cmath> // Pro M_PI a std::abs

// Konstanty pro rozsah klaviatury
const int FIRST_MIDI_NOTE = 21; // A0
const int LAST_MIDI_NOTE = 108; // C8

VideoRenderer::VideoRenderer(NoteNagaMidiSeq* sequence) 
    : m_sequence(sequence), m_lastLayoutSize(0, 0) 
{
    prepareNoteData();
    // Původní načítání pixmapy už není potřeba, ale necháme ho pro případné budoucí použití
    m_particlePixmap.load(":/images/sparkle.png");
}

void VideoRenderer::prepareNoteData() {
    for (const auto& track : m_sequence->getTracks()) {
        QColor trackColor = track->getColor().toQColor();
        for (const auto& note : track->getNotes()) {
            if (note.start.has_value() && note.length.has_value()) {
                m_notes.push_back({
                    note.note,
                    nn_ticks_to_seconds(note.start.value(), m_sequence->getPPQ(), m_sequence->getTempo()),
                    nn_ticks_to_seconds(note.start.value() + note.length.value(), m_sequence->getPPQ(), m_sequence->getTempo()),
                    trackColor
                });
            }
        }
    }
}

void VideoRenderer::prepareKeyboardLayout(const QSize& size) {
    if (size == m_lastLayoutSize) return;

    m_keyboardLayout.clear();
    const float keyboardHeight = size.height() * 0.25f;

    std::vector<bool> is_white_key = {true, false, true, false, true, true, false, true, false, true, false, true};
    int white_key_count = 0;
    for (int i = FIRST_MIDI_NOTE; i <= LAST_MIDI_NOTE; ++i) {
        if (is_white_key[i % 12]) white_key_count++;
    }

    float white_key_width = (float)size.width() / white_key_count;
    float black_key_width = white_key_width * 0.6f;
    float black_key_height = keyboardHeight * 0.6f;

    int current_white_key = 0;
    for (int i = FIRST_MIDI_NOTE; i <= LAST_MIDI_NOTE; ++i) {
        if (is_white_key[i % 12]) {
            m_keyboardLayout[i] = {
                QRectF(current_white_key * white_key_width, size.height() - keyboardHeight, white_key_width, keyboardHeight),
                true
            };
            current_white_key++;
        }
    }
    for (int i = FIRST_MIDI_NOTE; i <= LAST_MIDI_NOTE; ++i) {
        if (!is_white_key[i % 12]) {
            float center_x = m_keyboardLayout[i - 1].rect.right();
            m_keyboardLayout[i] = {
                QRectF(center_x - black_key_width / 2, size.height() - keyboardHeight, black_key_width, black_key_height),
                false
            };
        }
    }
    m_lastLayoutSize = size;
}

void VideoRenderer::spawnParticles(const NoteInfo& note) {
    const KeyInfo& key = m_keyboardLayout.at(note.note_val);
    QPointF spawn_pos = QPointF(key.rect.center().x(), key.rect.top());

    int particle_count = 10;
    for (int i = 0; i < particle_count; ++i) {
        double angle = (rand() % 180) * M_PI / 180.0;
        double speed = 50 + (rand() % 50);
        qreal lifetime = 0.5 + (rand() % 50) / 100.0;

        m_particles.push_back({
            spawn_pos,
            QPointF(cos(angle) * speed, -sin(angle) * speed),
            lifetime,
            lifetime,
            note.color
        });
    }
}

// --- UPRAVENÁ METODA PRO KRESLENÍ ČÁSTIC ---
void VideoRenderer::updateAndDrawParticles(double deltaTime, QPainter& painter) {
    painter.setRenderHint(QPainter::Antialiasing, true); // Zapneme pro hezké kroužky
    for (auto it = m_particles.begin(); it != m_particles.end(); ) {
        it->pos += it->vel * deltaTime;
        it->vel.ry() += 200.0 * deltaTime; // Gravitace
        it->lifetime -= deltaTime;

        if (it->lifetime <= 0) {
            it = m_particles.erase(it);
        } else {
            // Výpočet průhlednosti na základě zbývajícího života
            qreal opacity = it->lifetime / it->initial_lifetime;
            
            // Nastavení barvy (z noty) a štětce
            QColor particleColor = it->color;
            particleColor.setAlphaF(opacity * 0.8); // Zmírníme maximální opacitu
            painter.setBrush(particleColor);
            painter.setPen(Qt::NoPen); // Kreslíme bez okraje

            // Výpočet velikosti - částice se zvětšují, jak mizí
            qreal scale = 1.0 - opacity; 
            qreal radius = 2.0 + scale * 8.0;

            // Kreslíme barevný kruh místo původní pixmapy
            painter.drawEllipse(it->pos, radius, radius);
            
            ++it;
        }
    }
}

QImage VideoRenderer::renderFrame(double currentTime, const QSize& size) {
    prepareKeyboardLayout(size);

    QImage frame(size, QImage::Format_ARGB32);
    frame.fill(QColor(25, 25, 35));

    QPainter painter(&frame);
    painter.setRenderHint(QPainter::Antialiasing);

    double deltaTime = (m_lastFrameTime < 0) ? 0.0 : currentTime - m_lastFrameTime;
    m_lastFrameTime = currentTime;

    const float keyboardHeight = size.height() * 0.25f;
    const float render_area_height = size.height() - keyboardHeight;
    const float pixels_per_second = render_area_height / m_secondsVisible;

    std::map<int, bool> currentActiveNotes;

    for (const auto& note : m_notes) {
        if (note.end_time < currentTime - 1.0 || note.start_time > currentTime + m_secondsVisible) continue;
        
        bool isActive = (currentTime >= note.start_time && currentTime < note.end_time);
        if (isActive) {
            currentActiveNotes[note.note_val] = true;
        }

        const KeyInfo& key = m_keyboardLayout.at(note.note_val);
        float y_start = render_area_height - (float)(note.start_time - currentTime) * pixels_per_second;
        float y_end = render_area_height - (float)(note.end_time - currentTime) * pixels_per_second;
        QRectF note_rect(key.rect.x(), y_end, key.rect.width(), y_start - y_end);
        
        QColor note_color = note.color;

        if (isActive) {
            painter.setPen(Qt::NoPen);
            for (int i = 0; i < 10; ++i) {
                QColor glow_color = note_color;
                glow_color.setAlphaF(0.05);
                painter.setBrush(glow_color);
                painter.drawRect(note_rect.adjusted(-i, -i, i, i));
            }
        }
        
        painter.setBrush(note_color);
        painter.setPen(note_color.darker(120));
        painter.drawRect(note_rect);
    }
    
    // --- UPRAVENÁ LOGIKA PRO SPOUŠTĚNÍ ČÁSTIC ---
    for(const auto& pair : currentActiveNotes) {
        // Zkontrolujeme, zda nota nebyla aktivní v předchozím snímku
        if (!m_activeNotesState[pair.first]) {
            // Toto je nová "note on" událost. Najdeme k ní data pro barvu atd.
            for(const auto& noteInfo : m_notes) {
                // Spojíme podle čísla noty
                if(noteInfo.note_val == pair.first) {
                     // Tato podmínka zajistí, že částice vytvoříme jen pro tu notu,
                     // která právě teď začala (tolerance 50ms). Zabraňuje to
                     // vytváření částic při seekování doprostřed dlouhé noty.
                     if (std::abs(noteInfo.start_time - currentTime) < 0.05) {
                        spawnParticles(noteInfo);
                        break; // Našli jsme správnou notu, přestaneme hledat
                     }
                }
            }
        }
    }
    m_activeNotesState = currentActiveNotes;

    for (const auto& pair : m_keyboardLayout) {
        if (pair.second.is_white) {
             painter.setBrush(QColor(245, 245, 245)); painter.setPen(QColor(200, 200, 200));
             painter.drawRect(pair.second.rect);
        }
    }
    for (const auto& pair : m_keyboardLayout) {
         if (!pair.second.is_white) {
             painter.setBrush(QColor(40, 40, 40)); painter.setPen(Qt::black);
             painter.drawRect(pair.second.rect);
        }
    }
     for (const auto& note : m_notes) {
         if (currentTime >= note.start_time && currentTime < note.end_time) {
             const KeyInfo& key = m_keyboardLayout.at(note.note_val);
             painter.setBrush(note.color.lighter(120));
             painter.setPen(note.color);
             painter.drawRect(key.rect);
         }
     }

    updateAndDrawParticles(deltaTime, painter);

    painter.end();
    return frame;
}
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
    // Načteme výchozí pixmapu
    m_resourceParticlePixmapCache.load(":/images/sparkle.png");
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

// Metoda přijímá 'resolutionScale' 
void VideoRenderer::spawnParticles(const NoteInfo& note, double resolutionScale) {
    // Ověření, zda je nota v layoutu (prevence pádu)
    if (m_keyboardLayout.find(note.note_val) == m_keyboardLayout.end()) return;
    const KeyInfo& key = m_keyboardLayout.at(note.note_val);
    QPointF spawn_pos = QPointF(key.rect.center().x(), key.rect.top());

    // Použijeme nastavení z m_settings 
    int particle_count = m_settings.particleCount;
    
    for (int i = 0; i < particle_count; ++i) {
        // Mírná náhodnost úhlu
        double angle_deg = (rand() % 160) + 10; // Mezi 10 a 170 stupni (nahoru)
        double angle_rad = angle_deg * M_PI / 180.0;

        // Škálování rychlosti
        double speed_base = m_settings.particleSpeed * resolutionScale;
        double speed_rand = speed_base * 0.5;
        double speed = speed_base - (speed_rand / 2) + (rand() % (int)(speed_rand + 1)); // +1 pro zamezení dělení nulou

        // Nastavení délky života
        qreal lifetime = m_settings.particleLifetime + (rand() % 50) / 100.0 - 0.25; // +- 0.25s
        if (lifetime < 0.1) lifetime = 0.1;

        m_particles.push_back({
            spawn_pos,
            QPointF(cos(angle_rad) * speed, -sin(angle_rad) * speed),
            lifetime,
            lifetime,
            note.color
        });
    }
}

// Metoda přijímá 'resolutionScale'
void VideoRenderer::updateAndDrawParticles(double deltaTime, QPainter& painter, double resolutionScale) {
    
    // Pro kruhy a pixmapy
    painter.setRenderHint(QPainter::Antialiasing, true); 
    
    for (auto it = m_particles.begin(); it != m_particles.end(); ) {
        it->pos += it->vel * deltaTime;
        
        // Škálování gravitace
        it->vel.ry() += m_settings.particleGravity * resolutionScale * deltaTime; 
        it->lifetime -= deltaTime;

        if (it->lifetime <= 0) {
            it = m_particles.erase(it);
        } else {
            qreal opacity = it->lifetime / it->initial_lifetime;
            qreal scale = 1.0 - opacity; // 0 (start života) -> 1 (konec života)

            // Výběr typu částice
            if (m_settings.particleType == RenderSettings::Circle)
            {
                QColor particleColor = it->color;
                particleColor.setAlphaF(opacity * 0.8);
                painter.setBrush(particleColor);
                painter.setPen(Qt::NoPen);

                // --- ZMĚNĚNÁ LOGIKA VELIKOSTI ---
                qreal radiusMultiplier = m_settings.particleStartSize + (m_settings.particleEndSize - m_settings.particleStartSize) * scale;
                // Původní "2.0 + scale * 8.0" nahradíme flexibilnějším násobičem
                qreal baseRadius = 10.0 * resolutionScale; 
                qreal radius = baseRadius * radiusMultiplier;
                if (radius < 1.0) radius = 1.0; // Zajistíme minimální velikost
                
                painter.drawEllipse(it->pos, radius, radius);
            }
            else // Resource nebo Custom
            {
                // --- Oprava chyby vláken ---
                QPixmap* pixmap = &m_resourceParticlePixmapCache;

                if (m_settings.particleType == RenderSettings::Custom && !m_settings.customParticleImage.isNull()) {
                    if (m_customParticlePixmapCache.isNull() || m_customParticlePixmapCache.cacheKey() != m_settings.customParticleImage.cacheKey()) {
                         m_customParticlePixmapCache = QPixmap::fromImage(m_settings.customParticleImage);
                    }
                    pixmap = &m_customParticlePixmapCache;
                }

                if (!pixmap || pixmap->isNull()) {
                    ++it;
                    continue; // Přeskočíme kreslení, pokud pixmapa není platná
                }
                
                // Nastavíme celkovou průhlednost pro fade-out
                painter.setOpacity(opacity);

                // --- ZMĚNĚNÁ LOGIKA VELIKOSTI ---
                qreal baseSize = 32.0 * resolutionScale; // Základní velikost
                // Lineární interpolace velikosti
                qreal sizeMultiplier = m_settings.particleStartSize + (m_settings.particleEndSize - m_settings.particleStartSize) * scale;
                qreal size = baseSize * sizeMultiplier;
                if (size < 1.0) size = 1.0; // Minimální velikost
                
                QRectF targetRect(it->pos.x() - size/2, it->pos.y() - size/2, size, size);
                
                // --- NOVÁ LOGIKA TINTU (OVERLAY) ---

                // 1. Vždy vykreslíme původní pixmapu (zachová alfu i barvy)
                painter.drawPixmap(targetRect, *pixmap, pixmap->rect());

                // 2. Pokud je povolen tint, překryjeme ji barvou
                if (m_settings.tintParticles) {
                    // Overlay mód smíchá barvy a zachová světlost/tmavost originálu
                    // a hlavně RESPEKTUJE ALFA KANÁL (nebude kreslit čtverec)
                    painter.setCompositionMode(QPainter::CompositionMode_Overlay);
                    
                    // Kreslíme barvu přes obrázek
                    painter.fillRect(targetRect, it->color);
                    
                    // 3. Vrátíme režim prolnutí zpět na normální
                    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                }
                
                painter.setOpacity(1.0); // Vrátíme opacitu pro další kreslení
            }
            
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

    // Vypočítáme škálovací faktor pro částice 
    // Založeno na výšce 720p jako základ
    double resolutionScale = (double)size.height() / 720.0;

    const float keyboardHeight = size.height() * 0.25f;
    const float render_area_height = size.height() - keyboardHeight;
    const float pixels_per_second = render_area_height / m_secondsVisible;

    std::map<int, bool> currentActiveNotes;

    // --- Kreslení padajících not (Bod 5) ---
    if (m_settings.renderNotes) {
        for (const auto& note : m_notes) {
            if (note.end_time < currentTime - 1.0 || note.start_time > currentTime + m_secondsVisible) continue;
            
            bool isActive = (currentTime >= note.start_time && currentTime < note.end_time);
            if (isActive) {
                currentActiveNotes[note.note_val] = true;
            }

            // Vždy získáme info o klávese, i když je mimo rozsah (pro případnou chybu)
            if (m_keyboardLayout.find(note.note_val) == m_keyboardLayout.end()) continue;
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
    } else {
        // Pokud nekreslíme noty, musíme přesto zjistit, které jsou aktivní
         for (const auto& note : m_notes) {
            if (currentTime >= note.start_time && currentTime < note.end_time) {
                 currentActiveNotes[note.note_val] = true;
            }
         }
    }
    
    // --- Spouštění částic ---
    // Zjistíme nově stisknuté noty
    for(const auto& pair : currentActiveNotes) {
        if (m_activeNotesState.find(pair.first) == m_activeNotesState.end() || !m_activeNotesState[pair.first]) 
        {
            // Toto je nová "note on" událost. Najdeme k ní data pro barvu atd.
            for(const auto& noteInfo : m_notes) {
                if(noteInfo.note_val == pair.first) {
                     // Zkontrolujeme, zda nota skutečně začíná "téměř" teď
                     if (std::abs(noteInfo.start_time - currentTime) < 0.05) { // 50ms tolerance
                        // Pouze pokud je povoleno
                        if (m_settings.renderParticles) {
                             if (m_keyboardLayout.find(noteInfo.note_val) != m_keyboardLayout.end()) {
                                spawnParticles(noteInfo, resolutionScale); // Předáme škálování
                             }
                        }
                        break; // Našli jsme správnou notu, přestaneme hledat
                     }
                }
            }
        }
    }
    m_activeNotesState = currentActiveNotes;

    // --- Kreslení klávesnice ---
    if (m_settings.renderKeyboard) {

        // 1. KROK: Kreslíme VŠECHNY bílé klávesy (aktivní i neaktivní)
        for (const auto& pair : m_keyboardLayout) {
            if (pair.second.is_white) {
                bool isActive = (currentActiveNotes.find(pair.first) != currentActiveNotes.end());
                if (isActive) {
                    // Najdeme barvu aktivní noty
                    QColor activeColor = QColor(150, 150, 255); // Výchozí modrá
                    for (const auto& note : m_notes) {
                        if (note.note_val == pair.first && currentTime >= note.start_time && currentTime < note.end_time) {
                            activeColor = note.color;
                            break;
                        }
                    }
                    painter.setBrush(activeColor.lighter(120));
                    painter.setPen(activeColor);
                } else {
                    painter.setBrush(QColor(245, 245, 245));
                    painter.setPen(QColor(200, 200, 200));
                }
                painter.drawRect(pair.second.rect);
            }
        }
        
        // 2. KROK: Kreslíme VŠECHNY černé klávesy (přes bílé)
        for (const auto& pair : m_keyboardLayout) {
            if (!pair.second.is_white) {
                 bool isActive = (currentActiveNotes.find(pair.first) != currentActiveNotes.end());
                 if (isActive) {
                    QColor activeColor = QColor(150, 150, 255); // Výchozí modrá
                    for (const auto& note : m_notes) {
                        if (note.note_val == pair.first && currentTime >= note.start_time && currentTime < note.end_time) {
                            activeColor = note.color;
                            break;
                        }
                    }
                    painter.setBrush(activeColor.lighter(120));
                    painter.setPen(activeColor);
                 } else {
                    painter.setBrush(QColor(40, 40, 40)); 
                    painter.setPen(Qt::black);
                 }
                 painter.drawRect(pair.second.rect);
            }
        }
    }

    // --- Kreslení částic (Bod 5) ---
    if (m_settings.renderParticles) {
        updateAndDrawParticles(deltaTime, painter, resolutionScale);
    }

    painter.end();
    return frame;
}
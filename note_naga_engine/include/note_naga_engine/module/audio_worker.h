#pragma once

#include <note_naga_engine/core/note_naga_component.h>

class AudioWorker : public NoteNagaEngineComponent<MixedBuffer, 32> {
public:
    AudioWorker();
protected:
    void onItem(const MixedBuffer& buffer) override;
};
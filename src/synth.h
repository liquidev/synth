#ifndef SYNTH_H
#define SYNTH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    float frequency;

    bool playing;
    bool should_stop;
    float phase;
    float phase_step;
} synth_voice_t;

typedef struct
{
    float sample_rate;
    float time_step;

    // This minimal example implements a monosynth.
    // Turn this into an array for polyphony.
    synth_voice_t mono_voice;
} synth_state_t;

/// Initializes the synth for playback at the given sample rate.
void
synth_init(synth_state_t *state, float sample_rate);

/// Starts playing a voice at the given frequency.
/// Returns the voice's index, used for stopping the voice in `synth_stop_voice`.
size_t
synth_start_voice(synth_state_t *state, float frequency);

/// Stops the voice with the given index (returned by `synth_start_voice`.)
void
synth_stop_voice(synth_state_t *state, size_t index);

/// Synthesizes sound into the provided output buffer.
void
synth_output(synth_state_t *state, float *output_buffer, size_t sample_count);

#endif

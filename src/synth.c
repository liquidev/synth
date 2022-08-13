#include "synth.h"

#include <assert.h>
#include <math.h>
#include <string.h>

void
synth_init(synth_state_t *state, float sample_rate)
{
    state->sample_rate = sample_rate;
    state->time_step = 1.0f / sample_rate;
    state->mono_voice = (synth_voice_t){
        .playing = false,
        .should_stop = false,
        .frequency = 0.0f,
        .phase = 0.0f,
    };
}

size_t
synth_start_voice(synth_state_t *state, const float frequency)
{
    state->mono_voice.frequency = frequency;
    state->mono_voice.playing = true;
    state->mono_voice.phase = 0.0f;
    state->mono_voice.phase_step = state->time_step * frequency;

    // For polyphony, instead of returning 0 allocate an index in the voice array.
    return 0;
}

void
synth_stop_voice(synth_state_t *state, const size_t index)
{
    // You know what to do.
    assert("this implementation is monophonic; voice index must be zero" && index == 0);
    state->mono_voice.should_stop = true;
}

static inline float
synthesize_voice(synth_voice_t *voice)
{
    if (voice->should_stop && voice->phase < 0.001f) {
        voice->playing = false;
        voice->should_stop = false;
    }
    if (!voice->playing) {
        return 0.0f;
    }

    const float TWO_PI = 6.283185307f;
    float sample = sinf(voice->phase * TWO_PI);
    voice->phase += voice->phase_step;
    voice->phase = fmodf(voice->phase, 1.0f);

    return sample;
}

void
synth_output(synth_state_t *state, float *output_buffer, size_t sample_count)
{
    memset(output_buffer, 0, sample_count * sizeof(float));
    for (size_t i = 0; i < sample_count; ++i) {
        output_buffer[i] = synthesize_voice(&state->mono_voice) * 0.2f;
    }
}

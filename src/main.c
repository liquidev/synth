#include <math.h>
#include <stdbool.h>

#include <SDL.h>

#include "synth.h"

void
audio_callback(void *userdata, uint8_t *byte_stream, int byte_len);

float
get_frequency_for_key(SDL_Scancode scancode);

int
main(void)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_AudioSpec desired_audio_spec = {
        .freq = 48000,
        .format = AUDIO_F32SYS,
        .channels = 1,
        // Choose a fairly small buffer size for lower latency.
        .samples = 256,
        .callback = audio_callback,
    };

    synth_state_t state;
    desired_audio_spec.userdata = &state;

    synth_init(&state, (float)desired_audio_spec.freq);

    SDL_AudioSpec obtained_audio_spec = { 0 };
    SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(
       NULL, // device
       0,    // iscapture
       &desired_audio_spec,
       &obtained_audio_spec,
       // This example does not assume a particular buffer size for wider compatibility.
       // It's not like supporting varying buffer sizes is that hard anyways, unlike different
       // channel layouts/sample formats, which only obfuscates code.
       // Those conversions aren't hard to handle but aren't the primary purpose of this example.
       SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
    SDL_PauseAudioDevice(audio_device, 0);

    // Need a window to receive keyboard events.
    SDL_Window *window = SDL_CreateWindow(
       "Synth", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 128, 128, SDL_WINDOW_RESIZABLE);

    SDL_Scancode pressed_scancode = 0;
    size_t active_voice = 0;

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            // Warning. ACHTUNG, EVEN!!!
            // This implementation DOES result in data races. In this case they're quite harmless
            // (and possibly even desirable for lower latency) but PLEASE take care.
            // A better solution would involve a lockless ring buffer for pushing control messages
            // to the audio thread.
            case SDL_KEYDOWN: {
                if (event.key.repeat == 0) {
                    pressed_scancode = event.key.keysym.scancode;
                    float frequency = get_frequency_for_key(pressed_scancode);
                    active_voice = synth_start_voice(&state, frequency);
                }
            } break;
            case SDL_KEYUP: {
                if (event.key.keysym.scancode == pressed_scancode) {
                    synth_stop_voice(&state, active_voice);
                }
            } break;
            }
        }
    }

    SDL_CloseAudioDevice(audio_device);
    SDL_DestroyWindow(window);

    return 0;
}

void
audio_callback(void *userdata, uint8_t *byte_stream, int byte_len)
{
    synth_state_t *state = (synth_state_t *)userdata;
    float *output_buffer = (float *)byte_stream;
    // Divide by the size of a single sample and the number of channels.
    size_t sample_count = byte_len / sizeof(float) / 1;

    synth_output(state, output_buffer, sample_count);
}

float
get_frequency_for_key(SDL_Scancode scancode)
{
    float index = 0.0f;
    switch (scancode) {
    case SDL_SCANCODE_A:
        index = 0.0f;
        break;
    case SDL_SCANCODE_W:
        index = 1.0f;
        break;
    case SDL_SCANCODE_S:
        index = 2.0f;
        break;
    case SDL_SCANCODE_E:
        index = 3.0f;
        break;
    case SDL_SCANCODE_D:
        index = 4.0f;
        break;
    case SDL_SCANCODE_F:
        index = 5.0f;
        break;
    case SDL_SCANCODE_T:
        index = 6.0f;
        break;
    case SDL_SCANCODE_G:
        index = 7.0f;
        break;
    case SDL_SCANCODE_Y:
        index = 8.0f;
        break;
    case SDL_SCANCODE_H:
        index = 9.0f;
        break;
    case SDL_SCANCODE_U:
        index = 10.0f;
        break;
    case SDL_SCANCODE_J:
        index = 11.0f;
        break;
    case SDL_SCANCODE_K:
        index = 12.0f;
        break;
    case SDL_SCANCODE_O:
        index = 13.0f;
        break;
    case SDL_SCANCODE_L:
        index = 14.0f;
        break;
    case SDL_SCANCODE_P:
        index = 15.0f;
        break;
    case SDL_SCANCODE_SEMICOLON:
        index = 16.0f;
        break;
    case SDL_SCANCODE_APOSTROPHE:
        index = 17.0f;
        break;
    default:
        // Return 0 Hz if the user pressed an unknown button.
        return 0.0f;
    }
    index -= 9.0f;
    return 440.f * powf(2.0f, index / 12.0f);
}

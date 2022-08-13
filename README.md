# Synth

This small example shows how to build a simple oscillator that responds to computer keyboard input.
Maybe someday I'll expand on this, but the purpose is to show how to implement an oscillator
properly such that it doesn't cut off the signal suddenly upon trigger and release.

An alternate option would be to add a simple AR envelope to fade the sound in and out.

## Building

```sh
$ meson setup build
$ ninja -C build
```

To run:

```sh
$ ./build/src/synth
```

## Code structure

The most important bits can be found in [`src/synth.c`](src/synth.c), specifically in
`synth_start_voice` and `synth_output`. The most important things to note:

- Starting a voice resets its phase.
- Stopping a voice does not actually stop playback immediately, rather it sets a flag that is polled
  for every sample generated. Playback stops only if the flag is set and the voice's phase is
  close to 0.

This will work for any type of waveform, not just sine waves; they were chosen because they're way
easier to generate without aliasing.

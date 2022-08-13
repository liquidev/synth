#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

#include <SDL.h>
#include <cairo.h>

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Surface *sdl_surface;
    SDL_Texture *texture;

    cairo_surface_t *cairo_surface;
    cairo_t *cairo;

    uint32_t width, height;
    float widthf, heightf;
} window_state_t;

void
window_init(window_state_t *state);

void
window_resize(window_state_t *state, size_t width, size_t height);

void
window_destroy(window_state_t *state);

#endif

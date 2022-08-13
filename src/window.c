#include "window.h"

static void
window_update_size(window_state_t *state, uint32_t width, uint32_t height)
{
    state->width = width;
    state->widthf = (float)width;
    state->height = height;
    state->heightf = (float)height;
}

static void
window_init_cairo(window_state_t *state)
{
    state->cairo_surface =
       cairo_image_surface_create(CAIRO_FORMAT_ARGB32, state->width, state->height);
    state->cairo = cairo_create(state->cairo_surface);
}

static void
window_init_texture(window_state_t *state)
{
    state->sdl_surface = SDL_CreateRGBSurfaceFrom(
       cairo_image_surface_get_data(state->cairo_surface),
       state->width,
       state->height,
       32,
       cairo_image_surface_get_stride(state->cairo_surface),
       0x00FF0000,
       0x0000FF00,
       0x000000FF,
       0);
    state->texture = SDL_CreateTextureFromSurface(state->renderer, state->sdl_surface);
}

void
window_init(window_state_t *state)
{
    memset(state, 0, sizeof(window_state_t));

    const uint32_t DEFAULT_WIDTH = 256;
    const uint32_t DEFAULT_HEIGHT = 256;

    window_update_size(state, DEFAULT_WIDTH, DEFAULT_HEIGHT);

    state->window = SDL_CreateWindow(
       "Synth",
       SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED,
       DEFAULT_WIDTH,
       DEFAULT_HEIGHT,
       SDL_WINDOW_RESIZABLE);
    state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_PRESENTVSYNC);

    window_init_cairo(state);
    window_init_texture(state);
}

static void
window_destroy_cairo(window_state_t *state)
{
    if (state->cairo != NULL) {
        cairo_destroy(state->cairo);
        state->cairo = NULL;
    }
    if (state->cairo_surface != NULL) {
        cairo_surface_destroy(state->cairo_surface);
        state->cairo_surface = NULL;
    }
}

static void
window_destroy_texture(window_state_t *state)
{
    if (state->texture != NULL) {
        SDL_DestroyTexture(state->texture);
        state->texture = NULL;
    }
    if (state->sdl_surface != NULL) {
        SDL_FreeSurface(state->sdl_surface);
        state->sdl_surface = NULL;
    }
}

void
window_resize(window_state_t *state, size_t width, size_t height)
{
    window_destroy_cairo(state);
    window_destroy_texture(state);
    window_update_size(state, width, height);
    window_init_cairo(state);
    window_init_texture(state);
}

void
window_destroy(window_state_t *state)
{
    window_destroy_cairo(state);
    window_destroy_texture(state);

    SDL_DestroyWindow(state->window);
    SDL_DestroyRenderer(state->renderer);
}

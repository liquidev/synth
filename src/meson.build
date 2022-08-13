cc = meson.get_compiler('c')

executable(
    'synth',
    [
        'main.c',
        'synth.c',
    ],

    dependencies: [
        cc.find_library('m', required: false),
        dependency('sdl2'),
        dependency('cairo'),
    ]
)

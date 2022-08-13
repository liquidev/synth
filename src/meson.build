cc = meson.get_compiler('c')

executable(
    'synth',
    [
        'main.c',
        'synth.c',
    ],

    dependencies: [
        dependency('sdl2'),
        cc.find_library('m', required: false)
    ]
)

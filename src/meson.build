config_h_conf = configuration_data()
config_h_conf.set_quoted('APP_ID', app_id)
config_h_conf.set_quoted('APP_PATH', app_path)
config_h = configure_file(configuration: config_h_conf, output: 'config.h')

deps = [
  dependency('peel'),
  dependency('gtk4'),
  dependency('libadwaita-1'),
  dependency('granite-7'),
]

peel_gen = find_program('peel-gen')

peel_h = custom_target(
  'peel-codegen',
  env: { 'GI_GIR_PATH': meson.project_source_root() / 'gir' },
  command: [
    peel_gen,
    '--recursive',
    '--out-dir', '@OUTDIR@',
    'Adw', '1',
    'Granite', '7.0',
  ],
  output: 'peel',
)

sources = [
  resources,
  config_h,
  peel_h,
  'chern-application.hpp',
  'chern-application.cpp',

  'dialogs/chern-application-window.hpp',
  'dialogs/chern-application-window.cpp'
]

executable(
  meson.project_name(),
  sources,
  dependencies: deps,
  install: true
)

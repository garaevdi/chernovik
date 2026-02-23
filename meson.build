project(
  'chernovik',
  'cpp',
  version: '0.1',
  license: 'GPL-3.0-or-later',
  meson_version: '>=0.59'
)

app_id = 'io.github.garaevdi.' + meson.project_name()
app_path = '/' + app_id.replace('.', '/')
gettext = meson.project_name()

prefix = get_option('prefix')
datadir = prefix / get_option('datadir')
pkgdatadir = datadir / meson.project_name()
schemadir = datadir / 'glib-2.0' / 'schemas'

gnome = import('gnome')
i18n = import('i18n')

subdir('data')
subdir('po')
subdir('src')

gnome.post_install(
  glib_compile_schemas: true
)

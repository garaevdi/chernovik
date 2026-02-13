project(
  'chernovik',
  'cpp',
  version: '0.1',
  license: 'GPL-3.0-or-later',
  meson_version: '>=0.59'
)

app_id = 'io.github.garaevdi.' + meson.project_name()
app_path = '/' + app_id.replace('.', '/')

gnome = import('gnome')

subdir('data')
subdir('src')

gnome.post_install()

project(
  'chernovik',
  'cpp',
  version: '0.1',
  license: 'GPL-3.0-or-later',
  meson_version: '>=0.59'
)

app_id = 'com.github.garaevdi.chernovik'
app_path = '/' + app_id.replace('.', '/')

gnome = import('gnome')

subdir('data')
subdir('src')

gnome.post_install()

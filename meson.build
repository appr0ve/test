project('rdb', 'c',
  version: '1',
  default_options: ['c_std=gnu17'])

add_project_arguments(
  f'-DVERSION="@meson.project_version()@"',
  language:'c')
add_project_arguments(
  '-DLOCALEDIR="@0@"'.format(get_option('localedir')),
  '-DPACKAGE="@0@"'.format(meson.project_name()),
  language:'c')

subdir('po')

glib = dependency('glib-2.0')
json = dependency('json-glib-1.0')

lib = shared_library(
  meson.project_name(),
  'src/lib.c',
  include_directories: 'src',
  dependencies: [glib, json],
  install: true)

bin = declare_dependency(
  include_directories: 'src',
  link_with: lib)

deps = [glib, json, bin]

exe = executable(
  meson.project_name(),
  'src/main.c',
  dependencies: deps,
  install: true)

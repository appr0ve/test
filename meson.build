project('rdb', 'c',
  version: '1',
  default_options: ['c_std=gnu17'])

add_project_arguments(
  '-DVERSION="@0@"'.format(meson.project_version()),
  language:'c')
add_project_arguments(
  '-DLOCALEDIR="@0@"'.format(get_option('localedir')),
  language:'c')

subdir('po')

inc = include_directories('include')

glib = dependency('glib-2.0')
json = dependency('json-glib-1.0')


lib = shared_library('rdb',
  'librdb.c',
  include_directories: inc,
  dependencies: [glib, json],
  install: true)

rdb = declare_dependency(
  include_directories: inc,
  link_with: lib)

deps = [glib, json, rdb]

exe = executable('rdb', 'rdb.c', dependencies: deps, install: true)

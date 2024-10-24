project('rdb', 'c')
version = '1'
localedir = get_option('localedir')
add_project_arguments(f'-DVERSION="@version@"', language:'c')
add_project_arguments(f'-DLOCALEDIR="@localedir@"', language:'c')
subdir('po')
glib = dependency('glib-2.0')
json = dependency('json-glib-1.0')
inc = include_directories('include')
lib = shared_library('rdb',
  'librdb.c',
  include_directories: inc,
  dependencies: [glib, json],
  install: true)
rdb = declare_dependency(include_directories: inc,
  link_with: lib)
deps = [glib, json, rdb]
exe = executable('rdb', 'rdb.c', dependencies: deps, install: true)

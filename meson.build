project('rdb', 'c')
subdir('po')
glib = dependency('glib-2.0')
json = dependency('json-glib-1.0')
inc = include_directories('include')
lib = shared_library('librdb',
  'librdb.c',
  include_directories: inc,
  dependencies: [glib, json],
  install: true)
rdb = declare_dependency(include_directories: inc,
  link_with: lib)
deps = [glib, json, rdb]
exe = executable('rdb', 'rdb.c', dependencies: deps, install: true)
test('run', exe)

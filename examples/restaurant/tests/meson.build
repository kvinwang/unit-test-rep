
e = executable('testexe', 'test.c',
             include_directories : include_directories('../src'),
                    dependencies : dependency('cmocka'),
                       link_args : '-Wl,--wrap=chef_cook',
                       link_with : librest,
              )

test('test restarant', e)


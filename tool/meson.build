project('interframe-supervisor', 'c', default_options : ['c_std=c11', 'warning_level=2', 'buildtype=debug'])

src_dir = 'src'
unity_dir = 'unity'
test_dir = 'tests'
out_dir = 'dist'
args = 'playground'

# Compiler flags
c_flags = ['-Wall', '-g']
inc_dirs = include_directories(src_dir, unity_dir)

# Source files for main executable and test executable
src_files = files('src/main.c')
test_files = files('tests/main.test.c')

# Main executable target
executable('interframe',
  src_files,
  c_args : c_flags,
  include_directories : inc_dirs,
  install : true,
  install_dir : out_dir
)

# Test executable target, including Unity for test framework
test_executable = executable('interframe_test-runner',
  test_files + files(unity_dir + '/unity.c'),
  c_args : c_flags,
  include_directories : inc_dirs,
  install : true,
  install_dir : out_dir
)

# Define a test that runs the test executable
test('unit_test', test_executable)

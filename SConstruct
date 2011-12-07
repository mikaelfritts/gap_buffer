import os

VariantDir('build','src', duplicate=0)

env = Environment(LIBS = 'gap_buffer', LIBPATH='lib')

env.Library('lib/gap_buffer', ['build/lib/gap_buffer.c'])
env.Program('bin/gap_buffer_demo','build/gap_buffer_demo.c')

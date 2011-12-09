import os

VariantDir('build','src', duplicate=0)


env = Environment()
env.SharedLibrary('lib/gap_buffer', ['src/lib/gap_buffer.c'])


env = Environment(LIBS = 'gap_buffer', LIBPATH='lib')

# force binary to look in the local lib folder for libgap_buffer.so at runtime

env.Append( LINKFLAGS = Split('-z origin') )
env.Append( RPATH = env.Literal(os.path.join('\\$$ORIGIN', os.pardir, 'lib')))
env.Program('bin/gap_buffer_demo','build/gap_buffer_demo.c')



import os

# set up the build config for native

files = [
    'src/app_main.cpp',

    'test/rtos.cpp',
    'test/main.cpp',
    'test/thread.cpp',

    'lib/printf/src/printf/printf.c',
]

panglos = [
    'logger.cpp',
    'list.cpp',
    'io.cpp',
    'object.cpp',
    'time.cpp',

    'linux/mutex.cpp',
    'linux/thread.cpp',
    'linux/time.cpp',
    'linux/arch.cpp',
]

for name in panglos:
    files += [ '../panglos/src/' + name ]

cpppath = [
    'src/',
    'lib/panglos/src/',
    'lib/printf/src/',
]

libs = []
libpath = []
cflags = [
    '-g',
    '-Wall',
    '-Wextra',
    '-Werror',
    '-Wformat=2', # strict level
    '-Werror=format',
    
    '-DARCH_LINUX=1',
]
cxxflags = [
    '-std=c++14',
    '-Wall',
    '-Wextra',
    '-Werror',
]
lflags = [
    '-lgtest_main',
    '-lgtest',
    '-lpthread',
    '-g',
]

env = Environment(CCFLAGS=cflags, CXXFLAGS=cxxflags, LINKFLAGS=lflags, CPPPATH=cpppath)
env["CC"] = os.getenv("CC") or env["CC"]
env["CXX"] = os.getenv("CXX") or env["CXX"]
env["ENV"].update(x for x in os.environ.items() if x[0].startswith("CCC_"))
env["ENV"].update(x for x in os.environ.items() if x[0].startswith("BITBUCKET_"))
#env["COMPILER"] = os.getenv("COMPILER") or env["COMPILER"]
env["XTRA_COMPILER_FLAGS"] = os.getenv("XTRA_COMPILER_FLAGS")
env["CPLUS_INCLUDE_PATH"] = os.getenv("CPLUS_INCLUDE_PATH") 
env["LD_PATH"] = os.getenv("LD_PATH") 

env.SConscript("SConscript", exports="env")

tdd = env.Program(target='tdd', source=files, LIBS=libs, LIBPATH=libpath)

#   FIN

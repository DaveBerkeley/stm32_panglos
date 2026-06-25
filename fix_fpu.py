
#
#   Ensure that the build flags are set for the framework as well as the source

Import("env")

BUILD_FLAGS = env.get("BUILD_FLAGS", [])

if isinstance(BUILD_FLAGS, str):
    BUILD_FLAGS = BUILD_FLAGS.split()

lflags = []
for flag in BUILD_FLAGS:
    for start in [ "-mfloat-abi=", "-mfpu=" ]:
        if flag.startswith(start):
            lflags += [ flag ]

print("Applying", lflags, "to linker")

env.Append(LINKFLAGS=lflags)



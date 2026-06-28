#
#   Build all the libraries

Import("env")

IS_TARGET = 'PIOPLATFORM' in env
#for k, v in os.environ.items():
#    print(k, v)

if IS_TARGET:
    print("Building for target")
else:
    print("Building native")

if IS_TARGET:
    # We don't want the RTOS when building native
    env.SConscript("third_party/SConscript.rtos", exports="env")
    # Test has a special build for the target
    env.SConscript("test/SConscript.test", exports="env")

#   FIN

#!/bin/env python

Import("env")

print("Building library")

env.SConscript("SConscript", exports="env")

#   FIN

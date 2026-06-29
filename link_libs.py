#!/usr/bin/env python3

import os

def make_link(src, dst, path):
    s = src + path
    d = dst + path
    d_dir = os.path.dirname(d)
    s_dir = os.path.dirname(s)
    _, name = os.path.split(path)

    d_path = os.path.join(dst, path)
    r, _ = os.path.split(d_path)
    rel = '../' * len(r.split('/'))
    s_path = os.path.join(rel, s_dir, name)

    #raise Exception((s_dir, name, s_path, path, d, rel))

    if not os.path.exists(d_dir):
        print('create dir', d_dir)
        os.makedirs(d_dir)

    if not os.path.lexists(d):
        print('create link', d)
        os.symlink(s_path, d)

#
#

files = [
    'time.cpp',
    'object.cpp',
    'list.cpp',
    'io.cpp',
    'logger.cpp',
    'thread.cpp',

    'panglos/debug.h',
    'panglos/mutex.h',
    'panglos/semaphore.h',
    'panglos/time.h',
    'panglos/object.h',
    'panglos/list.h',
    'panglos/io.h',
    'panglos/logger.h',
    'panglos/thread.h',
    'panglos/queue.h',

    'panglos/drivers/gpio.h',
    'panglos/drivers/uart.h',

    'panglos/stm32/hal.h',
    'panglos/stm32/uart.h',
    'panglos/stm32/gpio_arm.h',

    'panglos/arch.h',
    'panglos/stm32/arch.h',
    'stm32/arch.cpp',
    'stm32/hal.cpp',
    'stm32/gpio.cpp',
    'stm32/uart.cpp',

    'freertos/yield.h',
    'freertos/time.cpp',
    'freertos/mutex.cpp',
    'freertos/semaphore.cpp',
    'freertos/thread.cpp',
    'freertos/queue.cpp',

    'panglos/freertos/queue.h',

    'panglos/linux/arch.h',
    'linux/thread.cpp',
    'linux/queue.cpp',
]

for path in files:
    make_link('../panglos/src/', 'lib/panglos/src/', path)

#
#   Put the google test simulation in its own lib

files = [
    'gtest/gtest.h',
    'gtest.cpp',
]

for path in files:
    make_link('../panglos/', 'lib/gtest/src/', path)

#
#   Panglos Unit Tests in their own lib

files = [
    #'list_test.cpp',
    #'buffer_test.cpp',
    #'deque_test.cpp',
    #'queue.cpp',
    #'thread.cpp',
]

for path in files:
    make_link('../panglos/unit-tests/', 'test/', path)

print("built library links ...")

# FIN

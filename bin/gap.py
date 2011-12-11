#!/bin/env python

# just messing around with python ctypes here.

import sys
from ctypes import cdll,        \
                   c_char,      \
                   c_char_p,    \
                   c_int,       \
                   c_void_p,    \
                   Structure,   \
                   POINTER

class GapBuffer(Structure):
    _fields_ = [("buffer", c_char_p),
                ("start", c_int),
                ("end", c_int),
                ("gap_start", c_int),
                ("gap_end", c_int),
                ("cursor", c_int),
                ("mode", c_int)]

class LibGap():
    def __init__(self, lib):
        self.libgap = cdll.LoadLibrary(lib)
        self.libgap.gap_buffer_new.restype = POINTER(GapBuffer)
        self.libgap.gap_buffer_move_gap.restype = c_void_p
        self.libgap.gap_buffer_resize_buffer.restype = c_void_p
        self.libgap.gap_buffer_put.restype = c_void_p
        self.libgap.gap_buffer_move_cursor.restype = c_void_p
        self.libgap.gap_buffer_insert.restype = c_void_p
        self.libgap.gap_buffer_replace.restype = c_void_p
        self.libgap.gap_buffer_delete.restype = c_void_p
        self.libgap.gap_buffer_put_str.restype = c_void_p
        self.libgap.gap_buffer_set_mode.restype = c_void_p
        self.libgap.gap_buffer_destroy.restype = c_void_p
        self.libgap.gap_buffer_distance_to_end.restype = c_int
        self.libgap.gap_buffer_distance_to_start.restype = c_int
        self.gap_buffer = self.libgap.gap_buffer_new()

    def move_gap(self):
        self.libgap.gap_buffer_move_gap(self.gap_buffer)

    def resize(self):
        self.libgap.gap_buffer_resize_buffer(self.gap_buffer)

    def move_cursor(self, distance=0):
        self.libgap.gap_buffer_move_cursor(self.gap_buffer, c_int(distance))

    def insert(self, ch):
        self.libgap.gap_buffer_insert(self.gap_buffer, c_char(ch))

    def replace(self, ch):
        self.libgap.gap_buffer_replace(self.gap_buffer, c_char(ch))

    def delete(self):
        self.libgap.gap_buffer_delete(self.gap_buffer)

    def put_str(self, string):
        self.libgap.gap_buffer_put_str(self.gap_buffer, c_char_p(string))

    def set_mode(self, mode):
        self.libgap.gap_buffer_set_mode(self.gap_buffer, c_int(mode))

    def put(self, ch):
        self.libgap.gap_buffer_put(self.gap_buffer, c_char(ch))

    def g_print(self):
        self.libgap.gap_buffer_print(self.gap_buffer)

    def destroy(self, distance=0):
        self.libgap.gap_buffer_move_gap(self.gap_buffer, c_int(distance))

    def distance_to_end(self):
        return self.libgap.gap_buffer_distance_to_end(self.gap_buffer)

    def distance_to_start(self):
        return self.libgap.gap_buffer_distance_to_start(self.gap_buffer)

if __name__ == '__main__':
    try:
        lg = LibGap('lib/libgap_buffer.so')
    except OSError, e:
        print(e)
        print("Try building the project with 'scons', and run again")
        sys.exit()
    lg.g_print()
    lg.put('x')
    lg.g_print()
    lg.put_str("Hello World")
    lg.g_print()
    lg.move_cursor(-4)
    for x in range(0, 5):
        lg.replace('X')
        lg.g_print()
    lg.move_cursor(lg.distance_to_start()+1)
    lg.g_print()
    lg.delete()
    lg.g_print()
    lg.move_cursor(5)
    lg.put_str(" there,")
    lg.g_print()

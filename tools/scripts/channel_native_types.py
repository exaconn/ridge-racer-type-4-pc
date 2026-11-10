"""The actual native PadChannel owner, mirrored for ctypes callers."""
import ctypes as C
class Combination(C.Structure):
    _fields_=[('count00',C.c_uint8),('reserved01',C.c_uint8*3),('buttons04',C.c_void_p)]
class OutputFields(C.Structure):
    _fields_=[('gates',C.c_uint8*9),('payload',C.c_uint8*6)]
class Output(C.Union):
    _fields_=[('bytes',C.c_uint8*15),('fields',OutputFields)]
class PadChannel(C.Structure):
    _fields_=[('modes00',C.c_void_p),('entries04',C.c_void_p),('combs08',C.c_void_p),
      ('children0C',C.c_void_p),('owner10',C.c_void_p),('update14',C.c_void_p),('complete18',C.c_void_p),
      ('reserved1C',C.c_uint8*4),('layout20',C.c_void_p),('argument24',C.c_uint8),('reserved25',C.c_uint8*3),
      ('data28',C.c_void_p),('data2C',C.c_void_p),('input30',C.c_void_p),('length34',C.c_uint8),('pad35',C.c_uint8),
      ('length36',C.c_uint8),('command37',C.c_uint8),('previous38',C.c_uint8),('flag39',C.c_uint8),('reserved3A',C.c_uint8*2),
      ('packet',C.c_void_p),('buffer40',C.c_void_p),('b44',C.c_uint8),('b45',C.c_uint8),('phase',C.c_uint8),
      ('state47',C.c_uint8),('flag48',C.c_uint8),('state49',C.c_uint8),('retry4A',C.c_uint8),('reserved4B',C.c_uint8),
      ('type4C',C.c_int32),('b50',C.c_uint8),('modeArgs51',C.c_uint8*2),('modeEqual53',C.c_uint8),('output54',Output),
      ('metadata63',C.c_uint8*(128+16*(C.sizeof(Combination)-8)+(C.alignment(Combination)-4))),
      ('modeCountE3',C.c_uint8),('argumentE4',C.c_uint8),('padE5',C.c_uint8),('pending',C.c_uint16),
      ('responseE8',C.c_uint8),('entry_countE9',C.c_uint8),('combCountEA',C.c_uint8),('scanIndexEB',C.c_uint8),
      ('scanOffsetEC',C.c_uint16),('scanSeenEE',C.c_uint16)]

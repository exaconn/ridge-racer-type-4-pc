"""Native SIO owners and a reversible transport for old 32-bit scalar fixtures.

The old oracles describe request words, including opaque, undereferenced buffer
addresses. Native owners use real pointers and compiler alignment. The adapter
maps the old signed word bijectively to its unsigned 32-bit address, and rejects
any high pointer instead of silently truncating it. Actual high buffer addresses
are exercised without this adapter in test_serial_buffer_pipeline.py.
"""
import ctypes as C

class SerialRequest(C.Structure):
    _fields_ = [('active', C.c_int32), ('cursor', C.c_void_p), ('remaining', C.c_int32)]

class SerialReceiveRequest(C.Structure):
    _fields_ = [('request', SerialRequest), ('receive_status', C.c_uint32)]

class RequestWords:
    def __init__(self, owner):
        self.owner = owner
        self.receive = isinstance(owner, SerialReceiveRequest)
        self.request = owner.request if self.receive else owner

    @classmethod
    def in_dll(cls, library, name, receive=False):
        owner = (SerialReceiveRequest if receive else SerialRequest).in_dll(library, name)
        return cls(owner)

    def __len__(self):
        return 4 if self.receive else 3

    def __getitem__(self, index):
        if isinstance(index, slice):
            return [self[i] for i in range(*index.indices(len(self))) ]
        if not 0 <= index < len(self):
            raise IndexError(index)
        if index == 1:
            pointer = self.request.cursor or 0
            if pointer > 0xFFFFFFFF:
                raise AssertionError('A native pointer cannot be compared through the word oracle')
            return C.c_int32(pointer).value
        if index == 0:
            return self.request.active
        if index == 2:
            return self.request.remaining
        return C.c_int32(self.owner.receive_status).value

    def __setitem__(self, index, value):
        if isinstance(index, slice):
            indices = list(range(*index.indices(len(self))))
            values = list(value)
            if len(indices) != len(values):
                raise ValueError('Request word count changed')
            for i, v in zip(indices, values):
                self[i] = v
            return
        if not 0 <= index < len(self):
            raise IndexError(index)
        if index == 0:
            self.request.active = value
        elif index == 1:
            self.request.cursor = C.c_uint32(value).value
        elif index == 2:
            self.request.remaining = value
        else:
            self.owner.receive_status = value

    def __bytes__(self):
        return C.string_at(C.addressof(self.owner), C.sizeof(self.owner))

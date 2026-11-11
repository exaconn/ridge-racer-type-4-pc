"""Reference runs on DuckStation for behaviour comparison with the port.

Drives our modified duckstation-regtest (AI control port + GDB stub):
boot the CUE paused, advance frames, press pad 1 buttons, take screenshots
and read PS1 RAM (mode(): ModeGroup/ModeIndex/SubmodeState).

    from duckstation import DuckStation
    d = DuckStation(); d.advance(400); d.press('Start'); print(d.mode())

Pad 2 (VS battle): DuckStation(pad2=True) connects a digital pad in port 2
(REGTEST_PAD2=1); drive it with press(name, port=2) / buttons(port=2, ...).
poke(addr, bytes) writes PS1 RAM through the GDB 'M' packet.

Environment: RR4_DUCKSTATION (regtest binary), RR4_DISC (CUE path).
"""
import socket, subprocess, time, os, struct, sys

DS = os.environ.get('RR4_DUCKSTATION', os.path.expanduser('~/Projects/duckstation-pe2-reference/build-local/bin/duckstation-regtest'))
CUE = os.environ.get('RR4_DISC') or os.path.expanduser('~/Projects/ridge-racer-type-4-decomp/raw/Ridge Racer Type 4 (Europe) (En,Fr,De,Es,It)/Ridge Racer Type 4 (Europe) (En,Fr,De,Es,It).cue')


class DuckStation:
    def __init__(self, ai=47011, gdb=47012, log=None, pad2=False):
        env = dict(os.environ, SDL_AUDIODRIVER='dummy')
        if pad2:
            env['REGTEST_PAD2'] = '1'
        self.p = subprocess.Popen([DS, '-ai-port', str(ai), '-gdb', str(gdb), '-start-paused', '-frames', '10000000',
                                   '--', CUE], stdout=log or subprocess.DEVNULL,
                                  stderr=subprocess.STDOUT, env=env, cwd=os.path.dirname(DS))
        for _ in range(200):
            try:
                self.ai = socket.create_connection(('127.0.0.1', ai))
                break
            except OSError:
                time.sleep(0.1)
        self.aif = self.ai.makefile('rb')
        self.greeting = self.aif.readline().decode().strip()
        for _ in range(100):
            try:
                self.g = socket.create_connection(('127.0.0.1', gdb))
                break
            except OSError:
                time.sleep(0.1)
        self.g.settimeout(10)

    def cmd(self, line):
        self.ai.sendall((line + '\n').encode())
        return self.aif.readline().decode().strip()

    def advance(self, n):
        r = self.cmd(f'advance {n}')
        while True:
            st = self.cmd('status')
            if 'advance_remaining=0' in st:
                return r
            time.sleep(0.01)

    def buttons(self, port=1, **kw):
        verb = 'buttons2' if port == 2 else 'buttons'
        return self.cmd(verb + ' ' + ' '.join(f'{k}={v}' for k, v in kw.items()))

    def release(self, port=1):
        return self.cmd('release2' if port == 2 else 'release')

    def press(self, name, frames=4, port=1):
        self.buttons(port=port, **{name: 1})
        self.advance(frames)
        self.release(port)

    def screenshot(self, path):
        """Write the displayed frame as a PPM. DuckStation's regtest reads its
        RGB5A1 display texture back and expands each 5-bit channel c to
        (c << 3) | (c & 7) (9 for c=1), while the port presents and captures
        round(c * 255 / 31) (8 for c=1), the full-range PS1 DAC level that
        DuckStation's own display shaders also show. Compare frames from the
        two at 5 bits (value >> 3 recovers c from either), or compare VRAM:
        `vram <path>` here, RR4_GPU_DUMP_AT in the port (GPUDUMP replay)."""
        self.ai.sendall(b'screenshot\n')
        hdr = self.aif.readline().decode().split()
        if hdr[0] != 'SCREENSHOT':
            raise RuntimeError(hdr)
        w, h, pitch, size = map(int, hdr[1:5])
        data = self.aif.read(size)
        with open(path, 'wb') as f:
            f.write(b'P6\n%d %d\n255\n' % (w, h))
            for y in range(h):
                row = data[y * pitch:y * pitch + w * 4]
                f.write(bytes(b for i in range(0, len(row), 4) for b in row[i:i + 3]))

    # --- GDB remote protocol
    def _gsend(self, payload):
        cs = sum(payload.encode()) & 0xFF
        self.g.sendall(f'${payload}#{cs:02x}'.encode())
        while True:
            r = self._gread()
            if not (len(r) == 3 and r[0] in 'ST') and not r.startswith('T'):
                return r

    def _gread(self):
        buf = b''
        while True:
            ch = self.g.recv(1)
            if ch in (b'+',):
                continue
            if ch == b'$':
                break
        while True:
            ch = self.g.recv(1)
            if ch == b'#':
                self.g.recv(2)
                break
            buf += ch
        self.g.sendall(b'+')
        return buf.decode()

    def mem(self, addr, n):
        out = b''
        while n:
            k = min(n, 0x400)
            r = self._gsend(f'm{addr:x},{k:x}')
            try:
                out += bytes.fromhex(r)
            except ValueError:
                raise RuntimeError('gdb reply %r' % r[:80])
            addr += k
            n -= k
        return out

    def poke(self, addr, data):
        r = self._gsend(f'M{addr:x},{len(data):x}:{bytes(data).hex()}')
        if r != 'OK':
            raise RuntimeError('gdb reply %r' % r[:80])

    def u16(self, a):
        return struct.unpack('<H', self.mem(a, 2))[0]

    def s16(self, a):
        return struct.unpack('<h', self.mem(a, 2))[0]

    def u32(self, a):
        return struct.unpack('<I', self.mem(a, 4))[0]

    def mode(self):
        return (self.u16(0x800F4C6A), self.s16(0x800F3A26), self.u16(0x800F4D8A))

    def audiodump(self, path=None):
        """Append the mixed SPU output (raw 44100 Hz s16 stereo) to path from
        now on; without a path, close the dump. Set DUCKSTATION_SPU_REG_TRACE=1
        (or DUCKSTATION_SPU_KEY_TRACE=1) in the environment before creating
        the instance, with log= a file, to log SPU register writes; compare
        with the port's PSYZ_SPU_REG_TRACE / RR4_AUDIO_CAPTURE."""
        return self.cmd('audiodump ' + path if path else 'audiodump')

    def close(self):
        try:
            self.cmd('quit')
        except Exception:
            pass
        self.p.kill()

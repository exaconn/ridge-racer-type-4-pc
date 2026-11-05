"""emu_probe.py: drive the DuckStation fork (~/Projects/duckstation-pe2-reference, duckstation-regtest) from Python.

Used to find out what a field *does* instead of guessing from the code: start the game paused,
        duckstation-regtest -frames 2000000 -ai-port 5555 -gdb 5556 -start-paused -console <disc>.cue
then `AI` (advance/buttons/vram/pause/resume) and `GDB` (memory read/write, breakpoint + registers).
Example: gdb_break_regs(g, 0x80027C54)[4] is the Car* passed to func_80027C54 in a race/demo;
poking Car.posX (+0x180000) moves car and camera to another place on the track, which is how
Car.w0 was identified as a world coordinate.  Evidence from running the game, not from the EXE alone."""
import socket,time,struct
class AI:
    def __init__(s,port=5555):
        s.s=socket.create_connection(("127.0.0.1",port),timeout=60); s.s.recv(4096)
    def cmd(s,c):
        s.s.sendall((c+"\n").encode()); buf=b""
        while not buf.endswith(b"\n"): buf+=s.s.recv(65536)
        return buf.decode().strip()
class GDB:
    def __init__(s,port=5556):
        s.s=socket.create_connection(("127.0.0.1",port),timeout=120)
    def _send(s,p):
        s.s.sendall(b"$"+p.encode()+b"#%02x"%(sum(p.encode())&255))
        buf=b""
        while True:
            buf+=s.s.recv(65536)
            i=buf.find(b"$")
            if i>=0 and b"#" in buf[i:] and len(buf)>=buf.find(b"#",i)+3:
                j=buf.find(b"#",i); s.s.sendall(b"+"); return buf[i+1:j].decode()
    def mem(s,addr,n):
        for _ in range(5):
            r=s._send("m%x,%x"%(addr,n))
            if len(r)==2*n and all(c in "0123456789abcdef" for c in r): return bytes.fromhex(r)
        raise RuntimeError("gdb read failed: "+r)
    def w32(s,addr): return struct.unpack("<I",s.mem(addr,4))[0]
def adv(a,n):
    a.cmd("advance %d"%n)
    while True:
        r=a.cmd("status")
        if "advance_remaining=0" in r: return r
        time.sleep(0.2)
import zlib
def vram_png(src,dst,x0=0,y0=0,w=1024,h=512):
    d=open(src,"rb").read(); rows=[]
    for y in range(y0,y0+h):
        row=bytearray([0])
        for x in range(x0,x0+w):
            v=d[(y*1024+x)*2]|(d[(y*1024+x)*2+1]<<8)
            row+=bytes(((v&31)*255//31,((v>>5)&31)*255//31,((v>>10)&31)*255//31))
        rows.append(bytes(row))
    raw=b"".join(rows)
    def ch(t,b): c=struct.pack(">I",len(b))+t+b; return c+struct.pack(">I",zlib.crc32(t+b)&0xffffffff)
    open(dst,"wb").write(b"\x89PNG\r\n\x1a\n"+ch(b"IHDR",struct.pack(">IIBBBBB",w,h,8,2,0,0,0))+ch(b"IDAT",zlib.compress(raw))+ch(b"IEND",b""))
def gdb_break_regs(g,addr,timeout=60):
    g._send("Z0,%x,4"%addr)
    g.s.sendall(b"$c#63"); g.s.settimeout(timeout)
    buf=b""
    while b"#" not in buf[buf.find(b"$"):] if b"$" in buf else True: buf+=g.s.recv(65536)
    g.s.sendall(b"+")
    r=g._send("g"); g._send("z0,%x,4"%addr)
    regs=struct.unpack("<%dI"%(len(r)//8),bytes.fromhex(r))
    return regs
def gdb_write(g,addr,data):
    r=g._send("M%x,%x:%s"%(addr,len(data),data.hex())); return r

from collections import Counter
import math
import pathlib
import sys

def flp2(x):
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x - (x >> 1)

def silly_clp2ffs(x):
    for b in range(0, 8):
        if (x-1) <= (1 << b): return b;

def clp2(x):
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x + 1;


def ffs(x):
    return int(math.log2(x & -x) + 1)

ispow2or0 = lambda x: (x & (x - 1)) == 0

ispow2 = lambda x: (x != 0 and ispow2or0(x))

# this method failed the tests
#def iterdrawvalues(entropy, popsize):
#    for b in range(0, 8):
#        if (popsize-1) <= (1 << b): break;
#    entropyidx = 0
#    entropynextbit = 0
#    while entropyidx < 256:
#        k = 0xFF
#        while popsize-1 < k:
#            if k == 0xFF:
#                if (8 < entropynextbit + b):
#                    k = (entropy[entropyidx % 256] >> entropynextbit) + ((((entropy[(entropyidx+1) % 256] << (16 - b - entropynextbit)) & 0xFF) >> (8 - b)));
#                    entropyidx += 1;
#                    entropynextbit = (entropynextbit + b) - 8;
#                else:
#                    k = ( (entropy[entropyidx % 256] << (8 - b - entropynextbit) ) & 0xFF ) >> (8 - b);
#                    entropynextbit += b;
#                    if (entropynextbit == 8):
#                        entropyidx += 1;
#                        entropynextbit = 0;
#            else:
#                k = ((((k >> 1) + (((entropy[entropyidx % 256] >> entropynextbit) & 0x01) << (b - 1))) << (8 - b)) & 0xFF) >> (8 - b);
#                entropynextbit += 1;
#                if (entropynextbit == 8):
#                    entropyidx += 1;
#                    entropynextbit = 0;
#        yield k

def iterdrawvalues(entropy, minv, maxv, i=0, br=8):
    popsize = maxv-minv+1
    for b in range(0, 8):
        if (popsize-1) <= (1 << b): break;
    entropyidx = 0
    entropynextbit = 0
    while entropyidx < len(entropy) - 10:
        k = 0xFF
        while popsize-1 < k:
            if (8 < entropynextbit + b):
                k = (entropy[entropyidx] >> entropynextbit) + ((((entropy[entropyidx+1] << (16 - b - entropynextbit)) & 0xFF) >> (8 - b)));
                entropyidx += 1;
                entropynextbit = (entropynextbit + b) - 8;
            else:
                k = ( (entropy[entropyidx] << (8 - b - entropynextbit) ) & 0xFF ) >> (8 - b);
                entropynextbit += b;
                if (entropynextbit == 8):
                    entropyidx += 1;
                    entropynextbit = 0;
        yield k

takebits = lambda b, s, o: ((b >> (o if 0 <= o else 0)) << (0 if 0 <= o else -o) ) & ((1 << s) - 1)

def drawvalue(entropy, minv, maxv, log, i=0, br=8):
    popsize = maxv-minv+1
    size0 = clp2(popsize)
    mask0 = size0 - 1
    s0 = ffs(size0) - 1
    s1 = s0
    mask1 = mask0
    size1 = size0 - popsize
    size1ispow2 = ispow2(size1)
    vo = 0
    if size1ispow2:
        vo = size1//2
        s1 -= ffs(size1) - 1
        mask1 = size1 - 1
    #print(f'popsize={popsize}; s0={s0}; s1={s1}')
    s = s0
    mask = mask0
    v0 = 0
    overflow = False
    while True:
        #print(f's={s}; mask={mask} 0x{mask:0>2X}')
        o = br - s
        va = takebits(entropy[i], s, o)
        v0 += va
        #print(f'i={i}; br={br}; o={o}; entropy_byte={entropy[i]} 0x{entropy[i]:0>2X}; va={va} 0x{va:0>2X}; v0={v0} 0x{v0:0>2X}')
        if o == 0:
            i += 1
            o = 8
        elif o < 0:
            i += 1
            o = 8 + o
            va = takebits(entropy[i], s, o)
            v0 += va
            #print(f'i={i}; br={br}; o={o}; entropy_byte={entropy[i]} 0x{entropy[i]:0>2X}; va={va} 0x{va:0>2X}; v0={v0} 0x{v0:0>2X}')
        br = o
        #print(f'i={i}; o={o}; entropy_byte={entropy[i]} {entropy[i]:0>2X}; va={va} 0x{va:0>2X}; v0={v0} 0x{v0:0>2X}')
        if v0 < popsize:
            break
        else:
            overflow = True
            v0 = (v0 << s1) & mask0
            s = s1
            mask = mask1
        #print(f'v0={v0} 0x{v0:0>2X}')
    v = v0 + minv
    v = (-v if overflow else v)
    #print(f'v={v} 0x{v0:0>2X}; i={i}, br=o={br}')
    return v, i, br


#def drawvalue(entropy, minv, maxv, i=0, br=8):
#    popsize = maxv-minv+1
#    size0 = clp2(popsize)
#    mask0 = size0 - 1
#    s0 = ffs(size0) - 1
#    s1 = s0
#    mask1 = mask0
#    size1 = size0 - popsize
#    size1ispow2 = ispow2(size1)
#    vo = 0
#    if size1ispow2:
#        vo = size1//2
#        s1 -= ffs(size1) - 1
#        mask1 = size1 - 1
#    #print(f'popsize={popsize}; s0={s0}; s1={s1}')
#    s = s0
#    mask = mask0
#    v0 = 0
#    overflow = False
#    while True:
#        #print(f's={s}; mask={mask} 0x{mask:0>2X}')
#        o = br - s
#        va = takebits(entropy[i], s, o)
#        v0 += va
#        #print(f'i={i}; o={o}; entropy_byte={entropy[i]} {entropy[i]:0>2X}; va={va} 0x{va:0>2X}; v0={v0} 0x{v0:0>2X}')
#        if o <= 0:
#            i += 1
#            o = 8 + o
#            if o < 0:
#                va = takebits(entropy[i], s, o)
#                v0 += va
#        br = o
#        #print(f'i={i}; o={o}; entropy_byte={entropy[i]} {entropy[i]:0>2X}; va={va} 0x{va:0>2X}; v0={v0} 0x{v0:0>2X}')
#        if vo <= v0 and v0 <= size0-vo:
#            break
#        else:
#            overflow = True
#            v0 = (v0 << s1) & mask0
#            s = s1
#            mask = mask1
#        #print(f'v0={v0} 0x{v0:0>2X}')
#    v = v0 - vo - 1 + minv
#    v = (-v if overflow else v)
#    #print(f'v={v} 0x{v0:0>2X}; i={i}, br=o={br}')
#    return v, i, br


#def drawvalue(entropy, minv, maxv, log, i=0, br=8):
#    s = 3
#    v0 = 0
#    while True:
#        o = br - s
#        va = takebits(entropy[i], s, o)
#        v0 += va
#        log.append(f'i={i}; br={br}; o={o}; entropy_byte={entropy[i]} 0x{entropy[i]:0>2X}; va={va} 0x{va:0>2X}; v0={v0} 0x{v0:0>2X}')
#        if o == 0:
#            i += 1
#            o = 8
#        elif o < 0:
#            i += 1
#            o = 8 + o
#            va = takebits(entropy[i], s, o)
#            v0 += va
#            log.append(f'i={i}; br={br}; o={o}; entropy_byte={entropy[i]} 0x{entropy[i]:0>2X}; va={va} 0x{va:0>2X}; v0={v0} 0x{v0:0>2X}')
#        br = o
#        return v0, i , br



def iterdrawvalues(entropy, minv, maxv, i=0, br=8):
    log = []
    while True:
        try:
            v, i, br = drawvalue(entropy, minv, maxv, log, i=i, br=br)
        except IndexError:
            with open("log.txt", "w") as _f:
                _f.write("\n".join(log))
            return
        #log.append("")
        #input()
        yield v

def draw(entropy, minv, maxv, i=0, br=8):
    return list(iterdrawvalues(entropy, minv, maxv, i=i, br=br))

if __name__ == "__main__":
    minv = int(sys.argv[1])
    maxv = int(sys.argv[2])
    if len(sys.argv) >= 4:
        filepath = pathlib.Path(sys.argv[3])
    else:
        filepath = pathlib.Path("../entropy.dat")
    with filepath.open("rb") as _f:
        entropy = _f.read()
    drawL = draw(entropy, minv, maxv)
    c = Counter(drawL)
    for k in sorted(c):
        print(f'{k}: {c[k]}')

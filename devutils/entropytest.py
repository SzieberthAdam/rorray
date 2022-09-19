from collections import Counter
import pathlib



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

def iterdrawvalues(entropy, popsize):
    for b in range(0, 8):
        if (popsize-1) <= (1 << b): break;
    entropyidx = 0
    entropynextbit = 0
    while entropyidx < 256:
        k = 0xFF
        while popsize-1 < k:
            if (8 < entropynextbit + b):
                k = (entropy[entropyidx % 256] >> entropynextbit) + ((((entropy[(entropyidx+1) % 256] << (16 - b - entropynextbit)) & 0xFF) >> (8 - b)));
                entropyidx += 1;
                entropynextbit = (entropynextbit + b) - 8;
            else:
                k = ( (entropy[entropyidx % 256] << (8 - b - entropynextbit) ) & 0xFF ) >> (8 - b);
                entropynextbit += b;
                if (entropynextbit == 8):
                    entropyidx += 1;
                    entropynextbit = 0;
        yield k

def draw(entropy, popsize):
    return list(iterdrawvalues(entropy, popsize))

if __name__ == "__main__":
    filepath = pathlib.Path("../entropy.dat")
    with filepath.open("rb") as _f:
        entropy = _f.read()
    drawL = draw(entropy, 30)
    c = Counter(drawL)
    for k in sorted(c):
        print(f'{k}: {c[k]}')

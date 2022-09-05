import pathlib
import sys


def str2int(s):
    s = s.lower()
    if s.startswith("0b"):
        v = int(s, 2)
    elif s.startswith("0x"):
        v = int(s, 16)
    else:
        v = int(s)
    return v


def str2int_with_addr(s, keydata):
    if ":" in s:
        sp = [s_.strip() for s_ in s.split(":")]
        return keydata[sp[0]] + str2int(sp[1])
    return str2int(s)
    

def frmstr2rorbytes(frmstr):
    # pass 1
    VAL_SIZE = 2
    BASE_TYP_LSC = 11 
    # 2 bytes: VVVVVTYP>....<ID
    # bit length of TYP (type) and ID varies
    # V: value category; V==0 is the object (location) itself
    keydata = {}
    key = None
    keyfrm = None
    entitycount = None
    keyidbits = None
    keynrbits = None
    keyvalbits = None
    for sline in frmstr.split("\n"):
        sline = sline.partition(";")[0].rstrip() # trim comments
        if sline.startswith("K "):
            key = sline[2:]
        elif sline.startswith("F "):
            keyfrm = "".join(sline[2:].upper().split())
            keyfrm, _, keyshift = keyfrm.partition("+")
            keyshift = (0 if keyshift=="" else str2int(keyshift)) # TODO
            if entitycount is None:
                entitycount = 2**len(keyfrm)
            else:
                #print([keyfrm, entitycount, 2**len(keyfrm)])
                assert entitycount == 2**len(keyfrm)
            keyidstr = "".join(c for c in keyfrm if c in "01")
            keyidval = int(f'0b{keyidstr}', 2)
            keyidbits = len(keyidstr)
            keynrbits = sum(1 for c in keyfrm if c=="N")
            keyvalbits = sum(1 for c in keyfrm if c=="V")
            assert keyidbits + keynrbits + keyvalbits == len(keyfrm)
            keyrefaddr = keyidval << (keynrbits + keyvalbits)
            keydata[key] = (keyrefaddr, keyidbits, keynrbits, keyvalbits)
    # pass 2
    b = bytearray(entitycount * VAL_SIZE)
    for sline in frmstr.split("\n"):
        sline = sline.partition(";")[0].rstrip() # trim comments
        if sline.startswith("K "):
            key = sline[2:]
            keyrefaddr, keyidbits, keynrbits, keyvalbits = keydata[key]
        elif sline.startswith("V "):
            a = tuple(str2int(s.strip()) for s in sline[2:].split())
            n, v, val = a
            addr = keyrefaddr + (n << keyvalbits) + v
            b[VAL_SIZE * addr : VAL_SIZE * addr + VAL_SIZE] = divmod(val, 256)
            print(f'0x{VAL_SIZE * addr:0>4X} <- 0x{val:0>4X}')
        elif sline.startswith("L "):
            a = [s.strip() for s in sline[2:].split()]
            n = str2int(a[0])
            addr = keyrefaddr + (n << keyvalbits)
            linkkey = a[1]
            linkn = (str2int(a[2]) if len(a) == 3 else 0)
            linkrefaddr, linkidbits, linknrbits, linkvalbits = keydata[linkkey]
            linkaddr = linkrefaddr + (linkn << linkvalbits)
            b[VAL_SIZE * addr : VAL_SIZE * addr + VAL_SIZE] = divmod(linkaddr, 256)
            print(f'0x{VAL_SIZE * addr:0>4X} <- 0x{linkaddr:0>4X}')
    return bytes(b)


if __name__ == "__main__":

    usage = """Usage: python frm2ror.py [scenario.frm]"""

    if len(sys.argv) == 1:
        frm_path = pathlib.Path("scenario.frm")
    else:
        try:
            frm_path = pathlib.Path(sys.argv[1])
        except (IndexError, ValueError):
            print(usage)
            sys.exit(1)

    with frm_path.open("r", encoding="utf-8") as _f:
        frmstr = _f.read()

    rorbytes = frmstr2rorbytes(frmstr)

    with frm_path.with_suffix(".ror").open("wb") as _f:
        c = _f.write(rorbytes)

    assert c == len(rorbytes)

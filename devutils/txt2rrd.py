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


def str2int_with_addr(s, refaddr):
    if ":" in s:
        sp = [s_.strip() for s_ in s.split(":")]
        return refaddr[sp[0]] + str2int(sp[1])
    return str2int(s)
    



def txtstr2rrdbytes(txtstr):
    # pass 1
    VAL_SIZE = 2
    BASE_TYP_LSC = 11 
    # 2 bytes: VVVVVTYP>....<ID
    # bit length of TYP (type) and ID varies
    # V: value category; V==0 is the object (location) itself
    refaddr = {}
    key = None
    for sline in txtstr.split("\n"):
        if sline.startswith("$"):
            key = sline[1:].strip()
        elif sline.startswith("T"):
            tstr = sline[1:].strip()
            lsc = BASE_TYP_LSC - len(tstr)
            v = str2int(f'0b{tstr}')
            refaddr[key] = v << lsc
    # pass 2
    b = bytearray((256**VAL_SIZE)*VAL_SIZE)
    for sline in txtstr.split("\n"):
        if sline.startswith("$"):
            key = sline[1:].strip()
        elif sline.startswith(":"):
            a = tuple(str2int_with_addr(s.strip(), refaddr) for s in sline[1:].strip().split("#"))
            addr = str2int_with_addr(f'{key}:{a[0]}', refaddr) + (a[1] << BASE_TYP_LSC)
            # print(str2int_with_addr(f'{key}:{a[0]}', refaddr), a[1], (a[1] << BASE_TYP_LSC), addr, a[2])
            b[VAL_SIZE * addr : VAL_SIZE * addr + VAL_SIZE] = divmod(a[2], 256)
    return bytes(b)


if __name__ == "__main__":

    usage = """Usage: python txt2rrd.py [scenario-rrd.txt]"""

    if len(sys.argv) == 1:
        txt_path = pathlib.Path("scenario-rrd.txt")
    else:
        try:
            txt_path = pathlib.Path(sys.argv[1])
        except (IndexError, ValueError):
            print(usage)
            sys.exit(1)

    with txt_path.open("r", encoding="utf-8") as _f:
        txtstr = _f.read()

    rrdbytes = txtstr2rrdbytes(txtstr)

    with (txt_path.parent / "scenario.rrd").open("wb") as _f:
        c = _f.write(rrdbytes)

    assert c == len(rrdbytes)

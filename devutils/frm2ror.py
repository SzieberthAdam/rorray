import pathlib
import sys


VAL_SIZE = 2
#SECTION_ALIGN = 0x1000 # may be used if more parts will be added


RORH_TEMPLATE = """#ifndef __DEFINE_RORFILEH__
#define __DEFINE_RORFILEH__

{{rorh}}

#endif  /* #ifndef __DEFINE_RORFILEH__ */
;"""


def str2int(s):
    s = s.lower()
    if s.startswith("0b"):
        v = int(s, 2)
    elif s.startswith("0x"):
        v = int(s, 16)
    else:
        v = int(s)
    return v


def frmstrpass1(frmstr):
    # 2 bytes: VVVVVTYP>....<ID
    # bit length of TYP (type) and ID varies
    # V: value category; V==0 is the object (location) itself
    d = {}
    keys = []
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
            if not keys or keys[-1] != key:
                keys.append(key)
            d.setdefault(key, [None, None, None, None, 0, 0])
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
            keyidbits = d[key][1] = len(keyidstr)
            keynrbits = d[key][2] = sum(1 for c in keyfrm if c=="N")
            keyvalbits = d[key][3] = sum(1 for c in keyfrm if c=="V")
            assert keyidbits + keynrbits + keyvalbits == len(keyfrm)
            keyrefaddr = d[key][0] = (keyidval << (keynrbits + keyvalbits)) * VAL_SIZE
        elif sline.startswith("L "):
            n = str2int(sline[2:].split()[0].strip())
            keymaxn = d[key][4] = max(n, (d[key][4] or 0))
        elif sline.startswith("S "):
            n = str2int(sline[2:].split()[0].strip())
            keymaxn = d[key][4] = max(n, (d[key][4] or 0))
            s = str2int(sline[2:].split()[1].strip())
            keymaxs = d[key][5] = max(s, (d[key][5] or 0))
    keys = tuple(keys)
    assert len(keys) == len(set(keys))
    return entitycount, keys, d


def frmstr2rorarrofbytes(*args):
    assert len(args) in {1, 4}
    frmstr = args[0]
    if len(args) == 1:
        entitycount, keys, d = frmstrpass1(frmstr)
    elif len(args) == 4:
        entitycount, keys, d = args[1:]
    d_keystrs = {}

    for key in keys:
        keyrefaddr, keyidbits, keynrbits, keyvalbits, keymaxn, keymaxs = d[key]
        # print(key, d[key])
        d_keystrs[key] = [""] * (keymaxn + 1) * (keymaxs + 1)

    # pass 2
    b = bytearray(entitycount * VAL_SIZE)
    for sline in frmstr.split("\n"):
        sline = sline.partition(";")[0].rstrip() # trim comments
        if sline.startswith("K "):
            key = sline[2:]
            keyrefaddr, keyidbits, keynrbits, keyvalbits, keymaxn, keymaxs = d[key]
        elif sline.startswith("V "):
            a = tuple(str2int(s.strip()) for s in sline[2:].split())
            n, v, val = a
            addr = keyrefaddr + ((n << keyvalbits) + v) * VAL_SIZE
            b[addr : addr + VAL_SIZE] = divmod(val, 256)
            # print(f'0x{addr:0>4X} <- 0x{val:0>4X}')
        elif sline.startswith("L "):
            a = [s.strip() for s in sline[2:].split()]
            n = str2int(a[0])
            addr = keyrefaddr + (n << keyvalbits) * VAL_SIZE
            linkkey = a[1]
            linkn = (str2int(a[2]) if len(a) == 3 else 0)
            linkrefaddr, _, _, linkvalbits, _, _ = d[linkkey]
            linkaddr = linkrefaddr + (linkn << linkvalbits) * VAL_SIZE
            b[addr : addr + VAL_SIZE] = divmod(linkaddr, 256)
            # print(f'0x{addr:0>4X} <- 0x{linkaddr:0>4X}')
        elif sline.startswith("S "):
            a = [s.strip() for s in sline[2:].split()]
            n = str2int(a[0])
            s = str2int(a[1])
            keyitemstr = " ".join(a[2:])
            # print([len(d_keystrs[key]), d_keystrs[key]])
            # print([d[key][4] * s + n, d[key][4], s, n])
            d_keystrs[key][d[key][4] * s + n] = keyitemstr

    mainbytes = bytes(b)
    stringbytes = b'\0'.join(bytearray(b'\0'.join(keyitemstr.encode("utf-8") for keyitemstr in d_keystrs[key])) for key in keys) + b'\0'
    #stringbytes += b'\0' * (SECTION_ALIGN - (len(stringbytes) % SECTION_ALIGN))
    return mainbytes, stringbytes


def frmstr2rorbytes(*args):
    assert len(args) in {1, 2}
    if len(args) == 1:
        frmstr = args[0]
        mainbytes, stringbytes = frmstr2rorarrofbytes(frmstr)
    elif len(args) == 2:
        mainbytes, stringbytes = args
    return mainbytes + stringbytes


def frmstr2rorhstr(*args):
    assert len(args) in {1, 4, 5}
    if len(args) == 1:
        frmstr = args[0]
        entitycount, keys, d = frmstrpass1(frmstr)
        mainbytes, stringbytes = frmstr2rorarrofbytes(frmstr, entitycount, keys, d)
    elif len(args) == 4:
        frmstr = args[0]
        entitycount, keys, d = args[1:]
        mainbytes, stringbytes = frmstr2rorarrofbytes(frmstr, entitycount, keys, d)
    elif len(args) == 5:
        entitycount, keys, d, mainbytes, stringbytes = args

    lines = [
        f'#define RORH_VAL_SIZE {VAL_SIZE}',

        f'#define RORH_MAINLENGTH {len(mainbytes)}',
        f'#define RORH_STRINGLENGTH {len(stringbytes)}',
        "",
        f'#define RORH_TOTALLENGTH {len(mainbytes) + len(stringbytes)}',
        "",
        f'#define RORH_KEYCOUNT {len(keys)}',
        "",
    ]

    stridx = 0
    for key in keys:
        keyrefaddr, keyidbits, keynrbits, keyvalbits, keymaxn, keymaxs = d[key]
        strcount = (keymaxn + 1) * (keymaxs + 1)
        lines.extend([
            f'#define RORH_{key}_REF_ADDR 0x{keyrefaddr:0>4X}',
            f'#define RORH_{key}_ID_BITS {keyidbits}',
            f'#define RORH_{key}_NR_BITS {keynrbits}',
            f'#define RORH_{key}_VAL_BITS {keyvalbits}',
            f'#define RORH_{key}_MAX_N {keymaxn}',
            f'#define RORH_{key}_MAX_S {keymaxs}',
            f'#define RORH_{key}_STR_IDX {stridx}',
            f'#define RORH_{key}_STR_COUNT {strcount}',
            "",
        ])
        stridx += strcount

    lines.extend([
        f'#define RORH_TOTALSTRINGCOUNT {stridx}',
    ])

    return RORH_TEMPLATE.replace("{{rorh}}", "\n".join(lines))



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

    entitycount, keys, d = frmstrpass1(frmstr)
    arrofbytes = frmstr2rorarrofbytes(frmstr, entitycount, keys, d)
    mainbytes, stringbytes = arrofbytes

    rorbytes = frmstr2rorbytes(mainbytes, stringbytes)

    with frm_path.with_suffix(".ror").open("wb") as _f:
        c = _f.write(rorbytes)

    assert c == len(rorbytes)

    rorhstr = frmstr2rorhstr(entitycount, keys, d, mainbytes, stringbytes)

    with (frm_path.parent / "rorfile.h").open("w", encoding="utf-8") as _f:
        c = _f.write(rorhstr)

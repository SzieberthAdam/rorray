import json
import pathlib
import struct
import sys

RORH_TEMPLATE = """#ifndef __DEFINE_RORFILEH__
#define __DEFINE_RORFILEH__

{{rorh}}

#endif  /* #ifndef __DEFINE_RORFILEH__ */
;"""

ATTRTYPE = {
    "attr"  : {"id": 0x0A, "name": "attr"  , "length": 2, "type": int, "struct": "H", "range": range(0, 65536)},
    "bool"  : {"id": 0x01, "name": "bool"  , "length": 1, "type": int, "struct": "?", "range": range(0, 2)},
    "group" : {"id": 0x06, "name": "group" , "length": 2, "type": int, "struct": "H", "range": range(0, 65536)},
    "int16" : {"id": 0x50, "name": "int16" , "length": 2, "type": int, "struct": "h", "range": range(-32768, 32768)},
    "int32" : {"id": 0x60, "name": "int32" , "length": 4, "type": int, "struct": "i", "range": range(-2147483648, 2147483648)},
    "int8"  : {"id": 0x48, "name": "int8"  , "length": 1, "type": int, "struct": "b", "range": range(-128, 128)},
    "item"  : {"id": 0x12, "name": "item"  , "length": 2, "type": int, "struct": "H", "range": range(0, 65536)},
    "string": {"id": 0x80, "name": "string", "length": 0, "type": str, "struct": "s"},
    "uint16": {"id": 0x10, "name": "uint16", "length": 2, "type": int, "struct": "H", "range": range(0, 65536)},
    "uint32": {"id": 0x20, "name": "uint32", "length": 4, "type": int, "struct": "I", "range": range(0, 4294967296)},
    "uint8" : {"id": 0x08, "name": "uint8" , "length": 1, "type": int, "struct": "B", "range": range(0, 256)},
}

for _n in range(1, 128):
    ATTRTYPE[f'string({_n})'] = {"id": 0x80+_n, "name": f'string({_n})', "length": _n, "type": str, "struct": f'{_n}s'}

for _k, _d in tuple(ATTRTYPE.items()):
    ATTRTYPE[_d["id"]] = _d


def str2val(s, type_=None):
    if type_ is not None and ATTRTYPE[type_]["type"] is int:
        val = str2int(s, type_=type_)
    else:
        val = json.loads(s)
    return val


def str2int(s, type_=None):
    s = s.lower()
    if s.startswith("0b"):
        v = int(s, 2)
    elif s.startswith("0x"):
        v = int(s, 16)
    else:
        v = int(s)
    if type_ is not None:
        assert v in ATTRTYPE[type_]["range"]
    return v


ALIGN = 8
GROUPDEF_STRUCTFMT = "=HHL"
ATTRDEF_STRUCTFMT = "=HBBL"


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

    ba = bytearray() # this is the whole data which consist of:
    ba_header = bytearray()    # - header (padded to 32 bytes)
    ba_groupdef = bytearray()  # - group definitions (16 bytes each)
    ba_attrdef = bytearray()   # - group attribute definitions (8 bytes each; block paddded to 16 bytes)
    ba_attrvals = bytearray()  # - area of attribute values (area padded to 16 bytes)

    # objects used:
    group = None        # b'name'
    groupdef = None     # [b'name', itemcount:uint16, attrcount:uit16, firstitemlink:uint32, firstattrtablelink:uint32]
    groupdict = {}      #
    groupidx = -1       # Group ID
    groupinfo = {}      # {b'name': {"idx": groupidx, "def": groupdef}}
    attrdef = None      # [b'nam', attrtype:uint8, firstattrlink:uint32]
    attridx = -1        # attribute index
    attrtypeinfo = None # 
    vals = None         # [val1, val2, ...]
    
    mode = "SIG"  # loop control
 
    for li0 in frmstr.split("\n"):
        li = li0.partition(";")[0].strip() # trim comments
        if not li: continue
        # print(f'{mode:<3} <- {li}')
        if mode == "SIG":
            assert li.startswith("SIG ")
            _s = str(json.loads(li.split(" ")[1]))
            _b = _s.encode("ascii")
            assert len(_b) == 3
            ba_header.extend(_b)
            mode = "VER"
            continue
        if mode == "VER":
            assert li.startswith("VER ")
            _v = str2val(li.split(" ")[1], "uint8")
            ba_header.append(_v)
            mode = "G"
            continue
        if mode == "G":
            assert li.startswith("G ")
            groupidx += 1
            attridx = -1
            _, group, _items, _attrs = li.split(" ")
            group = group.encode("ascii")
            _items = str2int(_items)
            _attrs = str2int(_attrs)
            groupdef = [_items, _attrs, len(ba_attrdef)]
            _b = struct.pack(GROUPDEF_STRUCTFMT, *groupdef)
            _b += b'\x00' * (len(_b) % ALIGN)
            ba_groupdef.extend(_b)
            _d = {"groupidx": groupidx, "group": group, "groupdef": groupdef}
            groupdict[group] = groupdict[groupidx] = _d
            mode = ("A" if attridx + 1 < groupdef[1] else "G")
            continue
        if mode == "A":
            assert li.startswith("A ")
            attridx += 1
            _t = li.split(" ")
            _, _attr, _attrtype = _t[:3]
            _attr = _attr.encode("ascii")
            attrtypeinfo = ATTRTYPE[_attrtype]
            attrdefaddr = len(ba_attrvals)
            attrdef = [groupidx, attrtypeinfo["id"], attrtypeinfo["length"], len(ba_attrvals)]
            _b = struct.pack(ATTRDEF_STRUCTFMT, *attrdef)
            _b += b'\x00' * (len(_b) % ALIGN)
            ba_attrdef.extend(_b)
            vals = []
            mode = "A.."
            if not _t[3:]:
                continue
            else:
                li = " ".join(_t[3:]).lstrip() # ATTENTION! SHORTCUT!
        if mode == "A..":
            if attrtypeinfo["name"] == "group":
                _sg = [s.strip() for s in li.rstrip("*").rstrip(" ").rstrip(",").split(",")]
                _v = [groupdict[group.encode("ascii")]["groupidx"] for group in _sg] # TODO: forward ref capability
            elif attrtypeinfo["type"] is int:
                _sg = [s.strip() for s in li.rstrip("*").rstrip(" ").rstrip(",").split(",")]
                _v = [str2int(s, type_=attrtypeinfo["id"]) for s in _sg]
            else:
                _v = json.loads(f'[{li.rstrip("*").rstrip(" ").rstrip(",")}]')
            if attrtypeinfo["type"] is str:
                _v = [s.encode("utf-8") for s in _v]  # struct.error: argument for 's' must be a bytes object
            vals.extend(_v)
            if li.endswith("*"):
                vals.extend([vals[-1]] * max(0, groupdef[0] - len(vals)))
            assert len(vals) <= groupdef[0]
            if len(vals) < groupdef[0]:
                assert li.endswith(",")
                continue
            _structfmt = attrtypeinfo["struct"] * groupdef[0]
            _b = struct.pack(_structfmt, *vals)
            _b += b'\x00' * (len(_b) % ALIGN)
            ba_attrvals.extend(_b)
            mode = ("A" if attridx + 1 < groupdef[1] else "G")
            continue

    # at this point, len(ba_header) = 4
    _v = len(ba_header) + struct.calcsize("LLLLL") # calculate header size (_v = 24)
    _v += _v % ALIGN  # cosmetic

    ba_header.extend(struct.pack("L", 0))   # header absolute address
    ba_header.extend(struct.pack("L", _v))  # groupdef absolute address
    _v += len(ba_groupdef)
    ba_header.extend(struct.pack("L", _v))  # attrdef absolute address
    _v += len(ba_attrdef)
    ba_header.extend(struct.pack("L", _v))  # attrvals absolute address
    _v += len(ba_attrvals)
    ba_header.extend(struct.pack("L", _v))  # data size

    ba_header.extend(b'\x00' * (len(ba_header) % ALIGN))  # cosmetic

    ba.extend(ba_header)
    ba.extend(ba_groupdef)
    ba.extend(ba_attrdef)
    ba.extend(ba_attrvals)

    with frm_path.with_suffix(".ror").open("wb") as _f:
        print(f'{_f.write(ba)} bytes')

import json
import pathlib
import struct
import sys

ALIGN = 8
#GROUPDEF_STRUCTFMT = "=HHLL"
#ATTRDEF_STRUCTFMT = "=HHL"
GROUPDEF_STRUCTFMT = "=4sHHLL"  # debug version
ATTRDEF_STRUCTFMT = "=4s4sHHL"  # debug vesion

TEMPLATE = """#ifndef __DEFINE_RORHFILE__
#define __DEFINE_RORHFILE__

{{rorh}}

#endif  /* #ifndef __DEFINE_RORHFILE__ */
;"""

ATTRTYPE = {
    "attr"  : {"id": 0x0A02, "name": "attr"  , "rootname": "attr"  , "length": 2, "type": int  , "ctype": "uint16_t"       , "struct": "H", "range": range(0, 65536)},
    "bool"  : {"id": 0x0F11, "name": "bool"  , "rootname": "bool"  , "length": 1, "type": int  , "ctype": "bool"           , "struct": "?", "range": range(0, 2)},
    "group" : {"id": 0x0602, "name": "group" , "rootname": "group" , "length": 2, "type": int  , "ctype": "uint16_t"       , "struct": "H", "range": range(0, 65536)},
    "int16" : {"id": 0x0102, "name": "int16" , "rootname": "int16" , "length": 2, "type": int  , "ctype": "int16_t"        , "struct": "h", "range": range(-32768, 32768)},
    "int32" : {"id": 0x0104, "name": "int32" , "rootname": "int32" , "length": 4, "type": int  , "ctype": "int32_t"        , "struct": "i", "range": range(-2147483648, 2147483648)},
    "int8"  : {"id": 0x0101, "name": "int8"  , "rootname": "int8"  , "length": 1, "type": int  , "ctype": "int8_t"         , "struct": "b", "range": range(-128, 128)},
    "elem"  : {"id": 0x0E02, "name": "elem"  , "rootname": "elem"  , "length": 2, "type": int  , "ctype": "uint16_t"       , "struct": "H", "range": range(0, 65536)},
    "string": {"id": 0x4000, "name": "string", "rootname": "string", "length": 0, "type": str  , "ctype": "char *"         , "struct": "s"},
    "raw"   : {"id": 0x8000, "name": "raw"   , "rootname": "raw"   , "length": 0, "type": bytes, "ctype": "unsigned char *", "struct": None},
    "uint16": {"id": 0x0002, "name": "uint16", "rootname": "uint16", "length": 2, "type": int  , "ctype": "uint16_t"       , "struct": "H", "range": range(0, 65536)},
    "uint32": {"id": 0x0004, "name": "uint32", "rootname": "uint32", "length": 4, "type": int  , "ctype": "uint32_t"       , "struct": "I", "range": range(0, 4294967296)},
    "uint8" : {"id": 0x0001, "name": "uint8" , "rootname": "uint8" , "length": 1, "type": int  , "ctype": "uint8_t"        , "struct": "B", "range": range(0, 256)},
}

for _n in range(1, 256**2 // 4):
    ATTRTYPE[f'string({_n})'] = {"id": 0x4000+_n, "name": f'string({_n})', "rootname": "string", "length": _n, "type": str, "ctype": f"char[{_n}]", "struct": f'{_n}s'}

for _n in range(1, 256**2 // 2):
    ATTRTYPE[f'raw({_n})'] = {"id": 0x8000+_n, "name": f'raw({_n})', "rootname": "raw", "length": _n, "type": bytes, "ctype": f"unsigned char[{_n}]", "struct": None}

for _k, _d in tuple(ATTRTYPE.items()):
    ATTRTYPE[_d["id"]] = _d


def align_length(length, align=None):
    if align is None: align = ALIGN
    m = length % align
    return bool(m) * (align - m)


def align_bytes(b, align=None):
    return b'\x00' * align_length(len(b), align=align)


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
    ba_grouptoc = bytearray()  # - group definitions (16 bytes each)
    ba_attrtoc = bytearray()   # - group attribute definitions (8 bytes each; block paddded to 16 bytes)
    ba_attrvals = bytearray()  # - area of attribute values (area padded to 16 bytes)

    attrdict = {}
    attrlist = []
    groupdict = {}
    grouplist = []
    

    _L0 = (li.partition(";")[0].strip() for li in frmstr.split("\n"))
    _L = [li for li in _L0 if li]

    for i, li in enumerate(li for li in _L if li.startswith("|")):
        key, _, _s = li.partition(" ")
        if i == 0:
            assert key == "|SIG"
            _s = str(json.loads(li.split(" ")[1]))
            _b = _s.encode("ascii")
            assert len(_b) == 3
            ba_header.extend(_b)
        elif i == 1:
            assert key == "|VER"
            _v = str2val(li.split(" ")[1], "uint8")
            ba_header.append(_v)

    _i = 0
    while _i < len(_L):
        li = _L[_i]
        print(li)
        # TODO: global string :S with TOC
        # TODO: global raw bytes :B with TOC
        if li.startswith(":G "):
            _, _group = [s.strip() for s in li.split(" ")]
            _group = _group.encode("ascii")
            _groupidx = len(grouplist)
            _dg = {"groupidx": _groupidx, "group": _group, "elems": -1, "attrs": []}
            grouplist.append(_dg)
            groupdict[_group] = _dg
            if (_i+1 < len(_L) and _L[_i+1].startswith(":GELEMS ")):
                _dg["elems"] = str2val(_L[_i+1].split(" ")[1], "uint8")
                _i += 1
        elif li.startswith(":A "):
            assert _group is not None
            _t = li.split(" ")
            _, _attr, _attrtype = [s.strip() for s in _t[:3]]
            _attr = _attr.encode("ascii")
            _attrtypeinfo = ATTRTYPE[_attrtype]
            if _t[3:]:
                _s = " ".join(_t[3:]).lstrip()
            else:
                _s = ""
            while True:
                if (
                    len(_L) <= _i+1 
                    or _L[_i+1].startswith(":A") 
                    or _L[_i+1].startswith(":G")
                ):
                    break
                else:
                    assert not _s or _s.endswith(",")
                    _i += 1
                    _s += _L[_i]
            if _s[-1] == "*":
                _sstar = True
                _s = _s.rstrip("*").rstrip(" ").rstrip(",")
            else:
                _sstar = False
            print(f'{_s!r}')
            if _attrtypeinfo["name"] == "group":
                _v = [s.strip().encode("ascii") for s in _s.split(",")]
            elif _attrtypeinfo["type"] is int:
                _v = [str2int(s.strip(), type_=_attrtypeinfo["id"]) for s in _s.split(",")]
            elif _attrtypeinfo["rootname"] == "raw":
                _v = [bytes([int(s1, 16) for s1 in s0.split("\\x") if s1]) for s0 in _s.split(",")]
                assert all(len(b) == _attrtypeinfo["length"] for b in _v)
            else:
                _v = json.loads(f'[{_s}]')
            if _attrtypeinfo["type"] is str:
                _v = [s.encode("utf-8") for s in _v]  # struct.error: argument for 's' must be a bytes object
            _attridx = len(attrlist)
            _da = {"attridx": _attridx, "attr": _attr, "groupidx": _groupidx, "group": _group, "type": _attrtypeinfo["name"], "values": _v, "values_star": _sstar}
            attrlist.append(_da)
            attrdict[_group + b'.' + _attr] = _da
            _dg["attrs"].append(_da)
            if (_i+1 < len(_L) and _L[_i+1] == ":ANOPAD"):
                _da["nopad"] = True
                _i += 1
            #if _group == b'SENA' and _attr == b'NAME':
            #    raise Exception
        _i += 1


    _elem0idx = 0
    for _dg in grouplist:

        for i, _da in enumerate(_dg["attrs"]):
            _da["groupattridx"] = i

        _vcnts0 = [len(_da["values"]) for _da in _dg["attrs"] if not _da["values_star"]]
        assert not _vcnts0 or len(set(_vcnts0)) == 1
        _vcnts1 = [len(_da["values"]) for _da in _dg["attrs"] if _da["values_star"]]
        _vstar = [_da["values_star"] for _da in _dg["attrs"]]
        if _dg["elems"] == -1:
            if not _dg["attrs"]:
                _dg["elems"] = 0
            else:
                _dg["elems"] = _vcnts0[0]
        assert all(_v == _dg["elems"] for _v in _vcnts0)
        assert all(_v < _dg["elems"] for _v in _vcnts1)
        _dg["elem0idx"] = _elem0idx
        _elem0idx += _dg["elems"]


        #_grouptocitem = [_dg["elems"], len(_dg["attrs"]), _dg["elem0idx"], (0 if not _dg["attrs"] else _dg["attrs"][0]["attridx"])]
        _grouptocitem = [_dg["group"], _dg["elems"], len(_dg["attrs"]), _dg["elem0idx"], (0 if not _dg["attrs"] else _dg["attrs"][0]["attridx"])]  # debug version
        _dg["grouptocitem"] = _grouptocitem
        _b = struct.pack(GROUPDEF_STRUCTFMT, *_grouptocitem)
        _b += align_bytes(_b)
        _dg["grouptocitem_bytes"] = _b
        _dg["grouptocitem_offset"] = len(ba_grouptoc)
        ba_grouptoc.extend(_b)

    for _da in attrlist:
        if _da["type"] == "group":
            assert all(_group in groupdict for _group in _da["values"])
        _elems = groupdict[_da["group"]]["elems"]
        _da["values_final"] = list(_da["values"])  # copy
        if _da["values_star"]:
            _da["values_final"].extend([_da["values"][-1]] * (_elems - len(_da["values"])))
        _attrtypeinfo = ATTRTYPE[_da["type"]]
        if _attrtypeinfo["rootname"] == "group":
            _structfmt = _attrtypeinfo["struct"] * _elems
            _b = struct.pack(_structfmt, *[groupdict[_group]["groupidx"] for _group in _da["values_final"]])
        elif _attrtypeinfo["rootname"] == "raw":
            _b = b''.join(_da["values_final"])
        else:
            _structfmt = _attrtypeinfo["struct"] * _elems
            _b = struct.pack(_structfmt, *_da["values_final"])
        if not _da.get("nopad", False):
            _b += align_bytes(_b)
        _da["values_bytes"] = _b
        _da["values_offset"] = len(ba_attrvals)
        ba_attrvals.extend(_b)

        # _attrtocitem = [_da["groupidx"], _attrtypeinfo["id"], _da["values_offset"]]
        _attrtocitem = [_da["group"], _da["attr"], _da["groupidx"], _attrtypeinfo["id"], _da["values_offset"]]  # debug version
        _da["attrtocitem"] = _attrtocitem
        _b = struct.pack(ATTRDEF_STRUCTFMT, *_attrtocitem)
        _b += align_bytes(_b)
        _da["attrtocitem_bytes"] = _b
        _da["attrtocitem_offset"] = len(ba_attrtoc)
        ba_attrtoc.extend(_b)


    # at this point, len(ba_header) = 4
    _v = len(ba_header) + struct.calcsize("LLLLL") # calculate header size (_v = 24)
    _v += align_length(_v)  # cosmetic

    ba_header.extend(struct.pack("=L", _elem0idx))   # elem count
    ba_header.extend(struct.pack("=L", len(attrlist)))   # elem count
    ba_header.extend(struct.pack("=L", _v))  # grouptoc absolute address
    _v += len(ba_grouptoc)
    ba_header.extend(struct.pack("=L", _v))  # attrtoc absolute address
    _v += len(ba_attrtoc)
    ba_header.extend(struct.pack("=L", _v))  # attrvals absolute address

    ba_header.extend(align_bytes(ba_header))  # cosmetic

    ba.extend(ba_header)
    ba.extend(ba_grouptoc)
    ba.extend(ba_attrtoc)
    ba.extend(ba_attrvals)

    _file = frm_path.with_suffix(".ror")
    with _file.open("wb") as _f:
        print(f'{_file}: {_f.write(ba)} bytes')

    rorh_prefix = """
typedef struct _Header Header;
typedef struct _Group Group;
typedef struct _Attr Attr;

#define __INITRORAPI__    \\
                          \\
struct _Header {          \\
   char     sig[3];       \\
   uint8_t  ver;          \\
   uint32_t elems;        \\
   uint32_t attrs;        \\
   uint32_t grouptocaddr; \\
   uint32_t attrtocaddr;  \\
   uint32_t attrvalsaddr; \\
};                        \\
                          \\
                          \\
struct _Group {           \\
   char     group[4];     \\
   uint16_t elems;        \\
   uint16_t attrs;        \\
   uint32_t elem0idx;     \\
   uint32_t attr0idx;     \\
};                        \\
                          \\
                          \\
struct _Attr {            \\
   char     group[4];     \\
   char     attr[4];      \\
   uint16_t groupidx;     \\
   uint16_t type;         \\
   uint32_t addr;         \\
};
"""

    rorh_lines = [
        "",
        "#define header(rordata)  (*(Header*)(rordata))",
        "#define group(rordata, G)  (*((Group*)(rordata+header(rordata).grouptocaddr) + G))",
        "#define attr(rordata, G, A)  (*((Attr*)(rordata+header(rordata).attrtocaddr) + (group(rordata, G).attr0idx) + A))",
        "#define val0reladdr(rordata, G, A)  (header(rordata).attrvalsaddr + attr(rordata, G, A).addr)",
        "#define val0absaddr(rordata, G, A)  (rordata+val0reladdr(rordata, G, A))",
        "#define valsize(rordata, G, A)  (attr(rordata, G, A).type & 0x00FF)",  # TODO: fix that hacky that 0x00FF, for strings it should be 0x3FFF etc
        "#define valreladdr(rordata, G, A, i)  (header(rordata).attrvalsaddr + attr(rordata, G, A).addr) + (i*valsize(rordata, G, A))",
        "#define valabsaddr(rordata, G, A, i)  (rordata+valreladdr(rordata, G, A, i))",
        "",
        f'#define SIZE {len(ba)}',
        "",
    ]

    for _dg in grouplist:
        rorh_lines.append(f'#define G_{_dg["group"].decode("ascii")} {_dg["groupidx"]}',)
        for _da in _dg["attrs"]:
            rorh_lines.append(f'#define A_{_da["group"].decode("ascii")}_{_da["attr"].decode("ascii")} {_da["groupattridx"]}',)
            rorh_lines.append(f'#define A_{_da["group"].decode("ascii")}_{_da["attr"].decode("ascii")}_t {ATTRTYPE[_da["type"]]["ctype"]}',)

    rorh_lines.append("")
    for _k, _d in ATTRTYPE.items():
        if not isinstance(_k, str): continue
        if ATTRTYPE["string"]["id"] <= _d["id"]: continue
        rorh_lines.append(f'#define T_{_k.upper()} 0x{_d["id"]:0>4X}')

    rorh_lines.extend([
        "",
        "#define FACT_ELEMCOUNT 7",
        "",
        "#define PHSE_PREP 0",
        "",
        "#define SPHS_PREP_TAKEFACTIONS 201200",
        "#define SPHS_PREP_DEALSENATORS 301420",
        "#define SPHS_PREP_DEALSENATORS_RANDOM_ENTROPYREQ 301421",
        "#define SPHS_PREP_TEMPORARYROMECONSUL 301600",
        "#define SPHS_PREP_SELECTFACTIONLEADERS 301700",
    ])

    rorhstr = TEMPLATE.replace("{{rorh}}", rorh_prefix + "\n" +"\n".join(rorh_lines))

    _file = frm_path.parent / "rorfile.h"
    with _file.open("w", encoding="utf-8") as _f:
        print(f'{_file}: {_f.write(rorhstr)} bytes')

import json
import pathlib
import struct
import sys

RORH_TEMPLATE = """#ifndef __DEFINE_RORFILEH__
#define __DEFINE_RORFILEH__

{{rorh}}

#endif  /* #ifndef __DEFINE_RORFILEH__ */
;"""


INT_TYPES = (
    "uint8_t",
    "uint16_t",
)


def str2val(s, type_=None):
    if type_ in INT_TYPES:
        val = str2int(s)
    else:
        val = json.loads(s)
    return val


def str2int(s):
    s = s.lower()
    if s.startswith("0b"):
        v = int(s, 2)
    elif s.startswith("0x"):
        v = int(s, 16)
    else:
        v = int(s)
    return v


def frmparse(frmstr):
    mode = ("0",)
    d = {"ANCHOR": {}}
    i = 0
    L = frmstr.split("\n")
    while i < len(L):
        sline = L[i].partition(";")[0].strip() # trim comments
        if not sline:
            i += 1
            continue
        if mode[0] == "0":
            if sline.startswith("FILESIGNATURE"):
                d["FILESIGNATURE"] = str(json.loads(sline.split(" ")[1]))
            elif sline.startswith("FILEVERSION"):
                _, type_, valstr = sline.split(" ")
                val = str2val(valstr, type_)
                d["FILEVERSIONTYPE"] = type_
                d["FILEVERSION"] = val
            elif sline.startswith("ADDRTYPE"):
                _, d["ADDRTYPE"] = sline.split(" ")
            elif sline.startswith("ALIGN"):
                _, valstr = sline.split(" ")
                val = str2int(valstr)
                d["ALIGN"] = val
            elif sline.startswith("ANCHOR"):
                _, anchor = sline.split(" ")
                danchor = d["ANCHOR"][anchor] = {"VAR": {}}
                mode = ("ANCHOR", anchor)
        elif mode[0] == "ANCHOR":
            if sline.startswith("ITEM"):
                _, valstr = sline.split()
                val = str2int(valstr)
                danchor["ITEM"] = val
            elif sline.startswith("VAR"):
                _t = sline.split(" ")
                _, var, type_ = _t[:3]
                dval = danchor["VAR"][var] = {"TYPE": type_, "VAL": None}
                if _t[3:]:
                    valstr = " ".join(_t[3:]).lstrip()
                    val = json.loads(f'[{valstr.rstrip(",")}]')
                    assert type(val) is list
                    dval["VAL"] = val
                    if valstr.endswith(","):
                        mode = ("VAR", anchor, var)
                    else:
                        if dval["VAL"][-1] == "*":
                            lastval = dval["VAL"][-2]
                            dval["VAL"].extend([lastval] * (danchor["ITEM"] - len(dval["VAL"])))
                        assert len(dval["VAL"]) == danchor["ITEM"]
                        mode = ("ANCHOR", anchor)
                else:
                    mode = ("VAR", anchor, var)
#           elif sline.startswith("ADDR"):
#               _t = sline.split(" ")
#               daddr = danchor["ADDR"] = None
        i += 1
    return d


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



    mode = ("0",)
    d = {"ANCHOR": {}}
    i = 0
    L = frmstr.split("\n")
    while i < len(L):
        sline = L[i].partition(";")[0].strip() # trim comments
        print([i, mode, sline])
        if not sline:
            i += 1
            continue
        if mode[0] == "0":
            if sline.startswith("FILESIGNATURE"):
                d["FILESIGNATURE"] = str(json.loads(sline.split(" ")[1]))
            elif sline.startswith("FILEVERSION"):
                _, type_, valstr = sline.split(" ")
                val = str2val(valstr, type_)
                d["FILEVERSIONTYPE"] = type_
                d["FILEVERSION"] = val
            elif sline.startswith("ADDRTYPE"):
                _, d["ADDRTYPE"] = sline.split(" ")
            elif sline.startswith("ALIGN"):
                _, valstr = sline.split(" ")
                val = str2int(valstr)
                d["ALIGN"] = val
            elif sline.startswith("ANCHOR"):
                _, anchor = sline.split(" ")
                danchor = d["ANCHOR"][anchor] = {"VAR": {}}
                mode = ("ANCHOR", anchor)
        elif mode[0] == "ANCHOR":
            if sline.startswith("ITEM"):
                _, valstr = sline.split(" ")
                val = str2int(valstr)
                danchor["ITEM"] = val
            elif sline.startswith("VAR"):
                _t = sline.split(" ")
                _, var, type_ = _t[:3]
                dval = danchor["VAR"][var] = {"TYPE": type_, "VAL": None}
                if _t[3:]:
                    valstr = " ".join(_t[3:]).lstrip()
                    val = json.loads(f'[{valstr.rstrip(",")}]')
                    assert type(val) is list
                    dval["VAL"] = val
                    if valstr.endswith(","):
                        mode = ("VAR", anchor, var)
                    else:
                        if dval["VAL"][-1] == "*":
                            lastval = dval["VAL"][-2]
                            dval["VAL"].extend([lastval] * (danchor["ITEM"] - len(dval["VAL"])))
                        assert len(dval["VAL"]) == danchor["ITEM"]
                        mode = ("ANCHOR", anchor)
                else:
                    dval["VAL"] = []
                    mode = ("VAR", anchor, var)
#           elif sline.startswith("ADDR"):
#               _t = sline.split(" ")
#               daddr = danchor["ADDR"] = None
            elif sline.startswith("ANCHOR"):
                mode = ("0",)
                continue
        elif mode[0] == "VAR":
            valstr = sline.strip()
            val = json.loads(f'[{valstr.rstrip(",")}]')
            assert type(val) is list
            dval["VAL"].extend(val)
            if not valstr.endswith(","):
                if dval["VAL"][-1] == "*":
                    lastval = dval["VAL"][-2]
                    dval["VAL"].extend([lastval] * (danchor["ITEM"] - len(dval["VAL"])))
                assert len(dval["VAL"]) == danchor["ITEM"]
                mode = ("ANCHOR", anchor)
        i += 1

    D = d
    #D = frmparse(frmstr)

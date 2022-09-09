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
    if type_ == "uint8_t":
        assert v in range(0, 256)
    elif type_ == "uint16_t":
        assert v in range(0, 65536)
    return v

ALIGN = 4

GROUPDEF_STRUCTFMT = "=HHL"
ATTRDEF_STRUCTFMT = "=BBHL"


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
    group = None       # b'name'
    groupdef = None    # [b'name', itemcount:uint16, attrcount:uit16, firstitemlink:uint32, firstattrtablelink:uint32]
    groupidx = 0       # Group ID
    groupinfo = {}     # {b'name': {"idx": groupidx, "def": groupdef}}
    attrdef = None     # [b'nam', attrtype:uint8, firstattrlink:uint32]
    attridx = 0        # attribute index
    vals = None        # [val1, val2, ...]
    
    mode = ("FILESIGNATURE",)  # loop control
 
    for li0 in frmstr.split("\n"):
        li = li0.partition(";")[0].strip() # trim comments
        print([i, mode, li])
        if not li:
            i += 1
            continue
        if mode[0] == "FILESIGNATURE":
            assert li.startswith("FILESIGNATURE ")
            _s = str(json.loads(li.split(" ")[1]))
            _b = _s.encode("ascii")
            assert len(_b) == 3
            ba_header.extend(_b)
            mode = ("FILEVERSION",)
            continue
        if mode[0] == "FILEVERSION":
            assert li.startswith("FILEVERSION ")
            _v = str2val(li.split(" ")[1], "uint8_t")
            ba_header.append(_v)
            mode = ("NEWGROUP",)
            continue
        if mode[0] == "NEWGROUP":
            assert li.startswith("NEWGROUP ")
            _, group, _items, _attrs = li.split(" ")
            group = group.encode("ascii")
            _items = str2int(_items)
            _attrs = str2int(_attrs)
            groupdef = [_items, _attrs, len(ba_attrvals)]
            _b = struct.pack(GROUPDEF_STRUCTFMT, *groupdef)
            ba_groupdef.extend(_b)
            groupidx += 1
            attridx = 0
            mode = ("ITEMS", group)
            continue
        if mode[0] == "ITEMS":
            assert li.startswith("ITEMS ")
            assert mode[1] == groupdef[0]
            _v = str2int(li.split(" ")[1])
            groupdef[1] = _v
            mode = ("ATTRS", group)
            continue
        if mode[0] == "ATTRS":
            assert li.startswith("ATTRS ")
            assert mode[1] == groupdef[0]
            _v = str2int(li.split(" ")[1])
            groupdef[2] = _v
            _b = struct.pack(GROUPDEF_STRUCTFMT, *groupdef)
            ba_groupdef.extend(_b)
            attridx = 0
            mode = ("ATTR", group)
            continue
        if mode[0] == "ATTR":
            assert li.startswith("ATTR ")
            _t = li.split(" ")
            _, _attr, _attrtype = _t[:3]
            _attr = _attr.encode("ascii")
            assert len(_attr) == 3
            attrdef = [_attr, _attrtype, len(ba_attrvals)]
            _b = struct.pack(ATTRDEF_STRUCTFMT, *attrdef)
            ba_attrdef.extend(_b)
            vals = []
            mode = ("ATTRVALS", group, _attr)
            if not _t[3:]:
                continue
            else:
                li = " ".join(_t[3:]).lstrip() # ATTENTION! SHORTCUT!
        if mode[0] == "ATTRVALS":
            _v = json.loads(f'[{li.rstrip(",")}]')
            vals.extend(_v)
            assert len(vals) <= groupdef[1]
            if len(vals) < groupdef[1]:
                assert li.endswith(",")
                continue
            elif len(vals) == groupdef[1]:










                elif li.endswith("*"):
                    vals = vals[:-1]
                    vals.extend([vals[-1]] * (groupdef[1] - len(vals)))


                continue







            attrtableitem = 

            assert mode[1] == groupdef[0]
            assert li.startswith("ATTR ") or li.startswith("LINK ")
            if li.startswith("ATTR "):


                raise Exception

                dval = dgroup["ATTR"][attr] = {"TYPE": type_, "VAL": None}
                if _t[3:]:
                    valstr = " ".join(_t[3:]).lstrip()
                    val = json.loads(f'[{valstr.rstrip(",")}]')
                    assert type(val) is list
                    dval["VAL"] = val
                    if valstr.endswith(","):
                        mode = ("ATTR", group, attr)
                    else:
                        if dval["VAL"][-1] == "*":
                            lastval = dval["VAL"][-2]
                            dval["VAL"].extend([lastval] * (dgroup["ITEMS"] - len(dval["VAL"])))
                        assert len(dval["VAL"]) == dgroup["ITEMS"]
                        mode = ("NEWGROUP", group)
                else:
                    dval["VAL"] = []
                    mode = ("ATTR", group, attr)
            elif li.startswith("LINK "):
                # done with ATTRs, groupdef is done
                _b = struct.pack(GROUPDEF_STRUCTFMT, *groupdef)
                groupdeftbl.extend(_b)





                raise Exception



                links = group, [], bytearray()
                for s in li[len("LINK "):].split(","):
                    _t = s.strip().split(".")
                    if len(_t) == 1:
                        _t += ["1"]
                    t[1] = str2int(t[1])
                    links[1].append(_t)
                if links[1] == groupdef[1]:

                    mode = ("0",)

        

        i += 1


#            if li.startswith("FILESIGNATURE"):
#                d["FILESIGNATURE"] = str(json.loads(li.split(" ")[1]))
#            elif li.startswith("FILEVERSION"):
#                _, type_, valstr = li.split(" ")
#                val = str2val(valstr, type_)
#                d["FILEVERSIONTYPE"] = type_
#                d["FILEVERSION"] = val
#            elif li.startswith("LINKTYPE"):
#                _, d["LINKTYPE"] = li.split(" ")
#            elif li.startswith("ALIGN"):
#                _, valstr = li.split(" ")
#                val = str2int(valstr)
#                d["ALIGN"] = val
#            elif li.startswith("NEWGROUP"):
#                _, group = li.split(" ")
#                dgroup = d["NEWGROUP"][group] = {"ATTR": {}}
#                mode = ("NEWGROUP", group)
#        elif mode[0] == "NEWGROUP":
#            if li.startswith("ITEMS"):
#                _, valstr = li.split(" ")
#                val = str2int(valstr)
#                dgroup["ITEMS"] = val
#            elif li.startswith("ATTR"):
#                _t = li.split(" ")
#                _, attr, type_ = _t[:3]
#                dval = dgroup["ATTR"][attr] = {"TYPE": type_, "VAL": None}
#                if _t[3:]:
#                    valstr = " ".join(_t[3:]).lstrip()
#                    val = json.loads(f'[{valstr.rstrip(",")}]')
#                    assert type(val) is list
#                    dval["VAL"] = val
#                    if valstr.endswith(","):
#                        mode = ("ATTR", group, attr)
#                    else:
#                        if dval["VAL"][-1] == "*":
#                            lastval = dval["VAL"][-2]
#                            dval["VAL"].extend([lastval] * (dgroup["ITEMS"] - len(dval["VAL"])))
#                        assert len(dval["VAL"]) == dgroup["ITEMS"]
#                        mode = ("NEWGROUP", group)
#                else:
#                    dval["VAL"] = []
#                    mode = ("ATTR", group, attr)
##           elif li.startswith("LINK"):
##               _t = li.split(" ")
##               dlink = dgroup["LINK"] = None
#            elif li.startswith("NEWGROUP"):
#                mode = ("0",)
#                continue
#        elif mode[0] == "ATTR":
#            valstr = li.strip()
#            val = json.loads(f'[{valstr.rstrip(",")}]')
#            assert type(val) is list
#            dval["VAL"].extend(val)
#            if not valstr.endswith(","):
#                if dval["VAL"][-1] == "*":
#                    lastval = dval["VAL"][-2]
#                    dval["VAL"].extend([lastval] * (dgroup["ITEMS"] - len(dval["VAL"])))
#                assert len(dval["VAL"]) == dgroup["ITEMS"]
#                mode = ("NEWGROUP", group)
#        i += 1
#
#    D = d
#    #D = frmparse(frmstr)

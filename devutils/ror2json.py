import copy
import json
import pathlib
import struct
import sys

default_dump_kwargs = {
    "skipkeys": False,
    "ensure_ascii": False,  # +human readable (UTF)
    "check_circular": True,
    "allow_nan": True,
    "cls": None,
    "indent": "\t",  # +human readable
    "separators": None,
    "default": None,
    "sort_keys": False,  # keep order
}

ORDER = [
    "Header",
    "Temp",
    "ItemTypeInfo",
    "EraItem",
    "FactionItem",
    "LocationItem",
    "DeckItem",
    "OfficeItem",
    "MagistrateItem",
    "SenatorItem",
    "StatesmanItem",
]

TEMPSIZE = 12

def structprep(lis):
    return [(element.encode("utf8") if (type(element) is str) else element) for element in lis]

def unpack2(format_obj, buffer):
    flatformat = ">" + "".join(flat(format_obj))
    size = struct.calcsize(flatformat)
    A0 = struct.unpack(flatformat, buffer[:size])
    L1 = []
    for element in A0:
        if (type(element) is bytes):
            try:
                v = element.decode("utf8").split('\x00', 1)[0]
            except UnicodeDecodeError:
                v = list(element)
        else:
            v = element
        L1.append(v)
    L2 = []
    F_stack = [[format_obj, 0]]
    L_stack = [[L2, 0]]
    i = 0
    while True:
        fo, fi = F_stack[-1]
        lo, li = L_stack[-1]
        if (len(fo) == fi):
            F_stack = F_stack[:-1]
            L_stack = L_stack[:-1]
            if F_stack:
                F_stack[-1][-1] += 1
                L_stack[-1][-1] += 1
            else:
                break
            continue
        fe = fo[fi]
        if (type(fe) is list):
            F_stack.append([fe, 0])
            le = []
            lo.append(le)
            L_stack.append([le, 0])
            continue
        lo.append(L1[i])
        i += 1
        F_stack[-1][-1] += 1
        L_stack[-1][-1] += 1
        if len(L1) == i:
            break
    return size, L2


def flat(lis):
    flatList = []
    # Iterate with outer list
    for element in lis:
        if type(element) is list:
            # Check if type is list than iterate through the sublist
            for item in element:
                flatList.append(item)
        else:
            flatList.append(element)
    return flatList

if __name__ == "__main__":
    pf = pathlib.Path(".") / "ror-001.json"
    with pf.open("r", encoding="utf8") as _f:
        F = json.load(_f)
    pr = pathlib.Path(sys.argv[1])
    with pr.open("rb") as _f:
        b = _f.read()
    J = {}
    i = 0
    size, J["Header"] = unpack2(F["Header"], b[i:])
    i += size
    size, J["Temp"] = unpack2(["B"] * TEMPSIZE, b[i:])
    i += size
    itemcount = sum((1 if k.endswith("Item") else 0) for k in F)
    size, J["ItemTypeInfo"] = unpack2([F["ItemTypeInfo"]] * itemcount, b[i:])
    i += size
    for j, k in enumerate([k for k in ORDER if k.endswith("Item")]):
        #print(k)
        J[k] = []
        for m in range(J["ItemTypeInfo"][j][0]):
            size, L = unpack2(F[k], b[i:])
            J[k].append(L)
            i += size
    with pr.with_suffix(".ror.json").open("w", encoding="utf8") as _f:
        json.dump(J, _f, **default_dump_kwargs)

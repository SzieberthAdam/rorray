import copy
import json
import pathlib
import struct

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
    "ConcessionItem",
    "IntrigueItem",
]

TEMPSIZE = 12

def structprep(lis):
    return [(element.encode("utf8") if (type(element) is str) else element) for element in lis]


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
    for p0 in pathlib.Path(".").glob("*.json"):
        if p0.stem.startswith("ror-"): continue
        p1 = p0.resolve().parent.parent / f'{p0.stem}.ror'
        with p0.open("r", encoding="utf8") as _f:
            O0 = json.load(_f)
        O1 = copy.deepcopy(O0)
        itemcount = sum((1 if k.endswith("Item") else 0) for k in F)
        O1["ItemTypeInfo"] = [[0, 0] for _ in range(itemcount)]
        B = {}
        B["Header"] = struct.pack(">" + "".join(flat(F["Header"])), *structprep(flat(O1["Header"])))
        B["Temp"] = b'\0' * TEMPSIZE
        B["ItemTypeInfo"] = b''.join(struct.pack(">" + "".join(flat(F["ItemTypeInfo"])), *structprep(flat(lis))) for lis in O1["ItemTypeInfo"])  # temporary
        adr0 = O1["ItemTypeInfo"][0][1] = len(B["Header"]) + len(B["Temp"]) + len(B["ItemTypeInfo"])
        for i, k in enumerate([k for k in ORDER if k.endswith("Item")]):
            _cnt = O1["ItemTypeInfo"][i][0] = len(O1[k])
            B[k] = b''
            for j in range(_cnt):
                B[k] += struct.pack(">" + "".join(flat(F[k])), *structprep(flat(O1[k][j])))
            if i + 1 < itemcount:
                O1["ItemTypeInfo"][i+1][1] = O1["ItemTypeInfo"][i][1] + len(B[k])
        B["ItemTypeInfo"] = b''.join(struct.pack(">" + "".join(flat(F["ItemTypeInfo"])), *structprep(flat(lis))) for lis in O1["ItemTypeInfo"])
        b = b''.join(B[k] for k in ORDER)
        with p1.open("wb") as _f:
            _f.write(b)

import pathlib
import sys


def txtstr2rrsbytes(txtstr):
    b1 = txtstr.replace("$\n", "$").encode("utf-8")
    b2 = b1.replace(b"$", b"\0")
    return b2


if __name__ == "__main__":

    usage = """Usage: python txt2rrs.py [scenario.txt]"""

    if len(sys.argv) == 1:
        txt_path = pathlib.Path("scenario.txt")
    else:
        try:
            txt_path = pathlib.Path(sys.argv[1])
        except (IndexError, ValueError):
            print(usage)
            sys.exit(1)

    with txt_path.open("r", encoding="utf-8") as _f:
        txtstr = _f.read()

    rrsbytes = txtstr2rrsbytes(txtstr)

    with txt_path.with_suffix(".rrs").open("wb") as _f:
        c = _f.write(rrsbytes)

    assert c == len(rrsbytes)

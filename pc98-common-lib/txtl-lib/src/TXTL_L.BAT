DEL *.OBJ
TCC -ml -X+ -k- -r+ -O -G- -Z- -c -d+ -v- @TXTL.FIL
DEL TXTL_L.LIB
TLIB /c /e TXTL_L.LIB @TXTL.RSP , NUL

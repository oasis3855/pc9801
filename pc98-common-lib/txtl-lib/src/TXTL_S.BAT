DEL *.OBJ
TCC -ms -X+ -k- -r+ -O -G- -Z- -c -d+ -v- @TXTL.FIL
DEL TXTL_S.LIB
TLIB /c /e TXTL_S.LIB @TXTL.RSP , NUL

DEL *.OBJ
TCC -ms -X+ -k- -r+ -O -G+ -Z- -c -d+ -v- @HDCPY.FIL
DEL HDCPY_S.LIB
TLIB /c /e HDCPY_S.LIB @HDCPY.RSP , NUL

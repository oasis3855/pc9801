DEL *.OBJ
TCC -ml -X+ -k- -r+ -O -G+ -Z- -c -d+ -v- @HDCPY.FIL
DEL HDCPY_L.LIB
TLIB /c /e HDCPY_L.LIB @HDCPY.RSP , NUL

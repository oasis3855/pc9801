DEL *.OBJ
TCC -ml -X+ -k- -r+ -O -G- -Z- -c -d+ -v- @WINP.FIL
DEL WINP_L.LIB
TLIB /c /e WINP_L.LIB @WINP.RSP , NUL

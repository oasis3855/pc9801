DEL *.OBJ
TCC -ms -X+ -k- -r+ -O -G- -Z- -c -d+ -v- @WINP.FIL
DEL WINP_S.LIB
TLIB /c /e WINP_S.LIB @WINP.RSP , NUL

DEL *.OBJ
TCC -ms -X+ -k- -r+ -O -G- -Z- -c -d+ -v- @ETC.FIL
DEL ETC_S.LIB
TLIB /c /e ETC_S.LIB @ETC.RSP , NUL

DEL *.OBJ
TCC -ml -X+ -k- -r+ -O -G- -Z- -c -d+ -v- @ETC.FIL
DEL ETC_L.LIB
TLIB /c /e ETC_L.LIB @ETC.RSP , NUL

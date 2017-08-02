
@ECHO OFF

DEL ".\OBJ\%1.B*"

FOR %%X IN (00) DO hex2bin32 -l 0x10000 -e X%%X ".\OBJ\%1.H%%X"
MD BIN
COPY/B ".\OBJ\%1.X*" ".\BIN\%2"
DEL ".\OBJ\%1.X*"
DEL ".\OBJ\%1.H*"

Checksum ".\BIN\%2"

set yyyymmdd_hhmmss=%date:~0,4%%date:~5,2%%date:~8,2%_%time:~0,2%%time:~3,2%%time:~6,2%

echo %yyyymmdd_hhmmss%
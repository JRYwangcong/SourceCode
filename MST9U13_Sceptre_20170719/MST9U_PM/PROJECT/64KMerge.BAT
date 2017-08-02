@echo off
HEX2BIN /Q /L65536 obj\TSUM.H00 obj\TSUM56.BIN
MD BIN
DEL /Q/S BIN\*.*
cd obj
COPY "TSUM56.BIN" %1
DEL "TSUM56.*"
copy %1 ..\bin
del %1
cd..
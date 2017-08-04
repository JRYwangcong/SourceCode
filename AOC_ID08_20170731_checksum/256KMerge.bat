@echo off


HEX2BIN /Q /L262144 obj\TSUM.H00 obj\TSUM56.BIN
HEX2BIN /Q /O65536 /M obj\TSUM.H01 obj\TSUM56.BIN
HEX2BIN /Q /O131072 /M obj\TSUM.H02 obj\TSUM56.BIN
HEX2BIN /Q /O196608 /M obj\TSUM.H03 obj\TSUM56.BIN

IF NOT EXIST bin md bin
copy Checksum.exe bin\Checksum.exe
cd obj
COPY "TSUM56.BIN" %1.bin
DEL "TSUM56.*"
copy %1.bin ..\bin
del %1.bin
cd..
cd bin
if exist %1-*.bin del %1-*.bin
Checksum %1.bin /b
del Checksum.exe
Pause
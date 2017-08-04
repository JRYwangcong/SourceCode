@echo off
HEX2BIN /Q /L262144 obj\%1.H00 obj\%1.BIN
HEX2BIN /Q /O65536 /M obj\%1.H01 obj\%1.BIN
HEX2BIN /Q /O131072 /M obj\%1.H02 obj\%1.BIN
HEX2BIN /Q /O196608 /M obj\%1.H03 obj\%1.BIN
Checksum_256 Obj\%1.BIN Obj\%2.BIN

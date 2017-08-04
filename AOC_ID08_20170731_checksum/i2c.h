extern void i2c_WriteTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count);
extern void i2c_ReadTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count);

//#if Enable_Lightsensor
extern void  LightSensor_On(void);
extern void  LightSensor_Off(void);
extern void LightSensorHandler(void);
//#endif
//#if  IT6251
 #define DP_I2C_ADDR 0xB8
  #define LVDS_I2C_ADDR 0xBC

 BYTE  ReadI2C_Byte(BYTE addr, BYTE offset);
 BYTE  WriteI2C_Byte(BYTE addr, BYTE offset, BYTE d);
 BYTE i2c_write( BYTE address, BYTE offset, BYTE byteno, BYTE dataIn );
 BYTE i2c_read( BYTE address, BYTE offset, BYTE byteno, BYTE* rddata );
 BYTE i2c_read_byte( BYTE address, BYTE offset, BYTE byteno, BYTE *p_data, BYTE device );
 BYTE i2c_write_byte( BYTE address, BYTE offset, BYTE byteno, BYTE *p_data, BYTE device );
//#endif 


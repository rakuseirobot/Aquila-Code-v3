/*
 * gyro_control.cpp
 *
 * Created: 2018/08/28 11:18:39
 *  Author: shun2
 */ 

#include "gyro_control.hpp"

float xAccl=0.00;
float yAccl=0.00;
float zAccl=0.00;
float xGyro=0.00;
float yGyro=0.00;
float zGyro=0.00;
int xMag=0;
int yMag=0;
int zMag=0;


void init_bmx055(void){
	twi bmx(&TWIC,bmx_fre);

	//------------------------------------------------------------//
	bmx.Address(Addr_Accl,0);
	bmx.WriteSingle(0x0F); // Select PMU_Range register
	bmx.WriteSingle(0x03);   // Range = +/- 2g
	bmx.Stop();
	_delay_ms(100);
	//------------------------------------------------------------//
	bmx.Address(Addr_Accl,0);
	bmx.WriteSingle(0x10);  // Select PMU_BW register
	bmx.WriteSingle(0x08);  // Bandwidth = 7.81 Hz
	bmx.Stop();
	_delay_ms(100);
	//------------------------------------------------------------//
	bmx.Address(Addr_Accl,0);
	bmx.WriteSingle(0x11);  // Select PMU_LPW register
	bmx.WriteSingle(0x00);  // Normal mode, Sleep duration = 0.5ms
	bmx.Stop();
	_delay_ms(100);
	//------------------------------------------------------------//
	bmx.Address(Addr_Gyro,0);
	bmx.WriteSingle(0x0F);  // Select Range register
	bmx.WriteSingle(0x04);  // Full scale = +/- 125 degree/s
	bmx.Stop();
	_delay_ms(100);
	//------------------------------------------------------------//
	bmx.Address(Addr_Gyro,0);
	bmx.WriteSingle(0x10);  // Select Bandwidth register
	bmx.WriteSingle(0x07);  // ODR = 100 Hz
	bmx.Stop();
	_delay_ms(100);
	//------------------------------------------------------------//
	bmx.Address(Addr_Gyro,0);
	bmx.WriteSingle(0x11);  // Select LPM1 register
	bmx.WriteSingle(0x00);  // Normal mode, Sleep duration = 2ms
	bmx.Stop();
	_delay_ms(100);
	//------------------------------------------------------------//
	bmx.Address(Addr_Mag,0);
	bmx.WriteSingle(0x4B);  // Select Mag register
	bmx.WriteSingle(0x83);  // Soft reset
	bmx.Stop();
	_delay_ms(100);
	//------------------------------------------------------------//
	bmx.Address(Addr_Mag,0);
	bmx.WriteSingle(0x4B);  // Select Mag register
	bmx.WriteSingle(0x01);  // Soft reset
	bmx.Stop();
	_delay_ms(100);
	//------------------------------------------------------------//
	bmx.Address(Addr_Mag,0);
	bmx.WriteSingle(0x4C);  // Select Mag register
	bmx.WriteSingle(0x00);  // Normal Mode, ODR = 10 Hz
	bmx.Stop();
	//------------------------------------------------------------//
	bmx.Address(Addr_Mag,0);
	bmx.WriteSingle(0x4E);  // Select Mag register
	bmx.WriteSingle(0x84);  // X, Y, Z-Axis enabled
	bmx.Stop();
	//------------------------------------------------------------//
	bmx.Address(Addr_Mag,0);
	bmx.WriteSingle(0x51);  // Select Mag register
	bmx.WriteSingle(0x04);  // No. of Repetitions for X-Y Axis = 9
	bmx.Stop();
	//------------------------------------------------------------//
	bmx.Address(Addr_Mag,0);
	bmx.WriteSingle(0x52);  // Select Mag register
	bmx.WriteSingle(0x16);  // No. of Repetitions for Z-Axis = 15
	bmx.Stop();
}

void BMX055_Accl(void)
{
	twi bmx(&TWIC,bmx_fre);
	int data[6];
	for (int i = 0; i < 6; i++)
	{
		bmx.Address(Addr_Accl,0);
		bmx.WriteSingle((2 + i));// Select data register
		bmx.Stop();
		bmx.Address(Addr_Accl, 1);// Request 1 byte of data
		// Read 6 bytes of data
		// xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
		data[i] = bmx.ReadSingle(0);
		bmx.Stop();
	}
	// Convert the data to 12-bits
	xAccl = ((data[1] * 256) + (data[0] & 0xF0)) / 16;
	if (xAccl > 2047)  xAccl -= 4096;
	yAccl = ((data[3] * 256) + (data[2] & 0xF0)) / 16;
	if (yAccl > 2047)  yAccl -= 4096;
	zAccl = ((data[5] * 256) + (data[4] & 0xF0)) / 16;
	if (zAccl > 2047)  zAccl -= 4096;
	/*xAccl = xAccl * 0.0098; // renge +-2g
	yAccl = yAccl * 0.0098; // renge +-2g
	zAccl = zAccl * 0.0098; // renge +-2g*/
	return;
}
//=====================================================================================//
void BMX055_Gyro(void)
{
	twi bmx(&TWIC,bmx_fre);
	int data[6];
	for (int i = 0; i < 6; i++)
	{
		bmx.Address(Addr_Gyro,0);
		bmx.WriteSingle((2 + i));    // Select data register
		bmx.Stop();
		bmx.Address(Addr_Gyro, 1);    // Request 1 byte of data
		// Read 6 bytes of data
		// xGyro lsb, xGyro msb, yGyro lsb, yGyro msb, zGyro lsb, zGyro msb
		data[i] = bmx.ReadSingle(0);
		bmx.Stop();
	}
	// Convert the data
	xGyro = (data[1] * 256) + data[0];
	if (xGyro > 32767)  xGyro -= 65536;
	yGyro = (data[3] * 256) + data[2];
	if (yGyro > 32767)  yGyro -= 65536;
	zGyro = (data[5] * 256) + data[4];
	if (zGyro > 32767)  zGyro -= 65536;

	/*xGyro = xGyro * 0.0038; //  Full scale = +/- 125 degree/s
	yGyro = yGyro * 0.0038; //  Full scale = +/- 125 degree/s
	zGyro = zGyro * 0.0038; //  Full scale = +/- 125 degree/s*/
	return;
}
//=====================================================================================//
void BMX055_Mag(void)
{
	twi bmx(&TWIC,bmx_fre);
	int data[8];
	for (int i = 0; i < 8; i++)
	{
		bmx.Address(Addr_Mag,0);
		bmx.WriteSingle((0x42 + i));    // Select data register
		bmx.Stop();
		bmx.Address(Addr_Mag, 1);    // Request 1 byte of data
		// Read 6 bytes of data
		// xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
		data[i] = bmx.ReadSingle(0);
		bmx.Stop();‚š
	}
	// Convert the data
	xMag = ((data[1] <<8) | (data[0]>>3));
	//if (xMag > 4095)  xMag -= 8192;
	yMag = ((data[3] <<8) | (data[2]>>3));
	//if (yMag > 4095)  yMag -= 8192;
	zMag = ((data[5] <<8) | (data[4]>>3));
	//if (zMag > 16383)  zMag -= 32768;
	return;
}

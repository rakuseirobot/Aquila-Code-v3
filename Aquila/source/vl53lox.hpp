/*
 * vl53lox.h
 *
 * Created: 2018/12/19 18:20:21
 *  Author: shun2
 */ 


#ifndef VL53LOX_H_
#define VL53LOX_H_


#define VL_TWI &TWID
#include "xmega_twi.hpp"
#include "xmega_usart.hpp"
#include "ui_control.hpp"
#include "initializing.hpp"
#include <util/delay.h>

#define ADDRESS_DEFAULT 0b0101001


// Decode VCSEL (vertical cavity surface emitting laser) pulse period in PCLKs
// from register value
// based on VL53L0X_decode_vcsel_period()
#define decodeVcselPeriod(reg_val)      (((reg_val) + 1) << 1)

// Encode VCSEL pulse period register value from period in PCLKs
// based on VL53L0X_encode_vcsel_period()
#define encodeVcselPeriod(period_pclks) (((period_pclks) >> 1) - 1)

// Calculate macro period in *nanoseconds* from VCSEL period in PCLKs
// based on VL53L0X_calc_macro_period_ps()
// PLL_period_ps = 1655; macro_period_vclks = 2304
#define calcMacroPeriod(vcsel_period_pclks) ((((uint32_t)2304 * (vcsel_period_pclks) * 1655) + 500) / 1000)

//
//uint8_t vl_addr = 0x29<<1;//0x14;//0x29//0b0101001;
//uint8_t stop_variable = 0;
//uint32_t measurement_timing_budget_us = 0;

enum vcselPeriodType { VcselPeriodPreRange, VcselPeriodFinalRange };
struct SequenceStepEnables
{
	bool tcc, msrc, dss, pre_range, final_range;
};

struct SequenceStepTimeouts
{
	uint16_t pre_range_vcsel_period_pclks, final_range_vcsel_period_pclks;

	uint16_t msrc_dss_tcc_mclks, pre_range_mclks, final_range_mclks;
	uint32_t msrc_dss_tcc_us,    pre_range_us,    final_range_us;
};

void writeReg(uint8_t reg, uint8_t value);
void writeReg16Bit(uint8_t reg,uint16_t value);
void writeMulti(uint8_t reg, uint8_t const * src, uint8_t count);
uint16_t readReg16Bit(uint8_t reg);
uint8_t readReg(uint8_t reg);
void readMulti(uint8_t reg, uint8_t * dst, uint8_t count);
uint8_t getVcselPulsePeriod(vcselPeriodType type);
bool getSpadInfo(uint8_t * count, bool * type_is_aperture);
uint32_t getMeasurementTimingBudget(void);
bool setMeasurementTimingBudget(uint32_t budget_us);
void getSequenceStepEnables(SequenceStepEnables * enables);
uint32_t timeoutMicrosecondsToMclks(uint32_t timeout_period_us, uint8_t vcsel_period_pclks);
bool performSingleRefCalibration(uint8_t vhv_init_byte);
bool init_vl(bool io_2v8);
uint16_t decodeTimeout(uint16_t reg_val);
uint16_t readRangeSingleMillimeters(void);
uint16_t readRangeContinuousMillimeters(void);
void getSequenceStepTimeouts(SequenceStepEnables const * enables, SequenceStepTimeouts * timeouts);
bool setVcselPulsePeriod(vcselPeriodType type, uint8_t period_pclks);
uint16_t encodeTimeout(uint16_t timeout_mclks);
uint32_t timeoutMclksToMicroseconds(uint16_t timeout_period_mclks, uint8_t vcsel_period_pclks);


/*

init_vl();
_delay_ms(10);
// lower the return signal rate limit (default is 0.25 MCPS)
setSignalRateLimit(0.1);
// increase laser pulse periods (defaults are 14 and 10 PCLKs)
setVcselPulsePeriod(VcselPeriodPreRange, 18);
setVcselPulsePeriod(VcselPeriodFinalRange, 14);
// increase laser pulse periods (defaults are 14 and 10 PCLKs)
//setVcselPulsePeriod(VcselPeriodPreRange, 18);
//setVcselPulsePeriod(VcselPeriodFinalRange, 14);
setMeasurementTimingBudget(200000);
usart_string("initfinish!");
_delay_ms(10);
rtc();
while(1){
	usart_puthex2(RTC.CNT);
	usart_puthex2(readRangeSingleMillimeters());
	usart_string("\n\r");
*/




#endif /* VL53LOX_H_ */
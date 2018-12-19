/*
 * vl53lox.cpp
 *
 * Created: 2018/12/19 18:19:55
 *  Author: shun2
 */ 

#include "vl53lox.hpp"

twi vl(VL_TWI,200000);

uint8_t vl_addr = 0x29<<1;//0x14;//0x29//0b0101001;
uint8_t stop_variable = 0;
uint32_t measurement_timing_budget_us = 0;

void writeReg(uint8_t reg, uint8_t value){
	uint8_t data[2] = {reg,value};
	vl.Address(vl_addr,0);
	//twiWriteMultiple(VL_TWI,(char *)data,2);
	vl.WriteSingle(data[0]);
	//twiStop(VL_TWI);
	//twiAddress(VL_TWI,vl_addr,0);
	vl.WriteSingle(data[1]);
	vl.Stop();
	return;
}

void writeReg16Bit(uint8_t reg,uint16_t value){
	vl.Address(vl_addr,0);
	vl.WriteSingle(reg);
	//twiStop(VL_TWI);
	//twiAddress(VL_TWI,vl_addr,0);
	vl.WriteSingle((value >> 8) & 0xFF);
	//twiStop(VL_TWI);
	//twiAddress(VL_TWI,vl_addr,0);
	vl.WriteSingle(value & 0xFF);
	vl.Stop();
	return;
}

void writeMulti(uint8_t reg, uint8_t const * src, uint8_t count){
	vl.Address(vl_addr,0);
	vl.WriteSingle(reg);
	//twiStop(VL_TWI);
	while (count-- > 0)
	{
		//twiAddress(VL_TWI,vl_addr,0);
		vl.WriteSingle(*(src++));
	}
	vl.Stop();
	return;
}

uint16_t readReg16Bit(uint8_t reg){
	uint16_t value;
	vl.Address(vl_addr,0);
	vl.WriteSingle(reg);
	vl.Stop();
	vl.Address(vl_addr,1);
	value  = (uint16_t)vl.ReadSingle(1) << 8; // value high byte
	value |= vl.ReadSingle(0);      // value low byte
	vl.Stop();
	return value;
}

uint8_t readReg(uint8_t reg){
	vl.Address(vl_addr,0);
	vl.WriteSingle(reg);
	vl.Stop();
	vl.Address(vl_addr,1);
	uint8_t val = vl.ReadSingle(0);
	vl.Stop();
	return val;
}

void readMulti(uint8_t reg, uint8_t* dst, uint8_t count){
	vl.Address(vl_addr,0);
	vl.WriteSingle(reg);
	vl.Stop();
	vl.Address(vl_addr,1);
	vl.ReadMultiple(dst,count);
	vl.Stop();
	return;
}

uint8_t getVcselPulsePeriod(vcselPeriodType type){
	if (type == VcselPeriodPreRange)
	{
		return decodeVcselPeriod(readReg(0x50));
	}
	else if (type == VcselPeriodFinalRange)
	{
		return decodeVcselPeriod(readReg(0x70));
	}
	else { return 255; }
}


bool getSpadInfo(uint8_t * count, bool * type_is_aperture){
	uint8_t tmp;

	writeReg(0x80, 0x01);
	writeReg(0xFF, 0x01);
	writeReg(0x00, 0x00);

	writeReg(0xFF, 0x06);
	writeReg(0x83, readReg(0x83) | 0x04);
	writeReg(0xFF, 0x07);
	writeReg(0x81, 0x01);

	writeReg(0x80, 0x01);

	writeReg(0x94, 0x6b);
	writeReg(0x83, 0x00);
	//startTimeout();
	serial.string("getSpadInfo 161!\n\r");
	while (readReg(0x83) == 0x00)
	{
		//if (checkTimeoutExpired()) { return false; }  //TimeOut !!?
	}
	serial.string("getSpadInfo 166!\n\r");

	writeReg(0x83, 0x01);
	tmp = readReg(0x92);

	*count = tmp & 0x7f;
	*type_is_aperture = (tmp >> 7) & 0x01;

	writeReg(0x81, 0x00);
	writeReg(0xFF, 0x06);
	writeReg(0x83, readReg(0x83)  & ~0x04);
	writeReg(0xFF, 0x01);
	writeReg(0x00, 0x01);

	writeReg(0xFF, 0x00);
	writeReg(0x80, 0x00);

	return true;
}

uint32_t getMeasurementTimingBudget(void){
	SequenceStepEnables enables;
	SequenceStepTimeouts timeouts;
	
	uint16_t const StartOverhead     = 1910; // note that this is different than the value in set_
	uint16_t const EndOverhead        = 960;
	uint16_t const MsrcOverhead       = 660;
	uint16_t const TccOverhead        = 590;
	uint16_t const DssOverhead        = 690;
	uint16_t const PreRangeOverhead   = 660;
	uint16_t const FinalRangeOverhead = 550;

	// "Start and end overhead times always present"
	uint32_t budget_us = StartOverhead + EndOverhead;
	getSequenceStepEnables(&enables);
	getSequenceStepTimeouts(&enables, &timeouts);

	if (enables.tcc)
	{
		budget_us += (timeouts.msrc_dss_tcc_us + TccOverhead);
	}

	if (enables.dss)
	{
		budget_us += 2 * (timeouts.msrc_dss_tcc_us + DssOverhead);
	}
	else if (enables.msrc)
	{
		budget_us += (timeouts.msrc_dss_tcc_us + MsrcOverhead);
	}

	if (enables.pre_range)
	{
		budget_us += (timeouts.pre_range_us + PreRangeOverhead);
	}

	if (enables.final_range)
	{
		budget_us += (timeouts.final_range_us + FinalRangeOverhead);
	}

	measurement_timing_budget_us = budget_us; // store for internal reuse
	return budget_us;
}

// Get sequence step enables
// based on VL53L0X_GetSequenceStepEnables()

bool setMeasurementTimingBudget(uint32_t budget_us){
	SequenceStepEnables enables;
	SequenceStepTimeouts timeouts;

	uint16_t const StartOverhead      = 1320; // note that this is different than the value in get_
	uint16_t const EndOverhead        = 960;
	uint16_t const MsrcOverhead       = 660;
	uint16_t const TccOverhead        = 590;
	uint16_t const DssOverhead        = 690;
	uint16_t const PreRangeOverhead   = 660;
	uint16_t const FinalRangeOverhead = 550;

	uint32_t const MinTimingBudget = 20000;

	if (budget_us < MinTimingBudget) { return false; }

	uint32_t used_budget_us = StartOverhead + EndOverhead;

	getSequenceStepEnables(&enables);
	getSequenceStepTimeouts(&enables, &timeouts);

	if (enables.tcc)
	{
		used_budget_us += (timeouts.msrc_dss_tcc_us + TccOverhead);
	}

	if (enables.dss)
	{
		used_budget_us += 2 * (timeouts.msrc_dss_tcc_us + DssOverhead);
	}
	else if (enables.msrc)
	{
		used_budget_us += (timeouts.msrc_dss_tcc_us + MsrcOverhead);
	}

	if (enables.pre_range)
	{
		used_budget_us += (timeouts.pre_range_us + PreRangeOverhead);
	}

	if (enables.final_range)
	{
		used_budget_us += FinalRangeOverhead;

		// "Note that the final range timeout is determined by the timing
		// budget and the sum of all other timeouts within the sequence.
		// If there is no room for the final range timeout, then an error
		// will be set. Otherwise the remaining time will be applied to
		// the final range."

		if (used_budget_us > budget_us)
		{
			// "Requested timeout too big."
			return false;
		}

		uint32_t final_range_timeout_us = budget_us - used_budget_us;

		// set_sequence_step_timeout() begin
		// (SequenceStepId == VL53L0X_SEQUENCESTEP_FINAL_RANGE)

		// "For the final range timeout, the pre-range timeout
		//  must be added. To do this both final and pre-range
		//  timeouts must be expressed in macro periods MClks
		//  because they have different vcsel periods."

		uint16_t final_range_timeout_mclks =
		timeoutMicrosecondsToMclks(final_range_timeout_us,
		timeouts.final_range_vcsel_period_pclks);

		if (enables.pre_range)
		{
			final_range_timeout_mclks += timeouts.pre_range_mclks;
		}

		writeReg16Bit(0x71,encodeTimeout(final_range_timeout_mclks));

		// set_sequence_step_timeout() end

		measurement_timing_budget_us = budget_us; // store for internal reuse
	}
	return true;
}
void getSequenceStepEnables(SequenceStepEnables * enables){
	uint8_t sequence_config = readReg(0x01);

	enables->tcc          = ((sequence_config >> 4) & 0x1 > 0) ? true : false;
	enables->dss          = ((sequence_config >> 3) & 0x1 > 0) ? true : false;
	enables->msrc         = ((sequence_config >> 2) & 0x1 > 0) ? true : false;
	enables->pre_range    = ((sequence_config >> 6) & 0x1 > 0) ? true : false;
	enables->final_range  = ((sequence_config >> 7) & 0x1 > 0) ? true : false;
	return;
}


// Get sequence step timeouts
// based on get_sequence_step_timeout(),
// but gets all timeouts instead of just the requested one, and also stores
// intermediate values
void getSequenceStepTimeouts(SequenceStepEnables const * enables, SequenceStepTimeouts * timeouts)
{
	timeouts->pre_range_vcsel_period_pclks = getVcselPulsePeriod(VcselPeriodPreRange);

	timeouts->msrc_dss_tcc_mclks = readReg(0x46) + 1;
	timeouts->msrc_dss_tcc_us =
	timeoutMclksToMicroseconds(timeouts->msrc_dss_tcc_mclks,
	timeouts->pre_range_vcsel_period_pclks);

	timeouts->pre_range_mclks =
	decodeTimeout(readReg16Bit(0x51));
	timeouts->pre_range_us =
	timeoutMclksToMicroseconds(timeouts->pre_range_mclks,
	timeouts->pre_range_vcsel_period_pclks);

	timeouts->final_range_vcsel_period_pclks = getVcselPulsePeriod(VcselPeriodFinalRange);

	timeouts->final_range_mclks =
	decodeTimeout(readReg16Bit(0x71));

	if (enables->pre_range)
	{
		timeouts->final_range_mclks -= timeouts->pre_range_mclks;
	}

	timeouts->final_range_us =
	timeoutMclksToMicroseconds(timeouts->final_range_mclks,
	timeouts->final_range_vcsel_period_pclks);
}

uint32_t timeoutMclksToMicroseconds(uint16_t timeout_period_mclks, uint8_t vcsel_period_pclks){
	uint32_t macro_period_ns = calcMacroPeriod(vcsel_period_pclks);

	return ((timeout_period_mclks * macro_period_ns) + (macro_period_ns / 2)) / 1000;
}

// Encode sequence step timeout register value from timeout in MCLKs
// based on VL53L0X_encode_timeout()
// Note: the original function took a uint16_t, but the argument passed to it
// is always a uint16_t.
uint16_t encodeTimeout(uint16_t timeout_mclks){
	// format: "(LSByte * 2^MSByte) + 1"

	uint32_t ls_byte = 0;
	uint16_t ms_byte = 0;

	if (timeout_mclks > 0)
	{
		ls_byte = timeout_mclks - 1;

		while ((ls_byte & 0xFFFFFF00) > 0)
		{
			ls_byte >>= 1;
			ms_byte++;
		}

		return (ms_byte << 8) | (ls_byte & 0xFF);
	}
	else { return 0; }
}

uint16_t decodeTimeout(uint16_t reg_val){
	// format: "(LSByte * 2^MSByte) + 1"
	return (uint16_t)((reg_val & 0x00FF) <<
	(uint16_t)((reg_val & 0xFF00) >> 8)) + 1;
}

bool setSignalRateLimit(float limit_Mcps)
{
  if (limit_Mcps < 0 || limit_Mcps > 511.99) { return false; }

  // Q9.7 fixed point format (9 integer bits, 7 fractional bits)
  writeReg16Bit(0x44, limit_Mcps * (1 << 7));
  return true;
}

bool setVcselPulsePeriod(vcselPeriodType type, uint8_t period_pclks)
{
	uint8_t vcsel_period_reg = encodeVcselPeriod(period_pclks);

	SequenceStepEnables enables;
	SequenceStepTimeouts timeouts;

	getSequenceStepEnables(&enables);
	getSequenceStepTimeouts(&enables, &timeouts);

	// "Apply specific settings for the requested clock period"
	// "Re-calculate and apply timeouts, in macro periods"

	// "When the VCSEL period for the pre or final range is changed,
	// the corresponding timeout must be read from the device using
	// the current VCSEL period, then the new VCSEL period can be
	// applied. The timeout then must be written back to the device
	// using the new VCSEL period.
	//
	// For the MSRC timeout, the same applies - this timeout being
	// dependant on the pre-range vcsel period."


	if (type == VcselPeriodPreRange)
	{
		// "Set phase check limits"
		switch (period_pclks)
		{
			case 12:
			writeReg(0x57, 0x18);
			break;

			case 14:
			writeReg(0x57, 0x30);
			break;

			case 16:
			writeReg(0x57, 0x40);
			break;

			case 18:
			writeReg(0x57, 0x50);
			break;

			default:
			// invalid period
			return false;
		}
		writeReg(0x56, 0x08);

		// apply new VCSEL period
		writeReg(0x50, vcsel_period_reg);

		// update timeouts

		// set_sequence_step_timeout() begin
		// (SequenceStepId == VL53L0X_SEQUENCESTEP_PRE_RANGE)

		uint16_t new_pre_range_timeout_mclks =
		timeoutMicrosecondsToMclks(timeouts.pre_range_us, period_pclks);

		writeReg16Bit(0x51,encodeTimeout(new_pre_range_timeout_mclks));

		// set_sequence_step_timeout() end

		// set_sequence_step_timeout() begin
		// (SequenceStepId == VL53L0X_SEQUENCESTEP_MSRC)

		uint16_t new_msrc_timeout_mclks =
		timeoutMicrosecondsToMclks(timeouts.msrc_dss_tcc_us, period_pclks);

		writeReg(0x46,(new_msrc_timeout_mclks > 256) ? 255 : (new_msrc_timeout_mclks - 1));

		// set_sequence_step_timeout() end
	}
	else if (type == VcselPeriodFinalRange)
	{
		switch (period_pclks)
		{
			case 8:
			writeReg(0x48, 0x10);
			writeReg(0x47,  0x08);
			writeReg(0x32, 0x02);
			writeReg(0x30, 0x0C);
			writeReg(0xFF, 0x01);
			writeReg(0x30, 0x30);
			writeReg(0xFF, 0x00);
			break;

			case 10:
			writeReg(0x48, 0x28);
			writeReg(0x47,  0x08);
			writeReg(0x32, 0x03);
			writeReg(0x30, 0x09);
			writeReg(0xFF, 0x01);
			writeReg(0x30, 0x20);
			writeReg(0xFF, 0x00);
			break;

			case 12:
			writeReg(0x48, 0x38);
			writeReg(0x47,  0x08);
			writeReg(0x32, 0x03);
			writeReg(0x30, 0x08);
			writeReg(0xFF, 0x01);
			writeReg(0x30, 0x20);
			writeReg(0xFF, 0x00);
			break;

			case 14:
			writeReg(0x48, 0x48);
			writeReg(0x47,  0x08);
			writeReg(0x32, 0x03);
			writeReg(0x30, 0x07);
			writeReg(0xFF, 0x01);
			writeReg(0x30, 0x20);
			writeReg(0xFF, 0x00);
			break;

			default:
			// invalid period
			return false;
		}

		// apply new VCSEL period
		writeReg(0x70, vcsel_period_reg);

		// update timeouts

		// set_sequence_step_timeout() begin
		// (SequenceStepId == VL53L0X_SEQUENCESTEP_FINAL_RANGE)

		// "For the final range timeout, the pre-range timeout
		//  must be added. To do this both final and pre-range
		//  timeouts must be expressed in macro periods MClks
		//  because they have different vcsel periods."

		uint16_t new_final_range_timeout_mclks =
		timeoutMicrosecondsToMclks(timeouts.final_range_us, period_pclks);

		if (enables.pre_range)
		{
			new_final_range_timeout_mclks += timeouts.pre_range_mclks;
		}

		writeReg16Bit(0x71,encodeTimeout(new_final_range_timeout_mclks));

		// set_sequence_step_timeout end
	}
	else
	{
		// invalid type
		return false;
	}

	// "Finally, the timing budget must be re-applied"

	setMeasurementTimingBudget(measurement_timing_budget_us);

	// "Perform the phase calibration. This is needed after changing on vcsel period."
	// VL53L0X_perform_phase_calibration() begin

	uint8_t sequence_config = readReg(0x01);
	writeReg(0x01, 0x02);
	performSingleRefCalibration(0x0);
	writeReg(0x01, sequence_config);

	// VL53L0X_perform_phase_calibration() end

	return true;
}

uint32_t timeoutMicrosecondsToMclks(uint32_t timeout_period_us, uint8_t vcsel_period_pclks){
	uint32_t macro_period_ns = calcMacroPeriod(vcsel_period_pclks);

	return (((timeout_period_us * 1000) + (macro_period_ns / 2)) / macro_period_ns);
}

bool performSingleRefCalibration(uint8_t vhv_init_byte){
	writeReg(0x00, 0x01 | vhv_init_byte); // VL53L0X_REG_SYSRANGE_MODE_START_STOP

	//startTimeout();
	while ((readReg(0x13) & 0x07) == 0)
	{
		//if (checkTimeoutExpired()) { return false; }
	}

	writeReg(0x0B, 0x01);

	writeReg(0x00, 0x00);

	return true;
}


bool init_vl(bool io_2v8 = false){
	if(io_2v8){
		writeReg(0x89,readReg(0x89)|0x01);
	}
	writeReg(0x88, 0x00);
	writeReg(0x80, 0x01);
	writeReg(0xFF, 0x01);
	writeReg(0x00, 0x00);
	serial.string("600!\n\r");
	stop_variable = readReg(0x91);
	serial.putdec(stop_variable);
	serial.string("\n\r602!\n\r");
	writeReg(0x00, 0x01);
	writeReg(0xFF, 0x00);
	writeReg(0x80, 0x00);	
	// disable SIGNAL_RATE_MSRC (bit 1) and SIGNAL_RATE_PRE_RANGE (bit 4) limit checks
	serial.string("607!\n\r");
	uint8_t temp = readReg(0x60);
	writeReg(0x60, temp|0x12);
	serial.string("609!\n\r");
	// set final range signal rate limit to 0.25 MCPS (million counts per second)
	writeReg16Bit(0x44, (float)0.25 * (1 << 7));
	writeReg(0x01,0xFF);
	// VL53L0X_DataInit() end
	// VL53L0X_StaticInit() begin
	 uint8_t spad_count;
	 bool spad_type_is_aperture;
	 serial.string("617!\n\r");
	 if (!getSpadInfo(&spad_count, &spad_type_is_aperture)) { return false; }
	 serial.string("619!\n\r");
	 // The SPAD map (RefGoodSpadMap) is read by VL53L0X_get_info_from_device() in
	 // the API, but the same data seems to be more easily readable from
	 // GLOBAL_CONFIG_SPAD_ENABLES_REF_0 through _6, so read it from there
	 uint8_t ref_spad_map[6];
	 readMulti(0xB0,ref_spad_map,6);
	 
	 // -- VL53L0X_set_reference_spads() begin (assume NVM values are valid)

	 writeReg(0xFF, 0x01);
	 writeReg(0x4F, 0x00);
	 writeReg(0x4E, 0x2C);
	 writeReg(0xFF, 0x00);
	 writeReg(0xB6, 0xB4);

	 uint8_t first_spad_to_enable = spad_type_is_aperture ? 12 : 0; // 12 is the first aperture spad
	 uint8_t spads_enabled = 0;

	 for (uint8_t i = 0; i < 48; i++)
	 {
		 if (i < first_spad_to_enable || spads_enabled == spad_count)
		 {
			 // This bit is lower than the first one that should be enabled, or
			 // (reference_spad_count) bits have already been enabled, so zero this bit
			 ref_spad_map[i / 8] &= ~(1 << (i % 8));
		 }
		 else if ((ref_spad_map[i / 8] >> (i % 8)) & 0x1)
		 {
			 spads_enabled++;
		 }
	 }

	 writeMulti(0xB0, ref_spad_map, 6);

	 // -- VL53L0X_set_reference_spads() end

	 // -- VL53L0X_load_tuning_settings() begin
	 // DefaultTuningSettings from vl53l0x_tuning.h

	 writeReg(0xFF, 0x01);
	 writeReg(0x00, 0x00);

	 writeReg(0xFF, 0x00);
	 writeReg(0x09, 0x00);
	 writeReg(0x10, 0x00);
	 writeReg(0x11, 0x00);

	 writeReg(0x24, 0x01);
	 writeReg(0x25, 0xFF);
	 writeReg(0x75, 0x00);

	 writeReg(0xFF, 0x01);
	 writeReg(0x4E, 0x2C);
	 writeReg(0x48, 0x00);
	 writeReg(0x30, 0x20);

	 writeReg(0xFF, 0x00);
	 writeReg(0x30, 0x09);
	 writeReg(0x54, 0x00);
	 writeReg(0x31, 0x04);
	 writeReg(0x32, 0x03);
	 writeReg(0x40, 0x83);
	 writeReg(0x46, 0x25);
	 writeReg(0x60, 0x00);
	 writeReg(0x27, 0x00);
	 writeReg(0x50, 0x06);
	 writeReg(0x51, 0x00);
	 writeReg(0x52, 0x96);
	 writeReg(0x56, 0x08);
	 writeReg(0x57, 0x30);
	 writeReg(0x61, 0x00);
	 writeReg(0x62, 0x00);
	 writeReg(0x64, 0x00);
	 writeReg(0x65, 0x00);
	 writeReg(0x66, 0xA0);

	 writeReg(0xFF, 0x01);
	 writeReg(0x22, 0x32);
	 writeReg(0x47, 0x14);
	 writeReg(0x49, 0xFF);
	 writeReg(0x4A, 0x00);

	 writeReg(0xFF, 0x00);
	 writeReg(0x7A, 0x0A);
	 writeReg(0x7B, 0x00);
	 writeReg(0x78, 0x21);

	 writeReg(0xFF, 0x01);
	 writeReg(0x23, 0x34);
	 writeReg(0x42, 0x00);
	 writeReg(0x44, 0xFF);
	 writeReg(0x45, 0x26);
	 writeReg(0x46, 0x05);
	 writeReg(0x40, 0x40);
	 writeReg(0x0E, 0x06);
	 writeReg(0x20, 0x1A);
	 writeReg(0x43, 0x40);

	 writeReg(0xFF, 0x00);
	 writeReg(0x34, 0x03);
	 writeReg(0x35, 0x44);

	 writeReg(0xFF, 0x01);
	 writeReg(0x31, 0x04);
	 writeReg(0x4B, 0x09);
	 writeReg(0x4C, 0x05);
	 writeReg(0x4D, 0x04);

	 writeReg(0xFF, 0x00);
	 writeReg(0x44, 0x00);
	 writeReg(0x45, 0x20);
	 writeReg(0x47, 0x08);
	 writeReg(0x48, 0x28);
	 writeReg(0x67, 0x00);
	 writeReg(0x70, 0x04);
	 writeReg(0x71, 0x01);
	 writeReg(0x72, 0xFE);
	 writeReg(0x76, 0x00);
	 writeReg(0x77, 0x00);

	 writeReg(0xFF, 0x01);
	 writeReg(0x0D, 0x01);

	 writeReg(0xFF, 0x00);
	 writeReg(0x80, 0x01);
	 writeReg(0x01, 0xF8);

	 writeReg(0xFF, 0x01);
	 writeReg(0x8E, 0x01);
	 writeReg(0x00, 0x01);
	 writeReg(0xFF, 0x00);
	 writeReg(0x80, 0x00);

	 // -- VL53L0X_load_tuning_settings() end

	 // "Set interrupt config to new sample ready"
	 // -- VL53L0X_SetGpioConfig() begin

	 writeReg(0x0A, 0x04);
	 writeReg(0x84, readReg(0x84) & ~0x10); // active low
	 writeReg(0x0B, 0x01);

	// -- VL53L0X_SetGpioConfig() end

	measurement_timing_budget_us = getMeasurementTimingBudget();

	// "Disable MSRC and TCC by default"
	// MSRC = Minimum Signal Rate Check
	// TCC = Target CentreCheck
	// -- VL53L0X_SetSequenceStepEnable() begin

	writeReg(0x01, 0xE8);

	// -- VL53L0X_SetSequenceStepEnable() end
	// "Recalculate timing budget"
	
	setMeasurementTimingBudget(measurement_timing_budget_us);
	
	writeReg(0x01, 0x01);
	
	serial.string("774!\n\r");
	
	if (!performSingleRefCalibration(0x40)) { return false; }

	serial.string("803!\n\r");
	// -- VL53L0X_perform_vhv_calibration() end

	// -- VL53L0X_perform_phase_calibration() begin
	// _delay_ms(10);
	writeReg(0x01, 0x02);
	// _delay_ms(10);
	if (!performSingleRefCalibration(0x00)) { return false; }

	serial.string("811!\n\r");
	// -- VL53L0X_perform_phase_calibration() end

	// "restore the previous Sequence Config"
	writeReg(0x01, 0xE8);

	// VL53L0X_PerformRefCalibration() end

	return true;
}

uint16_t readRangeSingleMillimeters(void)  //This is for Checking Distance
{
	writeReg(0x80, 0x01);
	writeReg(0xFF, 0x01);
	writeReg(0x00, 0x00);
	writeReg(0x91, stop_variable);
	writeReg(0x00, 0x01);
	writeReg(0xFF, 0x00);
	writeReg(0x80, 0x00);

	writeReg(0x00, 0x01);

	// "Wait until start bit has been cleared"
	//startTimeout();
	while (readReg(0x00) & 0x01)
	{
		serial.string("Wait!");
		/*if (checkTimeoutExpired())
		{
			did_timeout = true;
			return 65535;
		}*/
	}

	return readRangeContinuousMillimeters();
}

uint16_t readRangeContinuousMillimeters(void){
	//startTimeout();
	while ((readReg(0x13) & 0x07) == 0)
	{
		/*if (checkTimeoutExpired())
		{
			did_timeout = true;
			return 65535;
		}*/
	}

	// assumptions: Linearity Corrective Gain is 1000 (default);
	// fractional ranging is not enabled
	uint16_t range = readReg16Bit(0x14 + 10);

	writeReg(0x0B, 0x01);

	return range;
}
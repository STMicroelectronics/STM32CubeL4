/**
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef _VL53L0X_API_CALIBRATION_H_
#define _VL53L0X_API_CALIBRATION_H_

#include "vl53l0x_def.h"
#include "vl53l0x_platform.h"


#ifdef __cplusplus
extern "C" {
#endif

VL53L0X_Error VL53L0X_perform_xtalk_calibration(VL53L0X_DEV Dev,
		FixPoint1616_t XTalkCalDistance,
		FixPoint1616_t *pXTalkCompensationRateMegaCps);

VL53L0X_Error VL53L0X_perform_offset_calibration(VL53L0X_DEV Dev,
		FixPoint1616_t CalDistanceMilliMeter,
		int32_t *pOffsetMicroMeter);

VL53L0X_Error VL53L0X_set_offset_calibration_data_micro_meter(VL53L0X_DEV Dev,
		int32_t OffsetCalibrationDataMicroMeter);

VL53L0X_Error VL53L0X_get_offset_calibration_data_micro_meter(VL53L0X_DEV Dev,
		int32_t *pOffsetCalibrationDataMicroMeter);

VL53L0X_Error VL53L0X_apply_offset_adjustment(VL53L0X_DEV Dev);

VL53L0X_Error VL53L0X_perform_ref_spad_management(VL53L0X_DEV Dev,
		uint32_t *refSpadCount, uint8_t *isApertureSpads);

VL53L0X_Error VL53L0X_set_reference_spads(VL53L0X_DEV Dev,
		uint32_t count, uint8_t isApertureSpads);

VL53L0X_Error VL53L0X_get_reference_spads(VL53L0X_DEV Dev,
		uint32_t *pSpadCount, uint8_t *pIsApertureSpads);

VL53L0X_Error VL53L0X_perform_phase_calibration(VL53L0X_DEV Dev,
	uint8_t *pPhaseCal, const uint8_t get_data_enable,
	const uint8_t restore_config);

VL53L0X_Error VL53L0X_perform_ref_calibration(VL53L0X_DEV Dev,
	uint8_t *pVhvSettings, uint8_t *pPhaseCal, uint8_t get_data_enable);

VL53L0X_Error VL53L0X_set_ref_calibration(VL53L0X_DEV Dev,
		uint8_t VhvSettings, uint8_t PhaseCal);

VL53L0X_Error VL53L0X_get_ref_calibration(VL53L0X_DEV Dev,
		uint8_t *pVhvSettings, uint8_t *pPhaseCal);




#ifdef __cplusplus
}
#endif

#endif /* _VL53L0X_API_CALIBRATION_H_ */

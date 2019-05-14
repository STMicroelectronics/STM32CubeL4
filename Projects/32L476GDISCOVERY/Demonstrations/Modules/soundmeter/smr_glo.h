/* -----------------------------------------------------------------------------    
* Copyright (C) 2014 STMicroelectronics. All rights reserved.    
*    
* $Date:        27 JUne 2014
* $Revision: 	V1.0.0
*    
* Project: 	    MCD Audio Library    
* Title:	    smr_glo.h
*    
* Description:	Global header for the SoundMeter module.
*               This header includes variables and functions which are visible from the audio FW.
*    
* Target Processor: Cortex-M4
*  
*   Version     Date           Author              Comment
*   -------     --------      ---------            ---------------------------------
*   1.0.0       27 Jun 2014   F. Ober              - Creation.
*    
* --------------------------------------------------------------------------- */

#include <stdio.h>

#ifndef SMR_GLO_H
#define SMR_GLO_H


/*============================================================================*/
/*      E R R O R   V A L U E S                                               */
/*============================================================================*/
#define SMR_ERROR_NONE                           0
#define SMR_UNSUPPORTED_INTERLEAVING_MODE       -1
#define SMR_UNSUPPORTED_NUMBER_OF_CHANNELS      -2
#define SMR_UNSUPPORTED_NUMBER_OF_BYTEPERSAMPLE -3
#define SMR_UNSUPPORTED_AVERAGING_TIME          -4
#define SMR_UNSUPPORTED_FILTER_TYPE             -5
#define SMR_UNSUPPORTED_SAMPLING_RATE           -6
#define SMR_BAD_HW                              -7


/*============================================================================*/
/*      PRE-FILTER VALUES                                                     */
/*============================================================================*/
#define SMR_PREFILTER_NONE            0
#define SMR_PREFILTER_AWEIGHTING      1
#define SMR_PREFILTER_CWEIGHTING      2
#define SMR_PREFILTER_DCREMOVE        3


//// FOB BEGIN
extern FILE *file_trace;
//// FOB END



/*============================================================================*/
/*      D Y N A M I C   P A R A M E T E R S   S T R U C T U R E               */
/*============================================================================*/
struct smr_dynamic_param {
    int32_t enable;
    int16_t averaging_time;
    int16_t filter_type;
    int32_t mean_level_left;
    int32_t mean_level_right;
};
typedef struct smr_dynamic_param smr_dynamic_param_t;


/*============================================================================*/
/*      S T A T I C   P A R A M E T E R S   S T R U C T U R E                 */
/*============================================================================*/
struct smr_static_param {
    int32_t sampling_rate;
};
typedef struct smr_static_param smr_static_param_t;


/*============================================================================*/
/*      I N T E R N A L   S T A T I C   M E M O R Y                           */
/*============================================================================*/
extern const uint32_t smr_dynamic_mem_size;


/*============================================================================*/
/*      I N T E R N A L   D Y N A M I C   M E M O R Y                         */
/*============================================================================*/
extern const uint32_t smr_static_mem_size;


/*============================================================================*/
/*      I N T E R F A C E S   D E C L A R A T I O N S                         */
/*============================================================================*/
/* smr_reset() : initializes static memory, states machines, ... */
extern int32_t smr_reset(void *static_mem_ptr, void *dynamic_mem_ptr);

/* smr_setConfig() : use to change dynamically some parameters */
extern int32_t smr_setConfig(smr_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);

/* smr_getConfig() : use to get values of dynamic parameters */
extern int32_t smr_getConfig(smr_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);

extern int32_t smr_getParam(smr_static_param_t *input_static_param_ptr, void *static_mem_ptr);
extern int32_t smr_setParam(smr_static_param_t *input_static_param_ptr, void *static_mem_ptr);

/* smr_process() : this is the main processing routine */
extern int32_t smr_process(buffer_t *input_buffer, buffer_t *output_buffer, void *static_mem_ptr);


#endif // SMR_GLO_H

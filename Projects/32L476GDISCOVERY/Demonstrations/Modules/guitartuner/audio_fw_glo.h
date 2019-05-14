/**
  ******************************************************************************
  * @file    audio_fw_glo.h
  * @author  MCD Application Team
  * @brief   This file contains audio fw definitions common to all audio modules
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _AUDIO_FW_GLO_H_
#define _AUDIO_FW_GLO_H_

/* Includes ------------------------------------------------------------------*/
//#include "arm_math.h"


/*============================================================================*/
/*      D E F I N I T I O N S                                                 */
/*============================================================================*/
#define NOT_INTERLEAVED                 0
#define INTERLEAVED                     1


typedef struct {
    int32_t       nb_channels;            /* Number of channels */
    int32_t       nb_bytes_per_Sample;    /* Nb bytes per sample    */
    void          *data_ptr;              /* Pointer to data buffer */
    int32_t       buffer_size;            /* Number of samples per channel in the data buffer   */
    int32_t       mode;                   /* buffer mode (interleaved, non interleaved) */
} buffer_t;



#endif /*_AUDIO_FW_GLO_H_*/

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/

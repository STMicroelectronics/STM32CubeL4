/**
  ******************************************************************************
  * @file    lib_NDEF_config.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file is a template to be modified in the project to configure
  *          how the lib_NDEF is supposed to access the nfctag memory.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty  
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#include "lib_NDEF_config.h"
#include "nfc04a1_nfctag.h"

/**
  * @brief  Reads data in the nfctag at specific address
  * @param  pData : pointer to store read data
  * @param  offset: Address to read
  * @param  Size : Size in bytes of the value to be read
  * @retval NDEF_OK if success, NDEF_ERROR in case of failure
  */
int32_t NDEF_Wrapper_ReadData(uint8_t* pData, uint32_t offset, uint32_t length )
{
  if(NFC04A1_NFCTAG_ReadData(NFC04A1_NFCTAG_INSTANCE, pData, offset, length ) != NFCTAG_OK)
    return NDEF_ERROR;
  return NDEF_OK;
}

/**
  * @brief  Zrites data in the nfctag at specific address
  * @param  pData : pointer to the data to be written
  * @param  offset: Address to write
  * @param  Size : Number of bytes to be written
  * @retval NDEF_OK if success, NDEF_ERROR in case of failure
  */
int32_t NDEF_Wrapper_WriteData(const uint8_t* pData, uint32_t offset, uint32_t length )
{
  if(NFC04A1_NFCTAG_WriteData(NFC04A1_NFCTAG_INSTANCE, pData, offset, length ) != NFCTAG_OK)
    return NDEF_ERROR;
  return NDEF_OK;
}


/**
  * @brief  Compute the NFCTAG Memory Size.
  * @return uint32_t Memory size in bytes.
  */
uint32_t NDEF_Wrapper_GetMemorySize(void)
{
  return NFC04A1_NFCTAG_GetByteSize(NFC04A1_NFCTAG_INSTANCE);
}

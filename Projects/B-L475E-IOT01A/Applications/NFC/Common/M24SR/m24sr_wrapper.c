/**
  ******************************************************************************
  * @file    m24sr_wrapper.c
  * @author  MCD Application Team
  * @brief   This file makes the bridge between middleware generic NCF TagType4 
  *          function calls and the M24SR component. Furthermore it simplifies 
  *          the use of the M24SR driver by sequencing some commands.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "m24sr_wrapper.h"
#include "stm32l475e_iot01.h"
#include "stm32l4xx.h"

/** @addtogroup M24SR_WRAPPER
  * @{
  */

/** @defgroup M24SR_WRAPPER
  * @{
  *  @brief  This is the library to interface with the M24SR dynamic tag.
  *         This layer simplify the use of the M24SR driver by sequencing 
  *         some commands.
  */

  /** @defgroup M24SR_WRAPPER_Private_Defines  M24SR_WRAPPER Private Defines
  * @{
  */
#define NDEF_SESSION_CLOSED                 0xDEADBEEF
/* M24SR buffer size is 0xF6 can be retrieve dynamicaly in CC file */
#define M24SR_READ_MAX_NBBYTE               0xF6 
#define M24SR_WRITE_MAX_NBBYTE              0xF6
/**
  * @}
  */

/** @defgroup M24SR_WRAPPER_Private_Variables   M24SR_WRAPPER Private Global Variables
  * @{
  */
  
uint8_t I2CPassword[16]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint32_t NDEFSessionOpenID=NDEF_SESSION_CLOSED;

/* Init NDEF_FileID with bad value in case Init failed */
static uint16_t NDEF_FileID = 0xDEAD;

/**
  * @}
  */


/** @defgroup M24SR_WRAPPER_Private_Functions
  * @{
  */

/**
  * @}
  */


/** @defgroup M24SR_WRAPPER_Exported_Functions   M24SR_WRAPPER Exported Functions 
  * @{
  */

/**
  * @brief  This fonction initialize the M24SR
  * @param  CCBuffer : pointer on the buffer to store CC file
  * @param  size : number of byte of data to read
  * @retval SUCCESS : Initalization done
  * @retval ERROR : Not able to Initialize. 
  */
uint16_t NFC_TT4_Initialization ( uint8_t* CCBuffer, uint8_t size )
{
  uint16_t status = ERROR;
  uint16_t trials = 5; /* wait 1sec, driver is configured to let 200ms for command to complete */
                       /* which is enough for all commands except GetSession if RF session is already opened */
                       /* Smartphone generaly release the session within the second, but customer can modify this value */
  
  /* Perform HW initialization */
  M24SR_Init(M24SR_I2C_ADDR, M24SR_I2C_GPO_MODE);
  
  /* Read CC file */
  while( status != M24SR_ACTION_COMPLETED && trials)
  {
    status = M24SR_GetSession(M24SR_I2C_ADDR);
    trials--;
  }
  if (status != M24SR_ACTION_COMPLETED)
  {
    return ERROR;
  }
  /*===================================*/
  /* Select the NFC type 4 application */ 
  /*===================================*/
  status = M24SR_SelectApplication(M24SR_I2C_ADDR); 
  if (status != M24SR_ACTION_COMPLETED)
  {
    return ERROR;
  }
    
  /*==================*/
  /* select a CC file */ 
  /*==================*/
  status = M24SR_SelectCCfile(M24SR_I2C_ADDR);
  if (status != M24SR_ACTION_COMPLETED)
  {
    return ERROR;
  }
      
  /* read the first 15 bytes of the CC file */
  if( NFC_TT4_ReadData ( 0x0000 , 0x0F , CCBuffer )== M24SR_ACTION_COMPLETED)
  {      
    NDEF_FileID = (uint16_t) ((CCBuffer[0x09]<<8) | CCBuffer[0x0A]);
    status = M24SR_Deselect(M24SR_I2C_ADDR);
    if (status != M24SR_ACTION_COMPLETED)
    {
      return ERROR;
    }
  }
  else
  {
    M24SR_Deselect(M24SR_I2C_ADDR);
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  This fonction retrieve the NDEF file ID of NDEF file present in M24SR
  * @param  NDEF_fileID : To store NDEF ID
  * @retval SUCCESS : File ID read
  * @retval ERROR : Not able to read file ID. 
  */
uint16_t NFC_TT4_GetNDEFFileId ( uint16_t *NDEF_fileID )
{
  if( NDEF_FileID != 0xDEAD)
  {
    *NDEF_fileID = NDEF_FileID;
    return SUCCESS;
  }
  else
  {
    return ERROR;
  }
}


/**
  * @brief  This fonction configure the M24SR to access NDEF message by I2C
  * @param  NDEF_fileID : NDEF identification to select NDEF in M24SR
  * @param  Priority: 2 options: check if M24SR available to open session (no RF session on going)
  *                               Kill RF session and open I2C sesssion.
  * @retval SUCCESS : Session is opened
  * @retval ERROR : Not able to open session. 
  */
uint16_t NFC_TT4_OpenNDEFSession ( uint16_t NDEF_fileID, uint16_t Priority )
{
  uint16_t status = ERROR;
  uint16_t trials = 5; /* wait 1sec, driver is configured to let 200ms for command to complete */
                       /* which is enough for all commands except GetSession if RF session is already opened */
                       /* Smartphone generaly release the session within the second, but customer can modify this value */
  
  if(NDEFSessionOpenID == NDEF_SESSION_CLOSED)
  {
    if( Priority == NFC_TT4_TAKE_SESSION)
    {
      status = M24SR_KillSession(M24SR_I2C_ADDR);
    }
    else
    {
      while( status != M24SR_ACTION_COMPLETED && trials)
      {
        status = M24SR_GetSession(M24SR_I2C_ADDR);
        trials--;
      }
    }
    if (status != M24SR_ACTION_COMPLETED)
    {
      /* seems session already open on RF side */
      /* But in case of I2C issue try to init again */
      M24SR_Init(M24SR_I2C_ADDR, M24SR_I2C_GPO_MODE);
      return ERROR;
    }
    
    /*===================================*/
    /* Select the NFC type 4 application */ 
    /*===================================*/
    status = M24SR_SelectApplication(M24SR_I2C_ADDR);
    if (status != M24SR_ACTION_COMPLETED)
    {
      return ERROR;
    }    
    /*====================*/
    /* select NDEF file   */
    /*====================*/
    status = M24SR_SelectNDEFfile(M24SR_I2C_ADDR, NDEF_fileID);
    if (status != M24SR_ACTION_COMPLETED)
    {
      return ERROR;
    } 
    
    NDEFSessionOpenID = (uint32_t)(NDEF_fileID);
    
    return SUCCESS;
  }
  else if(NDEFSessionOpenID == NDEF_fileID)
  {
    /* Session already Open not an issue caller can perform access in NDEF file */
    return SUCCESS;
  }
  
  return ERROR;  
}

/**
  * @brief  This fonction close the NDEF Session.
  * @param  NDEF_fileID : NDEF identification to select NDEF in M24SR
  * @retval SUCCESS : Session is closed
  * @retval ERROR : Not able to close session. 
  */
uint16_t NFC_TT4_CloseNDEFSession ( uint16_t NDEF_fileID )
{
  uint16_t status = ERROR;
  
  if(NDEFSessionOpenID == (uint32_t)(NDEF_fileID))
  {
    status = M24SR_Deselect(M24SR_I2C_ADDR);
    if (status != M24SR_ACTION_COMPLETED)
    {
      return ERROR;
    }
    NDEFSessionOpenID = NDEF_SESSION_CLOSED;
    
    return SUCCESS;
  }
  else if(NDEFSessionOpenID == NDEF_SESSION_CLOSED)
  {
    /* Not an error as session is already closed */
    return SUCCESS;
  }

  return ERROR;
}

/**
  * @brief  This fonction read the data stored in M24SR at defined offset
  * @param  Offset : Offset in the NDEF file in M24SR
  * @param  DataSize : Number of byte to read
  * @param  pData : pointer on buffer to store read data
  * @retval Status (SW1&SW2) : Status of the operation. 
  */
uint16_t NFC_TT4_ReadData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData)
{
  uint16_t status;

  if( DataSize > M24SR_READ_MAX_NBBYTE)
  {
    do
    {
      status = M24SR_ReadBinary (M24SR_I2C_ADDR, Offset, M24SR_READ_MAX_NBBYTE , pData);
      Offset += M24SR_READ_MAX_NBBYTE;
      pData += M24SR_READ_MAX_NBBYTE;
      DataSize -= M24SR_READ_MAX_NBBYTE;
    }while( DataSize > M24SR_READ_MAX_NBBYTE && status == M24SR_ACTION_COMPLETED);
    if( status == M24SR_ACTION_COMPLETED && DataSize)
      status = M24SR_ReadBinary (M24SR_I2C_ADDR, Offset, (uint8_t)(DataSize) , pData);
  }
  else
    status = M24SR_ReadBinary (M24SR_I2C_ADDR, Offset, (uint8_t)(DataSize) , pData);

  return status;
}

/**
  * @brief  This fonction read the data stored in M24SR at defined offset without NDEF concideration
  * @param  Offset : Offset in the NDEF file in M24SR
  * @param  DataSize : Number of byte to read
  * @param  pData : pointer on buffer to store read data
  * @retval Status (SW1&SW2) : Status of the operation.  
  */
uint16_t NFC_TT4_ForceReadData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData)
{
  uint16_t status;

  if( DataSize > M24SR_READ_MAX_NBBYTE)
  {
    do
    {
      status = M24SR_STReadBinary (M24SR_I2C_ADDR, Offset, M24SR_READ_MAX_NBBYTE , pData);
      Offset += M24SR_READ_MAX_NBBYTE;
      pData += M24SR_READ_MAX_NBBYTE;
      DataSize -= M24SR_READ_MAX_NBBYTE;
    }while( DataSize > M24SR_READ_MAX_NBBYTE && status == M24SR_ACTION_COMPLETED);
    if( status == M24SR_ACTION_COMPLETED && DataSize)
      status = M24SR_STReadBinary (M24SR_I2C_ADDR, Offset, (uint8_t)(DataSize) , pData);
  }
  else
    status = M24SR_STReadBinary (M24SR_I2C_ADDR, Offset, (uint8_t)(DataSize) , pData);

  return status;
}

/**
  * @brief  This fonction write data in M24SR at defined offset
  * @param  Offset : Offset in the NDEF file in M24SR
  * @param  DataSize : Number of byte to read
  * @param  pData : pointer on buffer to copy in M24SR
  * @retval Status (SW1&SW2) : Status of the operation.  
  */
uint16_t NFC_TT4_WriteData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData)
{
  uint16_t status;

  if( DataSize > M24SR_WRITE_MAX_NBBYTE)
  {
    do
    {
      status = M24SR_UpdateBinary (M24SR_I2C_ADDR, Offset, M24SR_WRITE_MAX_NBBYTE , pData);
      Offset += M24SR_WRITE_MAX_NBBYTE;
      pData += M24SR_WRITE_MAX_NBBYTE;
      DataSize -= M24SR_WRITE_MAX_NBBYTE;
    }while( DataSize > M24SR_WRITE_MAX_NBBYTE && status == M24SR_ACTION_COMPLETED);
    if( status == M24SR_ACTION_COMPLETED && DataSize)
      status = M24SR_UpdateBinary (M24SR_I2C_ADDR, Offset, (uint8_t)(DataSize) , pData);
  }
  else
    status = M24SR_UpdateBinary (M24SR_I2C_ADDR, Offset, (uint8_t)(DataSize) , pData);

  return status;
}

/**
  * @brief  This fonction activate the need of a password for next read access
  * @param  pCurrentWritePassword : Write password is needed to have the right to enable Read Password
  * @param  pNewPassword : The password that will be requiered for next read access
  * @retval SUCCESS : Read password is activated
  * @retval ERROR : operation does not complete  
  */
uint16_t NFC_TT4_EnableReadPassword( uint8_t* pCurrentWritePassword, uint8_t* pNewPassword)
{
  uint16_t status = SUCCESS;
  
  if(M24SR_Verify(M24SR_I2C_ADDR, M24SR_WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
  {
    /* Set new password */
    M24SR_ChangeReferenceData (M24SR_I2C_ADDR, M24SR_READ_PWD, pNewPassword );
    M24SR_EnableVerificationRequirement(M24SR_I2C_ADDR, M24SR_READ_PWD );
    status = SUCCESS;
  }
  else
  {
    /* M24SR already lock but password not known */
    status = ERROR;
  }
  
  return status;
}

/**
  * @brief  This fonction desactivate the need of a password for next read access
  * @param  pCurrentWritePassword : Write password is needed to have the right to disable Read Password
  * @retval SUCCESS : Read password is desactivated
  * @retval ERROR : operation does not complete  
  */
uint16_t NFC_TT4_DisableReadPassword( uint8_t* pCurrentWritePassword)
{
  uint16_t status = SUCCESS;
  
  if(M24SR_Verify(M24SR_I2C_ADDR, M24SR_WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
  {
    /* Set new password */
    M24SR_DisableVerificationRequirement(M24SR_I2C_ADDR, M24SR_READ_PWD );
    status = SUCCESS;
  }
  else
  {
    /* M24SR already lock but password not known */
    status = ERROR;
  }
  
  return status;
}

/**
  * @brief  This fonction activate the need of a password for next write access
  * @param  pCurrentWritePassword : Write password must be prensented to have the right to modify write Password
  * @param  pNewPassword : The password that will be requiered for next write access
  * @retval SUCCESS : Write password is activated
  * @retval ERROR : operation does not complete   
  */
uint16_t NFC_TT4_EnableWritePassword( uint8_t* pCurrentWritePassword, uint8_t* pNewPassword)
{
  uint16_t status;
  
  /* check we have the good password */
  if (M24SR_Verify(M24SR_I2C_ADDR, M24SR_WRITE_PWD ,0x10 ,pCurrentWritePassword )== M24SR_PWD_CORRECT)
  {
    /* Set new password */
    M24SR_ChangeReferenceData (M24SR_I2C_ADDR, M24SR_WRITE_PWD, pNewPassword );
    M24SR_EnableVerificationRequirement(M24SR_I2C_ADDR, M24SR_WRITE_PWD );
    status = SUCCESS;
  }
  else /* we don't have the good password */
  {
    status = ERROR;
  }

  return status;
}

/**
  * @brief  This fonction desactivate the need of a password for next write access
  * @param  pCurrentWritePassword : Write password must be prensented to have the right to disable it
  * @retval SUCCESS : Write password is desactivated
  * @retval ERROR : operation does not complete   
  */
uint16_t NFC_TT4_DisableWritePassword( uint8_t* pCurrentWritePassword)
{
  uint16_t status = SUCCESS;
  
  if(M24SR_Verify(M24SR_I2C_ADDR, M24SR_WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
  {
    M24SR_DisableVerificationRequirement(M24SR_I2C_ADDR, M24SR_WRITE_PWD );
    status = SUCCESS;
  }
  else
  {
    /* M24SR already lock but password not known */
    status = ERROR;
  }
  
  return status;
}

/**
  * @brief  This fonction desactivate the need of read and write password for next access
  * @param  pSuperUserPassword : I2C super user password to overwrite read and write password
  * @retval SUCCESS : M24SR access is now free (no password needed)
  * @retval ERROR : operation does not complete   
  */
uint16_t NFC_TT4_DisableAllPassword( uint8_t* pSuperUserPassword)
{
  uint16_t status = SUCCESS;
  
  if(M24SR_Verify(M24SR_I2C_ADDR, M24SR_I2C_PWD ,0x10 ,pSuperUserPassword ) == M24SR_PWD_CORRECT)
  {
    M24SR_DisablePermanentState(M24SR_I2C_ADDR, M24SR_READ_PWD );
    M24SR_DisablePermanentState(M24SR_I2C_ADDR, M24SR_WRITE_PWD );
    
    M24SR_DisableVerificationRequirement(M24SR_I2C_ADDR, M24SR_READ_PWD );
    M24SR_DisableVerificationRequirement(M24SR_I2C_ADDR, M24SR_WRITE_PWD );
    
    /* reset password */
    M24SR_ChangeReferenceData (M24SR_I2C_ADDR, M24SR_READ_PWD, pSuperUserPassword );
    M24SR_ChangeReferenceData (M24SR_I2C_ADDR, M24SR_WRITE_PWD, pSuperUserPassword );
    status = SUCCESS;
  }
  else
  {
    /* M24SR already lock but password not known */
    status = ERROR;
  }
  
  return status;
}

/**
  * @brief  This fonction enable read only mode
  * @param  pCurrentWritePassword : Write password is needed to have right to enable read only mode
  * @retval SUCCESS : M24SR access is now forbidden in write mode
  * @retval ERROR : operation does not complete   
  */
uint16_t NFC_TT4_EnableReadOnly( uint8_t* pCurrentWritePassword)
{
  uint16_t status = SUCCESS;
  
  if(M24SR_Verify(M24SR_I2C_ADDR, M24SR_WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
  {
    M24SR_EnablePermanentState(M24SR_I2C_ADDR, M24SR_WRITE_PWD ); /* lock write to have read only */
    status = SUCCESS;
  }
  else
  {
    /* M24SR already lock but password not known */
    status = ERROR;
  }
  
  return status;
}

/**
  * @brief  This fonction disable read only mode
  * @param  pCurrentWritePassword : Write password is needed to have right to disable read only mode
  * @retval SUCCESS : M24SR write access is now allowed 
  * @retval ERROR : operation does not complete   
  */
uint16_t NFC_TT4_DisableReadOnly( uint8_t* pCurrentWritePassword)
{
  uint16_t status = SUCCESS;
  
  if(M24SR_Verify(M24SR_I2C_ADDR, M24SR_I2C_PWD ,0x10 ,I2CPassword ) == M24SR_PWD_CORRECT)
  {
    M24SR_DisablePermanentState(M24SR_I2C_ADDR, M24SR_WRITE_PWD ); /* disable write protection to disable read only mode */
    M24SR_DisableVerificationRequirement(M24SR_I2C_ADDR, M24SR_WRITE_PWD );
    status = SUCCESS;
  }
  else
  {
    /* we don't have the good I2C password nothing to do anymore */
    status = ERROR;
  }
  
  return status;
}

/**
  * @brief  This fonction enable write only mode
  * @param  pCurrentWritePassword : Write password is needed to have right to enable write only mode
  * @retval SUCCESS : M24SR access is now forbidden in read mode
  * @retval ERROR : operation does not complete   
  */
uint16_t NFC_TT4_EnableWriteOnly( uint8_t* pCurrentWritePassword)
{
  uint16_t status = SUCCESS;
  
  if(M24SR_Verify(M24SR_I2C_ADDR, M24SR_WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
  {
    M24SR_EnablePermanentState(M24SR_I2C_ADDR, M24SR_READ_PWD ); /* disable read access and keep write */
    status = SUCCESS;
  }
  else
  {
    /* M24SR already lock but password not known */
    status = ERROR;
  }

  return status;
}

/**
  * @brief  This fonction disable write only mode
  * @param  pCurrentWritePassword : Write password is needed to have right to disable write only mode
  * @retval SUCCESS : M24SR read access is now allowed 
  * @retval ERROR : operation does not complete   
  */
uint16_t NFC_TT4_DisableWriteOnly( uint8_t* pCurrentWritePassword)
{
  uint16_t status = SUCCESS;
  
  if(M24SR_Verify(M24SR_I2C_ADDR, M24SR_I2C_PWD ,0x10 ,I2CPassword ) == M24SR_PWD_CORRECT)
  {
    M24SR_DisablePermanentState(M24SR_I2C_ADDR, M24SR_READ_PWD ); /* disable write only -> enable write acces */
    M24SR_DisableVerificationRequirement(M24SR_I2C_ADDR, M24SR_READ_PWD );
    status = SUCCESS;
  }
  else
  {
    /* M24SR already lock but password not known */
    status = ERROR;
  }

  return status;
}

/**
  * @brief  This function configure GPO purpose for RF session
  * @param  GPO_config: GPO configuration to set
  * @param  mode: M24SR_GPO_MODE enum: select RF or I2C,  
  * @retval Status : Status of the operation.
  */
uint16_t NFC_TT4_ManageGPO( uint8_t gpo_config, uint8_t mode)
{
  uint16_t status;

  if( mode == M24SR_RF_GPO)
  {
    status = M24SR_ManageRFGPO (M24SR_I2C_ADDR, gpo_config );
  }
  else
  {
    status = M24SR_ManageI2CGPO (M24SR_I2C_ADDR, gpo_config );
  }
  return status;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */




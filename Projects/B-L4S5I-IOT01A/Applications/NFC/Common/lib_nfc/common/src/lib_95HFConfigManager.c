/**
  ******************************************************************************
  * @file    lib_95HFConfigManager.c 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides set of firmware functions to manages device modes. 
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

/* Includes ------------------------------------------------------------------------------ */
#include "lib_95HFConfigManager.h"

/** @addtogroup _95HF_Libraries
 * 	@{
 *	@brief  <b>This is the library used by the whole 95HF family (RX95HF, CR95HF, ST95HF) <br />
 *				  You will find ISO libraries ( 14443A, 14443B, 15693, ...) for PICC and PCD <br />
 *				  The libraries selected in the project will depend of the application targetted <br />
 *				  and the product chosen (RX95HF emulate PICC, CR95HF emulate PCD, ST95HF can do both)</b>
 */

/** @addtogroup Config_Manager
 * 	@{
 *	@brief  This part of the library manage the configuration of the chip.
 */
/**
 * @brief  This buffer contains the data send/received by xx95HF
 */
extern uint8_t	u95HFBuffer[RFTRANS_95HF_MAX_BUFFER_SIZE+3];
extern bool RF_TechnoFounded;
extern ISO14443A_CARD ISO14443A_Card;
extern bool lockKEYUpDown;

extern bool LLCPSymActivated;
extern bool PP2PSymActivated;

extern uint16_t random(uint16_t min, uint16_t max);

/** @addtogroup Configuration_Manager
 * 	@{
 *	@brief  This file is used to select a configuration, PICC or PCD or Initiator/Target.
*/

/** @addtogroup lib_ConfigManager_Private_Functions
 * 	@{
 */
static void ConfigManager_Init( void);
static int8_t ConfigManager_IDN(uint8_t *pResponse);
static void ConfigManager_Start(void );
static int8_t ConfigManager_PORsequence( void );

#ifdef ST95HF
static void IsSymActivated(void);
static uint8_t RandumStart = 0;
#endif /* ST95HF */

/* Variables for the different modes */
DeviceMode_t devicemode = UNDEFINED_MODE;
TagType_t nfc_tagtype = UNDEFINED_TAG_TYPE;

/* Variable to know IC version */
IC_VERSION IcVers = QJE;  /* default set last IC version */

bool StopProcess = false;
bool TargetMode = true;
extern volatile bool	uAppliTimeOut;
uint8_t TagUID[16];

#if defined(ST95HF) || defined(RX95HF)
	PICCEMULATOR_SELECT_TAG_TYPE commandReceived = PICCEMULATOR_TAG_TYPE_UNKNOWN;
#endif

/**
 *	@brief  This function initialize the PICC
 *  @param  None 
 *  @retval None
 */
static void ConfigManager_Init( void)
{
	/* initialize the structure of the Rf tranceiver */
	drv95HF_InitConfigStructure ();
	
#ifdef SPI_INTERRUPT_MODE_ACTIVATED	
	/* inform driver to use interrupt mode */
	drv95HF_EnableInterrupt ( );
#endif /* SPI_INTERRUPT_MODE_ACTIVATED */
	
	/* configure the Serial interface to communicate with the RF transceiver */
	drv95HF_InitilizeSerialInterface ( );
}

/**
 *	@brief  this function sends an IDN command to the PICC device
 *  @param  pResponse : pointer on the PICC device reply
 *  @retval PICC_SUCCESSCODE : the function is succesful 
 */
static int8_t ConfigManager_IDN(uint8_t *pResponse)
{
	uint8_t DataToSend[] = {IDN	,0x00};

	/* send the command to the PICC and retrieve its response */
	drv95HF_SendReceive(DataToSend, pResponse);

	return MANAGER_SUCCESSCODE;
}

/**
 *	@brief  This function set a variable to inform Manager that a task is on going
 *  @param  none
 *  @retval none
 */
static void ConfigManager_Start( void )
{
	StopProcess = false;
	uAppliTimeOut = false;
  
  devicemode = UNDEFINED_MODE;
  nfc_tagtype = UNDEFINED_TAG_TYPE;
}

/**
 *	@brief  This function sends POR sequence. It is used to initialize chip after a POR.
 *  @param  none
 *  @retval MANAGER_ERRORCODE_PORERROR : the POR sequence doesn't succeded
 *  @retval MANAGER_SUCCESSCODE : chip is ready
 */
static int8_t ConfigManager_PORsequence( void )
{
	uint16_t NthAttempt=0;
	uint8_t command[]= {ECHO};

	/* Power up sequence: Pulse on IRQ_IN to select UART or SPI mode */
	drv95HF_SendIRQINPulse();
	
	/* SPI Reset */
	if(drv95HF_GetSerialInterface() == RFTRANS_95HF_INTERFACE_SPI)
	{
		drv95HF_ResetSPI();		
	}
	
	do
	{
		/* send an ECHO command and checks response */
		drv95HF_SendReceive(command, u95HFBuffer);

		if (u95HFBuffer[0]==ECHORESPONSE)
			return MANAGER_SUCCESSCODE;	

		/* if the SPI interface is selected then send a reset command*/
		if(drv95HF_GetSerialInterface() == RFTRANS_95HF_INTERFACE_SPI)
		{	
			drv95HF_ResetSPI();				
		}
#ifdef CR95HF		
		/* if the UART interface is selected then send 255 ECHO commands*/
		else if(drv95HF_GetSerialInterface() == RFTRANS_95HF_INTERFACE_UART)
		{
			do {
				/* send an ECHO command and checks response */
				drv95HF_SendReceive(command, u95HFBuffer);
				if (u95HFBuffer[0] == ECHORESPONSE)
					return MANAGER_SUCCESSCODE;	
			}while(NthAttempt++ < RFTRANS_95HF_MAX_ECHO);
		}
#endif /* CR95HF */
	} while (u95HFBuffer[0]!=ECHORESPONSE && NthAttempt++ <5);

	return MANAGER_ERRORCODE_PORERROR;
}


/**
  * @}
  */ 

 /** @addtogroup lib_ConfigManager_Public_Functions
 * 	@{
 */

/**
 *	@brief  This interface function inform Manager that current task must be stopped
 *  @param  none
 *  @retval none
 */
void ConfigManager_Stop(void )
{
	StopProcess = true;
}

/**
 * @brief  This function initialize the NFC chip 
 * @brief  Physical communication with chip enabled, RF communication not enabled
 * @param  None
 * @retval None
 */
void ConfigManager_HWInit (void)
{
	
	/* Initialize HW according to protocol to use */
	ConfigManager_Init();
	
	/* initilialize the RF transceiver */
	if (ConfigManager_PORsequence( ) != MANAGER_SUCCESSCODE)
	{
		/* nothing to do, this is a trap for debug purpose you can use it to detect HW issue */
		/* or GPIO config issue */
	}
	
	/* Retrieve the IC version of the chip */
	ConfigManager_IDN(u95HFBuffer);
	
	IcVers = (IC_VERSION) (u95HFBuffer[ROM_CODE_REVISION_OFFSET]);

}


#ifdef ST95HF
/**
 * @brief  This function launch an automatic mode where NFC chip change is state 
 * @brief  (card reader, card emulation, P2P)
 * @param  *pManagerConfig configuration that must be done by the NFC chip
 * @retval None
 */
void ConfigManager_AutoMode (MANAGER_CONFIG *pManagerConfig)
{
	
	ConfigManager_Start();

  if (RandumStart == 0)
		goto PCD;
	else if (RandumStart == 1)
		goto PICC;
	else if (RandumStart == 2)
		goto P2P;
  
	pManagerConfig->Result = P2P_NOTHING;
	

PCD:    
		/* Add a delay before switching mode (in case this function is looped )*/
		HAL_Delay(5);
	
		/** PCD looking for a tag or a card emulator **/
		if(!StopProcess&&(pManagerConfig->SelectedMode&SELECT_PCD) != 0)
		{
			if (ConfigManager_TagHunting(pManagerConfig->PcdMode) != TRACK_NOTHING)
				goto exit;	
			else
				devicemode = UNDEFINED_MODE;
		}	
PICC:	
		/* add a delay for smartphone, if they have been detected in Card Emul they need time to restart a tag hunting */
		HAL_Delay(5);	
		
		/** PICC looking for a PCD **/
		if(!StopProcess&&(pManagerConfig->SelectedMode&SELECT_PICC) != 0)
		{
			/* TT3 emulation */
			if(!StopProcess&&(pManagerConfig->PiccMode&TRACK_NFCTYPE3) != 0)
			{
				if (ConfigManager_TagEmulation(PICCEMULATOR_TAG_TYPE_3,pManagerConfig->timeoutEmul,pManagerConfig->timeoutReadEmul) != TRACK_NOTHING)
					goto exit;
				else
				{
					devicemode = UNDEFINED_MODE;
					nfc_tagtype = UNDEFINED_TAG_TYPE;
				}
			}
			/* TT4A emulation */
			if(!StopProcess&&(pManagerConfig->PiccMode&TRACK_NFCTYPE4A) != 0)
			{
				if (ConfigManager_TagEmulation(PICCEMULATOR_TAG_TYPE_4A,pManagerConfig->timeoutEmul,pManagerConfig->timeoutReadEmul) != TRACK_NOTHING)
					goto exit;
				else
				{
					devicemode = UNDEFINED_MODE;
					nfc_tagtype = UNDEFINED_TAG_TYPE;
				}
			}
			/* TT4B emulation */
			if(!StopProcess&&(pManagerConfig->PiccMode&TRACK_NFCTYPE4B) != 0)
			{
				if (ConfigManager_TagEmulation(PICCEMULATOR_TAG_TYPE_4B,pManagerConfig->timeoutEmul,pManagerConfig->timeoutReadEmul) != TRACK_NOTHING)
					goto exit;
				else
				{
					devicemode = UNDEFINED_MODE;
					nfc_tagtype = UNDEFINED_TAG_TYPE;
				}
			}
		}
		
P2P:		
		/* Add a delay before switching mode */
		HAL_Delay(5);	
		
		/** Initiator/Target NFC Forum mode **/
		if(!StopProcess&&(pManagerConfig->SelectedMode&SELECT_P2P) != 0)
		{
			pManagerConfig->Result = ConfigManager_P2P(pManagerConfig->P2pMode, 200);
			StartAppliTimeOut(200); /* start a timeout to not stay in a infinite loop */
			if( pManagerConfig->Result == TARGET_LLCPA)
			  ConfigManager_P2PTarget(IsSymActivated);
			else if( pManagerConfig->Result == TARGET_LLCPF)
			  ConfigManager_P2PTarget(IsSymActivated);
			else if( pManagerConfig->Result == INITIATOR_LLCPA)
			  ConfigManager_P2PInitiator(IsSymActivated);
			else if( pManagerConfig->Result == INITIATOR_LLCPF)
			  ConfigManager_P2PInitiator(IsSymActivated);
			else
			{
				devicemode = UNDEFINED_MODE;
				nfc_tagtype = UNDEFINED_TAG_TYPE;
		}			
			
			if(!LLCPSymActivated)
					pManagerConfig->Result = P2P_NOTHING;
	}	
  
exit:	
  /* We will start with a new config each time function is called */
  RandumStart++;
  if( RandumStart >=3)
    RandumStart=0;  
	
}

void IsSymActivated(void)
{
	if(LLCPSymActivated)
		ConfigManager_Stop();
	else if(uAppliTimeOut)
		ConfigManager_Stop();
}
	
#endif

#if defined(ST95HF) || defined(CR95HF)
/**  
* @brief  	this function searches if a NFC or RFID tag is in the RF field. 
* @brief  	The method used is this described by the NFC specification
* @param  	tagsToFind : Flags to select the different kinds of tag to track, same as return value
* @retval 	TRACK_NOTHING : No tag in the RF field
* @retval 	TRACK_NFCTYPE1 : A NFC type1 tag is present in the RF field
* @retval 	TRACK_NFCTYPE2 : A NFC type2 tag is present in the RF field
* @retval 	TRACK_NFCTYPE3 : A NFC type3 tag is present in the RF field
* @retval 	TRACK_NFCTYPE4A : A NFC type4A tag is present in the RF field
* @retval 	TRACK_NFCTYPE4B : A NFC type4B tag is present in the RF field
* @retval 	TRACK_NFCTYPE5 : A ISO/IEC 15693 type A tag is present in the RF field
*/
uint8_t ConfigManager_TagHunting ( uint8_t tagsToFind )
{
	// Start the config manager
	ConfigManager_Start();
	
	/******* NFC type 1 ********/
	if (tagsToFind&TRACK_NFCTYPE1)
	{
		PCD_FieldOff();
		HAL_Delay(5);
		ISO14443A_Init( );
		if(ISO14443A_IsPresent() == RESULTOK)
		{		
			if(TOPAZ_ID(TagUID) == RESULTOK)
				return TRACK_NFCTYPE1;	
		}
	}

	/******* NFC type 2 and 4A ********/
	if ((tagsToFind&TRACK_NFCTYPE2) || (tagsToFind&TRACK_NFCTYPE4A))
	{
		PCD_FieldOff();
		HAL_Delay(5);
		ISO14443A_Init( );
		if(ISO14443A_IsPresent() == RESULTOK)
		{			
			if(ISO14443A_Anticollision() == RESULTOK)
			{	
				if (((ISO14443A_Card.SAK&0x60) == 0x00) && tagsToFind&TRACK_NFCTYPE2) /* TT2 */
					return TRACK_NFCTYPE2;
				else if (((ISO14443A_Card.SAK&0x20) != 0x00) && tagsToFind&TRACK_NFCTYPE4A)/* TT4A */
					return TRACK_NFCTYPE4A;
			}
		}
	}

	/******* NFC type 3 ********/
	if (tagsToFind&TRACK_NFCTYPE3)
	{
		PCD_FieldOff();
		HAL_Delay(5);
		FELICA_Initialization();
		if(FELICA_IsPresent() == RESULTOK )
			return TRACK_NFCTYPE3;
	}

	/******* NFC type 4B ********/
	if (tagsToFind&TRACK_NFCTYPE4B)
	{
		PCD_FieldOff();
		HAL_Delay(5);
		if(ISO14443B_IsPresent() == RESULTOK )
		{
			if(ISO14443B_Anticollision() == RESULTOK)
			{
				return TRACK_NFCTYPE4B;
			}
		}
	}

	/******* ISO15693 ********/
	if (tagsToFind&TRACK_NFCTYPE5)
	{
		PCD_FieldOff();
		HAL_Delay(5);
		if(ISO15693_GetUID (TagUID) == RESULTOK)	
			return TRACK_NFCTYPE5;
	}

	
	// Turn off the field if no tag has been detected
	PCD_FieldOff();
	
	/* No tag found */
	return TRACK_NOTHING;
}
#endif

#if defined(ST95HF) || defined(RX95HF)
/**  
* @brief  	this function configure the chip in tag emulation mode 
* @param  	TagEmulated : mode of emulation
* @param  	timeoutEmul : time to receive the first command corresponding to the emulated tag (0 for infinite)
* @param  	timeoutReadEmul : time after reception of the first command (useless if timeoutEmul=0)
* @retval 	TRACK_NOTHING : No tag in the RF field
*/
uint8_t ConfigManager_TagEmulation (PICCEMULATOR_SELECT_TAG_TYPE TagEmulated, uint16_t timeoutEmul, uint16_t timeoutReadEmul)
{
	// This boolean is used to know when the first command is found
	bool readerFound = false;
	
	// Start the configmanager
	ConfigManager_Start();
	
	PICCEmul_InitPICCEmulation(TagEmulated);
	
	// If the timeout is set to 0 it means that we want to emulate a tag until the user stops
	if( timeoutEmul != 0)
		StartAppliTimeOut(timeoutEmul);

	// This variable is used to check if the command received correspond to the good type of tag
	commandReceived = PICCEMULATOR_TAG_TYPE_UNKNOWN;
	
	// While there is no interruption by the user nor timeout
	while(!StopProcess && !uAppliTimeOut)
	{
		// Function to manage the emulation
		PICCEmul_ManagePICCEmulation();
		
		// if data are exchanged change the timeout to increase the time for reader to retrieve all the data
		if (commandReceived == TagEmulated && timeoutEmul != 0 && !readerFound)
		{
			StopAppliTimeOut();
			StartAppliTimeOut(timeoutReadEmul);
			readerFound = true;
		}
	}
	
	// Disable the emulation
	PICCEmul_DisablePICCEmulation();
	if (readerFound)
		return TRACK_ALL;
	else
		return TRACK_NOTHING;
}

/**  
* @brief  	this function configure the chip in card emulation mode 
* @param  	TagEmulated : tag emulated force the NFC techno
*/
void ConfigManager_CardEmulation (PICCEMULATOR_SELECT_TAG_TYPE TagEmulated, void(*pCallBackFunction)(void))
{
	// Start the configmanager
	ConfigManager_Start();
	
	//Initialize chip in PICC mode
	PICCEmul_InitPICCEmulation(TagEmulated);
	
	// While there is no interruption by the user nor timeout
	while(!StopProcess)
	{
		// Function to manage the emulation
		PICCEmul_ManagePICCEmulation();
		
		// If caller has given a CallBackFunction
		if( pCallBackFunction != NULL)
			(*pCallBackFunction)();
	}
	
	// Disable the emulation
	PICCEmul_DisablePICCEmulation();

}

#endif

#ifdef ST95HF
/**  
* @brief  	this function configure the chip in P2P initiator or target 
* @param  	P2Pmode : mode of emulation
* @param  	HAL_Delay : chip must stay in the tag emulation mode for xx ms
* @retval 	TRACK_NOTHING : No tag in the RF field
*/
uint8_t ConfigManager_P2P(uint8_t P2Pmode, uint16_t timeout)
{
	uint8_t initState = 0;
	PICCEMULATOR_STATE EmulState = PICCSTATE_UNKNOWN;	
	
	/* Enable LLCP, SNEP and SDP services */
	ConfigManager_ActivateP2PStack();
	
	ConfigManager_Start(); /* Initialize StopProcess to false */
	
	/* Random state (now need to use only NFC-DEP)*/
	initState = random(4,7);
	
	if (initState == 0)
		goto INFCA;
	else if (initState == 1)
		goto TNFCA;
	else if (initState == 2)
		goto INFCF;
	else if (initState == 3)
		goto TNFCF;
	else if (initState == 4)
		goto TLLCPA;
	else if (initState == 5)
		goto TLLCPF;
	else if (initState == 6)
		goto ILLCPA;
	else if (initState == 7)
		goto ILLCPF;
	
	while(!StopProcess)
	{
		
INFCA: /* Initiator NFCA */
		if( (P2Pmode & INITIATOR_NFCA) == INITIATOR_NFCA)
		{
			PCDNFCDEP_Init(NFCDEP_ON_NFCA, false);
			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			else
				StartAppliTimeOut(10); /* Force a timeout to have field ON/OFF */
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				if(PCDNFCDEP_IsPresentNFCA() == PCDNFCDEP_SUCESSSCODE)
					return INITIATOR_NFCA;
			}
		}
		
INFCF: /* Initiator NFCF */
		if( (P2Pmode & INITIATOR_NFCF) == INITIATOR_NFCF)	
		{
			PCDNFCDEP_Init(NFCDEP_ON_NFCF, false);
			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			else
				StartAppliTimeOut(10); /* Force a timeout to have field ON/OFF */
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				if( PCDNFCDEP_IsPresentNFCF() == PCDNFCDEP_SUCESSSCODE)
					return INITIATOR_NFCF;
			}
		}		
		
TNFCA: /* Target NFCA */
		if( (P2Pmode & TARGET_NFCA) == TARGET_NFCA)
		{						
			PICCEmul_InitPICCEmulation(PICCEMULATOR_NFCDEP_TYPE_A);
			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				EmulState = PICCEmul_ManagePICCEmulation ();	
				/* if data are exchanged inform appli */
				if( EmulState == PICCSTATE_DATA_EXCHANGED  && RF_TechnoFounded == true)
					return TARGET_NFCA;
			}
		}

TNFCF: /* Target NFCF */
		if( (P2Pmode & TARGET_NFCF) == TARGET_NFCF)
		{		
			PICCEmul_InitPICCEmulation(PICCEMULATOR_NFCDEP_TYPE_F);
			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				EmulState = PICCEmul_ManagePICCEmulation ();	
				/* if data are exchanged inform appli */
				if( EmulState == PICCSTATE_DATA_EXCHANGED && RF_TechnoFounded == true)
					return TARGET_NFCF;
			}
		}		
				
ILLCPA: /* Initiator LLCP on NFCA */
		TargetMode = false;
		if( (P2Pmode & INITIATOR_LLCPA) == INITIATOR_LLCPA)
		{
			/* initialize NFCDEP */
			PCDNFCDEP_Init(NFCDEP_ON_NFCA, true);
			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			else
				StartAppliTimeOut(10); /* Force a timeout to have field ON/OFF */
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				if(PCDNFCDEP_IsPresentNFCA() == PCDNFCDEP_SUCESSSCODE)
					return INITIATOR_LLCPA;
			}
		}
		
ILLCPF: /* Initiator LLCP on NFCF */
		TargetMode = false;
		if( (P2Pmode & INITIATOR_LLCPF) == INITIATOR_LLCPF)	
		{
			/* initialize NFCDEP */
			PCDNFCDEP_Init(NFCDEP_ON_NFCF, true);
			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			else
				StartAppliTimeOut(10); /* Force a timeout to have field ON/OFF */
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				if( PCDNFCDEP_IsPresentNFCF() == PCDNFCDEP_SUCESSSCODE)
					return INITIATOR_LLCPF;
			}
		}				
		
TLLCPA: /* Target LLCP on NFCA */		
		TargetMode = true;
		if( (P2Pmode & TARGET_LLCPA) == TARGET_LLCPA)
		{					
			PICCEmul_InitPICCEmulation(PICCEMULATOR_LLCP_TYPE_A);
			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				EmulState = PICCEmul_ManagePICCEmulation ();	
				/* if data are exchanged inform appli */
				if( EmulState == PICCSTATE_DATA_EXCHANGED && RF_TechnoFounded == true)
					return TARGET_LLCPA;
			}
		}
		
TLLCPF: /* Target LLCP on NFCF */
		TargetMode = true;
		if( (P2Pmode & TARGET_LLCPF) == TARGET_LLCPF)
		{				
			PICCEmul_InitPICCEmulation(PICCEMULATOR_LLCP_TYPE_F);
			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				EmulState = PICCEmul_ManagePICCEmulation ();	
				
				/* if data are exchanged inform appli */
				if( EmulState == PICCSTATE_DATA_EXCHANGED && RF_TechnoFounded == true)
					return TARGET_LLCPF;
			}
		}		
		
                /* Caller don't want to stay in a infinite loop */
		if(timeout)
		  StopProcess = true;
	}
	
	/* If we exit due to timeout (nothing found) */
	ConfigManager_DesactivateP2PStack();
	
	return P2P_NOTHING;
}

/**  
* @brief  	This function configure the SW ressource needed for NFC forum P2P communication 
* @param  	None
* @retval 	None
*/
void ConfigManager_ActivateP2PStack( void )
{
	/* Initialize LLCP */
	LLCPSymActivated = false;
	LLCP_Init();
	/* SDP server initialization */
 // SDP_Init( SERVER_SDP, SERVER_SDP_URI );
  /* SNEP server initialization */
  SNEP_Init( SERVER_SNEP, SERVER_SNEP_URI );	
}


/**  
* @brief  	This function configure the SW ressource needed for NFC forum P2P communication 
* @param  	None
* @retval 	None
*/
void ConfigManager_DesactivateP2PStack( void )
{
  /* Remove SNEP server */
  SNEP_RemoveService(SERVER_SNEP);
	/* Remove SDP server */
  //SDP_Disconnect( );	
	/* Remove LLCP server */
	// LLCP_Stop(); //Nothing to do now as init set nbconnection to 0.
}



/**  
* @brief  	this function manage the chip in NFC Forum target mode
* @param  	pCallBackFunction : pointer on the application function to be called
*/
uint8_t ConfigManager_P2PTarget (void(*pCallBackFunction)(void))
{
	PICCEMULATOR_STATE State;
	
	ConfigManager_Start(); /* Initialize StopProcess to false */
	
	// While there is no interruption by the user nor timeout
	while(!StopProcess)
	{
		// we are in P2P target mode check if RF Data has been received
		State = PICCEmul_ManagePICCEmulation();
		
		if(State < PICCSTATE_ACTIVATED && LLCPSymActivated) /* a RF field cut off occurs */
		{
			ConfigManager_Stop();
			LLCPSymActivated=false;
			return MANAGER_ERRORCODE_COMMUNICATION_LOST;
		}
		else
		{	
			// we are in P2P target mode check for RF Data consistency
			//Server_SDP( );
			Server_SNEP( );
		}
		
		// If caller has given a CallBackFunction
		if( pCallBackFunction != NULL )
			(*pCallBackFunction)();
	}
	
	/* Disable LLCP, SNEP and SDP services */
	ConfigManager_DesactivateP2PStack();
	
	// Disable the emulation
	PICCEmul_DisablePICCEmulation();
	
	return MANAGER_SUCCESSCODE;
}

/**  
* @brief  	this function manage the chip in NFC Forum target mode
* @param  	pCallBackFunction : pointer on the application function to be called
*/
uint8_t ConfigManager_P2PInitiator (void(*pCallBackFunction)(void))
{	
	
	ConfigManager_Start(); /* Initialize StopProcess to false */

	// While there is no interruption by the user nor timeout
	while(!StopProcess)
	{
		// we are in P2P initiator mode, we can send/receive request.
		//Server_SDP( );
		Server_SNEP( );
		
		/* Allow LLCP layer to take care of the message if error stop */
		if( LLCP_Send() != LLCP_SUCCESS_CODE)
		{	
			ConfigManager_Stop();
			return MANAGER_ERRORCODE_COMMUNICATION_LOST;
		}

		// If caller has given a CallBackFunction
		if( pCallBackFunction != NULL && LLCPSymActivated == true)
			(*pCallBackFunction)();
	}
	
	/* Disable LLCP, SNEP and SDP services */
	ConfigManager_DesactivateP2PStack();
	
	PCD_FieldOff();
	
	return MANAGER_SUCCESSCODE;
}

/**  
* @brief  	this function configure the chip in Proprietary P2P initiator or target 
* @param  	P2Pmode : mode of emulation
* @param  	HAL_Delay : chip must stay in the tag emulation mode for xx ms
* @retval 	TRACK_NOTHING : No tag in the RF field
*/
uint8_t ConfigManager_P2PP(uint8_t P2Pmode, uint16_t timeout)
{
	uint8_t initState = 0;
	PICCEMULATOR_STATE EmulState = PICCSTATE_UNKNOWN;	
	
	ConfigManager_Start(); /* Initialize StopProcess to false */
	
	/* Random state */
	initState = random(0,2);
	
	if (initState == 0)
		goto INFCA;
	else if (initState == 1)
		goto TNFCA;
	
	while(!StopProcess)
	{
		
INFCA: /* Initiator NFCA */
		if( (P2Pmode & PP2P_INITIATOR_NFCA) == PP2P_INITIATOR_NFCA)
		{			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			//else
				//StartAppliTimeOut(10); /* Force a timeout to have field ON/OFF */
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				if(ConfigManager_TagHunting(TRACK_NFCTYPE4A) == TRACK_NFCTYPE4A)
					return PP2P_INITIATOR_NFCA;
			}
		}
		
		
TNFCA: /* Target NFCA */
		if( (P2Pmode & PP2P_TARGET_NFCA) == PP2P_TARGET_NFCA)
		{						
			PICCEmul_InitPICCEmulation(PICCEMULATOR_TAG_TYPE_4A);
			
			/* Start a timeout to change configuration if needed */
			if( timeout != 0)
				StartAppliTimeOut(timeout);
			
			while(!StopProcess && !uAppliTimeOut )
			{	
				EmulState = PICCEmul_ManagePICCEmulation ();	
				
				/* if data are exchanged inform appli */
				if( EmulState == PICCSTATE_DATA_EXCHANGED  && RF_TechnoFounded == true)
					return PP2P_TARGET_NFCA;
			}
		}
		
	}
	return P2P_NOTHING;
}

/**  
* @brief  	this function manage the chip in NFC Forum target mode
* @param  	pCallBackFunction : pointer on the application function to be called
*/
uint8_t ConfigManager_P2PPInitiator (void(*pCallBackFunction)(void))
{	
	/* Perform the server Initialization */
	PP2P_ServerInit();
	
	// While there is no interruption by the user nor timeout
	while(!StopProcess)
	{
		/* Allow PP2P_Server to take care of the data if error stop */
		if( PP2P_Server() != PP2P_SUCCESS)
		{	
			ConfigManager_Stop();
			return MANAGER_ERRORCODE_COMMUNICATION_LOST;
		}

		// If caller has given a CallBackFunction
		if( pCallBackFunction != NULL && PP2PSymActivated)
			(*pCallBackFunction)();
	}
	
	PCD_FieldOff();
	
	return MANAGER_SUCCESSCODE;
}

/**  
* @brief  	this function manage the chip in NFC Forum target mode
* @param  	pCallBackFunction : pointer on the application function to be called
*/
uint8_t ConfigManager_PP2PTarget (void(*pCallBackFunction)(void))
{
	PICCEMULATOR_STATE State;
	
	// Init
	PP2P_TargetInit();
	
	// While there is no interruption by the user nor timeout
	while(!StopProcess)
	{
		// we are in P2P target mode check if RF Data has been received
		State = PICCEmul_ManagePICCEmulation();
		
		if(State < PICCSTATE_ACTIVATED && PP2PSymActivated) /* a RF field cut off occurs */
		{
			ConfigManager_Stop();
			PP2PSymActivated=false;
			return MANAGER_ERRORCODE_COMMUNICATION_LOST;
		}
		else
		{	
			/* Nothing to do */
		}
		
		// If caller has given a CallBackFunction
		if( pCallBackFunction != NULL && PP2PSymActivated == true)
			(*pCallBackFunction)();
	}
	
	// Disable the emulation
	PICCEmul_DisablePICCEmulation();
	
	return MANAGER_SUCCESSCODE;
}

#endif	

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

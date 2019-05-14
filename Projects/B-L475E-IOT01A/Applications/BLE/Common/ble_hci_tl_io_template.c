/**
 ******************************************************************************
 * @file    hci_tl_io_template.c
 * @author  MCD Application Team
 * @brief   This lower API is imposed by the MW to the applic TL (transport layer). 
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

/* Function prototypes -----------------------------------------------*/

/* HCI send request: generic API imposed by MW to the lower layers  */
/*   transport layer depends by the applic, e.g.: spi, ipcc, etc    */

int32_t hci_send_req(struct hci_request *r, uint8_t async)
{
  return 0; 
}

/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

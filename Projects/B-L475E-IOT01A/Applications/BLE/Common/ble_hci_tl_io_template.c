/**
 ******************************************************************************
 * @file    hci_tl_io_template.c
 * @author  MCD Application Team
 * @brief   This lower API is imposed by the MW to the applic TL (transport layer). 
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

/* Function prototypes -----------------------------------------------*/

/* HCI send request: generic API imposed by MW to the lower layers  */
/*   transport layer depends by the applic, e.g.: spi, ipcc, etc    */

int32_t hci_send_req(struct hci_request *r, uint8_t async)
{
  return 0; 
}


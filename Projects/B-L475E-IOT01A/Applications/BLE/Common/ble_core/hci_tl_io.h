/**
 ******************************************************************************
 * @file    hci_tl_io.h
 * @author  MCD Application Team
 * @brief   This lower API is imposed by the MW to the applic TL (transport layer). 
 *          API compianl to See Bluetooth Core v 4.0, Vol. 2, Part E.
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

#ifndef __HCI_TL_IO_H_
#define __HCI_TL_IO_H_

struct hci_request {
  uint16_t  ogf;
  uint16_t  ocf;
  uint16_t  event;
  void      *cparam;
  uint16_t  clen;
  void      *rparam;
  uint16_t  rlen;
};

/* HCI send request: generic API imposed by MW to the lower layers  */
/*   transport layer depends by the applic, e.g.: spi, ipcc, etc    */

int32_t hci_send_req(struct hci_request *r, uint8_t async);

#endif /* __HCI_TL_IO_H_ */

/******************** (C) COPYRIGHT 2014 STMicroelectronics ********************
* File Name          : bluenrg_hci.c
* Author             : AMS - HEA&RF BU
* Version            : V1.0.0
* Date               : 4-Oct-2013
* Description        : File with HCI commands for BlueNRG FW6.0 and above.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "bluenrg_private_hal_types.h"
#include "osal.h"
#include "ble_status.h"
#include "osal.h"
#include "hci_const.h"
#include "bluenrg_aci_const.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap.h"

#define MIN(a,b)            ((a) < (b) )? (a) : (b)
#define MAX(a,b)            ((a) > (b) )? (a) : (b)

#if BLUENRG_MS
tBleStatus aci_gap_init(uint8_t role, uint8_t privacy_enabled, uint8_t device_name_char_len, uint16_t* service_handle, uint16_t* dev_name_char_handle, uint16_t* appearance_char_handle)
{
  struct hci_request rq;
  gap_init_cp cp;
  gap_init_rp resp;
 
  cp.role = role;
  cp.privacy_enabled = privacy_enabled;
  cp.device_name_char_len = device_name_char_len;
    
  Osal_MemSet(&resp, 0, sizeof(resp));
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_INIT;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &resp;
  rq.rlen = GAP_INIT_RP_SIZE;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  if (resp.status) {
    return resp.status;
  }
  
  *service_handle = btohs(resp.service_handle);
  *dev_name_char_handle = btohs(resp.dev_name_char_handle);
  *appearance_char_handle = btohs(resp.appearance_char_handle);
  
  return 0;
}
#else
tBleStatus aci_gap_init(uint8_t role, uint16_t* service_handle, uint16_t* dev_name_char_handle, uint16_t* appearance_char_handle)
{
  struct hci_request rq;
  gap_init_cp cp;
  gap_init_rp resp;

  cp.role = role;
    
  Osal_MemSet(&resp, 0, sizeof(resp));
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_INIT;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &resp;
  rq.rlen = GAP_INIT_RP_SIZE;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  if (resp.status) {
    return resp.status;
  }
  
  *service_handle = btohs(resp.service_handle);
  *dev_name_char_handle = btohs(resp.dev_name_char_handle);
  *appearance_char_handle = btohs(resp.appearance_char_handle);
  
  return 0;
}
#endif

tBleStatus aci_gap_set_non_discoverable(void)
{
  struct hci_request rq;
  uint8_t status;
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_NON_DISCOVERABLE;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  return status;  
}

tBleStatus aci_gap_set_limited_discoverable(uint8_t AdvType, uint16_t AdvIntervMin, uint16_t AdvIntervMax,
              uint8_t OwnAddrType, uint8_t AdvFilterPolicy, uint8_t LocalNameLen,
              const char *LocalName, uint8_t ServiceUUIDLen, uint8_t* ServiceUUIDList,
              uint16_t SlaveConnIntervMin, uint16_t SlaveConnIntervMax)
{
  struct hci_request rq;
  uint8_t status;    
  uint8_t buffer[40];
  uint8_t indx = 0;
    
  if ((LocalNameLen+ServiceUUIDLen+14) > sizeof(buffer))
    return BLE_STATUS_INVALID_PARAMS;

  buffer[indx] = AdvType;
  indx++;
    
  AdvIntervMin = htobs(AdvIntervMin);
  Osal_MemCpy(buffer + indx, &AdvIntervMin, 2);
  indx +=  2;
    
  AdvIntervMax = htobs(AdvIntervMax);
  Osal_MemCpy(buffer + indx, &AdvIntervMax, 2);
  indx +=  2;
    
  buffer[indx] = OwnAddrType;
  indx++;
    
  buffer[indx] = AdvFilterPolicy;
  indx++;
    
  buffer[indx] = LocalNameLen;
  indx++;
    
  Osal_MemCpy(buffer + indx, LocalName, LocalNameLen);
  indx +=  LocalNameLen;

  buffer[indx] = ServiceUUIDLen;
  indx++;

  Osal_MemCpy(buffer + indx, ServiceUUIDList, ServiceUUIDLen);
  indx +=  ServiceUUIDLen;

  Osal_MemCpy(buffer + indx, &SlaveConnIntervMin, 2);
  indx +=  2;

  Osal_MemCpy(buffer + indx, &SlaveConnIntervMax, 2);
  indx +=  2;    

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_LIMITED_DISCOVERABLE;
  rq.cparam = (void *)buffer;
  rq.clen = indx;
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_set_discoverable(uint8_t AdvType, uint16_t AdvIntervMin, uint16_t AdvIntervMax,
                             uint8_t OwnAddrType, uint8_t AdvFilterPolicy, uint8_t LocalNameLen,
                             const char *LocalName, uint8_t ServiceUUIDLen, uint8_t* ServiceUUIDList,
                             uint16_t SlaveConnIntervMin, uint16_t SlaveConnIntervMax)
{
  struct hci_request rq;
  uint8_t status;    
  uint8_t buffer[40];
  uint8_t indx = 0;
  
  if ((LocalNameLen+ServiceUUIDLen+14) > sizeof(buffer))
    return BLE_STATUS_INVALID_PARAMS;

  buffer[indx] = AdvType;
  indx++;
  
  AdvIntervMin = htobs(AdvIntervMin);
  Osal_MemCpy(buffer + indx, &AdvIntervMin, 2);
  indx +=  2;
    
  AdvIntervMax = htobs(AdvIntervMax);
  Osal_MemCpy(buffer + indx, &AdvIntervMax, 2);
  indx +=  2;
    
  buffer[indx] = OwnAddrType;
  indx++;
    
  buffer[indx] = AdvFilterPolicy;
  indx++;
    
  buffer[indx] = LocalNameLen;
  indx++;
    
  Osal_MemCpy(buffer + indx, LocalName, LocalNameLen);
  indx +=  LocalNameLen;
  
  buffer[indx] = ServiceUUIDLen;
  indx++;

  Osal_MemCpy(buffer + indx, ServiceUUIDList, ServiceUUIDLen);
  indx +=  ServiceUUIDLen;  

  SlaveConnIntervMin = htobs(SlaveConnIntervMin);
  Osal_MemCpy(buffer + indx, &SlaveConnIntervMin, 2);
  indx +=  2;
  
  SlaveConnIntervMax = htobs(SlaveConnIntervMax);
  Osal_MemCpy(buffer + indx, &SlaveConnIntervMax, 2);
  indx +=  2;    

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_DISCOVERABLE;
  rq.cparam = (void *)buffer;
  rq.clen = indx;
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  if (status) {
    return status;
  }

  return 0;
}

#if BLUENRG_MS
tBleStatus aci_gap_set_direct_connectable(uint8_t own_addr_type, uint8_t directed_adv_type, uint8_t initiator_addr_type,
                                          const uint8_t *initiator_addr, uint16_t adv_interv_min, uint16_t adv_interv_max)
#else
tBleStatus aci_gap_set_direct_connectable(uint8_t own_addr_type, uint8_t initiator_addr_type, const uint8_t *initiator_addr)
#endif
{
  struct hci_request rq;
  gap_set_direct_conectable_cp cp;
  uint8_t status;    

  cp.own_bdaddr_type = own_addr_type;
#if BLUENRG_MS
  cp.directed_adv_type = directed_adv_type;
  cp.adv_interv_min = adv_interv_min;
  cp.adv_interv_max = adv_interv_max;
#endif  
  cp.direct_bdaddr_type = initiator_addr_type;
  Osal_MemCpy(cp.direct_bdaddr, initiator_addr, 6);

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_DIRECT_CONNECTABLE;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;
    
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
    
  return status;
}

tBleStatus aci_gap_set_io_capability(uint8_t io_capability)
{
  struct hci_request rq;
  uint8_t status;
  gap_set_io_capability_cp cp;
    
  cp.io_capability = io_capability;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_IO_CAPABILITY;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;
    
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
    
  return status;
}

tBleStatus aci_gap_set_auth_requirement(uint8_t mitm_mode,
                                        uint8_t oob_enable,
                                        uint8_t oob_data[16],
                                        uint8_t min_encryption_key_size,
                                        uint8_t max_encryption_key_size,
                                        uint8_t use_fixed_pin,
                                        uint32_t fixed_pin,
                                        uint8_t bonding_mode)
{
  struct hci_request rq;
  gap_set_auth_requirement_cp cp;    
  uint8_t status;
    
  cp.mitm_mode = mitm_mode;
  cp.oob_enable = oob_enable;
  Osal_MemCpy(cp.oob_data, oob_data, 16);
  cp.min_encryption_key_size = min_encryption_key_size;
  cp.max_encryption_key_size = max_encryption_key_size;
  cp.use_fixed_pin = use_fixed_pin;
  cp.fixed_pin = htobl(fixed_pin);
  cp.bonding_mode = bonding_mode;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_AUTH_REQUIREMENT;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  if (status) {
    return status;
  }
    
  return 0;
}

tBleStatus aci_gap_set_author_requirement(uint16_t conn_handle, uint8_t authorization_enable)
{
  struct hci_request rq;
  gap_set_author_requirement_cp cp;    
  uint8_t status;
    
  cp.conn_handle = htobs(conn_handle);
  cp.authorization_enable = authorization_enable;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_AUTHOR_REQUIREMENT;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  return status;
}

tBleStatus aci_gap_pass_key_response(uint16_t conn_handle, uint32_t passkey)
{
  struct hci_request rq;
  gap_passkey_response_cp cp;    
  uint8_t status;
    
  cp.conn_handle = htobs(conn_handle);
  cp.passkey = htobl(passkey);

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_PASSKEY_RESPONSE;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  return status;
}

tBleStatus aci_gap_authorization_response(uint16_t conn_handle, uint8_t authorize)
{
  struct hci_request rq;
  gap_authorization_response_cp cp;    
  uint8_t status;
    
  cp.conn_handle = htobs(conn_handle);
  cp.authorize = authorize;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_AUTHORIZATION_RESPONSE;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  return status;
}

#if BLUENRG_MS
tBleStatus aci_gap_set_non_connectable(uint8_t adv_type, uint8_t own_address_type)
#else
tBleStatus aci_gap_set_non_connectable(uint8_t adv_type)
#endif
{
  struct hci_request rq;
  gap_set_non_connectable_cp cp;    
  uint8_t status;
    
  cp.advertising_event_type = adv_type;  
#if BLUENRG_MS
  cp.own_address_type = own_address_type;
#endif

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_NON_CONNECTABLE;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  return status;
}

tBleStatus aci_gap_set_undirected_connectable(uint8_t own_addr_type, uint8_t adv_filter_policy)
{
  struct hci_request rq;
  gap_set_undirected_connectable_cp cp;    
  uint8_t status;
    
  cp.own_addr_type = own_addr_type;
  cp.adv_filter_policy = adv_filter_policy;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_UNDIRECTED_CONNECTABLE;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  return status;
}

tBleStatus aci_gap_slave_security_request(uint16_t conn_handle, uint8_t bonding, uint8_t mitm_protection)
{
  struct hci_request rq;
  gap_slave_security_request_cp cp;
  uint8_t status;

  cp.conn_handle = htobs(conn_handle);
  cp.bonding = bonding;
  cp.mitm_protection = mitm_protection;
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SLAVE_SECURITY_REQUEST;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;

}

tBleStatus aci_gap_update_adv_data(uint8_t AdvLen, const uint8_t *AdvData)
{
  struct hci_request rq;
  uint8_t status;
  uint8_t buffer[32];
  uint8_t indx = 0;
    
  if (AdvLen > (sizeof(buffer)-1))
    return BLE_STATUS_INVALID_PARAMS;

  buffer[indx] = AdvLen;
  indx++;
    
  Osal_MemCpy(buffer + indx, AdvData, AdvLen);
  indx +=  AdvLen;
    
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_UPDATE_ADV_DATA;
  rq.cparam = (void *)buffer;
  rq.clen = indx;
  rq.rparam = &status;
  rq.rlen = 1;
    
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
    
  return status;
}

tBleStatus aci_gap_delete_ad_type(uint8_t ad_type)
{
  struct hci_request rq;
  gap_delete_ad_type_cp cp;
  uint8_t status;

  cp.ad_type = ad_type;
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_DELETE_AD_TYPE;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_get_security_level(uint8_t* mitm_protection, uint8_t* bonding,
                                      uint8_t* oob_data, uint8_t* passkey_required)
{
  struct hci_request rq;
  gap_get_security_level_rp resp;
    
  Osal_MemSet(&resp, 0, sizeof(resp));
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_GET_SECURITY_LEVEL;
  rq.rparam = &resp;
  rq.rlen = GAP_GET_SECURITY_LEVEL_RP_SIZE;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  if (resp.status) {
    return resp.status;
  }
  
  *mitm_protection = resp.mitm_protection;
  *bonding = resp.bonding;
  *oob_data = resp.oob_data;
  *passkey_required = resp.passkey_required;

  return resp.status;
}

tBleStatus aci_gap_configure_whitelist(void)
{
  struct hci_request rq;
  uint8_t status;
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_CONFIGURE_WHITELIST;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_terminate(uint16_t conn_handle, uint8_t reason)
{
  struct hci_request rq;
  gap_terminate_cp cp;
  uint8_t status;  

  cp.handle = htobs(conn_handle);
  cp.reason = reason;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_TERMINATE;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status; 
}

tBleStatus aci_gap_clear_security_database(void)
{
  struct hci_request rq;
  uint8_t status;
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_CLEAR_SECURITY_DB;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

#if BLUENRG_MS
tBleStatus aci_gap_allow_rebond(uint16_t conn_handle)
{
  struct hci_request rq;
  gap_allow_rebond_cp cp;
  uint8_t status;
  
  cp.conn_handle = conn_handle;
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_ALLOW_REBOND_DB;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}
#else  
tBleStatus aci_gap_allow_rebond(void)
{
  struct hci_request rq;
  uint8_t status;
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_ALLOW_REBOND_DB;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}
#endif

tBleStatus aci_gap_start_limited_discovery_proc(uint16_t scanInterval, uint16_t scanWindow,
            uint8_t own_address_type, uint8_t filterDuplicates)
{
  struct hci_request rq;
  gap_start_limited_discovery_proc_cp cp;
  uint8_t status;  

  cp.scanInterval = htobs(scanInterval);
  cp.scanWindow = htobs(scanWindow);
  cp.own_address_type = own_address_type;
  cp.filterDuplicates = filterDuplicates;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_LIMITED_DISCOVERY_PROC;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_start_general_discovery_proc(uint16_t scanInterval, uint16_t scanWindow,
            uint8_t own_address_type, uint8_t filterDuplicates)
{
  struct hci_request rq;
  gap_start_general_discovery_proc_cp cp;
  uint8_t status;  

  cp.scanInterval = htobs(scanInterval);
  cp.scanWindow = htobs(scanWindow);
  cp.own_address_type = own_address_type;
  cp.filterDuplicates = filterDuplicates;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_GENERAL_DISCOVERY_PROC;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}


tBleStatus aci_gap_start_name_discovery_proc(uint16_t scanInterval, uint16_t scanWindow,
             uint8_t peer_bdaddr_type, tBDAddr peer_bdaddr,  
             uint8_t own_bdaddr_type, uint16_t conn_min_interval,  
             uint16_t conn_max_interval, uint16_t conn_latency,  
             uint16_t supervision_timeout, uint16_t min_conn_length, 
             uint16_t max_conn_length)
{
  struct hci_request rq;
  gap_start_name_discovery_proc_cp cp;
  uint8_t status;  

  cp.scanInterval = htobs(scanInterval);
  cp.scanWindow = htobs(scanWindow);
  cp.peer_bdaddr_type = peer_bdaddr_type;
  Osal_MemCpy(cp.peer_bdaddr, peer_bdaddr, 6);
  cp.own_bdaddr_type = own_bdaddr_type;
  cp.conn_min_interval = htobs(conn_min_interval);
  cp.conn_max_interval = htobs(conn_max_interval);
  cp.conn_latency = htobs(conn_latency);
  cp.supervision_timeout = htobs(supervision_timeout);
  cp.min_conn_length = htobs(min_conn_length);
  cp.max_conn_length = htobs(max_conn_length);

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_NAME_DISCOVERY_PROC;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

#if BLUENRG_MS

tBleStatus aci_gap_start_auto_conn_establish_proc(uint16_t scanInterval, uint16_t scanWindow,
                         uint8_t own_bdaddr_type, uint16_t conn_min_interval,  
                         uint16_t conn_max_interval, uint16_t conn_latency,  
                         uint16_t supervision_timeout, uint16_t min_conn_length, 
                         uint16_t max_conn_length,
                         uint8_t num_whitelist_entries,
                         const uint8_t *addr_array)
{
  struct hci_request rq;
  uint8_t status;
  uint8_t buffer[HCI_MAX_PAYLOAD_SIZE];
  uint8_t indx = 0;
    
  if (((num_whitelist_entries*7)+25) > HCI_MAX_PAYLOAD_SIZE)
    return BLE_STATUS_INVALID_PARAMS;

  scanInterval = htobs(scanInterval);
  Osal_MemCpy(buffer + indx, &scanInterval, 2);
  indx += 2;
    
  scanWindow = htobs(scanWindow);
  Osal_MemCpy(buffer + indx, &scanWindow, 2);
  indx += 2;

  buffer[indx] = own_bdaddr_type;
  indx++;
  
  conn_min_interval = htobs(conn_min_interval);
  Osal_MemCpy(buffer + indx, &conn_min_interval, 2);
  indx +=  2;

  conn_max_interval = htobs(conn_max_interval);
  Osal_MemCpy(buffer + indx, &conn_max_interval, 2);
  indx +=  2;

  conn_latency = htobs(conn_latency);
  Osal_MemCpy(buffer + indx, &conn_latency, 2);
  indx +=  2;

  supervision_timeout = htobs(supervision_timeout);
  Osal_MemCpy(buffer + indx, &supervision_timeout, 2);
  indx +=  2;

  min_conn_length = htobs(min_conn_length);
  Osal_MemCpy(buffer + indx, &min_conn_length, 2);
  indx +=  2;

  max_conn_length = htobs(max_conn_length);
  Osal_MemCpy(buffer + indx, &max_conn_length, 2);
  indx +=  2;

  buffer[indx] = num_whitelist_entries;
  indx++;

  Osal_MemCpy(buffer + indx, addr_array, (num_whitelist_entries*7));
  indx +=  num_whitelist_entries * 7;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_AUTO_CONN_ESTABLISH_PROC;
  rq.cparam = (void *)buffer;
  rq.clen = indx;
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;  
}

#else

tBleStatus aci_gap_start_auto_conn_establish_proc(uint16_t scanInterval, uint16_t scanWindow,
                         uint8_t own_bdaddr_type, uint16_t conn_min_interval,  
                         uint16_t conn_max_interval, uint16_t conn_latency,  
                         uint16_t supervision_timeout, uint16_t min_conn_length, 
                         uint16_t max_conn_length,
                         uint8_t use_reconn_addr,
                         const tBDAddr reconn_addr,
                         uint8_t num_whitelist_entries,
                         const uint8_t *addr_array)
{
  struct hci_request rq;
  uint8_t status;
  uint8_t buffer[HCI_MAX_PAYLOAD_SIZE];
  uint8_t indx = 0;
    
  if (((num_whitelist_entries*7)+25) > HCI_MAX_PAYLOAD_SIZE)
    return BLE_STATUS_INVALID_PARAMS;

  scanInterval = htobs(scanInterval);
  Osal_MemCpy(buffer + indx, &scanInterval, 2);
  indx += 2;
    
  scanWindow = htobs(scanWindow);
  Osal_MemCpy(buffer + indx, &scanWindow, 2);
  indx += 2;

  buffer[indx] = own_bdaddr_type;
  indx++;
  
  conn_min_interval = htobs(conn_min_interval);
  Osal_MemCpy(buffer + indx, &conn_min_interval, 2);
  indx +=  2;

  conn_max_interval = htobs(conn_max_interval);
  Osal_MemCpy(buffer + indx, &conn_max_interval, 2);
  indx +=  2;

  conn_latency = htobs(conn_latency);
  Osal_MemCpy(buffer + indx, &conn_latency, 2);
  indx +=  2;

  supervision_timeout = htobs(supervision_timeout);
  Osal_MemCpy(buffer + indx, &supervision_timeout, 2);
  indx +=  2;

  min_conn_length = htobs(min_conn_length);
  Osal_MemCpy(buffer + indx, &min_conn_length, 2);
  indx +=  2;

  max_conn_length = htobs(max_conn_length);
  Osal_MemCpy(buffer + indx, &max_conn_length, 2);
  indx +=  2;
  
  buffer[indx] = use_reconn_addr;
  indx++;

  Osal_MemCpy(buffer + indx, reconn_addr, 6);
  indx += 6;

  buffer[indx] = num_whitelist_entries;
  indx++;

  Osal_MemCpy(buffer + indx, addr_array, (num_whitelist_entries*7));
  indx +=  num_whitelist_entries * 7;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_AUTO_CONN_ESTABLISH_PROC;
  rq.cparam = (void *)buffer;
  rq.clen = indx;
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;  
}

#endif

#if BLUENRG_MS
tBleStatus aci_gap_start_general_conn_establish_proc(uint8_t scan_type, uint16_t scan_interval, uint16_t scan_window,
             uint8_t own_address_type, uint8_t filter_duplicates)
{
  struct hci_request rq;
  gap_start_general_conn_establish_proc_cp cp;
  uint8_t status;  

  cp.scan_type = scan_type;
  cp.scan_interval = htobs(scan_interval);
  cp.scan_window = htobs(scan_window);
  cp.own_address_type = own_address_type;
  cp.filter_duplicates = filter_duplicates;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_GENERAL_CONN_ESTABLISH_PROC;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}
#else
tBleStatus aci_gap_start_general_conn_establish_proc(uint8_t scan_type, uint16_t scan_interval, uint16_t scan_window,
             uint8_t own_address_type, uint8_t filter_duplicates, uint8_t use_reconn_addr, const tBDAddr reconn_addr)
{
  struct hci_request rq;
  gap_start_general_conn_establish_proc_cp cp;
  uint8_t status;  

  cp.scan_type = scan_type;
  cp.scan_interval = htobs(scan_interval);
  cp.scan_window = htobs(scan_window);
  cp.own_address_type = own_address_type;
  cp.filter_duplicates = filter_duplicates;
  cp.use_reconn_addr = use_reconn_addr;
  Osal_MemCpy(cp.reconn_addr, reconn_addr, 6);

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_GENERAL_CONN_ESTABLISH_PROC;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}
#endif

tBleStatus aci_gap_start_selective_conn_establish_proc(uint8_t scan_type, uint16_t scan_interval, uint16_t scan_window,
                         uint8_t own_address_type, uint8_t filter_duplicates, uint8_t num_whitelist_entries,
                         const uint8_t *addr_array)
{
  struct hci_request rq;
  gap_start_selective_conn_establish_proc_cp cp;
  uint8_t status;
  
  if (((num_whitelist_entries*7)+GAP_START_SELECTIVE_CONN_ESTABLISH_PROC_CP_SIZE) > HCI_MAX_PAYLOAD_SIZE)
    return BLE_STATUS_INVALID_PARAMS;
  
  cp.scan_type = scan_type;
  cp.scan_interval = htobs(scan_interval);
  cp.scan_window = htobs(scan_window);
  cp.own_address_type = own_address_type;
  cp.filter_duplicates = filter_duplicates;
  cp.num_whitelist_entries = num_whitelist_entries;  
  
  Osal_MemCpy(cp.addr_array, addr_array, (num_whitelist_entries*7));

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_SELECTIVE_CONN_ESTABLISH_PROC;
  rq.cparam = &cp;
  rq.clen = GAP_START_SELECTIVE_CONN_ESTABLISH_PROC_CP_SIZE + (num_whitelist_entries*7);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_create_connection(uint16_t scanInterval, uint16_t scanWindow,
             uint8_t peer_bdaddr_type, tBDAddr peer_bdaddr,  
             uint8_t own_bdaddr_type, uint16_t conn_min_interval,  
             uint16_t conn_max_interval, uint16_t conn_latency,  
             uint16_t supervision_timeout, uint16_t min_conn_length, 
             uint16_t max_conn_length)
{
  struct hci_request rq;
  gap_create_connection_cp cp;
  uint8_t status;  

  cp.scanInterval = htobs(scanInterval);
  cp.scanWindow = htobs(scanWindow);
  cp.peer_bdaddr_type = peer_bdaddr_type;
  Osal_MemCpy(cp.peer_bdaddr, peer_bdaddr, 6);
  cp.own_bdaddr_type = own_bdaddr_type;
  cp.conn_min_interval = htobs(conn_min_interval);
  cp.conn_max_interval = htobs(conn_max_interval);
  cp.conn_latency = htobs(conn_latency);
  cp.supervision_timeout = htobs(supervision_timeout);
  cp.min_conn_length = htobs(min_conn_length);
  cp.max_conn_length = htobs(max_conn_length);

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_CREATE_CONNECTION;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_terminate_gap_procedure(uint8_t procedure_code)
{
  struct hci_request rq;
  uint8_t status;  

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_TERMINATE_GAP_PROCEDURE;
  rq.cparam = &procedure_code;
  rq.clen = 1;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;

}

tBleStatus aci_gap_start_connection_update(uint16_t conn_handle, uint16_t conn_min_interval,  
                                           uint16_t conn_max_interval, uint16_t conn_latency,  
                                           uint16_t supervision_timeout, uint16_t min_conn_length, 
                                           uint16_t max_conn_length)
{
  struct hci_request rq;
  gap_start_connection_update_cp cp;
  uint8_t status;  

  cp.conn_handle = htobs(conn_handle);
  cp.conn_min_interval = htobs(conn_min_interval);
  cp.conn_max_interval = htobs(conn_max_interval);
  cp.conn_latency = htobs(conn_latency);
  cp.supervision_timeout = htobs(supervision_timeout);
  cp.min_conn_length = htobs(min_conn_length);
  cp.max_conn_length = htobs(max_conn_length);

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_CONNECTION_UPDATE;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_send_pairing_request(uint16_t conn_handle, uint8_t force_rebond)
{
  struct hci_request rq;
  gap_send_pairing_request_cp cp;
  uint8_t status;  

  cp.conn_handle = htobs(conn_handle);
  cp.force_rebond = force_rebond;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SEND_PAIRING_REQUEST;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

#if BLUENRG_MS
tBleStatus aci_gap_resolve_private_address(const tBDAddr private_address, tBDAddr actual_address)
{
  struct hci_request rq;
  gap_resolve_private_address_cp cp;
  gap_resolve_private_address_rp rp;

  Osal_MemCpy(cp.address, private_address, 6);

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_RESOLVE_PRIVATE_ADDRESS;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &rp;
  rq.rlen = sizeof(rp);
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  if(rp.status)
    return rp.status;
  
  Osal_MemCpy(actual_address, rp.address, sizeof((int)(actual_address)));

  return 0;
}
#else
tBleStatus aci_gap_resolve_private_address(const tBDAddr address)
{
  struct hci_request rq;
  gap_resolve_private_address_cp cp;
  uint8_t status;

  Osal_MemCpy(cp.address, address, 6);

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_RESOLVE_PRIVATE_ADDRESS;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}
#endif
  
tBleStatus aci_gap_set_broadcast_mode(uint16_t adv_interv_min, uint16_t adv_interv_max, uint8_t adv_type,
          uint8_t own_addr_type, uint8_t adv_data_length, const uint8_t *adv_data,  uint8_t num_whitelist_entries,
                                  const uint8_t *addr_array)
{
  struct hci_request rq;
  gap_set_broadcast_mode_cp cp;
  uint8_t status;
  uint8_t indx = 0;  
  uint8_t variable_size =  1 + adv_data_length + 1 + num_whitelist_entries*7;
  
  if (variable_size > sizeof(cp.var_len_data) )
    return BLE_STATUS_INVALID_PARAMS;
  
  cp.adv_interv_min = htobs(adv_interv_min);
  cp.adv_interv_max = htobs(adv_interv_max);
  cp.adv_type = adv_type;
  cp.own_addr_type = own_addr_type;
  
  cp.var_len_data[indx] = adv_data_length;
  indx++;
  Osal_MemCpy(cp.var_len_data + indx, adv_data, adv_data_length);
  indx += adv_data_length;
  cp.var_len_data[indx] = num_whitelist_entries;
  indx ++;
  Osal_MemCpy(cp.var_len_data + indx, addr_array, num_whitelist_entries*7);

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_SET_BROADCAST_MODE;
  rq.cparam = &cp;
  rq.clen = GAP_SET_BROADCAST_MODE_CP_SIZE + variable_size;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_start_observation_procedure(uint16_t scan_interval, uint16_t scan_window, uint8_t scan_type,
             uint8_t own_address_type, uint8_t filter_duplicates)
{
  struct hci_request rq;
  gap_start_observation_proc_cp cp;
  uint8_t status;
  
  cp.scan_interval = scan_interval;
  cp.scan_window = scan_window;
  cp.scan_type = scan_type;
  cp.own_address_type = own_address_type;
  cp.filter_duplicates = filter_duplicates;

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_START_OBSERVATION_PROC;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_is_device_bonded(uint8_t peer_address_type, const tBDAddr peer_address)
{
  struct hci_request rq;
  gap_is_device_bonded_cp cp;
  uint8_t status;
  
  cp.peer_address_type = peer_address_type;
  Osal_MemCpy(cp.peer_address, peer_address, sizeof(cp.peer_address));

  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_IS_DEVICE_BONDED;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_gap_get_bonded_devices(uint8_t *num_devices, uint8_t *device_list, uint8_t device_list_size)
{
  struct hci_request rq;
  gap_get_bonded_devices_rp rp;
  
  Osal_MemSet(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_GAP_GET_BONDED_DEVICES;
  rq.rparam = &rp;
  rq.rlen = sizeof(rp);
  
  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;
  
  if (rp.status) {
    return rp.status;
  }
  
  *num_devices = rp.num_addr;
  if(device_list != NULL)
    Osal_MemCpy(device_list, rp.dev_list, MIN(device_list_size,rp.num_addr*7));
  
  return 0;
}

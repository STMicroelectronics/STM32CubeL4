/******************************************************************************
*
*  File Description 
*  ---------------------
* This file defines constants and functions for HCI layer.
* See Bluetooth Core v 4.0, Vol. 2, Part E.
* 
*******************************************************************************/

#ifndef __HCI_CONST_H_
#define __HCI_CONST_H_

#include "compiler.h"
#include "link_layer.h"


#ifndef ADDED_BY_MCD
/**
 * This shall be set to the maximum payload size of any HCI commands used in the application or any
 * command complete (CC)/status(CS) event received in return.
 * A unique buffer is used to send a HCI command and to receive its CC/CS.
 * The default value is set to 255 as this is the maximum payload length of any HCI command/events. Although this could
 * be reduced is case there is no way to send a so large command or to receive so long event, it is recommended to keep
 * the default value as the saving would be marginal.
 */
#define HCI_MAX_PAYLOAD_SIZE 255
#endif /* ADDED_BY_MCD */

/* HCI Packet types */
#define HCI_COMMAND_PKT    0x01
#define HCI_ACLDATA_PKT    0x02
#define HCI_SCODATA_PKT    0x03
#define HCI_EVENT_PKT      0x04
#define HCI_VENDOR_PKT     0xff

#ifdef __GNUC__
typedef struct __packed _hci_uart_pckt{
#else
typedef __packed struct _hci_uart_pckt{
#endif
  uint8_t  type;
  uint8_t  data[VARIABLE_SIZE];
} PACKED hci_uart_pckt;
#define HCI_HDR_SIZE           1

#ifdef __GNUC__
typedef struct __packed _hci_command_hdr{
#else
typedef __packed struct _hci_command_hdr{
#endif
  uint16_t  opcode;    /* OCF & OGF */
  uint8_t    plen;
} PACKED hci_command_hdr;
#define HCI_COMMAND_HDR_SIZE   3

#ifdef __GNUC__
typedef struct __packed _hci_event_pckt{
#else
typedef __packed struct _hci_event_pckt{
#endif
  uint8_t    evt;
  uint8_t    plen;
  uint8_t      data[VARIABLE_SIZE];
} PACKED hci_event_pckt;
#define HCI_EVENT_HDR_SIZE     2

#ifdef __GNUC__
typedef struct __packed _hci_acl_hdr{
#else
typedef __packed struct _hci_acl_hdr{
#endif
  uint16_t  handle;    /* Handle & Flags(PB, BC) */
  uint16_t  dlen;
} PACKED hci_acl_hdr;
#define HCI_ACL_HDR_SIZE       4


/* Link Control */
#define OGF_LINK_CTL                0x01

#define OCF_DISCONNECT              0x0006
#ifdef __GNUC__
typedef struct __packed _disconnect_cp{
#else
typedef __packed struct _disconnect_cp{
#endif
  uint16_t  handle;
  uint8_t    reason;
} PACKED disconnect_cp;
#define DISCONNECT_CP_SIZE 3


/* Host Controller and Baseband */
#define OGF_HOST_CTL                0x03

#define OCF_SET_EVENT_MASK          0x0001
#define OCF_RESET                   0x0003

#define OCF_READ_TRANSMIT_POWER_LEVEL  0x002D
#ifdef __GNUC__
typedef struct __packed _read_transmit_power_level_cp{
#else
typedef __packed struct _read_transmit_power_level_cp{
#endif
  uint16_t  handle;
  uint8_t    type;
} PACKED read_transmit_power_level_cp;
#define READ_TRANSMIT_POWER_LEVEL_CP_SIZE  3
#ifdef __GNUC__
typedef struct __packed _read_transmit_power_level_rp{
#else
typedef __packed struct _read_transmit_power_level_rp{
#endif
  uint8_t    status;
  uint16_t  handle;
  int8_t    level;
} PACKED read_transmit_power_level_rp;
#define READ_TRANSMIT_POWER_LEVEL_RP_SIZE  4

#define OCF_SET_CONTROLLER_TO_HOST_FC      0x0031
#define OCF_HOST_BUFFER_SIZE               0x0033
#define OCF_HOST_NUM_COMP_PKTS             0x0035

/* Informational Parameters */
#define OGF_INFO_PARAM                     0x04

#define OCF_READ_LOCAL_VERSION             0x0001
#ifdef __GNUC__
typedef struct __packed _read_local_version_rp{
#else
typedef __packed struct _read_local_version_rp{
#endif
  uint8_t    status;
  uint8_t    hci_version;
  uint16_t    hci_revision;
  uint8_t    lmp_pal_version;
  uint16_t  manufacturer_name;
  uint16_t  lmp_pal_subversion;
} PACKED read_local_version_rp;
#define READ_LOCAL_VERSION_RP_SIZE 9

#define OCF_READ_LOCAL_COMMANDS        0x0002
#define OCF_READ_LOCAL_FEATURES        0x0003

#define OCF_READ_BD_ADDR               0x0009
#ifdef __GNUC__
typedef struct __packed _read_bd_addr_rp{
#else
typedef __packed struct _read_bd_addr_rp{
#endif
  uint8_t    status;
  tBDAddr    bdaddr;
} PACKED read_bd_addr_rp;
#define READ_BD_ADDR_RP_SIZE 7

/* Status params */
#define OGF_STATUS_PARAM                0x05

#define OCF_READ_RSSI                   0x0005
#ifdef __GNUC__
typedef struct __packed _read_rssi_cp{
#else
typedef __packed struct _read_rssi_cp{
#endif
  uint16_t  handle;
} PACKED read_rssi_cp;

#define READ_RSSI_CP_SIZE 2
#ifdef __GNUC__
typedef struct __packed _read_rssi_rp{
#else
typedef __packed struct _read_rssi_rp{
#endif
  uint8_t    status;
  uint16_t  handle;
  int8_t    rssi;
} PACKED read_rssi_rp;
#define READ_RSSI_RP_SIZE 4


/* LE commands */
#define OGF_LE_CTL                   0x08

#define OCF_LE_SET_EVENT_MASK        0x0001
#ifdef __GNUC__
typedef struct __packed _le_set_event_mask_cp{
#else
typedef __packed struct _le_set_event_mask_cp{
#endif
  uint8_t    mask[8];
} PACKED le_set_event_mask_cp;
#define LE_SET_EVENT_MASK_CP_SIZE 8

#define OCF_LE_READ_BUFFER_SIZE      0x0002
#ifdef __GNUC__
typedef struct __packed _le_read_buffer_size_rp{
#else
typedef __packed struct _le_read_buffer_size_rp{
#endif
  uint8_t    status;
  uint16_t  pkt_len;
  uint8_t    max_pkt;
} PACKED le_read_buffer_size_rp;
#define LE_READ_BUFFER_SIZE_RP_SIZE   4

#define OCF_LE_READ_LOCAL_SUPPORTED_FEATURES  0x0003
#ifdef __GNUC__
typedef struct __packed _le_read_local_supported_features_rp{
#else
typedef __packed struct _le_read_local_supported_features_rp{
#endif
  uint8_t    status;
  uint8_t    features[8];
} PACKED le_read_local_supported_features_rp;
#define LE_READ_LOCAL_SUPPORTED_FEATURES_RP_SIZE   9

#define OCF_LE_SET_RANDOM_ADDRESS    0x0005
#ifdef __GNUC__
typedef struct __packed _le_set_random_address_cp{
#else
typedef __packed struct _le_set_random_address_cp{
#endif
  tBDAddr  bdaddr;
} PACKED le_set_random_address_cp;
#define LE_SET_RANDOM_ADDRESS_CP_SIZE   6

#define OCF_LE_SET_ADV_PARAMETERS    0x0006
#ifdef __GNUC__
typedef struct __packed _le_set_adv_parameters_cp{
#else
typedef __packed struct _le_set_adv_parameters_cp{
#endif
  uint16_t  min_interval;
  uint16_t  max_interval;
  uint8_t    advtype;
  uint8_t    own_bdaddr_type;
  uint8_t    direct_bdaddr_type;
  tBDAddr    direct_bdaddr;
  uint8_t    chan_map;
  uint8_t    filter;
} PACKED le_set_adv_parameters_cp;
#define LE_SET_ADV_PARAMETERS_CP_SIZE 15

#define OCF_LE_READ_ADV_CHANNEL_TX_POWER  0x0007
#ifdef __GNUC__
typedef struct __packed _le_read_adv_channel_tx_power_rp{
#else
typedef __packed struct _le_read_adv_channel_tx_power_rp{
#endif
  uint8_t    status;
  int8_t    level;
} PACKED le_read_adv_channel_tx_power_rp;
#define LE_READ_ADV_CHANNEL_TX_POWER_RP_SIZE 2

#define OCF_LE_SET_ADV_DATA    0x0008
#ifdef __GNUC__
typedef struct __packed _le_set_adv_data_cp{
#else
typedef __packed struct _le_set_adv_data_cp{
#endif
  uint8_t    length;
  uint8_t    data[31];
} PACKED le_set_adv_data_cp;
#define LE_SET_ADV_DATA_CP_SIZE 32

#define OCF_LE_SET_SCAN_RESPONSE_DATA    0x0009
#ifdef __GNUC__
typedef struct __packed _le_set_scan_response_data_cp{
#else
typedef __packed struct _le_set_scan_response_data_cp{
#endif
  uint8_t    length;
  uint8_t    data[31];
} PACKED le_set_scan_response_data_cp;
#define LE_SET_SCAN_RESPONSE_DATA_CP_SIZE 32

#define OCF_LE_SET_ADVERTISE_ENABLE    0x000A
#ifdef __GNUC__
typedef struct __packed _le_set_advertise_enable_cp{
#else
typedef __packed struct _le_set_advertise_enable_cp{
#endif
  uint8_t    enable;
} PACKED le_set_advertise_enable_cp;
#define LE_SET_ADVERTISE_ENABLE_CP_SIZE 1

#define OCF_LE_SET_SCAN_PARAMETERS    0x000B
#ifdef __GNUC__
typedef struct __packed _le_set_scan_parameters_cp{
#else
typedef __packed struct _le_set_scan_parameters_cp{
#endif
  uint8_t    type;
  uint16_t  interval;
  uint16_t  window;
  uint8_t    own_bdaddr_type;
  uint8_t    filter;
} PACKED le_set_scan_parameters_cp;
#define LE_SET_SCAN_PARAMETERS_CP_SIZE 7

#define OCF_LE_SET_SCAN_ENABLE      0x000C
#ifdef __GNUC__
typedef struct __packed _le_set_scan_enable_cp{
#else
typedef __packed struct _le_set_scan_enable_cp{
#endif
  uint8_t    enable;
  uint8_t    filter_dup;
} PACKED le_set_scan_enable_cp;
#define LE_SET_SCAN_ENABLE_CP_SIZE 2

#define OCF_LE_CREATE_CONN      0x000D
#ifdef __GNUC__
typedef struct __packed _le_create_connection_cp{
#else
typedef __packed struct _le_create_connection_cp{
#endif
  uint16_t  interval;
  uint16_t  window;
  uint8_t    initiator_filter;
  uint8_t    peer_bdaddr_type;
  tBDAddr    peer_bdaddr;
  uint8_t    own_bdaddr_type;
  uint16_t  min_interval;
  uint16_t  max_interval;
  uint16_t  latency;
  uint16_t  supervision_timeout;
  uint16_t  min_ce_length;
  uint16_t  max_ce_length;
} PACKED le_create_connection_cp;
#define LE_CREATE_CONN_CP_SIZE 25

#define OCF_LE_CREATE_CONN_CANCEL    0x000E

#define OCF_LE_READ_WHITE_LIST_SIZE    0x000F
#ifdef __GNUC__
typedef struct __packed _le_read_white_list_size_rp{
#else
typedef __packed struct _le_read_white_list_size_rp{
#endif
  uint8_t    status;
  uint8_t    size;
} PACKED le_read_white_list_size_rp;
#define LE_READ_WHITE_LIST_SIZE_RP_SIZE 2

#define OCF_LE_CLEAR_WHITE_LIST      0x0010

#define OCF_LE_ADD_DEVICE_TO_WHITE_LIST    0x0011
#ifdef __GNUC__
typedef struct __packed _le_add_device_to_white_list_cp{
#else
typedef __packed struct _le_add_device_to_white_list_cp{
#endif
  uint8_t    bdaddr_type;
  tBDAddr   bdaddr;
} PACKED le_add_device_to_white_list_cp;
#define LE_ADD_DEVICE_TO_WHITE_LIST_CP_SIZE 7

#define OCF_LE_REMOVE_DEVICE_FROM_WHITE_LIST  0x0012
#ifdef __GNUC__
typedef struct __packed _le_remove_device_from_white_list_cp{
#else
typedef __packed struct _le_remove_device_from_white_list_cp{
#endif
  uint8_t    bdaddr_type;
  tBDAddr   bdaddr;
} PACKED le_remove_device_from_white_list_cp;
#define LE_REMOVE_DEVICE_FROM_WHITE_LIST_CP_SIZE 7

#define OCF_LE_CONN_UPDATE      0x0013
#ifdef __GNUC__
typedef struct __packed _le_connection_update_cp{
#else
typedef __packed struct _le_connection_update_cp{
#endif
  uint16_t  handle;
  uint16_t  min_interval;
  uint16_t  max_interval;
  uint16_t  latency;
  uint16_t  supervision_timeout;
  uint16_t  min_ce_length;
  uint16_t  max_ce_length;
} PACKED le_connection_update_cp;
#define LE_CONN_UPDATE_CP_SIZE 14

#define OCF_LE_SET_HOST_CHANNEL_CLASSIFICATION  0x0014
#ifdef __GNUC__
typedef struct __packed _le_set_host_channel_classification_cp{
#else
typedef __packed struct _le_set_host_channel_classification_cp{
#endif
  uint8_t    map[5];
} PACKED le_set_host_channel_classification_cp;
#define LE_SET_HOST_CHANNEL_CLASSIFICATION_CP_SIZE 5

#define OCF_LE_READ_CHANNEL_MAP      0x0015
#ifdef __GNUC__
typedef struct __packed _le_read_channel_map_cp{
#else
typedef __packed struct _le_read_channel_map_cp{
#endif
  uint16_t  handle;
} PACKED le_read_channel_map_cp;
#define LE_READ_CHANNEL_MAP_CP_SIZE 2

#ifdef __GNUC__
typedef struct __packed _le_read_channel_map_rp{
#else
typedef __packed struct _le_read_channel_map_rp{
#endif
  uint8_t    status;
  uint16_t  handle;
  uint8_t    map[5];
} PACKED le_read_channel_map_rp;
#define LE_READ_CHANNEL_MAP_RP_SIZE 8

#define OCF_LE_READ_REMOTE_USED_FEATURES  0x0016
#ifdef __GNUC__
typedef struct __packed _le_read_remote_used_features_cp{
#else
typedef __packed struct _le_read_remote_used_features_cp{
#endif
  uint16_t  handle;
} PACKED le_read_remote_used_features_cp;
#define LE_READ_REMOTE_USED_FEATURES_CP_SIZE 2

#define OCF_LE_ENCRYPT        0x0017
#ifdef __GNUC__
typedef struct __packed _le_encrypt_cp{
#else
typedef __packed struct _le_encrypt_cp{
#endif
  uint8_t    key[16];
  uint8_t    plaintext[16];
} PACKED le_encrypt_cp;
#define LE_ENCRYPT_CP_SIZE 32

#ifdef __GNUC__
typedef struct __packed _le_encrypt_rp{
#else
typedef __packed struct _le_encrypt_rp{
#endif
  uint8_t    status;
  uint8_t    encdata[16];
} PACKED le_encrypt_rp;
#define LE_ENCRYPT_RP_SIZE 17

#define OCF_LE_RAND        0x0018
#ifdef __GNUC__
typedef struct __packed _le_rand_rp{
#else
typedef __packed struct _le_rand_rp{
#endif
  uint8_t  status;
  uint8_t  random[8];
} PACKED le_rand_rp;
#define LE_RAND_RP_SIZE 9

#define OCF_LE_START_ENCRYPTION      0x0019
#ifdef __GNUC__
typedef struct __packed _le_start_encryption_cp{
#else
typedef __packed struct _le_start_encryption_cp{
#endif
  uint16_t  handle;
  uint8_t      random[8];
  uint16_t  diversifier;
  uint8_t    key[16];
} PACKED le_start_encryption_cp;
#define LE_START_ENCRYPTION_CP_SIZE 28

#define OCF_LE_LTK_REPLY      0x001A
#ifdef __GNUC__
typedef struct __packed _le_ltk_reply_cp{
#else
typedef __packed struct _le_ltk_reply_cp{
#endif
  uint16_t  handle;
  uint8_t    key[16];
} PACKED le_ltk_reply_cp;
#define LE_LTK_REPLY_CP_SIZE 18

#ifdef __GNUC__
typedef struct __packed _le_ltk_reply_rp{
#else
typedef __packed struct _le_ltk_reply_rp{
#endif
  uint8_t    status;
  uint16_t  handle;
} PACKED le_ltk_reply_rp;
#define LE_LTK_REPLY_RP_SIZE 3

#define OCF_LE_LTK_NEG_REPLY      0x001B
#ifdef __GNUC__
typedef struct __packed _le_ltk_neg_reply_cp{
#else
typedef __packed struct _le_ltk_neg_reply_cp{
#endif
  uint16_t  handle;
} PACKED le_ltk_neg_reply_cp;
#define LE_LTK_NEG_REPLY_CP_SIZE 2

#ifdef __GNUC__
typedef struct __packed _le_ltk_neg_reply_rp{
#else
typedef __packed struct _le_ltk_neg_reply_rp{
#endif
  uint8_t    status;
  uint16_t  handle;
} PACKED le_ltk_neg_reply_rp;
#define LE_LTK_NEG_REPLY_RP_SIZE 3

#define OCF_LE_READ_SUPPORTED_STATES    0x001C
#ifdef __GNUC__
typedef struct __packed _le_read_supported_states_rp{
#else
typedef __packed struct _le_read_supported_states_rp{
#endif
  uint8_t    status;
  uint8_t      states[8];
} PACKED le_read_supported_states_rp;
#define LE_READ_SUPPORTED_STATES_RP_SIZE 9

#define OCF_LE_RECEIVER_TEST      0x001D
#ifdef __GNUC__
typedef struct __packed _le_receiver_test_cp{
#else
typedef __packed struct _le_receiver_test_cp{
#endif
  uint8_t    frequency;
} PACKED le_receiver_test_cp;
#define LE_RECEIVER_TEST_CP_SIZE 1

#define OCF_LE_TRANSMITTER_TEST      0x001E
#ifdef __GNUC__
typedef struct __packed _le_transmitter_test_cp{
#else
typedef __packed struct _le_transmitter_test_cp{
#endif
  uint8_t    frequency;
  uint8_t    length;
  uint8_t    payload;
} PACKED le_transmitter_test_cp;
#define LE_TRANSMITTER_TEST_CP_SIZE 3

#define OCF_LE_TEST_END        0x001F
#ifdef __GNUC__
typedef struct __packed _le_test_end_rp{
#else
typedef __packed struct _le_test_end_rp{
#endif
  uint8_t    status;
  uint16_t  num_pkts;
} PACKED le_test_end_rp;
#define LE_TEST_END_RP_SIZE 3

/* Vendor specific commands */
#define OGF_VENDOR_CMD    0x3f


/*------------- Events -------------*/
#define EVT_CONN_COMPLETE    0x03
#ifdef __GNUC__
typedef struct __packed _evt_conn_complete{
#else
typedef __packed struct _evt_conn_complete{
#endif
  uint8_t    status;
  uint16_t  handle;
  tBDAddr   bdaddr;
  uint8_t    link_type;
  uint8_t    encr_mode;
} PACKED evt_conn_complete;
#define EVT_CONN_COMPLETE_SIZE 13

#define EVT_DISCONN_COMPLETE    0x05
#ifdef __GNUC__
typedef struct __packed _evt_disconn_complete{
#else
typedef __packed struct _evt_disconn_complete{
#endif
  uint8_t    status;
  uint16_t  handle;
  uint8_t    reason;
} PACKED evt_disconn_complete;
#define EVT_DISCONN_COMPLETE_SIZE 4

#define EVT_ENCRYPT_CHANGE    0x08
#ifdef __GNUC__
typedef struct __packed _evt_encrypt_change{
#else
typedef __packed struct _evt_encrypt_change{
#endif
  uint8_t    status;
  uint16_t  handle;
  uint8_t    encrypt;
} PACKED evt_encrypt_change;
#define EVT_ENCRYPT_CHANGE_SIZE 5

#define EVT_READ_REMOTE_VERSION_COMPLETE  0x0C

#define EVT_CMD_COMPLETE     0x0E
#ifdef __GNUC__
typedef struct __packed _evt_cmd_complete{
#else
typedef __packed struct _evt_cmd_complete{
#endif
  uint8_t    ncmd;
  uint16_t  opcode;
#ifndef ADDED_BY_MCD
  uint8_t return_parameters;
#endif
} PACKED evt_cmd_complete;
#define EVT_CMD_COMPLETE_SIZE 3

#define EVT_CMD_STATUS       0x0F
#ifdef __GNUC__
typedef struct __packed _evt_cmd_status{
#else
typedef __packed struct _evt_cmd_status{
#endif
  uint8_t    status;
  uint8_t    ncmd;
  uint16_t  opcode;
} PACKED evt_cmd_status;
#define EVT_CMD_STATUS_SIZE 4

#define EVT_HARDWARE_ERROR    0x10
#ifdef __GNUC__
typedef struct __packed _evt_hardware_error{
#else
typedef __packed struct _evt_hardware_error{
#endif
  uint8_t    code;
} PACKED evt_hardware_error;
#define EVT_HARDWARE_ERROR_SIZE 1

#define EVT_NUM_COMP_PKTS    0x13
#ifdef __GNUC__
typedef struct __packed _evt_num_comp_pkts{
#else
typedef __packed struct _evt_num_comp_pkts{
#endif
  uint8_t    num_hndl;
  /* variable length part */
} PACKED evt_num_comp_pkts;
#define EVT_NUM_COMP_PKTS_SIZE 1

/* variable length part of evt_num_comp_pkts. */
#ifdef __GNUC__
typedef struct __packed _evt_num_comp_pkts_param{
#else
typedef __packed struct _evt_num_comp_pkts_param{
#endif
  uint16_t    hndl;
  uint16_t    num_comp_pkts;
} PACKED evt_num_comp_pkts_param;
#define EVT_NUM_COMP_PKTS_PARAM_SIZE 1

#define EVT_DATA_BUFFER_OVERFLOW    0x1A
#ifdef __GNUC__
typedef struct __packed _evt_data_buffer_overflow{
#else
typedef __packed struct _evt_data_buffer_overflow{
#endif
  uint8_t    link_type;
} PACKED evt_data_buffer_overflow;
#define EVT_DATA_BUFFER_OVERFLOW_SIZE 1

#define EVT_ENCRYPTION_KEY_REFRESH_COMPLETE  0x30
#ifdef __GNUC__
typedef struct __packed _evt_encryption_key_refresh_complete{
#else
typedef __packed struct _evt_encryption_key_refresh_complete{
#endif
  uint8_t    status;
  uint16_t  handle;
} PACKED evt_encryption_key_refresh_complete;
#define EVT_ENCRYPTION_KEY_REFRESH_COMPLETE_SIZE 3

#define EVT_LE_META_EVENT  0x3E
#ifdef __GNUC__
typedef struct __packed _evt_le_meta_event{
#else
typedef __packed struct _evt_le_meta_event{
#endif
  uint8_t    subevent;
  uint8_t    data[VARIABLE_SIZE];
} PACKED evt_le_meta_event;
#define EVT_LE_META_EVENT_SIZE 1

#define EVT_LE_CONN_COMPLETE  0x01
#ifdef __GNUC__
typedef struct __packed _evt_le_connection_complete{
#else
typedef __packed struct _evt_le_connection_complete{
#endif
  uint8_t    status;
  uint16_t  handle;
  uint8_t    role;
  uint8_t    peer_bdaddr_type;
  tBDAddr      peer_bdaddr;
  uint16_t  interval;
  uint16_t  latency;
  uint16_t  supervision_timeout;
  uint8_t    master_clock_accuracy;
} PACKED evt_le_connection_complete;
#define EVT_LE_CONN_COMPLETE_SIZE 18

#define EVT_LE_ADVERTISING_REPORT  0x02
#ifdef __GNUC__
typedef struct __packed _le_advertising_info{
#else
typedef __packed struct _le_advertising_info{
#endif
  uint8_t               num_reports;
  uint8_t    evt_type;
  uint8_t    bdaddr_type;
  tBDAddr      bdaddr;
  uint8_t    data_length;
  uint8_t    data_RSSI[VARIABLE_SIZE]; // RSSI is last octet (signed integer).
} PACKED le_advertising_info;
#define LE_ADVERTISING_INFO_SIZE 11

#define EVT_LE_CONN_UPDATE_COMPLETE  0x03
#ifdef __GNUC__
typedef struct __packed _evt_le_connection_update_complete{
#else
typedef __packed struct _evt_le_connection_update_complete{
#endif
  uint8_t    status;
  uint16_t  handle;
  uint16_t  interval;
  uint16_t  latency;
  uint16_t  supervision_timeout;
} PACKED evt_le_connection_update_complete;
#define EVT_LE_CONN_UPDATE_COMPLETE_SIZE 9

#define EVT_LE_READ_REMOTE_USED_FEATURES_COMPLETE  0x04
#ifdef __GNUC__
typedef struct __packed _evt_le_read_remote_used_features_complete{
#else
typedef __packed struct _evt_le_read_remote_used_features_complete{
#endif
  uint8_t    status;
  uint16_t  handle;
  uint8_t    features[8];
} PACKED evt_le_read_remote_used_features_complete;
#define EVT_LE_READ_REMOTE_USED_FEATURES_COMPLETE_SIZE 11

#define EVT_LE_LTK_REQUEST  0x05
#ifdef __GNUC__
typedef struct __packed _evt_le_long_term_key_request{
#else
typedef __packed struct _evt_le_long_term_key_request{
#endif
  uint16_t  handle;
  uint8_t      random[8];
  uint16_t  ediv;
} PACKED evt_le_long_term_key_request;
#define EVT_LE_LTK_REQUEST_SIZE 12

/**
* The event code in the @ref hci_event_pckt structure. If event code is EVT_VENDOR,
* application can use @ref evt_blue_aci structure to parse the packet.
*/
#define EVT_VENDOR  0xFF


/* Command opcode pack/unpack */
#define cmd_opcode_pack(ogf, ocf)  (uint16_t)((ocf & 0x03ff)|(ogf << 10))
#define cmd_opcode_ogf(op)    (op >> 10)
#define cmd_opcode_ocf(op)    (op & 0x03ff)

#endif /* __HCI_CONST_H_ */

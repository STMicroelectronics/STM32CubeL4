/**
  ******************************************************************************
  * @file    PKA/PKA_ECCscalarMultiplication/Src/ecdsa_priv_prime256v1.c
  * @author  MCD Application Team
  * @brief   This file contains buffers reflecting the content of the file
  *          ecdsa_priv_prime256v1.pem provided as an example of private and
  *          public key pair to demonstrate functionnality of PKA.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* This file reflect the content of ecdsa_priv_prime256v1.pem */

/* ecdsa_priv_prime256v1.pem has been created using openssl 1.0.2k  26 Jan 2017 */
/* Buffer have been adapted from the output of the command: openssl ec -in ecdsa_priv_prime256v1.pem -text -noout */
/*
Private-Key: (256 bit)
priv:
    00:fe:22:d2:a5:e9:e8:1f:92:b0:bb:42:c2:fe:de:
    3e:63:ab:0a:b1:0f:14:9a:a8:3f:76:da:44:69:d3:
    be:69:57
pub:
    04:dd:79:95:da:1f:a1:c0:25:f3:e7:aa:6b:62:2c:
    9d:78:4a:37:22:dc:8d:64:6b:1b:14:f5:c3:a0:3c:
    a9:70:19:c6:d8:7e:b5:78:43:ff:15:a0:77:92:55:
    86:8e:5b:b4:0e:b0:79:c8:e3:42:ca:c4:55:f7:2c:
    f4:04:b1:99:82
*/
/* Remark: 00 at beginning of buffers are removed to ease the usage with PKA */
/* Remark: ecdsa_priv_prime256v1_pub contains 2 points, they are split to 
           ecdsa_priv_prime256v1_pubX and ecdsa_priv_prime256v1_pubY to ease the usage 
           with PKA */
/* Remark: ecdsa_priv_prime256v1_Generator contains 2 points, they are split to 
           ecdsa_priv_prime256v1_GeneratorX and ecdsa_priv_prime256v1_GeneratorY to 
           ease the usage with PKA */

#include "main.h"

const uint8_t ecdsa_priv_prime256v1_priv[] = {
/*0x00,*/ 0xfe, 0x22, 0xd2, 0xa5, 0xe9, 0xe8, 0x1f, 0x92, 0xb0, 0xbb, 0x42, 0xc2, 0xfe, 0xde,
    0x3e, 0x63, 0xab, 0x0a, 0xb1, 0x0f, 0x14, 0x9a, 0xa8, 0x3f, 0x76, 0xda, 0x44, 0x69, 0xd3,
    0xbe, 0x69, 0x57
};
const uint32_t ecdsa_priv_prime256v1_priv_len = 32;

const uint8_t ecdsa_priv_prime256v1_pub[] = {
    0x04, 0xdd, 0x79, 0x95, 0xda, 0x1f, 0xa1, 0xc0, 0x25, 0xf3, 0xe7, 0xaa, 0x6b, 0x62, 0x2c,
    0x9d, 0x78, 0x4a, 0x37, 0x22, 0xdc, 0x8d, 0x64, 0x6b, 0x1b, 0x14, 0xf5, 0xc3, 0xa0, 0x3c,
    0xa9, 0x70, 0x19, 0xc6, 0xd8, 0x7e, 0xb5, 0x78, 0x43, 0xff, 0x15, 0xa0, 0x77, 0x92, 0x55,
    0x86, 0x8e, 0x5b, 0xb4, 0x0e, 0xb0, 0x79, 0xc8, 0xe3, 0x42, 0xca, 0xc4, 0x55, 0xf7, 0x2c,
    0xf4, 0x04, 0xb1, 0x99, 0x82
};
const uint32_t ecdsa_priv_prime256v1_pub_len = 65;

/* This buffer is extracted from ecdsa_priv_prime256v1_pub as its first part */
const uint8_t ecdsa_priv_prime256v1_pubX[] = {
    0xdd, 0x79, 0x95, 0xda, 0x1f, 0xa1, 0xc0, 0x25, 0xf3, 0xe7, 0xaa, 0x6b, 0x62, 0x2c, 0x9d, 
    0x78, 0x4a, 0x37, 0x22, 0xdc, 0x8d, 0x64, 0x6b, 0x1b, 0x14, 0xf5, 0xc3, 0xa0, 0x3c, 0xa9, 
    0x70, 0x19
};
const uint32_t ecdsa_priv_prime256v1_pubX_len = 32;

/* This buffer is extracted from ecdsa_priv_prime256v1_pub as its second part */
const uint8_t ecdsa_priv_prime256v1_pubY[] = {
    0xc6, 0xd8, 0x7e, 0xb5, 0x78, 0x43, 0xff, 0x15, 0xa0, 0x77, 0x92, 0x55, 0x86, 0x8e, 0x5b, 
    0xb4, 0x0e, 0xb0, 0x79, 0xc8, 0xe3, 0x42, 0xca, 0xc4, 0x55, 0xf7, 0x2c, 0xf4, 0x04, 0xb1, 
    0x99, 0x82
};
const uint32_t ecdsa_priv_prime256v1_pubY_len = 32;

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

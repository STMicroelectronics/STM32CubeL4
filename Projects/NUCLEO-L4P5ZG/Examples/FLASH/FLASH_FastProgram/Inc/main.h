/**
  ******************************************************************************
  * @file    FLASH/FLASH_FastProgram/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_144.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Base address of the Flash sectors */

#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 0, 4 Kbytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08001000) /* Base @ of Page 1, 4 Kbytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08002000) /* Base @ of Page 2, 4 Kbytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08003000) /* Base @ of Page 3, 4 Kbytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08004000) /* Base @ of Page 4, 4 Kbytes */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x08005000) /* Base @ of Page 5, 4 Kbytes */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x08006000) /* Base @ of Page 6, 4 Kbytes */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x08007000) /* Base @ of Page 7, 4 Kbytes */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08008000) /* Base @ of Page 8, 4 Kbytes */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x08009000) /* Base @ of Page 9, 4 Kbytes */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x0800A000) /* Base @ of Page 10, 4 Kbytes */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x0800B000) /* Base @ of Page 11, 4 Kbytes */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x0800C000) /* Base @ of Page 12, 4 Kbytes */
#define ADDR_FLASH_PAGE_13    ((uint32_t)0x0800D000) /* Base @ of Page 13, 4 Kbytes */
#define ADDR_FLASH_PAGE_14    ((uint32_t)0x0800E000) /* Base @ of Page 14, 4 Kbytes */
#define ADDR_FLASH_PAGE_15    ((uint32_t)0x0800F000) /* Base @ of Page 15, 4 Kbytes */
#define ADDR_FLASH_PAGE_16    ((uint32_t)0x08010000) /* Base @ of Page 16, 4 Kbytes */
#define ADDR_FLASH_PAGE_17    ((uint32_t)0x08011000) /* Base @ of Page 17, 4 Kbytes */
#define ADDR_FLASH_PAGE_18    ((uint32_t)0x08012000) /* Base @ of Page 18, 4 Kbytes */
#define ADDR_FLASH_PAGE_19    ((uint32_t)0x08013000) /* Base @ of Page 19, 4 Kbytes */
#define ADDR_FLASH_PAGE_20    ((uint32_t)0x08014000) /* Base @ of Page 20, 4 Kbytes */
#define ADDR_FLASH_PAGE_21    ((uint32_t)0x08015000) /* Base @ of Page 21, 4 Kbytes */
#define ADDR_FLASH_PAGE_22    ((uint32_t)0x08016000) /* Base @ of Page 22, 4 Kbytes */
#define ADDR_FLASH_PAGE_23    ((uint32_t)0x08017000) /* Base @ of Page 23, 4 Kbytes */
#define ADDR_FLASH_PAGE_24    ((uint32_t)0x08018000) /* Base @ of Page 24, 4 Kbytes */
#define ADDR_FLASH_PAGE_25    ((uint32_t)0x08019000) /* Base @ of Page 25, 4 Kbytes */
#define ADDR_FLASH_PAGE_26    ((uint32_t)0x0801A000) /* Base @ of Page 26, 4 Kbytes */
#define ADDR_FLASH_PAGE_27    ((uint32_t)0x0801B000) /* Base @ of Page 27, 4 Kbytes */
#define ADDR_FLASH_PAGE_28    ((uint32_t)0x0801C000) /* Base @ of Page 28, 4 Kbytes */
#define ADDR_FLASH_PAGE_29    ((uint32_t)0x0801D000) /* Base @ of Page 29, 4 Kbytes */
#define ADDR_FLASH_PAGE_30    ((uint32_t)0x0801E000) /* Base @ of Page 30, 4 Kbytes */
#define ADDR_FLASH_PAGE_31    ((uint32_t)0x0801F000) /* Base @ of Page 31, 4 Kbytes */
#define ADDR_FLASH_PAGE_32    ((uint32_t)0x08020000) /* Base @ of Page 32, 4 Kbytes */
#define ADDR_FLASH_PAGE_33    ((uint32_t)0x08021000) /* Base @ of Page 33, 4 Kbytes */
#define ADDR_FLASH_PAGE_34    ((uint32_t)0x08022000) /* Base @ of Page 34, 4 Kbytes */
#define ADDR_FLASH_PAGE_35    ((uint32_t)0x08023000) /* Base @ of Page 35, 4 Kbytes */
#define ADDR_FLASH_PAGE_36    ((uint32_t)0x08024000) /* Base @ of Page 36, 4 Kbytes */
#define ADDR_FLASH_PAGE_37    ((uint32_t)0x08025000) /* Base @ of Page 37, 4 Kbytes */
#define ADDR_FLASH_PAGE_38    ((uint32_t)0x08026000) /* Base @ of Page 38, 4 Kbytes */
#define ADDR_FLASH_PAGE_39    ((uint32_t)0x08027000) /* Base @ of Page 39, 4 Kbytes */
#define ADDR_FLASH_PAGE_40    ((uint32_t)0x08028000) /* Base @ of Page 40, 4 Kbytes */
#define ADDR_FLASH_PAGE_41    ((uint32_t)0x08029000) /* Base @ of Page 41, 4 Kbytes */
#define ADDR_FLASH_PAGE_42    ((uint32_t)0x0802A000) /* Base @ of Page 42, 4 Kbytes */
#define ADDR_FLASH_PAGE_43    ((uint32_t)0x0802B000) /* Base @ of Page 43, 4 Kbytes */
#define ADDR_FLASH_PAGE_44    ((uint32_t)0x0802C000) /* Base @ of Page 44, 4 Kbytes */
#define ADDR_FLASH_PAGE_45    ((uint32_t)0x0802D000) /* Base @ of Page 45, 4 Kbytes */
#define ADDR_FLASH_PAGE_46    ((uint32_t)0x0802E000) /* Base @ of Page 46, 4 Kbytes */
#define ADDR_FLASH_PAGE_47    ((uint32_t)0x0802F000) /* Base @ of Page 47, 4 Kbytes */
#define ADDR_FLASH_PAGE_48    ((uint32_t)0x08030000) /* Base @ of Page 48, 4 Kbytes */
#define ADDR_FLASH_PAGE_49    ((uint32_t)0x08031000) /* Base @ of Page 49, 4 Kbytes */
#define ADDR_FLASH_PAGE_50    ((uint32_t)0x08032000) /* Base @ of Page 50, 4 Kbytes */
#define ADDR_FLASH_PAGE_51    ((uint32_t)0x08033000) /* Base @ of Page 51, 4 Kbytes */
#define ADDR_FLASH_PAGE_52    ((uint32_t)0x08034000) /* Base @ of Page 52, 4 Kbytes */
#define ADDR_FLASH_PAGE_53    ((uint32_t)0x08035000) /* Base @ of Page 53, 4 Kbytes */
#define ADDR_FLASH_PAGE_54    ((uint32_t)0x08036000) /* Base @ of Page 54, 4 Kbytes */
#define ADDR_FLASH_PAGE_55    ((uint32_t)0x08037000) /* Base @ of Page 55, 4 Kbytes */
#define ADDR_FLASH_PAGE_56    ((uint32_t)0x08038000) /* Base @ of Page 56, 4 Kbytes */
#define ADDR_FLASH_PAGE_57    ((uint32_t)0x08039000) /* Base @ of Page 57, 4 Kbytes */
#define ADDR_FLASH_PAGE_58    ((uint32_t)0x0803A000) /* Base @ of Page 58, 4 Kbytes */
#define ADDR_FLASH_PAGE_59    ((uint32_t)0x0803B000) /* Base @ of Page 59, 4 Kbytes */
#define ADDR_FLASH_PAGE_60    ((uint32_t)0x0803C000) /* Base @ of Page 60, 4 Kbytes */
#define ADDR_FLASH_PAGE_61    ((uint32_t)0x0803D000) /* Base @ of Page 61, 4 Kbytes */
#define ADDR_FLASH_PAGE_62    ((uint32_t)0x0803E000) /* Base @ of Page 62, 4 Kbytes */
#define ADDR_FLASH_PAGE_63    ((uint32_t)0x0803F000) /* Base @ of Page 63, 4 Kbytes */
#define ADDR_FLASH_PAGE_64    ((uint32_t)0x08040000) /* Base @ of Page 64, 4 Kbytes */
#define ADDR_FLASH_PAGE_65    ((uint32_t)0x08041000) /* Base @ of Page 65, 4 Kbytes */
#define ADDR_FLASH_PAGE_66    ((uint32_t)0x08042000) /* Base @ of Page 66, 4 Kbytes */
#define ADDR_FLASH_PAGE_67    ((uint32_t)0x08043000) /* Base @ of Page 67, 4 Kbytes */
#define ADDR_FLASH_PAGE_68    ((uint32_t)0x08044000) /* Base @ of Page 68, 4 Kbytes */
#define ADDR_FLASH_PAGE_69    ((uint32_t)0x08045000) /* Base @ of Page 69, 4 Kbytes */
#define ADDR_FLASH_PAGE_70    ((uint32_t)0x08046000) /* Base @ of Page 70, 4 Kbytes */
#define ADDR_FLASH_PAGE_71    ((uint32_t)0x08047000) /* Base @ of Page 71, 4 Kbytes */
#define ADDR_FLASH_PAGE_72    ((uint32_t)0x08048000) /* Base @ of Page 72, 4 Kbytes */
#define ADDR_FLASH_PAGE_73    ((uint32_t)0x08049000) /* Base @ of Page 73, 4 Kbytes */
#define ADDR_FLASH_PAGE_74    ((uint32_t)0x0804A000) /* Base @ of Page 74, 4 Kbytes */
#define ADDR_FLASH_PAGE_75    ((uint32_t)0x0804B000) /* Base @ of Page 75, 4 Kbytes */
#define ADDR_FLASH_PAGE_76    ((uint32_t)0x0804C000) /* Base @ of Page 76, 4 Kbytes */
#define ADDR_FLASH_PAGE_77    ((uint32_t)0x0804D000) /* Base @ of Page 77, 4 Kbytes */
#define ADDR_FLASH_PAGE_78    ((uint32_t)0x0804E000) /* Base @ of Page 78, 4 Kbytes */
#define ADDR_FLASH_PAGE_79    ((uint32_t)0x0804F000) /* Base @ of Page 79, 4 Kbytes */
#define ADDR_FLASH_PAGE_80    ((uint32_t)0x08050000) /* Base @ of Page 80, 4 Kbytes */
#define ADDR_FLASH_PAGE_81    ((uint32_t)0x08051000) /* Base @ of Page 81, 4 Kbytes */
#define ADDR_FLASH_PAGE_82    ((uint32_t)0x08052000) /* Base @ of Page 82, 4 Kbytes */
#define ADDR_FLASH_PAGE_83    ((uint32_t)0x08053000) /* Base @ of Page 83, 4 Kbytes */
#define ADDR_FLASH_PAGE_84    ((uint32_t)0x08054000) /* Base @ of Page 84, 4 Kbytes */
#define ADDR_FLASH_PAGE_85    ((uint32_t)0x08055000) /* Base @ of Page 85, 4 Kbytes */
#define ADDR_FLASH_PAGE_86    ((uint32_t)0x08056000) /* Base @ of Page 86, 4 Kbytes */
#define ADDR_FLASH_PAGE_87    ((uint32_t)0x08057000) /* Base @ of Page 87, 4 Kbytes */
#define ADDR_FLASH_PAGE_88    ((uint32_t)0x08058000) /* Base @ of Page 88, 4 Kbytes */
#define ADDR_FLASH_PAGE_89    ((uint32_t)0x08059000) /* Base @ of Page 89, 4 Kbytes */
#define ADDR_FLASH_PAGE_90    ((uint32_t)0x0805A000) /* Base @ of Page 90, 4 Kbytes */
#define ADDR_FLASH_PAGE_91    ((uint32_t)0x0805B000) /* Base @ of Page 91, 4 Kbytes */
#define ADDR_FLASH_PAGE_92    ((uint32_t)0x0805C000) /* Base @ of Page 92, 4 Kbytes */
#define ADDR_FLASH_PAGE_93    ((uint32_t)0x0805D000) /* Base @ of Page 93, 4 Kbytes */
#define ADDR_FLASH_PAGE_94    ((uint32_t)0x0805E000) /* Base @ of Page 94, 4 Kbytes */
#define ADDR_FLASH_PAGE_95    ((uint32_t)0x0805F000) /* Base @ of Page 95, 4 Kbytes */
#define ADDR_FLASH_PAGE_96    ((uint32_t)0x08060000) /* Base @ of Page 96, 4 Kbytes */
#define ADDR_FLASH_PAGE_97    ((uint32_t)0x08061000) /* Base @ of Page 97, 4 Kbytes */
#define ADDR_FLASH_PAGE_98    ((uint32_t)0x08062000) /* Base @ of Page 98, 4 Kbytes */
#define ADDR_FLASH_PAGE_99    ((uint32_t)0x08063000) /* Base @ of Page 99, 4 Kbytes */
#define ADDR_FLASH_PAGE_100   ((uint32_t)0x08064000) /* Base @ of Page 100, 4 Kbytes */
#define ADDR_FLASH_PAGE_101   ((uint32_t)0x08065000) /* Base @ of Page 101, 4 Kbytes */
#define ADDR_FLASH_PAGE_102   ((uint32_t)0x08066000) /* Base @ of Page 102, 4 Kbytes */
#define ADDR_FLASH_PAGE_103   ((uint32_t)0x08067000) /* Base @ of Page 103, 4 Kbytes */
#define ADDR_FLASH_PAGE_104   ((uint32_t)0x08068000) /* Base @ of Page 104, 4 Kbytes */
#define ADDR_FLASH_PAGE_105   ((uint32_t)0x08069000) /* Base @ of Page 105, 4 Kbytes */
#define ADDR_FLASH_PAGE_106   ((uint32_t)0x0806A000) /* Base @ of Page 106, 4 Kbytes */
#define ADDR_FLASH_PAGE_107   ((uint32_t)0x0806B000) /* Base @ of Page 107, 4 Kbytes */
#define ADDR_FLASH_PAGE_108   ((uint32_t)0x0806C000) /* Base @ of Page 108, 4 Kbytes */
#define ADDR_FLASH_PAGE_109   ((uint32_t)0x0806D000) /* Base @ of Page 109, 4 Kbytes */
#define ADDR_FLASH_PAGE_110   ((uint32_t)0x0806E000) /* Base @ of Page 110, 4 Kbytes */
#define ADDR_FLASH_PAGE_111   ((uint32_t)0x0806F000) /* Base @ of Page 111, 4 Kbytes */
#define ADDR_FLASH_PAGE_112   ((uint32_t)0x08070000) /* Base @ of Page 112, 4 Kbytes */
#define ADDR_FLASH_PAGE_113   ((uint32_t)0x08071000) /* Base @ of Page 113, 4 Kbytes */
#define ADDR_FLASH_PAGE_114   ((uint32_t)0x08072000) /* Base @ of Page 114, 4 Kbytes */
#define ADDR_FLASH_PAGE_115   ((uint32_t)0x08073000) /* Base @ of Page 115, 4 Kbytes */
#define ADDR_FLASH_PAGE_116   ((uint32_t)0x08074000) /* Base @ of Page 116, 4 Kbytes */
#define ADDR_FLASH_PAGE_117   ((uint32_t)0x08075000) /* Base @ of Page 117, 4 Kbytes */
#define ADDR_FLASH_PAGE_118   ((uint32_t)0x08076000) /* Base @ of Page 118, 4 Kbytes */
#define ADDR_FLASH_PAGE_119   ((uint32_t)0x08077000) /* Base @ of Page 119, 4 Kbytes */
#define ADDR_FLASH_PAGE_120   ((uint32_t)0x08078000) /* Base @ of Page 120, 4 Kbytes */
#define ADDR_FLASH_PAGE_121   ((uint32_t)0x08079000) /* Base @ of Page 121, 4 Kbytes */
#define ADDR_FLASH_PAGE_122   ((uint32_t)0x0807A000) /* Base @ of Page 122, 4 Kbytes */
#define ADDR_FLASH_PAGE_123   ((uint32_t)0x0807B000) /* Base @ of Page 123, 4 Kbytes */
#define ADDR_FLASH_PAGE_124   ((uint32_t)0x0807C000) /* Base @ of Page 124, 4 Kbytes */
#define ADDR_FLASH_PAGE_125   ((uint32_t)0x0807D000) /* Base @ of Page 125, 4 Kbytes */
#define ADDR_FLASH_PAGE_126   ((uint32_t)0x0807E000) /* Base @ of Page 126, 4 Kbytes */
#define ADDR_FLASH_PAGE_127   ((uint32_t)0x0807F000) /* Base @ of Page 127, 4 Kbytes */
#define ADDR_FLASH_PAGE_128   ((uint32_t)0x08080000) /* Base @ of Page 128, 4 Kbytes */
#define ADDR_FLASH_PAGE_129   ((uint32_t)0x08081000) /* Base @ of Page 129, 4 Kbytes */
#define ADDR_FLASH_PAGE_130   ((uint32_t)0x08082000) /* Base @ of Page 130, 4 Kbytes */
#define ADDR_FLASH_PAGE_131   ((uint32_t)0x08083000) /* Base @ of Page 131, 4 Kbytes */
#define ADDR_FLASH_PAGE_132   ((uint32_t)0x08084000) /* Base @ of Page 132, 4 Kbytes */
#define ADDR_FLASH_PAGE_133   ((uint32_t)0x08085000) /* Base @ of Page 133, 4 Kbytes */
#define ADDR_FLASH_PAGE_134   ((uint32_t)0x08086000) /* Base @ of Page 134, 4 Kbytes */
#define ADDR_FLASH_PAGE_135   ((uint32_t)0x08087000) /* Base @ of Page 135, 4 Kbytes */
#define ADDR_FLASH_PAGE_136   ((uint32_t)0x08088000) /* Base @ of Page 136, 4 Kbytes */
#define ADDR_FLASH_PAGE_137   ((uint32_t)0x08089000) /* Base @ of Page 137, 4 Kbytes */
#define ADDR_FLASH_PAGE_138   ((uint32_t)0x0808A000) /* Base @ of Page 138, 4 Kbytes */
#define ADDR_FLASH_PAGE_139   ((uint32_t)0x0808B000) /* Base @ of Page 139, 4 Kbytes */
#define ADDR_FLASH_PAGE_140   ((uint32_t)0x0808C000) /* Base @ of Page 140, 4 Kbytes */
#define ADDR_FLASH_PAGE_141   ((uint32_t)0x0808D000) /* Base @ of Page 141, 4 Kbytes */
#define ADDR_FLASH_PAGE_142   ((uint32_t)0x0808E000) /* Base @ of Page 142, 4 Kbytes */
#define ADDR_FLASH_PAGE_143   ((uint32_t)0x0808F000) /* Base @ of Page 143, 4 Kbytes */
#define ADDR_FLASH_PAGE_144   ((uint32_t)0x08090000) /* Base @ of Page 144, 4 Kbytes */
#define ADDR_FLASH_PAGE_145   ((uint32_t)0x08091000) /* Base @ of Page 145, 4 Kbytes */
#define ADDR_FLASH_PAGE_146   ((uint32_t)0x08092000) /* Base @ of Page 146, 4 Kbytes */
#define ADDR_FLASH_PAGE_147   ((uint32_t)0x08093000) /* Base @ of Page 147, 4 Kbytes */
#define ADDR_FLASH_PAGE_148   ((uint32_t)0x08094000) /* Base @ of Page 148, 4 Kbytes */
#define ADDR_FLASH_PAGE_149   ((uint32_t)0x08095000) /* Base @ of Page 149, 4 Kbytes */
#define ADDR_FLASH_PAGE_150   ((uint32_t)0x08096000) /* Base @ of Page 150, 4 Kbytes */
#define ADDR_FLASH_PAGE_151   ((uint32_t)0x08097000) /* Base @ of Page 151, 4 Kbytes */
#define ADDR_FLASH_PAGE_152   ((uint32_t)0x08098000) /* Base @ of Page 152, 4 Kbytes */
#define ADDR_FLASH_PAGE_153   ((uint32_t)0x08099000) /* Base @ of Page 153, 4 Kbytes */
#define ADDR_FLASH_PAGE_154   ((uint32_t)0x0809A000) /* Base @ of Page 154, 4 Kbytes */
#define ADDR_FLASH_PAGE_155   ((uint32_t)0x0809B000) /* Base @ of Page 155, 4 Kbytes */
#define ADDR_FLASH_PAGE_156   ((uint32_t)0x0809C000) /* Base @ of Page 156, 4 Kbytes */
#define ADDR_FLASH_PAGE_157   ((uint32_t)0x0809D000) /* Base @ of Page 157, 4 Kbytes */
#define ADDR_FLASH_PAGE_158   ((uint32_t)0x0809E000) /* Base @ of Page 158, 4 Kbytes */
#define ADDR_FLASH_PAGE_159   ((uint32_t)0x0809F000) /* Base @ of Page 159, 4 Kbytes */
#define ADDR_FLASH_PAGE_160   ((uint32_t)0x080A0000) /* Base @ of Page 160, 4 Kbytes */
#define ADDR_FLASH_PAGE_161   ((uint32_t)0x080A1000) /* Base @ of Page 161, 4 Kbytes */
#define ADDR_FLASH_PAGE_162   ((uint32_t)0x080A2000) /* Base @ of Page 162, 4 Kbytes */
#define ADDR_FLASH_PAGE_163   ((uint32_t)0x080A3000) /* Base @ of Page 163, 4 Kbytes */
#define ADDR_FLASH_PAGE_164   ((uint32_t)0x080A4000) /* Base @ of Page 164, 4 Kbytes */
#define ADDR_FLASH_PAGE_165   ((uint32_t)0x080A5000) /* Base @ of Page 165, 4 Kbytes */
#define ADDR_FLASH_PAGE_166   ((uint32_t)0x080A6000) /* Base @ of Page 166, 4 Kbytes */
#define ADDR_FLASH_PAGE_167   ((uint32_t)0x080A7000) /* Base @ of Page 167, 4 Kbytes */
#define ADDR_FLASH_PAGE_168   ((uint32_t)0x080A8000) /* Base @ of Page 168, 4 Kbytes */
#define ADDR_FLASH_PAGE_169   ((uint32_t)0x080A9000) /* Base @ of Page 169, 4 Kbytes */
#define ADDR_FLASH_PAGE_170   ((uint32_t)0x080AA000) /* Base @ of Page 170, 4 Kbytes */
#define ADDR_FLASH_PAGE_171   ((uint32_t)0x080AB000) /* Base @ of Page 171, 4 Kbytes */
#define ADDR_FLASH_PAGE_172   ((uint32_t)0x080AC000) /* Base @ of Page 172, 4 Kbytes */
#define ADDR_FLASH_PAGE_173   ((uint32_t)0x080AD000) /* Base @ of Page 173, 4 Kbytes */
#define ADDR_FLASH_PAGE_174   ((uint32_t)0x080AE000) /* Base @ of Page 174, 4 Kbytes */
#define ADDR_FLASH_PAGE_175   ((uint32_t)0x080AF000) /* Base @ of Page 175, 4 Kbytes */
#define ADDR_FLASH_PAGE_176   ((uint32_t)0x080B0000) /* Base @ of Page 176, 4 Kbytes */
#define ADDR_FLASH_PAGE_177   ((uint32_t)0x080B1000) /* Base @ of Page 177, 4 Kbytes */
#define ADDR_FLASH_PAGE_178   ((uint32_t)0x080B2000) /* Base @ of Page 178, 4 Kbytes */
#define ADDR_FLASH_PAGE_179   ((uint32_t)0x080B3000) /* Base @ of Page 179, 4 Kbytes */
#define ADDR_FLASH_PAGE_180   ((uint32_t)0x080B4000) /* Base @ of Page 180, 4 Kbytes */
#define ADDR_FLASH_PAGE_181   ((uint32_t)0x080B5000) /* Base @ of Page 181, 4 Kbytes */
#define ADDR_FLASH_PAGE_182   ((uint32_t)0x080B6000) /* Base @ of Page 182, 4 Kbytes */
#define ADDR_FLASH_PAGE_183   ((uint32_t)0x080B7000) /* Base @ of Page 183, 4 Kbytes */
#define ADDR_FLASH_PAGE_184   ((uint32_t)0x080B8000) /* Base @ of Page 184, 4 Kbytes */
#define ADDR_FLASH_PAGE_185   ((uint32_t)0x080B9000) /* Base @ of Page 185, 4 Kbytes */
#define ADDR_FLASH_PAGE_186   ((uint32_t)0x080BA000) /* Base @ of Page 186, 4 Kbytes */
#define ADDR_FLASH_PAGE_187   ((uint32_t)0x080BB000) /* Base @ of Page 187, 4 Kbytes */
#define ADDR_FLASH_PAGE_188   ((uint32_t)0x080BC000) /* Base @ of Page 188, 4 Kbytes */
#define ADDR_FLASH_PAGE_189   ((uint32_t)0x080BD000) /* Base @ of Page 189, 4 Kbytes */
#define ADDR_FLASH_PAGE_190   ((uint32_t)0x080BE000) /* Base @ of Page 190, 4 Kbytes */
#define ADDR_FLASH_PAGE_191   ((uint32_t)0x080BF000) /* Base @ of Page 191, 4 Kbytes */
#define ADDR_FLASH_PAGE_192   ((uint32_t)0x080C0000) /* Base @ of Page 192, 4 Kbytes */
#define ADDR_FLASH_PAGE_193   ((uint32_t)0x080C1000) /* Base @ of Page 193, 4 Kbytes */
#define ADDR_FLASH_PAGE_194   ((uint32_t)0x080C2000) /* Base @ of Page 194, 4 Kbytes */
#define ADDR_FLASH_PAGE_195   ((uint32_t)0x080C3000) /* Base @ of Page 195, 4 Kbytes */
#define ADDR_FLASH_PAGE_196   ((uint32_t)0x080C4000) /* Base @ of Page 196, 4 Kbytes */
#define ADDR_FLASH_PAGE_197   ((uint32_t)0x080C5000) /* Base @ of Page 197, 4 Kbytes */
#define ADDR_FLASH_PAGE_198   ((uint32_t)0x080C6000) /* Base @ of Page 198, 4 Kbytes */
#define ADDR_FLASH_PAGE_199   ((uint32_t)0x080C7000) /* Base @ of Page 199, 4 Kbytes */
#define ADDR_FLASH_PAGE_200   ((uint32_t)0x080C8000) /* Base @ of Page 200, 4 Kbytes */
#define ADDR_FLASH_PAGE_201   ((uint32_t)0x080C9000) /* Base @ of Page 201, 4 Kbytes */
#define ADDR_FLASH_PAGE_202   ((uint32_t)0x080CA000) /* Base @ of Page 202, 4 Kbytes */
#define ADDR_FLASH_PAGE_203   ((uint32_t)0x080CB000) /* Base @ of Page 203, 4 Kbytes */
#define ADDR_FLASH_PAGE_204   ((uint32_t)0x080CC000) /* Base @ of Page 204, 4 Kbytes */
#define ADDR_FLASH_PAGE_205   ((uint32_t)0x080CD000) /* Base @ of Page 205, 4 Kbytes */
#define ADDR_FLASH_PAGE_206   ((uint32_t)0x080CE000) /* Base @ of Page 206, 4 Kbytes */
#define ADDR_FLASH_PAGE_207   ((uint32_t)0x080CF000) /* Base @ of Page 207, 4 Kbytes */
#define ADDR_FLASH_PAGE_208   ((uint32_t)0x080D0000) /* Base @ of Page 208, 4 Kbytes */
#define ADDR_FLASH_PAGE_209   ((uint32_t)0x080D1000) /* Base @ of Page 209, 4 Kbytes */
#define ADDR_FLASH_PAGE_210   ((uint32_t)0x080D2000) /* Base @ of Page 210, 4 Kbytes */
#define ADDR_FLASH_PAGE_211   ((uint32_t)0x080D3000) /* Base @ of Page 211, 4 Kbytes */
#define ADDR_FLASH_PAGE_212   ((uint32_t)0x080D4000) /* Base @ of Page 212, 4 Kbytes */
#define ADDR_FLASH_PAGE_213   ((uint32_t)0x080D5000) /* Base @ of Page 213, 4 Kbytes */
#define ADDR_FLASH_PAGE_214   ((uint32_t)0x080D6000) /* Base @ of Page 214, 4 Kbytes */
#define ADDR_FLASH_PAGE_215   ((uint32_t)0x080D7000) /* Base @ of Page 215, 4 Kbytes */
#define ADDR_FLASH_PAGE_216   ((uint32_t)0x080D8000) /* Base @ of Page 216, 4 Kbytes */
#define ADDR_FLASH_PAGE_217   ((uint32_t)0x080D9000) /* Base @ of Page 217, 4 Kbytes */
#define ADDR_FLASH_PAGE_218   ((uint32_t)0x080DA000) /* Base @ of Page 218, 4 Kbytes */
#define ADDR_FLASH_PAGE_219   ((uint32_t)0x080DB000) /* Base @ of Page 219, 4 Kbytes */
#define ADDR_FLASH_PAGE_220   ((uint32_t)0x080DC000) /* Base @ of Page 220, 4 Kbytes */
#define ADDR_FLASH_PAGE_221   ((uint32_t)0x080DD000) /* Base @ of Page 221, 4 Kbytes */
#define ADDR_FLASH_PAGE_222   ((uint32_t)0x080DE000) /* Base @ of Page 222, 4 Kbytes */
#define ADDR_FLASH_PAGE_223   ((uint32_t)0x080DF000) /* Base @ of Page 223, 4 Kbytes */
#define ADDR_FLASH_PAGE_224   ((uint32_t)0x080E0000) /* Base @ of Page 224, 4 Kbytes */
#define ADDR_FLASH_PAGE_225   ((uint32_t)0x080E1000) /* Base @ of Page 225, 4 Kbytes */
#define ADDR_FLASH_PAGE_226   ((uint32_t)0x080E2000) /* Base @ of Page 226, 4 Kbytes */
#define ADDR_FLASH_PAGE_227   ((uint32_t)0x080E3000) /* Base @ of Page 227, 4 Kbytes */
#define ADDR_FLASH_PAGE_228   ((uint32_t)0x080E4000) /* Base @ of Page 228, 4 Kbytes */
#define ADDR_FLASH_PAGE_229   ((uint32_t)0x080E5000) /* Base @ of Page 229, 4 Kbytes */
#define ADDR_FLASH_PAGE_230   ((uint32_t)0x080E6000) /* Base @ of Page 230, 4 Kbytes */
#define ADDR_FLASH_PAGE_231   ((uint32_t)0x080E7000) /* Base @ of Page 231, 4 Kbytes */
#define ADDR_FLASH_PAGE_232   ((uint32_t)0x080E8000) /* Base @ of Page 232, 4 Kbytes */
#define ADDR_FLASH_PAGE_233   ((uint32_t)0x080E9000) /* Base @ of Page 233, 4 Kbytes */
#define ADDR_FLASH_PAGE_234   ((uint32_t)0x080EA000) /* Base @ of Page 234, 4 Kbytes */
#define ADDR_FLASH_PAGE_235   ((uint32_t)0x080EB000) /* Base @ of Page 235, 4 Kbytes */
#define ADDR_FLASH_PAGE_236   ((uint32_t)0x080EC000) /* Base @ of Page 236, 4 Kbytes */
#define ADDR_FLASH_PAGE_237   ((uint32_t)0x080ED000) /* Base @ of Page 237, 4 Kbytes */
#define ADDR_FLASH_PAGE_238   ((uint32_t)0x080EE000) /* Base @ of Page 238, 4 Kbytes */
#define ADDR_FLASH_PAGE_239   ((uint32_t)0x080EF000) /* Base @ of Page 239, 4 Kbytes */
#define ADDR_FLASH_PAGE_240   ((uint32_t)0x080F0000) /* Base @ of Page 240, 4 Kbytes */
#define ADDR_FLASH_PAGE_241   ((uint32_t)0x080F1000) /* Base @ of Page 241, 4 Kbytes */
#define ADDR_FLASH_PAGE_242   ((uint32_t)0x080F2000) /* Base @ of Page 242, 4 Kbytes */
#define ADDR_FLASH_PAGE_243   ((uint32_t)0x080F3000) /* Base @ of Page 243, 4 Kbytes */
#define ADDR_FLASH_PAGE_244   ((uint32_t)0x080F4000) /* Base @ of Page 244, 4 Kbytes */
#define ADDR_FLASH_PAGE_245   ((uint32_t)0x080F5000) /* Base @ of Page 245, 4 Kbytes */
#define ADDR_FLASH_PAGE_246   ((uint32_t)0x080F6000) /* Base @ of Page 246, 4 Kbytes */
#define ADDR_FLASH_PAGE_247   ((uint32_t)0x080F7000) /* Base @ of Page 247, 4 Kbytes */
#define ADDR_FLASH_PAGE_248   ((uint32_t)0x080F8000) /* Base @ of Page 248, 4 Kbytes */
#define ADDR_FLASH_PAGE_249   ((uint32_t)0x080F9000) /* Base @ of Page 249, 4 Kbytes */
#define ADDR_FLASH_PAGE_250   ((uint32_t)0x080FA000) /* Base @ of Page 250, 4 Kbytes */
#define ADDR_FLASH_PAGE_251   ((uint32_t)0x080FB000) /* Base @ of Page 251, 4 Kbytes */
#define ADDR_FLASH_PAGE_252   ((uint32_t)0x080FC000) /* Base @ of Page 252, 4 Kbytes */
#define ADDR_FLASH_PAGE_253   ((uint32_t)0x080FD000) /* Base @ of Page 253, 4 Kbytes */
#define ADDR_FLASH_PAGE_254   ((uint32_t)0x080FE000) /* Base @ of Page 254, 4 Kbytes */
#define ADDR_FLASH_PAGE_255   ((uint32_t)0x080FF000) /* Base @ of Page 255, 4 Kbytes */


/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */


/**
******************************************************************************
* @file    Demonstrations/Watermark/Src/main.c
* @author  MCD Application Team
* @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  APPLI_IDLE = 0,
  APPLI_USB_WAITING_DISCONNECT,
  APPLI_GET_FILE_WITHOUT_WATERMARK,
  APPLI_DATA_PROCESSING,
  APPLI_USB_CONNECT,
}APPLI_STATE;

/* BMP */
#pragma pack(1) /* Mandatory to remove any padding */
typedef struct BmpHeader
{
  uint8_t  B;
  uint8_t  M;
  uint32_t fsize;
  uint16_t res1;
  uint16_t res2;
  uint32_t offset;
  uint32_t hsize;
  uint32_t w;
  uint32_t h;
  uint16_t planes;
  uint16_t bpp;
  uint32_t ctype;
  uint32_t dsize;
  uint32_t hppm;
  uint32_t vppm;
  uint32_t colorsused;
  uint32_t colorreq;
}BmpHeader;

#define FILEMGR_FILE_NAME_SIZE 256
#define LOGO_FILE               "logo.png"
#define INPUT_FILE_EXTENSION    ".png"
#define INPUT_FILE_EXTENSION_UC ".PNG"
#define OUTPUT_FILE_EXTENSION   "_logo.png"
#define BITMAP_HEADER_SIZE      sizeof(BmpHeader) /* Bitmap specificity */
#define BITMAP_OFFSET           64 /* multiple of 4 to allow 8888 conversion */


/* PNG */
#define PNG_DECODE_BUF_SIZE 256
#define MAXCOLORMAPSIZE 256
#define PIXEL_ARGB(a, r, g, b) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

enum {
  STAGE_ABORT = -2,
  STAGE_ERROR = -1,
  STAGE_START =  0,
  STAGE_INFO,
  STAGE_IMAGE,
  STAGE_END
};

typedef struct {
  uint8_t a;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} color_t;

#pragma pack(push, 4)
typedef struct  PNGFileLoader
{
  unsigned char* png_data;
  unsigned int png_data_length;
  unsigned int png_data_index;
  int         stage;
  int         rows;
  png_structp png_ptr;
  png_infop   info_ptr;
  png_int_32  width;
  png_int_32  height;
  int         bpp;
  int         color_type;
  png_uint_32 color_key;
  int         color_keyed;
  void       *image;
  uint32_t    pitch;
  uint32_t    palette[256];
  color_t     colors[256];
  int         shared_buffer;
  FIL        *pngFile;
  uint32_t    pngFileSize;
  uint32_t    logo_width;
  uint32_t    logo_height;

} PNGFileLoader;
#pragma pack(pop)

PNGFileLoader pngLoader;

/* APS6408L APMemory memory through OSPI */

/* Read Operations */
#define READ_CMD_SRAM                           0x00
#define READ_LINEAR_BURST_CMD                   0x20
#define READ_HYBRID_BURST_CMD                   0x3F
/* Write Operations */
#define WRITE_CMD                               0x80
#define WRITE_LINEAR_BURST_CMD                  0xA0
#define WRITE_HYBRID_BURST_CMD                  0xBF
/* Reset Operations */
#define RESET_CMD                               0xFF
/* Register Operations */
#define READ_REG_CMD                            0x40
#define WRITE_REG_CMD                           0xC0
/* Default dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_SRAM_READ               5
#define DUMMY_CLOCK_CYCLES_SRAM_WRITE              4

#define OSPI_RAM_START    0x90000000
#define OSPI_RAM_SIZE     ((64*1024*1024)/8)  /* 64 Mbits */
#define OSPI_ZONE_1       OSPI_RAM_START
#define OSPI_ZONE_1_SIZE  0x10000  /* w : 150  y : 100 * 4 bytes per pixel = 0xEA60 */
#define OSPI_ZONE_2       (OSPI_ZONE_1 + OSPI_ZONE_1_SIZE)
#define OSPI_ZONE_2_SIZE  (OSPI_RAM_SIZE - OSPI_ZONE_1_SIZE)

#ifdef EXT_FLASH_ACCESS
#define OSPI_NOR_BUFFER_SIZE     ((uint32_t)0x0080)
#define OSPI_NOR_WRITE_READ_ADDR ((uint32_t)0x0050)
#define OSPI_NOR_BASE_ADDR       ((uint32_t)0x70000000)
#endif

OSPI_HandleTypeDef OSPIHandle = {0};

/* USB */
extern PCD_HandleTypeDef hpcd;
USBD_HandleTypeDef USBD_Device;
__IO uint32_t usbConnected;
__IO uint32_t dma2dTransfer;
__IO uint32_t usbPhysicallyConnected;
__IO uint32_t usb_never_connected = 1;
DMA2D_HandleTypeDef Dma2dHandle = {0};

/* FATFS */
FATFS MMCFatFs; /* File system object for eMMC logical drive */
char MMCPath[4]; /* MMC card logical drive path */
APPLI_STATE appliState = APPLI_IDLE;
uint8_t workBuffer[_MAX_SS];

char inName[FILEMGR_FILE_NAME_SIZE];
char outName[FILEMGR_FILE_NAME_SIZE];

#ifdef EXT_FLASH_ACCESS
uint8_t ospi_nor_aTxBuffer[OSPI_NOR_BUFFER_SIZE];
uint8_t ospi_nor_aRxBuffer[OSPI_NOR_BUFFER_SIZE];
#endif

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
#ifdef EXT_FLASH_ACCESS
void ospi_flash_init(void);
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
#endif
void ospi_ram_init(void);
void ospi_delay_calibration(void);
void usb_waiting_disconnect(void);
void usb_waiting_connected(void);
uint32_t get_file_without_watermark(void);
uint32_t data_processing(void);
uint32_t decode_png(FIL * pngFile, uint8_t * buf, uint32_t * bufSize);
uint32_t decode_image(FIL * pngFile, uint8_t * buf, uint32_t * bufSize);
uint32_t decode_logo(FIL * pngFile, uint8_t * buf, uint32_t * bufSize);
uint32_t encode_png(FIL * outFile, uint8_t * buf, uint32_t * bufSize);
void watermark_png(void);
void Dma2d_TransferError(DMA2D_HandleTypeDef* dma2dHandle);
void Dma2d_TransferComplete(DMA2D_HandleTypeDef* dma2dHandle);

void my_png_warning_fn(png_structp png_ptr, png_const_charp ptr);
void my_png_error_fn(png_structp png_ptr, png_const_charp ptr);
void png_info_callback(png_structp png_read_ptr,
                       png_infop   png_info_ptr);

void png_row_callback(png_structp png_read_ptr,
                      png_bytep   new_row,
                      png_uint_32 row_num,
                      int         pass_num);

void png_end_callback(png_structp png_read_ptr,
                      png_infop   png_info_ptr);
void png_write_callback(png_structp png_ptr,png_bytep data, png_uint_32 length);
void png_IO_flush_callback(png_structp png_ptr);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Main program
* @param  None
* @retval None
*/
int main(void)
{
  /* STM32L4xx HAL library initialization:
  - Configure the Flash prefetch
  - Systick timer is configured by default as source of time base, but user
  can eventually implement his proper time base source (a general purpose
  timer for example or other time source), keeping in mind that Time base
  duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
  handled in milliseconds basis.
  - Set NVIC Group Priority to 4
  - Low Level Initialization
  */

  /* Initialize the HAL Library */
  HAL_Init();

  /* Configure the system clock to 120 Mhz */
  SystemClock_Config();

  /* Enable Power Clock*/
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable USB power on Pwrctrl CR2 register */
  HAL_PWREx_EnableVddUSB();

  /* Configure LED1 (LED_GREEN), LED2 (LED_BLUE)*/
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);

  /* Turn on the LEDs for 0.5 sec. to indicate application is up and running */
  BSP_LED_On(LED_GREEN);
  BSP_LED_On(LED_BLUE);
  HAL_Delay(500);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);

  /* Configure joystick SEL button in interrupt mode */
  {
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Configure Joy SEL pin as input with External interrupt */
    /* Enable the Joy SEL pin clock */
    SEL_JOY_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = SEL_JOY_PIN;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    HAL_GPIO_Init(SEL_JOY_GPIO_PORT, &GPIO_InitStruct);
    /* Enable and set Joy SEL EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(SEL_JOY_EXTI_IRQn), 0x0F, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(SEL_JOY_EXTI_IRQn));
  }

#ifdef EXT_FLASH_ACCESS
  ospi_flash_init();
#endif

  /* Configure RAM Memory-mapped mode through OctoSPI */
  ospi_ram_init();
  ospi_delay_calibration();

  /* FATFS */
  /* Link the eMMC disk I/O driver */
  if (FATFS_LinkDriver(&MMC_Driver, MMCPath) == 0){
    if(f_mount(&MMCFatFs, (TCHAR const*)MMCPath, 1) != FR_OK){
      if(f_mkfs(MMCPath, FM_ANY, 0, workBuffer, sizeof(workBuffer)) != FR_OK){
        Error_Handler();
      }
    }
  }

  /* Initialize usb connection flag */
  usbConnected = 0;
  usbPhysicallyConnected = 0;

  /* Init MSC Application */
  USBD_Init(&USBD_Device, &MSC_Desc, 0);

  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_MSC_CLASS);

  /* Add Storage callbacks for MSC Class */
  USBD_MSC_RegisterStorage(&USBD_Device, &USBD_DISK_fops);

  /* Start Device Process */
  USBD_Start(&USBD_Device);

  appliState = APPLI_USB_CONNECT;

  /* Watermark state machine */
  while (1){
    /* Watermark state machine State Machine */
    switch(appliState){
    case APPLI_USB_CONNECT:
      BSP_LED_Off(LED_GREEN);
      usb_waiting_connected();
      appliState = APPLI_USB_WAITING_DISCONNECT;
      break;

    case APPLI_USB_WAITING_DISCONNECT:
      /* Enable again joystick SEL button interrupt */
      HAL_NVIC_EnableIRQ((IRQn_Type)(SEL_JOY_EXTI_IRQn));
      usb_waiting_disconnect();
      Warning_Clear();
      BSP_LED_On(LED_GREEN);
      appliState = APPLI_GET_FILE_WITHOUT_WATERMARK;
      break;

    case APPLI_GET_FILE_WITHOUT_WATERMARK:
      appliState = (get_file_without_watermark() != 0 ? APPLI_DATA_PROCESSING : APPLI_USB_CONNECT);
      break;

    case APPLI_DATA_PROCESSING:
      appliState = (data_processing() != 0 ? APPLI_USB_CONNECT : APPLI_GET_FILE_WITHOUT_WATERMARK);
      break;

    default:
      break;
    }
  }
}

/**
* @brief  Waiting USB disconnect, stop USB and mount a filesystem.
* @retval None
*/
void usb_waiting_disconnect(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  /* If usb connected Waiting for usb disconnection interrupt */
  while(usbConnected == 1){
  }

  /* Stop USB link */
  if (USBD_Stop(&USBD_Device) != USBD_OK){
    Error_Handler();
  }

  /* Mount file system */
  if(f_mount(&MMCFatFs, (TCHAR const*)MMCPath, 1) != FR_OK){
    Error_Handler();
  }
}

/**
* @brief  Unmount the filesystem, start and Waiting USB connect.
* @retval None
*/
void usb_waiting_connected(void)
{
  /* Unmount a Logical Drive */
  if(f_mount(NULL, (TCHAR const*)MMCPath, 1) != FR_OK){
    Error_Handler();
  }

  /* Start USB link */
  if (USBD_Start(&USBD_Device) != USBD_OK){
    Error_Handler();
  }

  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable again joystick SEL button interrupt */
  HAL_NVIC_EnableIRQ((IRQn_Type)(SEL_JOY_EXTI_IRQn));

  /* If usb connected Waiting for usb disconnection interrupt */
  while(usbConnected == 0)
  {
    if (usb_never_connected)
    {
      HAL_Delay(100);
      BSP_LED_Toggle(LED2);
    }
  }
  if (usb_never_connected)
  {
    usb_never_connected = 0;
    BSP_LED_Off(LED2);
  }
}

/**
* @brief  Find a xxx.png file without xxx_logo.png.
* @retval 1 if found, 0 if not found
*/
uint32_t get_file_without_watermark(void)
{
  DIR dir1, dir2;
  FILINFO fno;
  char *fn;
  FIL fileTmp;
  uint32_t bufSize;

  /* Open the root directory */
  if (f_opendir(&dir1, "") != FR_OK) {
    Error_Handler();
  }

  /* Get logo.png and decode it */
  if(f_open(&fileTmp, LOGO_FILE, FA_READ) != FR_OK) {
    /* No logo, nothing to do, warn the user */
    Warning_Handler();
    if (f_closedir(&dir1) != FR_OK){
      Error_Handler();
    }

    return 0;
  }

  /* Decode Logo in OSPI zone 1 */
  bufSize = OSPI_ZONE_1_SIZE;
  if(decode_logo(&fileTmp, (uint8_t *) OSPI_ZONE_1, &bufSize) != 0) {
    /* logo decode failed, warn the user */
    Warning_Handler();
    if(f_close(&fileTmp) != FR_OK){
      Error_Handler();
    }

    if (f_closedir(&dir1) != FR_OK){
      Error_Handler();
    }

    return 0;
  }

  if(f_close(&fileTmp) != FR_OK){
    Error_Handler();
  }

  while (1){
    if (f_readdir(&dir1, &fno) != FR_OK){
      Error_Handler();
    }

    fn = fno.fname;
    if (fn[0] == 0){
      if (f_closedir(&dir1) != FR_OK){
        Error_Handler();
      }

      return 0;
    }

    if (((strstr(fn, INPUT_FILE_EXTENSION) != 0) || (strstr(fn, INPUT_FILE_EXTENSION_UC) != 0)) && \
      (strstr(fn, OUTPUT_FILE_EXTENSION) == 0) && (strcmp(fn, LOGO_FILE) != 0) ){
        /* Compute the _logo.png filename */
        strcpy(inName, fn);
        strcpy(outName, fn);
        *(outName + strlen(outName) - sizeof (INPUT_FILE_EXTENSION) + 1) = 0;
        strcat(outName, OUTPUT_FILE_EXTENSION);

        if (f_opendir(&dir2, "") != FR_OK){
          Error_Handler();
        }

        uint32_t loop = 1;
        while (loop) {
          if (f_readdir(&dir2, &fno) != FR_OK){
            Error_Handler();
          }

          fn = fno.fname;
          if (fn[0] == 0){
            /* Close the root directory */
            if (f_closedir(&dir1) != FR_OK){
              Error_Handler();
            }

            return 1;
          }
          else{
            if(strcmp(outName, fn) == 0){
              loop = 0;
            }
          }
        }
      }
  }
}

/**
* @brief  Processing the file : decode, watermark and encode.
* @retval 1 if fails, 0 if OK
*/
uint32_t data_processing(void)
{
  FIL fileTmp;
  uint32_t bufSize;

  /* Open and decode the input file */
  if(f_open(&fileTmp, inName, FA_READ) != FR_OK) {
    Error_Handler();
  }

  /* Decode input file and use dma2d to incruste ST logo */
  bufSize = OSPI_ZONE_2_SIZE;

  if (decode_image(&fileTmp, (uint8_t *) OSPI_ZONE_2, &bufSize)) {
    Warning_Handler();
    if(f_close(&fileTmp) != FR_OK) {
      Error_Handler();
    }
    return 1;
  }

  /* Add logo as a banner */
  watermark_png();

  if(f_close(&fileTmp) != FR_OK) {
    Error_Handler();
  }

  /* Create a out file */
  if(f_open(&fileTmp, outName, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
    Error_Handler();
  }

  //  /* Save watermarked image as BMP (debug purpose) */
  //  if(f_write(&fileTmp, decodeBuf, bufSize, (void *)&bufSize) != FR_OK)
  //  {
  //    Error_Handler();
  //  }

  /* Encode PNG in the output file */
  if (encode_png(&fileTmp, (uint8_t*) OSPI_ZONE_2, &bufSize) !=0 ) {
    Warning_Handler();
    if(f_unlink(outName) != FR_OK) {
      Error_Handler();
    }
    return 1;
  }

  if(f_close(&fileTmp) != FR_OK) {
    Error_Handler();
  }

  return 0;
}

/**
* @brief  libPNG warning callback
* @retval None
*/
void my_png_warning_fn(png_structp png_ptr, png_const_charp ptr)
{
  (void)png_ptr;
  (void)ptr;
}

/**
* @brief  libPNG error callback
* @retval None
*/
void my_png_error_fn(png_structp png_ptr, png_const_charp ptr)
{
  (void)png_ptr;
  (void)ptr;
  Error_Handler();
}

/**
* @brief  Decode the logo.
* @retval 1 if fails, 0 if OK
*/
uint32_t decode_logo(FIL * pngFile, uint8_t * buf, uint32_t * bufSize)
{
  pngLoader.image = (void*) buf;

  if (decode_png(pngFile, buf, bufSize)){
    return 1;
  }

  pngLoader.logo_height = pngLoader.height;
  pngLoader.logo_width = pngLoader.width;
  return 0;
}

/**
* @brief  Decode image.
* @retval 1 if fails, 0 if OK
*/
uint32_t decode_image(FIL * pngFile, uint8_t * buf, uint32_t * bufSize)
{
  pngLoader.image = (void*) (buf + BITMAP_OFFSET);

  if (decode_png(pngFile, buf, bufSize)){
    return 1;
  }

  /* BMP can be used as debug */
  BmpHeader* pbmpheader = (BmpHeader*)buf;

  /* Header BMP */
  pbmpheader->B = 'B';
  pbmpheader->M = 'M' ;
  pbmpheader->fsize = pngLoader.height * pngLoader.pitch + BITMAP_OFFSET;
  pbmpheader->res1 = 0;
  pbmpheader->res2 = 0;
  pbmpheader->offset = BITMAP_OFFSET; /* 4 multiples to enable 8888 conversion */
  pbmpheader->hsize = BITMAP_HEADER_SIZE - 14;
  pbmpheader->w = pngLoader.width;
  pbmpheader->h = pngLoader.height;
  pbmpheader->planes = 0x001;
  pbmpheader->bpp = (pngLoader.pitch / pngLoader.width) * 8;
  pbmpheader->ctype = 0;
  pbmpheader->dsize = pbmpheader->fsize - pbmpheader->offset;
  pbmpheader->hppm = 0xB12;
  pbmpheader->vppm = 0xB12;
  pbmpheader->colorsused = 0;
  pbmpheader->colorreq = 0;

  *bufSize = pbmpheader->fsize;
  return 0;
}

/**
* @brief  Decode png file.
* @retval 1 if fails, 0 if OK
*/
uint32_t decode_png(FIL * pngFile, uint8_t * buf, uint32_t * bufSize)
{
  uint8_t localBuf[PNG_DECODE_BUF_SIZE];
  uint32_t size;

  pngLoader.png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, my_png_error_fn, my_png_warning_fn);
  if (!pngLoader.png_ptr) {
    Error_Handler();
  }

  if (setjmp(png_jmpbuf(pngLoader.png_ptr))) {
    Error_Handler();
  }

  pngLoader.info_ptr = png_create_info_struct(pngLoader.png_ptr);
  if (!pngLoader.info_ptr) {
    Error_Handler();
  }

  png_set_progressive_read_fn(pngLoader.png_ptr,
                              &pngLoader,
                              png_info_callback,
                              png_row_callback,
                              png_end_callback);

  pngLoader.stage = STAGE_START;

  /* Read header*/
  f_read(pngFile, localBuf, 8, (unsigned int*)&size);
  png_process_data(pngLoader.png_ptr, pngLoader.info_ptr, (png_bytep)localBuf, size);

  if (setjmp(png_jmpbuf(pngLoader.png_ptr))) {
    Error_Handler();
  }

  /* Decode pixel */
  uint32_t testSize = 1;
  do{
    if(f_read(pngFile, (void*) localBuf, PNG_DECODE_BUF_SIZE, (void *)&size) != FR_OK) {
      Error_Handler();
    }

    png_process_data(pngLoader.png_ptr, pngLoader.info_ptr, (png_bytep)localBuf, size);

    if (setjmp(png_jmpbuf(pngLoader.png_ptr))) {
      Error_Handler();
    }

    /* Check that the decoded image with with allocated memory */
    if ((pngLoader.stage == STAGE_IMAGE) && (testSize == 1)) {
      testSize = 0;
      if (pngLoader.pitch * pngLoader.height > *bufSize) {
        png_destroy_read_struct(&pngLoader.png_ptr, &pngLoader.info_ptr, NULL);
        pngLoader.png_ptr = NULL;
        pngLoader.info_ptr = NULL;
        return 1;
      }
    }
  } while(size != 0);

  if (pngLoader.stage != STAGE_END) {
    Error_Handler();
  }

  png_destroy_read_struct(&pngLoader.png_ptr, &pngLoader.info_ptr, NULL);
  pngLoader.png_ptr = NULL;
  pngLoader.info_ptr = NULL;

  return 0;
}

/**
* @brief  Watermark image with the logo thanks to dma2d.
* @retval 1 if fails, 0 if OK
*/
void watermark_png(void)
{
  uint32_t w, h, W, H, x, y, i, end;

  h = pngLoader.logo_height;
  W = pngLoader.width;
  H = pngLoader.height;

  /* Center image height */
  y = (H - h) / 2;

  end = 0;
  i = 0;

  do{
    w = pngLoader.logo_width;
    x = i * (w + 10);
    i++;

    if (x >= W){
      end = 1;
    }
    else {

      /* Last logo, truncate it */
      if ((x + w) > W){
        w = W - x;
      }

      /* Watermark with dma2d */
      Dma2dHandle.Init.Mode         = DMA2D_M2M_BLEND; /* Mode Memory To Memory with pixel conversion */
      Dma2dHandle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
      Dma2dHandle.Init.OutputOffset = W - w;
      Dma2dHandle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;      /* No R&B swap for the output image */
      Dma2dHandle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;   /* No alpha inversion for the output image */
      Dma2dHandle.XferCpltCallback  = Dma2d_TransferComplete;
      Dma2dHandle.XferErrorCallback = Dma2d_TransferError;

      /* Foreground Layer Configuration */
      Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_COMBINE_ALPHA; /* Keep original Alpha from ARGB8888 input */
      Dma2dHandle.LayerCfg[1].InputAlpha = 0x8F; /* Fully opaque */
      Dma2dHandle.LayerCfg[1].InputColorMode = DMA2D_OUTPUT_ARGB8888;
      Dma2dHandle.LayerCfg[1].InputOffset = pngLoader.logo_width - w; /* No offset in input excpet the last logo */
      Dma2dHandle.LayerCfg[1].RedBlueSwap   = DMA2D_RB_REGULAR;      /* No R&B swap for the input image */
      Dma2dHandle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;   /* No alpha inversion for the input image */
      Dma2dHandle.Instance = DMA2D;
      dma2dTransfer = 0;

      /* Background layer Configuration */
      Dma2dHandle.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
      /* Dma2dHandle.LayerCfg[0].InputAlpha = 0xxx; Not use with DMA2D_NO_MODIF_ALPHA */
      Dma2dHandle.LayerCfg[0].InputColorMode = DMA2D_OUTPUT_ARGB8888;
      Dma2dHandle.LayerCfg[0].InputOffset = W - w;  /* No offset in input */
      Dma2dHandle.LayerCfg[0].RedBlueSwap = DMA2D_RB_REGULAR;      /* No BackGround Red a Blue swap */
      Dma2dHandle.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No BackGround Alpha inversion */

      /*##-4- DMA2D Initialisation ###############################################*/
      if(HAL_DMA2D_Init(&Dma2dHandle) != HAL_OK) {
        Error_Handler();
      }

      /* Configure Foreground layer (ie layer 1) */
      if(HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1) != HAL_OK) {
        Error_Handler();
      }

      /* Configure Backgroung layer (ie layer 1) */
      if(HAL_DMA2D_ConfigLayer(&Dma2dHandle, 0) != HAL_OK) {
        Error_Handler();
      }

      /*##-5- Start DMA2D transfer ###############################################*/
      if(HAL_DMA2D_BlendingStart_IT(&Dma2dHandle,
                                    (uint32_t)OSPI_ZONE_1, /* Foreground */
                                    (uint32_t)((uint32_t)pngLoader.image + (pngLoader.pitch / W) * (y * W + x)), /* Back ground */
                                    (uint32_t)((uint32_t)pngLoader.image + (pngLoader.pitch / W) * (y * W + x)), /* Destination */
                                    w, /* Width of the 2D memory transfer in pixels */
                                    h) /* Height of the 2D memory transfer in lines */
         != HAL_OK) {
           Error_Handler();
         }

      /* Wait for dma2d transfer */
      while(dma2dTransfer == 0);

      if(HAL_DMA2D_DeInit(&Dma2dHandle) != HAL_OK) {
        Error_Handler();
      }
    }
  }
  while(end == 0);

}

/* Encode png */
uint32_t encode_png(FIL * outFile, uint8_t * buf, uint32_t * bufSize)
{
  pngLoader.pngFile = outFile;
  pngLoader.pngFileSize = 0;

  pngLoader.png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, my_png_error_fn, my_png_warning_fn);
  if (!pngLoader.png_ptr){
    Error_Handler();
  }

  pngLoader.info_ptr = png_create_info_struct(pngLoader.png_ptr);
  if (!pngLoader.info_ptr){
    Error_Handler();
  }

  if (setjmp(png_jmpbuf(pngLoader.png_ptr))){
    Error_Handler();
  }

  png_set_write_fn(pngLoader.png_ptr,
                   (void *)&pngLoader,
                   png_write_callback,
                   png_IO_flush_callback);

  png_set_IHDR( pngLoader.png_ptr, pngLoader.info_ptr,
               pngLoader.width, pngLoader.height,
               8, PNG_COLOR_TYPE_RGB_ALPHA,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);


  png_set_bgr(pngLoader.png_ptr);


  /* set the zlib compression level */
  png_set_compression_level(pngLoader.png_ptr, 1);  /* No Compression = 0 Highest compression = 9 */

  /* set other zlib parameters */

  /* The parameter mem_level corresponds directly to the memLevel parameter of the libz deflateInit2_() interface.
  This parameter shall specify how much memory to use for the internal state.
  The value of mem_level must be between 1 and MAX_MEM_LEVEL.
  Smaller values use less memory but are slower,
  while higher values use more memory to gain compression speed. */
  png_set_compression_mem_level(pngLoader.png_ptr, 1);

  /* Write header */
  png_write_info( pngLoader.png_ptr, pngLoader.info_ptr);

  /* Encode and right the row line */
  png_const_bytep row = (png_const_bytep) ((uint32_t)buf + BITMAP_OFFSET);

  /* Loop through the rows */
  for (uint32_t i = 0; i < pngLoader.height; i++) {
    png_write_row(pngLoader.png_ptr, row);
    row += pngLoader.pitch;
  }


  // Write the additional chunks to the PNG file (not really needed)
  png_write_end(pngLoader.png_ptr,  pngLoader.info_ptr);

  // Clean up after the write, and free any memory allocated
  png_destroy_write_struct(&(pngLoader.png_ptr), (png_infopp) NULL);

  pngLoader.png_ptr = NULL;
  pngLoader.info_ptr = NULL;

  return 0;
}

void png_info_callback(png_structp png_read_ptr,
                       png_infop   png_info_ptr)
{
  PNGFileLoader *pngLoader = (PNGFileLoader*)png_get_progressive_ptr(png_read_ptr);

  if (pngLoader->stage < 0)
    return;

  pngLoader->stage = STAGE_INFO;
  uint32_t  ret = png_get_IHDR(pngLoader->png_ptr, pngLoader->info_ptr,
                               (png_uint_32 *)&pngLoader->width, (png_uint_32 *)&pngLoader->height,
                               &pngLoader->bpp, &pngLoader->color_type,
                               NULL, NULL, NULL);

  if ((pngLoader->height == 0) || (pngLoader->width == 0) || (ret != 1)){
    return;
  }

  pngLoader->pitch = pngLoader->width * 4;

  if (!pngLoader->color_keyed) {
    png_set_strip_16(pngLoader->png_ptr);
  }

  if (!(pngLoader->color_type & PNG_COLOR_MASK_ALPHA)) {
    png_set_filler(pngLoader->png_ptr, 0xFF, PNG_FILLER_AFTER);
  }

  png_set_bgr(pngLoader->png_ptr);
  png_set_interlace_handling(pngLoader->png_ptr);
  png_read_update_info(pngLoader->png_ptr, pngLoader->info_ptr);
}

void png_row_callback(png_structp png_read_ptr,
                      png_bytep   new_row,
                      png_uint_32 row_num,
                      int         pass_num)
{
  PNGFileLoader *pngLoader = (PNGFileLoader*)png_get_progressive_ptr(png_read_ptr);

  if (pngLoader->stage < 0)
    return;

  pngLoader->stage = STAGE_IMAGE;
  png_progressive_combine_row(pngLoader->png_ptr,
                              (png_bytep)((uint8_t*)pngLoader->image + row_num * pngLoader->pitch),
                              new_row);
  pngLoader->rows++;
}

void png_end_callback(png_structp png_read_ptr,
                      png_infop   png_info_ptr)
{
  PNGFileLoader *pngLoader = (PNGFileLoader*)png_get_progressive_ptr(png_read_ptr);

  if (pngLoader->stage < 0){
    Error_Handler();
  }
  else{
    pngLoader->stage = STAGE_END;
  }
}

void png_write_callback(png_structp png_ptr,png_bytep data, png_uint_32 length)
{
  uint32_t bufSize = 0;
  PNGFileLoader *pngLoader = (PNGFileLoader*)(png_get_io_ptr(png_ptr));

  if(f_write(pngLoader->pngFile, data, length, (void *)&bufSize) != FR_OK){
    Error_Handler();
  }

  if (bufSize != length){
    Error_Handler();
  }

  pngLoader->pngFileSize += bufSize;
}


void png_IO_flush_callback(png_structp png_ptr)
{
  (void)png_ptr;
}

/**
* @brief  DMA2D Transfer completed callback
* @param  hdma2d: DMA2D handle.
* @note   This example shows a simple way to report end of DMA2D transfer, and
*         you can add your own implementation.
* @retval None
*/
void Dma2d_TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
  dma2dTransfer = 1;
}

/**
* @brief  DMA2D error callbacks
* @param  hdma2d: DMA2D handle
* @note   This example shows a simple way to report DMA2D transfer error, and you can
*         add your own implementation.
* @retval None
*/
void Dma2d_TransferError(DMA2D_HandleTypeDef *hdma2d)
{
  Error_Handler();
}

/**
* @brief  System Clock Configuration
*         The system Clock is configured as follow :
*
*         If define USB_USE_LSE_MSI_CLOCK enabled:
*            System Clock source            = PLL (MSI)
*            SYSCLK(Hz)                     = 120000000
*            HCLK(Hz)                       = 120000000
*            AHB Prescaler                  = 1
*            APB1 Prescaler                 = 1
*            APB2 Prescaler                 = 1
*            MSI Frequency(Hz)              = 48000000
*            PLL_M                          = 12
*            PLL_N                          = 60
*            PLL_Q                          = 2
*            PLL_R                          = 2
*            PLL_P                          = 7
*            Flash Latency(WS)              = 4
*
*         If define USE_USB_CLKSOURCE_CRSHSI48 enabled:
*            System Clock source            = PLL (MSI)
*            SYSCLK(Hz)                     = 120000000
*            HCLK(Hz)                       = 120000000
*            AHB Prescaler                  = 1
*            APB1 Prescaler                 = 1
*            APB2 Prescaler                 = 1
*            MSI Frequency(Hz)              = 4000000
*            PLL_M                          = 1
*            PLL_N                          = 60
*            PLL_Q                          = 2
*            PLL_R                          = 2
*            PLL_P                          = 7
*            Flash Latency(WS)              = 4
*
* @param  None
* @retval None
*/
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

#if defined (USE_USB_CLKSOURCE_CRSHSI48)
  RCC_CRSInitTypeDef RCC_CRSInitStruct;
#endif

  /* Enable voltage range 1 boost mode for frequency above 80 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  __HAL_RCC_PWR_CLK_DISABLE();

#if defined (USE_USB_CLKSOURCE_CRSHSI48)

  /* Enable the HSI48 Oscillator */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_OFF;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Enable MSI Oscillator and activate PLL with MSI as source   */
  /* (Default MSI Oscillator enabled at system reset remains ON) */
  RCC_OscInitStruct.OscillatorType = (RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_HSI48);
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /* Enable MSI Auto-calibration through LSE */
  HAL_RCCEx_EnableMSIPLLMode();

  /* Select HSI48 output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* To avoid undershoot due to maximum frequency, select PLL as system clock source */
  /* with AHB prescaler divider 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
    Error_Handler();
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    Error_Handler();
  }

  /*Configure the clock recovery system (CRS)**********************************/

  /* Enable CRS Clock */
  __HAL_RCC_CRS_CLK_ENABLE();

  /* Default Synchro Signal division factor (not divided) */
  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;

  /* Set the SYNCSRC[1:0] bits according to CRS_Source value */
  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;

  /* HSI48 is synchronized with USB SOF at 1KHz rate */
  RCC_CRSInitStruct.ReloadValue =  __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
  RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;

  /* Set the TRIM[5:0] to the default value*/
  RCC_CRSInitStruct.HSI48CalibrationValue = 0x20;

  /* Start automatic synchronization */
  HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);

#elif defined (USB_USE_LSE_MSI_CLOCK)

  /* Enable MSI Oscillator and activate PLL with MSI as source   */
  /* (Default MSI Oscillator enabled at system reset remains ON) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11; /*!< MSI = 48 MHz. Do not modify this value used as SAI Source, MMC  */
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /* Enable MSI Auto-calibration through LSE */
  HAL_RCCEx_EnableMSIPLLMode();

  /* Select MSI output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_MSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* To avoid undershoot due to maximum frequency, select PLL as system clock source */
  /* with AHB prescaler divider 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
    Error_Handler();
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    Error_Handler();
  }

#endif /* USB_USE_HSI48_CLOCK */
}


#ifdef EXT_FLASH_ACCESS
void ospi_flash_init(void)
{
  static OSPI_NOR_Info pOSPI_NOR_Info;

  if (OSPI_NOR_ERROR == BSP_OSPI_NOR_Init())
  {
    Error_Handler();
  }
  pOSPI_NOR_Info.FlashSize          = (uint32_t)0x00;
  pOSPI_NOR_Info.EraseSectorSize    = (uint32_t)0x00;
  pOSPI_NOR_Info.EraseSectorsNumber = (uint32_t)0x00;
  pOSPI_NOR_Info.ProgPageSize       = (uint32_t)0x00;
  pOSPI_NOR_Info.ProgPagesNumber    = (uint32_t)0x00;

  /* Read the OSPI NOR memory info */
  BSP_OSPI_NOR_GetInfo(&pOSPI_NOR_Info);

  /* Test the correctness */
  if((pOSPI_NOR_Info.FlashSize          != MX25LM51245G_FLASH_SIZE) ||
     (pOSPI_NOR_Info.EraseSectorSize    != MX25LM51245G_BLOCK_SIZE)  ||
       (pOSPI_NOR_Info.ProgPageSize       != MX25LM51245G_PAGE_SIZE)  ||
         (pOSPI_NOR_Info.EraseSectorsNumber != (MX25LM51245G_FLASH_SIZE/MX25LM51245G_BLOCK_SIZE)) ||
           (pOSPI_NOR_Info.ProgPagesNumber    != (MX25LM51245G_FLASH_SIZE/MX25LM51245G_PAGE_SIZE)))
  {
    Error_Handler();
  }

  if (OSPI_NOR_ERROR == BSP_OSPI_NOR_Erase_Block(OSPI_NOR_WRITE_READ_ADDR))
  {
    Error_Handler();
  }

  Fill_Buffer(ospi_nor_aTxBuffer, OSPI_NOR_BUFFER_SIZE, 0xD20F);

  /* Write data */
  if(OSPI_NOR_ERROR == BSP_OSPI_NOR_Write(ospi_nor_aTxBuffer, OSPI_NOR_WRITE_READ_ADDR, OSPI_NOR_BUFFER_SIZE))
  {
    Error_Handler();
  }

  /* Read back data from the OSPI NOR memory */
  if(OSPI_NOR_ERROR == BSP_OSPI_NOR_Read(ospi_nor_aRxBuffer, OSPI_NOR_WRITE_READ_ADDR, OSPI_NOR_BUFFER_SIZE))
  {
    Error_Handler();
  }

  if(Buffercmp(ospi_nor_aRxBuffer, ospi_nor_aTxBuffer, OSPI_NOR_BUFFER_SIZE) > 0)
  {
    Error_Handler();
  }

  if (OSPI_NOR_ERROR == BSP_OSPI_NOR_DeInit())
  {
    Error_Handler();
  }
}


/**
* @brief  Fills buffer with user predefined data.
* @param  pBuffer: pointer on the buffer to fill
* @param  uwBufferLenght: size of the buffer to fill
* @param  uwOffset: first value to fill on the buffer
* @retval None
*/
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
* @brief  Compares two buffers.
* @param  pBuffer1, pBuffer2: buffers to be compared.
* @param  BufferLength: buffer's length
* @retval 1: pBuffer identical to pBuffer1
*         0: pBuffer differs from pBuffer1
*/
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
#endif

void ospi_ram_init(void)
{
  OSPI_RegularCmdTypeDef  sCommand = {0};
  OSPI_MemoryMappedTypeDef sMemMappedCfg = {0};
  uint8_t reg[2];

  OSPIHandle.Instance = OCTOSPI1;
  HAL_OSPI_DeInit(&OSPIHandle);

  OSPIHandle.Init.FifoThreshold         = 2;
  OSPIHandle.Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  OSPIHandle.Init.MemoryType            = HAL_OSPI_MEMTYPE_APMEMORY;
  OSPIHandle.Init.DeviceSize            = 23; /* 64 MBits */
  OSPIHandle.Init.ChipSelectHighTime    = 1;
  OSPIHandle.Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  OSPIHandle.Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  OSPIHandle.Init.ClockPrescaler        = 0x03;
  OSPIHandle.Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  OSPIHandle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  OSPIHandle.Init.ChipSelectBoundary    = 4;
  OSPIHandle.Init.DelayBlockBypass      = HAL_OSPI_DELAY_BLOCK_USED;
  OSPIHandle.Init.MaxTran               = 0;

  if (HAL_OSPI_Init(&OSPIHandle) != HAL_OK) {
    Error_Handler();
  }

  sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize        = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_8_LINES;
  sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand.DQSMode            = HAL_OSPI_DQS_ENABLE;
  sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction        = READ_REG_CMD;
  sCommand.Address            = 0;
  sCommand.NbData             = 2;
  sCommand.DummyCycles        = 5;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    Error_Handler();
  }

  if (HAL_OSPI_Receive(&OSPIHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    Error_Handler();
  }

  sCommand.Instruction = WRITE_REG_CMD;
  sCommand.DummyCycles = 0;
  MODIFY_REG(reg[0], 0x03, 0x00);

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    Error_Handler();
  }

  if (HAL_OSPI_Transmit(&OSPIHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    Error_Handler();
  }

  sCommand.OperationType      = HAL_OSPI_OPTYPE_WRITE_CFG;
  sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction        = WRITE_CMD;
  sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize        = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_8_LINES;
  sCommand.NbData             = 10;
  sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_SRAM_WRITE;
  sCommand.DQSMode            = HAL_OSPI_DQS_ENABLE;
  sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    Error_Handler();
  }

  sCommand.OperationType = HAL_OSPI_OPTYPE_READ_CFG;
  sCommand.Instruction   = READ_CMD_SRAM;
  sCommand.DummyCycles   = DUMMY_CLOCK_CYCLES_SRAM_READ;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    Error_Handler();
  }

  sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;
  /* sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_ENABLE; */
  //sMemMappedCfg.TimeOutPeriod = 0x34;

  if (HAL_OSPI_MemoryMapped(&OSPIHandle, &sMemMappedCfg) != HAL_OK) {
    Error_Handler();
  }
}

void ospi_delay_calibration(void)
{
  __IO uint8_t *mem_addr = (uint8_t *)OCTOSPI1_BASE;
  uint8_t TX_Buffer[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ-ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ_ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  uint32_t test_ongoing = 1;
  uint32_t delay = 0; /* Start with lowest possible delay */
  uint32_t test_failed;

  while (test_ongoing)
  {
    RCC->DLYCFGR = delay;
    test_failed = 0;

    mem_addr = (uint8_t *)OCTOSPI1_BASE;

    for (uint32_t index = 0; index < sizeof(TX_Buffer); index++)
    {
      *mem_addr = TX_Buffer[index];
      mem_addr++;
    }

    HAL_Delay(1);

    mem_addr = (uint8_t *)OCTOSPI1_BASE;

    for (uint32_t index = 0; index < sizeof(TX_Buffer); index++)
    {
      if (*mem_addr != TX_Buffer[index])
      {
        test_failed = 1;
        break;
      }
      mem_addr++;
    }

    if (test_failed == 1)
    {
      if (delay < 15)
      {
        /* Increase delay and relaunch iteration */
        delay++;
      }
      else
      {
        /* If delay set to maximum and error still detected: can't use external PSRAM */
        Error_Handler();
      }
    }
    else
    {
      /* Proper delay found, exit calibration */
      test_ongoing = 0;
    }
  }
}

/**
* @brief  This function is executed in case of unexpected error occurrence. Appli is dead !
* @param  None
* @retval None
*/
void Error_Handler(void)
{
  while (1) {
    HAL_Delay(200);
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
  }
}

/**
* @brief  This function is executed in case of warning. User input are not correct and can be modified.
* @param  None
* @retval None
*/
void Warning_Handler(void)
{
  BSP_LED_On(LED2);
}

/**
* @brief  Clear Warning.
* @param  None
* @retval None
*/
void Warning_Clear(void)
{
  BSP_LED_Off(LED2);
}

/**
* @brief EXTI line detection callbacks
* @param GPIO_Pin: Specifies the pins connected EXTI line
* @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == SEL_JOY_PIN)
  {
    if ((appliState == APPLI_USB_CONNECT) && (usbPhysicallyConnected == 1)) /* USB physically connected */
    {
      /* Enable USB Vdd to reconnect USB device */
      HAL_PWREx_EnableVddUSB();
    }
    else
    {
      if (appliState == APPLI_USB_WAITING_DISCONNECT)
      {
        /* Disable USB Vdd to disconnect USB device */
        HAL_PWREx_DisableVddUSB();
      }
    }
  }
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


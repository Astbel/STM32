
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "flash.h"
#include "uartMsg.h"
/* Private includes ----------------------------------------------------------*/



#define DATA_32                 ((uint32_t)0x2356)
#define DATA_64                 ((uint64_t)0x12345678)
//flash adder
// #define FLASH_USER_START_ADDR  ADDR_FLASH_PAGE_4   /* Start @ of user Flash area */
// #define FLASH_USER_END_ADDR     (ADDR_FLASH_PAGE_5 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */

uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
uint32_t Address = 0, PageError = 0;
__IO uint32_t MemoryProgramStatus = 0;
__IO uint32_t data32 = 0;    /*!< defines 'read / write' permissions              */ 

static uint16_t fail_cnt;
Uint8 Flash_done_Flag;
/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Address);

/* USER CODE END PFP */
void Flash_Data(uint32_t Start_Addr,uint32_t End_Addr,Uint32 write_data);
/* Private user code ---------------------------------------------------------*/
/**
  * @brief  Templete of Flashing Data
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
void Flash_Data(uint32_t Start_Addr,uint32_t End_Addr,Uint32 write_data)
{
    fail_cnt =0;
    uart_send_msg("Start write data to flash memory\r\n");
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

     /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

     /* Get the 1st page to erase */
  FirstPage = GetPage(Start_Addr);

  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(End_Addr) - FirstPage + 1;

  /* Get the bank */
  BankNumber = GetBank(Start_Addr);

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
    /* Infinite loop */
        while (1)
        {
            /* Make LED2 blink (100ms on, 2s off) to indicate error in Erase operation */
            uart_send_msg("Erasing Flash memory\r\n");
        }
    }
  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
    Address = Start_Addr;

    while (Address < End_Addr)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, write_data) == HAL_OK)
        {
            Address = Address + 8;  /* increment to next double word*/
        }
        else
        {
            /* Error occurred while writing data in Flash memory.
            User can add here some code to deal with this error */
            while (1)
            {
                /* Make LED2 blink (100ms on, 2s off) to indicate error in Write operation */
                uart_send_msg("fail writing data\r\n");
            }
          
        }
    }
     /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();
    Flash_done_Flag =1;
    uart_send_msg("End with Write memory !!!\r\n");
  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
    Address = Start_Addr;
     MemoryProgramStatus = 0x0;

    while (Address < End_Addr)
    {
        data32 = *(__IO uint32_t *)Address;//讀FLASH中的資料，直接給出地址就行了。跟從記憶體中讀資料一樣

        if (data32 != write_data)
        {
            MemoryProgramStatus++;
        }
        Address = Address + 4;
    }

     /*Check if there is an issue to program data*/
    if (MemoryProgramStatus == 0)
    {
        /* No error detected. Switch on LED2*/
        uart_send_msg("Done with Flash memory !!!\r\n");
    }
//   else
//   {
//       /* Error detected. LED2 will blink with 1s period */
//       while (1)
//       {
//          //  uart_send_msg("Error with flash memory\r\n");
//            uart_send_msg(MemoryProgramStatus);
//           fail_cnt++;
//       }
//   }
}
//Erase Data
void Erase_Data(uint32_t Start_Addr,uint32_t End_Addr,Uint32 write_data)
{
    fail_cnt =0;
    uart_send_msg("Erase memory\r\n");
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

     /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

     /* Get the 1st page to erase */
  FirstPage = GetPage(Start_Addr);

  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(End_Addr) - FirstPage + 1;

  /* Get the bank */
  BankNumber = GetBank(Start_Addr);

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
    /* Infinite loop */
        while (1)
        {
            /* Make LED2 blink (100ms on, 2s off) to indicate error in Erase operation */
            uart_send_msg("Erasing Flash memory\r\n");
        }
    }
  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
    Address = Start_Addr;

    while (Address < End_Addr)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, write_data) == HAL_OK)
        {
            Address = Address + 8;  /* increment to next double word*/
        }
        else
        {
            /* Error occurred while writing data in Flash memory.
            User can add here some code to deal with this error */
            while (1)
            {
                /* Make LED2 blink (100ms on, 2s off) to indicate error in Write operation */
                uart_send_msg("fail writing data\r\n");
            }
          
        }
    }
     /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();
    Flash_done_Flag =1;
    uart_send_msg("End with Write memory !!!\r\n");
  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
    Address = Start_Addr;
     MemoryProgramStatus = 0x0;

    while (Address < End_Addr)
    {
        data32 = *(__IO uint32_t *)Address;//讀FLASH中的資料，直接給出地址就行了。跟從記憶體中讀資料一樣

        if (data32 != write_data)
        {
            MemoryProgramStatus++;
        }
        Address = Address + 4;
    }

     /*Check if there is an issue to program data*/
    if (MemoryProgramStatus == 0)
    {
        /* No error detected. Switch on LED2*/
        uart_send_msg("Done with Flash memory !!!\r\n");
    }
//   else
//   {
//       /* Error detected. LED2 will blink with 1s period */
//       while (1)
//       {
//          //  uart_send_msg("Error with flash memory\r\n");
//            uart_send_msg(MemoryProgramStatus);
//           fail_cnt++;
//       }
//   }
}

//UART Send Data
void UART_Send_Data(uint32_t Start_Addr,uint32_t End_Addr,Uint32 write_data)
{
    fail_cnt =0;
    uart_send_msg("Writting memory\r\n");
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

     /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

     /* Get the 1st page to erase */
  FirstPage = GetPage(Start_Addr);

  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(End_Addr) - FirstPage + 1;

  /* Get the bank */
  BankNumber = GetBank(Start_Addr);

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
    /* Infinite loop */
        while (1)
        {
            /* Make LED2 blink (100ms on, 2s off) to indicate error in Erase operation */
            uart_send_msg("Erasing Flash memory\r\n");
        }
    }
  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
    Address = Start_Addr;

    while (Address < End_Addr)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, write_data) == HAL_OK)
        {
            Address = Address + 8;  /* increment to next double word*/
        }
        else
        {
            /* Error occurred while writing data in Flash memory.
            User can add here some code to deal with this error */
            while (1)
            {
                /* Make LED2 blink (100ms on, 2s off) to indicate error in Write operation */
                uart_send_msg("fail writing data\r\n");
            }
          
        }
    }
     /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();
    Flash_done_Flag =1;
    uart_send_msg("End with Write memory !!!\r\n");
  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
    Address = Start_Addr;
     MemoryProgramStatus = 0x0;

    while (Address < End_Addr)
    {
        data32 = *(__IO uint32_t *)Address;//讀FLASH中的資料，直接給出地址就行了。跟從記憶體中讀資料一樣

        if (data32 != write_data)
        {
            MemoryProgramStatus++;
        }
        Address = Address + 4;
    }

     /*Check if there is an issue to program data*/
    if (MemoryProgramStatus == 0)
    {
        /* No error detected. Switch on LED2*/
        uart_send_msg("Done with Flash memory !!!\r\n");
    }
//   else
//   {
//       /* Error detected. LED2 will blink with 1s period */
//       while (1)
//       {
//          //  uart_send_msg("Error with flash memory\r\n");
//            uart_send_msg(MemoryProgramStatus);
//           fail_cnt++;
//       }
//   }
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
 static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
}











































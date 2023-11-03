
#include "main.h"

static uint32_t GetSector(uint32_t Address);

FLASH_EraseInitTypeDef flashstruct;
// initail Flash
void Flash_Init(void)
{
    HAL_FLASH_Unlock();
}

// Write memory
void Flash_Write_Flash_Memory(void)
{
    HAL_FLASH_Unlock();

    HAL_FLASH_Lock();
}

// Erase memory
void Flash_Erase_Flash_Memory(void)
{
    HAL_FLASH_Unlock();
    /* Get the 1st sector to erase */
    FirstSector = GetSector(FLASH_USER_START_ADDR);
    /* Get the number of sector to erase from 1st sector*/
    NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;
    /* Fill EraseInit structure*/
    flashstruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    flashstruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    flashstruct.Sector = FirstSector;
    flashstruct.NbSectors = NbOfSectors;
    // Waitting erasing the flash
    if (HAL_FLASHEx_Erase(&flashstruct, &SECTORError) != HAL_OK)
    {
        while (1)
        {
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            HAL_Delay(1000);
        }
    }

    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();

    Address = FLASH_USER_START_ADDR;

    while (Address < FLASH_USER_END_ADDR)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
        {
            Address = Address + 4;
        }
        else
        {
            while (1)
            {
                HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
                HAL_Delay(1000);
            }
        }
    }
    HAL_FLASH_Lock();
    Address = FLASH_USER_START_ADDR;
    MemoryProgramStatus = 0x0;

    while (Address < FLASH_USER_END_ADDR)
    {
        data32 = *(__IO uint32_t *)Address;

        if (data32 != DATA_32)
        {
            MemoryProgramStatus++;
        }
        Address = Address + 4;
    }
}

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_7) */
    {
        sector = FLASH_SECTOR_7;
    }

    return sector;
}

/**
 * @brief  Gets sector Size
 * @param  None
 * @retval The size of a given sector
 */
// uint32_t GetSectorSize(uint32_t Sector)
// {
//   uint32_t sectorsize = 0x00;

//   if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
//   {
//     sectorsize = 16 * 1024;
//   }
//   else if(Sector == FLASH_SECTOR_4)
//   {
//     sectorsize = 64 * 1024;
//   }
//   else
//   {
//     sectorsize = 128 * 1024;
//   }
//   return sectorsize;
// }

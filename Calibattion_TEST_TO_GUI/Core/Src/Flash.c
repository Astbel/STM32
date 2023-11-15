
#include "main.h"

FLASH_EraseInitTypeDef flashstruct;

//
/**
 * @brief
 * Write memory 在特定區間寫入資料 大小
 * @param data  要寫入資料
 * @param size  資料大小
 * @param startAddr 資料扇曲
 */
uint32_t Flash_Write_Flash_Memory(uint32_t *data, uint32_t size, uint32_t startAddr)
{
    HAL_FLASH_Unlock();
    flashstruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    flashstruct.Sector = startAddr;
    flashstruct.NbSectors = 1;
    flashstruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    uint32_t sectorError = 0;
    if (HAL_FLASHEx_Erase(&flashstruct, &sectorError) != HAL_OK)
    {
    }
    uint32_t flashAddress = startAddr;
    for (uint32_t i = 0; i < size; i++)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddress, data[i]) != HAL_OK)
        {
        }
        flashAddress += 4;
    }

    HAL_FLASH_Lock();
    return startAddr; // 返回写入的Flash地址
}

//
/**
 * @brief
 * Erase memory 只能擦除一個扇曲這裡是指一個區間到特定區間
 * @param startSector 開始區間
 * @param endSector   結束區間
 */
void Flash_Erase_Sectors(uint32_t startSector, uint32_t endSector)
{
    /*起始終點宣告*/
    static uint32_t Start_Address,End_Address;

    HAL_FLASH_Unlock();
    Start_Address = GetSector(startSector);
    End_Address  =GetSector(endSector);
    //   FLASH_EraseInitTypeDef eraseInitStruct;
    flashstruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    flashstruct.Sector = Start_Address;
    flashstruct.NbSectors = End_Address - Start_Address + 1; // 计算要擦除的扇区数量
    flashstruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;    // 选择适当的电压范围

    uint32_t sectorError = 0;
    if (HAL_FLASHEx_Erase(&flashstruct, &sectorError) != HAL_OK)
    {
        // 擦除失败
        // 在此处处理错误
    }

    HAL_FLASH_Lock();
}

// 查找扇曲資料
/**
 * @brief 查找扇曲資料
 *
 * @param targetData  目標資料
 * @param size        資料大小
 * @param flashAddress 扇曲位置
 * @return uint32_t
 */
uint32_t Flash_Find_Data(uint32_t *targetData, uint32_t size, uint32_t flashAddress)
{
    for (uint32_t i = 0; i < size; i++)
    {
        if (*(__IO uint32_t *)flashAddress == targetData[i])
        {
            return flashAddress; // 找到匹配的数据，返回其地址
        }
        flashAddress += 4; // 因为我们在这里使用32位字（4个字节）
    }

    return 0; // 未找到匹配的数据
}

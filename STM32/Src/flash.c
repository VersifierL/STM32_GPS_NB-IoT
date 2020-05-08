#include "flash.h"
uint32_t writeFlashData = 0x55555555;
uint32_t addr = 0x0801F000;

//FLASH写入测试
void writeFlashTest(void)
{
	//1解锁FLASH
  HAL_FLASH_Unlock();
	
	//2擦除FLASH
	//初始化FLASH_EraseInitTypeDef
	FLASH_EraseInitTypeDef flash;
	flash.TypeErase = FLASH_TYPEERASE_PAGES;
	flash.PageAddress = addr;
	flash.NbPages = 1;
	//设置擦除标志位PageError
	uint32_t PageError = 0;
	//调用擦除函数
	HAL_FLASHEx_Erase(&flash, &PageError);

	//3烧写FLASH
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, writeFlashData);
	
	//锁住FLASH
  HAL_FLASH_Lock();
}

//FLASH读取数据测试
uint32_t printFlashTest(void)
{
  uint32_t temp = *(__IO uint32_t*)(addr);
	return temp;
}

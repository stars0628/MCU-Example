#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"

// 移植修改区

/* FLASH大小：256K */
//这是一个宏定义，定义了一个常量STM32FLASH_SIZE，值为0x00040000UL
//它表示STM32芯片的Flash存储器的大小为262144字节（即256KB）。UL是C语言中的后缀，表示该常量是一个无符号长整型数。
#define STM32FLASH_SIZE  0x00040000UL

/* FLASH起始地址 */
#define STM32FLASH_BASE  FLASH_BASE

/* FLASH结束地址 */
#define STM32FLASH_END   (STM32FLASH_BASE | STM32FLASH_SIZE)

/* FLASH页大小 */
#define STM32FLASH_PAGE_SIZE FLASH_PAGE_SIZE

/* FLASH总页数 */
#define STM32FLASH_PAGE_NUM  (STM32FLASH_SIZE / STM32FLASH_PAGE_SIZE)

/// 导出函数声明 
void FLASH_Init(void);
uint32_t FLASH_Read(uint32_t Address, void *Buffer, uint32_t Size);
uint32_t FLASH_Write(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite);
int FLASH_ErasePage(uint32_t PageAddress, uint32_t NbPages);
uint32_t FLASH_WriteNotCheck(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite);

#endif // !__FLASH_H



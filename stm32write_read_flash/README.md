# STM32-HAL库内部Flash读写


本仓库包含以下内容：

1. flash文件
2. HAL工程
3. keil5工程


## 使用说明

1. 例程包括stm32f1和f4两种芯片
2. 区别：stm32f1系列的最小单元是页，而f4是扇区
3. 移植需根据flash修改相应的.h文件，如flash大小等

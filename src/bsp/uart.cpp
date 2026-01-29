#include "uart.hpp"

/**
 * @brief UART构造函数实现
 */
UART::UART()
{
    _baudRate = 9600;
    _isInitialized = false;
}

/**
 * @brief 初始化串口实现
 */
bool UART::begin(uint32_t baudRate) {
    if (_isInitialized) {
        this->end();
    }
    _baudRate = baudRate;

    Serial.begin(_baudRate);
    delay(100);
    
    // 清空缓冲区
    while (Serial.available()) Serial.read();
    
    _isInitialized = true;
    return true;
}

/**
 * @brief 关闭串口实现
 */
void UART::end() {
    if (_isInitialized) {
        Serial.end();
        _isInitialized = false;
    }
}

/**
 * @brief 检查串口是否已初始化实现
 */
bool UART::isInitialized() const {
    return _isInitialized;
}





/**
 * @brief 读取可用的串口数据实现
 */
size_t UART::readData(uint8_t* buffer, size_t maxLen) {
    if (!_isInitialized || buffer == nullptr || maxLen == 0) {
        return 0;
    }
    size_t bytesRead = 0;
    while (bytesRead < maxLen && Serial.available() > 0) {
        buffer[bytesRead] = Serial.read();
        bytesRead++;
    }
    return bytesRead;
}


/*
* 函数名 :CRC16
* 描述 : 计算CRC16
* 输入 : puchMsg---数据地址,usDataLen---数据长度
* 输出 : 校验值
*/
uint16 UART::CRC16_MudBus(uint8_t *puchMsg, uint8_t usDataLen)
{

    uint16_t uCRC = 0xffff;//CRC寄存器

    for(uint8_t num=0;num<usDataLen;num++){
        uCRC = (*puchMsg++)^uCRC;//把数据与16位的CRC寄存器的低8位相异或，结果存放于CRC寄存器。
        for(uint8_t x=0;x<8;x++){	//循环8次
            if(uCRC&0x0001){	//判断最低位为：“1”
                uCRC = uCRC>>1;	//先右移
                uCRC = uCRC^0xA001;	//再与0xA001异或
            }else{	//判断最低位为：“0”
                uCRC = uCRC>>1;	//右移
            }
        }
    }
    return uCRC;//返回CRC校验值
}


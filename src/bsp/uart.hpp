/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : uart.hpp
 * @brief          : Header for uart.cpp file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024.12.10 STMicroelectronics.
 * All rights reserved.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
#ifndef UART_HPP
#define UART_HPP

#include <Arduino.h>

/**
 * @brief ESP8266串口收发简易类
 * @details 适用于ESP8266的串口通信管理，仅支持UART0（全功能串口）
 */
class UART {
private:
    uint32_t _baudRate;
    bool _isInitialized;
    
public:
    /**
     * @brief 构造函数
     * 默认初始化UART0串口
     */
    UART();

    /**
     * @brief 初始化串口
     * @param baudRate 波特率
     * @return 初始化成功返回true，失败返回false
     */
    bool begin(uint32_t baudRate);

    /**
     * @brief CRC16 Modbus校验
     * @param puchMsg 数据地址
     * @param usDataLen 数据长度
     * @return CRC校验值
     */
    uint16_t CRC16_MudBus(uint8_t *puchMsg, uint8_t usDataLen);


};

#endif // UART_HPP


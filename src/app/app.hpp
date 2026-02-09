/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : mesh.hpp
 * @brief          : Header for mesh.cpp file.
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
#ifndef APP_HPP
#define APP_HPP

#include <Arduino.h>
#include "../bsp/led.hpp"
#include "../bsp/uart.hpp"
#include "../bsp/mesh.hpp"
#include "../bsp/modbus.hpp"


// 应用程序请求下位机命令
class APP {

public:
    // 构造函数
    APP();
    ~APP();
    // 初始化函数
    void begin();
    void modbus_exec();
    void received_handle();
    void exec();

private:
    uint16_t time_count;
    uint16_t blinkInterval;
    uint32_t last_led_time;
    uint32_t last_mesh_time;
    uint8_t slave_addr;//从机地址
    uint8_t slave_sta;//从机状态
    uint8_t slave_cmd;//从机命令0:停止，1:正转，2:反转，3:读取状态
    LEDDriver led;
    UART uart;
    MODBUS modbus;
    MeshNode mesh;
};

#endif // APP_HPP


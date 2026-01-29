/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : led.hpp
 * @brief          : Header for led.cpp file.   
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
#ifndef LED_HPP
#define LED_HPP

#include <Arduino.h>

class LEDDriver {
private:
    int pin;               // LED引脚号
    bool isOn;             // LED状态

public:
    LEDDriver(int pin);    // 构造函数
    void init();           // 初始化
    void on();             // 打开LED
    void off();            // 关闭LED
    void toggle();         // 切换LED状态
    bool getState();       // 获取当前状态
};

#endif // LED_HPP





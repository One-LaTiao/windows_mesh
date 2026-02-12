/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : modbus.hpp
 * @brief          : Header for modbus.cpp file.
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
#ifndef MODBUS_HPP
#define MODBUS_HPP

#include <ESP8266WiFi.h>
#include "queue.hpp"  // 引入你的SimpleQueue队列头文件

// 原有Modbus宏定义 完全保留
#define SERIAL_BAUD 9600
#define MODBUS_SERIAL Serial
#define MAX_MODBUS_FRAME 256
// 新增：SimpleQueue队列配置（byte类型，容量512，适配Modbus接收）
#define MODBUS_QUEUE_CAPACITY 512  // 队列最大容量，按需调整

typedef enum{
    G_SERIAL_STOP,
    G_SERIAL_CW,
    G_SERIAL_RW,
}SERIAL_STA;


class MODBUS
{
private:
    // 1. SimpleQueue专属：静态字节缓冲区（核心，SimpleQueue基于此实现，无动态内存）
    byte modbusQueueBuf[MODBUS_QUEUE_CAPACITY];
    // 2. SimpleQueue对象：绑定缓冲区、元素大小（byte=1）、容量
    SimpleQueue modbusQueue;

    // 原有Modbus成员变量 完全保留
    byte modbusFrameBuf[MAX_MODBUS_FRAME];
    uint16_t frameLen;
    unsigned long lastRecvTime;
    uint8_t calculateXOR(const uint8_t *data);

    uint8_t serial_addr;
    uint8_t serial_sta;
    uint8_t serial_cmd;



public:
    // 原有构造函数、方法声明 完全保留
    MODBUS();
    void begin();
    uint32_t parseModbusFrame();
    void serialEvent_callback();  // 串口接收事件处理方法（适配SimpleQueue::push）
    void set_slave(uint8_t addr, uint8_t cmd);
    
};



#endif // MODBUS_HPP

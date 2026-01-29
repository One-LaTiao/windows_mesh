/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : time.hpp
 * @brief          : Header for time.cpp file.
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
#ifndef TIME_HPP
#define TIME_HPP

#include <Arduino.h>

/**
 * @class Timer
 * @brief 非阻塞式定时器类，基于Arduino的millis()函数实现
 * 
 * 该类提供了设置、启动、停止和检查定时器状态的功能，
 * 适用于需要定时执行任务但不希望阻塞主循环的场景。
 */
class Timer {
private:
  unsigned long startTime;  // 定时器启动的时间戳
  unsigned long duration;   // 定时器持续时间(毫秒)
  bool isRunning;           // 定时器运行状态标志

public:
  /**
   * @brief 默认构造函数
   * 初始化定时器为停止状态，持续时间为0
   */
  Timer();

  /**
   * @brief 带参数的构造函数
   * @param dur 定时器持续时间(毫秒)
   * 初始化定时器为停止状态，设置指定的持续时间
   */
  Timer(unsigned long dur);

  /**
   * @brief 设置定时器持续时间
   * @param dur 定时器持续时间(毫秒)
   * 如果定时器正在运行，将重置开始时间
   */
  void setDuration(unsigned long dur);

  /**
   * @brief 启动定时器
   * 记录当前时间作为开始时间，并设置运行标志
   */
  void start();

  /**
   * @brief 停止定时器
   * 清除运行标志，但保留当前设置的持续时间
   */
  void stop();

  /**
   * @brief 重置定时器
   * 重新记录当前时间作为开始时间，保持运行状态不变
   */
  void reset();

  /**
   * @brief 检查定时器是否超时
   * @return 如果定时器运行且已超过设定时间返回true，否则返回false
   */
  bool isTimeout();

  /**
   * @brief 获取剩余时间
   * @return 剩余时间(毫秒)，如果定时器未运行则返回总持续时间
   */
  unsigned long getRemainingTime();

  /**
   * @brief 检查定时器是否运行中
   * @return 定时器运行状态
   */
  bool isTimerRunning();
};

#endif // TIME_HPP
#include "time.hpp"

/**
 * @brief Timer类默认构造函数实现
 * 初始化所有成员变量，将定时器设置为停止状态
 */
Timer::Timer() {
  startTime = 0;
  duration = 0;
  isRunning = false;
}

/**
 * @brief Timer类带参数构造函数实现
 * @param dur 定时器持续时间(毫秒)
 * 初始化成员变量，设置指定的持续时间，定时器处于停止状态
 */
Timer::Timer(unsigned long dur) {
  startTime = 0;
  duration = dur;
  isRunning = false;
}

/**
 * @brief 设置定时器持续时间
 * @param dur 定时器持续时间(毫秒)
 * 如果定时器正在运行，会重置开始时间以保证新的持续时间从当前时间开始计算
 */
void Timer::setDuration(unsigned long dur) {
  duration = dur;
  if (isRunning) {
    // 如果定时器正在运行，则重置开始时间
    startTime = millis();
  }
}

/**
 * @brief 启动定时器
 * 记录当前时间作为开始时间，并将运行状态设置为true
 */
void Timer::start() {
  startTime = millis();
  isRunning = true;
}

/**
 * @brief 停止定时器
 * 将运行状态设置为false，但保留之前设置的持续时间
 */
void Timer::stop() {
  isRunning = false;
}

/**
 * @brief 重置定时器
 * 重新记录当前时间作为开始时间，保持运行状态不变
 */
void Timer::reset() {
  startTime = millis();
}

/**
 * @brief 检查定时器是否超时
 * @return 如果定时器正在运行且已超过设定时间返回true，否则返回false
 */
bool Timer::isTimeout() {
  if (!isRunning) {
    return false;
  }
  return (millis() - startTime) >= duration;
}

/**
 * @brief 获取剩余时间
 * @return 剩余时间(毫秒)
 * 如果定时器未运行，返回总持续时间；如果已超时，返回0；否则返回剩余时间
 */
unsigned long Timer::getRemainingTime() {
  if (!isRunning) {
    return duration;
  }
  unsigned long elapsed = millis() - startTime;
  if (elapsed >= duration) {
    return 0;
  }
  return duration - elapsed;
}

/**
 * @brief 检查定时器是否运行中
 * @return 定时器运行状态
 */
bool Timer::isTimerRunning() {
  return isRunning;
}
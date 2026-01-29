#include "led.hpp"

// 构造函数：保存引脚号
LEDDriver::LEDDriver(int pin) : pin(pin), isOn(false) {}

// 初始化：设置引脚模式
void LEDDriver::init() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

// 打开LED
void LEDDriver::on() {
    digitalWrite(pin, HIGH);
    isOn = true;
}

// 关闭LED
void LEDDriver::off() {
    digitalWrite(pin, LOW);
    isOn = false;
}

// 切换LED状态
void LEDDriver::toggle() {
    isOn ? off() : on();
}

// 获取当前状态
bool LEDDriver::getState() {
    return isOn;
}





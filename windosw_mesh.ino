#include "src/app/app.hpp"

APP app;
/**
 * @brief 串口接收事件处理函数
 * @details 当串口接收到数据时，会调用此函数
 * @details 该函数会将接收到的字节添加到modbus队列中
 */
void serialEvent()
{
    app.modbus_exec();
}

void setup() {
  // put your setup code here, to run once:
  app.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  app.exec();
  // delay(1);
}

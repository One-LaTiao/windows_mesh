#include "app.hpp"


/**
 * @brief APP类的构造函数
 * @details 初始化APP类的实例，并设置LED引脚
 * @param ledPin LED所连接的引脚号
 */
APP::APP():led(LED_BUILTIN)
,time_count(0)//初始化时间计数器
,blinkInterval(200)//初始化闪烁间隔
,uart()
,modbus()
{
    this->last_led_time = 0;//初始化LED时间戳
}

/**
 * @brief APP类的析构函数
 * @details 用于释放APP类的实例占用的资源
 */
APP::~APP()
{

}

/**
 * @brief 初始化函数
 * @details 用于初始化系统资源，如串口、LED、传感器等
 * @details 应该在Arduino的setup()函数中调用一次
 */
void APP::begin() 
{
    // 在这里添加初始化代码
    // 例如：初始化串口、设置引脚模式等
    this->led.init();//初始化LED
    // this->uart.begin(115200);//初始化串口
    this->mesh.begin();//mesh节点初始化
    this->modbus.begin();//modbus初始化
}
/**
 * @brief modbus执行函数
 * @details 用于解析modbus帧
 * @details 应该在Arduino的loop()函数中调用多次
 */
void APP::modbus_exec() 
{
    this->modbus.serialEvent_callback();//解析modbus帧
}



/**
 * @brief 执行函数
 * @details 包含应用程序的主要逻辑，会被Arduino的loop()函数循环调用
 *          可以在这里实现各种任务，如读取传感器、控制输出、通信等
 * @details 添加了mesh连接检测功能：
 *          - 有连接时：LED慢闪（1秒一次）
 *          - 无连接时：LED快闪（200ms一次）
 */
void APP::exec() 
{
    this->mesh.update();//执行mesh节点
    // 检查mesh连接状态

    if(this->mesh.getNodeList().size() > 0){
        // 有连接：1秒一闪（慢闪）
        blinkInterval = 1000; // 1000ms = 1秒
    } else {
        // 无连接：200ms一闪（快闪）
        blinkInterval = 100; // 100ms
    }
    uint32_t sys_cnt = millis();
    // // 使用time_count进行时间计数，每1ms增加一次
    if(sys_cnt - this->last_led_time > blinkInterval){
        this->led.toggle();
        this->last_led_time = sys_cnt;//更新LED时间戳
    }
    this->modbus.parseModbusFrame();//解析modbus帧

}


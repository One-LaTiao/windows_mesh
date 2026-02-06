#include "modbus.hpp"

/**
 * @brief MODBUS构造函数实现：新增SimpleQueue对象初始化，原有逻辑不变
 */
MODBUS::MODBUS()
    : modbusQueue(modbusQueueBuf, sizeof(byte), MODBUS_QUEUE_CAPACITY)  // 初始化SimpleQueue：绑定缓冲区+byte大小+容量
{
    frameLen = 0;
    lastRecvTime = 0;
    serial_addr = 0;
    serial_sta = G_SERIAL_STOP;
}


/**
 * @brief 初始化Modbus实现 完全保留，仅调用类内clearQueue
 */
void MODBUS::begin()
{
    MODBUS_SERIAL.begin(SERIAL_BAUD, SERIAL_8N1);  // 原有代码，不动
    modbusQueue.reset();  // 初始化队列，逻辑不变
}

/**
 * @brief 串口接收事件处理实现：仅适配SimpleQueue::push（传地址），其余逻辑完全不变
 */
void MODBUS::serialEvent_callback()
{
    while (MODBUS_SERIAL.available() > 0) {
        byte recvByte = MODBUS_SERIAL.read();
        modbusQueue.push(&recvByte);
    }
}

/**
 * @brief 解析Modbus RTU帧实现：仅替换队列操作，Modbus核心逻辑完全不变
 */
uint16_t MODBUS::parseModbusFrame()
{
#define UART_CMD_HEAD 0x7b
#define UART_CMD_TAIL 0x7d
//7B 7B 09 10 03 01 00 00 00 00 0F 7D 7D
    uint8_t _data= 0;
    uint8_t crc = 0;
    static uint8_t uart_pos = 0;
    while (!modbusQueue.isEmpty()) {
        modbusQueue.pop(&_data);
		if(((uart_pos == 0) && (_data==UART_CMD_HEAD))||((uart_pos == 1) && (_data == UART_CMD_HEAD))){//判断是不是帧头
			modbusFrameBuf[uart_pos++] = _data;//启动接收
			continue;
		}
        if(uart_pos >= 2){//判断已收到准确的头帧
			modbusFrameBuf[uart_pos++] = _data;
			if(uart_pos >= 13){
				crc = calculateXOR(modbusFrameBuf);
				if(modbusFrameBuf[10] == crc){
					uart_pos = 0;
                    this->serial_addr = modbusFrameBuf[6];//获取从机地址
                    this->serial_sta = modbusFrameBuf[7];//获取从机状态
                    return (uint16_t)this->serial_addr << 8 | (uint16_t)this->serial_sta;
                    /* 校验通过，处理数据 */
				}else{uart_pos = 0;}
			}
		}
    }
    return 0;
}
/**
 * @brief 设置从机状态实现：新增校验逻辑，原有逻辑不变
 */
void MODBUS::set_slave(uint8_t addr, uint8_t cmd)
{
    uint8_t tx_data[16] = {0};
    tx_data[0] = 0x7b;
    tx_data[1] = 0x7b;
    tx_data[2] = 0x09;
    tx_data[3] = addr;
    tx_data[4] = 0x03;
    tx_data[5] = 0x01;
    tx_data[6] = addr;
    tx_data[7] = 0x00;
    tx_data[8] = cmd;
    tx_data[9] = 0x00;
    tx_data[10] = calculateXOR(tx_data);
    tx_data[11] = 0x7d;
    tx_data[12] = 0x7d;
    MODBUS_SERIAL.write(tx_data, 13);
}

uint8_t MODBUS::calculateXOR(const uint8_t *data)
{
    // 从第3字节开始计算8个字节的异或值 (跳过前两个0x7B)
    uint8_t xorValue = 0;
    for (uint8_t pos = 2; pos <= data[2]; pos++) {
        xorValue ^= data[pos];
    }
    return xorValue;
}



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
#ifndef MESH_HPP
#define MESH_HPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <painlessMesh.h>



#define MESH_PREFIX "MyMeshNet"
#define MESH_PASSWORD "myPassword"
#define MESH_PORT 5555



class MeshNode {
public:
    /**
     * @brief MeshNode类默认构造函数
     * 初始化成员变量，设置初始连接检查时间为0，并将实例指针赋值给静态成员
     */
    MeshNode();
    
    /**
     * @brief MeshNode类析构函数
     * 清理资源，在销毁对象前将静态实例指针置为nullptr
     */
    ~MeshNode();
    
    /**
     * @brief 初始化Mesh网络
     * 配置Mesh网络参数，注册回调函数，并输出初始化完成信息
     */
    void begin();
    
    /**
     * @brief 更新Mesh网络状态
     * 处理网络数据包，定期发送心跳，检查网络连接状态
     */
    void update();
    
    /**
     * @brief 发送心跳消息
     * 广播包含节点ID和时间戳的心跳消息，并打印当前连接节点数
     */
    void sendHeartbeat();
    
    /**
     * @brief 打印网络状态报告
     * 显示当前连接的节点列表、节点总数以及WiFi信号强度
     */
    void printNetworkStatus();
    
    /**
     * @brief 发送广播消息
     * @param msg 要广播的消息内容
     * @return 返回发送是否成功
     */
    bool sendBroadcast(String msg);
    
    /**
     * @brief 向指定节点发送单播消息
     * @param nodeId 目标节点ID
     * @param msg 要发送的消息内容
     * @return 返回发送是否成功
     */
    bool sendSingle(uint32_t nodeId, String msg);
    
    /**
     * @brief 获取当前节点ID
     * @return 返回当前节点的唯一标识符
     */
    uint32_t getNodeId();
    
    /**
     * @brief 获取网络中所有节点列表
     * @return 返回包含所有已知节点ID的列表
     */
    std::list<uint32_t> getNodeList();
    
    /**
     * @brief 获取WiFi信号强度
     * @return 返回当前WiFi的RSSI值（dBm）
     */
    int8_t getRSSI();

    uint8_t getSlaveSTA();///< 获取从机状态
    void set_SlaveSTA(uint8_t sta);///< 设置从机状态
    uint16_t getSlavedata();///< 获取从机命令

private:
    painlessMesh mesh; ///< painlessMesh实例，用于处理实际的网络通信
    unsigned long lastConnectionCheck; ///< 上次检查连接的时间戳
    const int CHECK_INTERVAL = 5000; ///< 连接检查间隔（毫秒），每5秒检查一次


    uint8_t slave_addr; ///< 从机地址
    uint8_t slave_cmd; ///< 从机状态
    uint8_t slave_sta; ///< 从机状态1为收到消息。0为未收到消息

    // Static instance pointer for callbacks
    static MeshNode* instance; ///< 静态实例指针，用于在静态回调函数中访问类成员
    // Callback functions
    /**
     * @brief 收到消息时的回调函数
     * @param from 发送消息的节点ID
     * @param msg 收到的消息内容
     * 处理收到的消息，如果是心跳消息则发送确认回复
     */
    static void receivedCallback(uint32_t from, String &msg);
    
    /**
     * @brief 新节点连接时的回调函数
     * @param nodeId 新连接的节点ID
     * 输出新节点连接信息并发送欢迎消息
     */
    static void newConnectionCallback(uint32_t nodeId);
    
    /**
     * @brief 网络拓扑变化时的回调函数
     * 输出网络拓扑变化信息并打印当前网络状态
     */
    static void changedConnectionCallback();
    
    /**
     * @brief 节点断开连接时的回调函数
     * @param nodeId 断开连接的节点ID
     * 输出断开连接信息并提示自动重路由
     */
    static void droppedConnectionCallback(uint32_t nodeId);
    
    // Helper functions
    /**
     * @brief 根据RSSI值获取信号质量描述
     * @param rssi RSSI值（dBm）
     * @return 返回信号质量的中文描述（优秀、良好、一般、差）
     */
    String getSignalQuality(int rssi);
};

#endif // MESH_HPP




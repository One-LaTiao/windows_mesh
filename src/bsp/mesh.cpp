#include "mesh.hpp"

// Initialize static member
MeshNode* MeshNode::instance = nullptr;

/**
 * @brief MeshNode类默认构造函数实现
 * 初始化成员变量，设置初始连接检查时间为0，并将实例指针赋值给静态成员
 */
MeshNode::MeshNode() {
    lastConnectionCheck = 0;
    instance = this;  // Store the instance pointer
    
}

/**
 * @brief MeshNode类析构函数实现
 * 清理资源，在销毁对象前将静态实例指针置为nullptr
 */
MeshNode::~MeshNode() {
    if(instance == this) {
        instance = nullptr;
    }
}

/**
 * @brief 初始化Mesh网络实现
 * 配置Mesh网络参数，注册回调函数，并输出初始化完成信息
 */
void MeshNode::begin() {
    // 设置 Mesh
    mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
    mesh.onReceive(&MeshNode::receivedCallback);
    mesh.onNewConnection(&MeshNode::newConnectionCallback);
    mesh.onChangedConnections(&MeshNode::changedConnectionCallback);
    mesh.onDroppedConnection(&MeshNode::droppedConnectionCallback);
    
    // 配置重连参数（可选）
    // mesh.initOTAReceive("ota");  // 初始化OTA，便于无线更新
    // Serial.println("Mesh 初始化完成，等待连接...");
}

 

/**
 * @brief 更新Mesh网络状态实现
 * 处理网络数据包，定期发送心跳，检查网络连接状态
 */
void MeshNode::update() {
    mesh.update();
    
    // 定期发送心跳并显示网络状态
    static unsigned long lastHeartbeat = 0;
    // if (millis() - lastHeartbeat > 3000) {
    //     lastHeartbeat = millis();
    //     sendHeartbeat();
    // }
    
    // 定期检查连接状态
    // if (millis() - lastConnectionCheck > CHECK_INTERVAL) {
    //     lastConnectionCheck = millis();
    //     printNetworkStatus();
    // }
}

/**
 * @brief 发送心跳消息实现
 * 广播包含节点ID和时间戳的心跳消息，并打印当前连接节点数
 */
void MeshNode::sendHeartbeat() {
    String msg = "HEARTBEAT_" + String(getNodeId()) + "_" + String(millis()/1000);
    sendBroadcast(msg);
    
    int nodeCount = getNodeList().size();
    Serial.printf("[%lu] 发送心跳，连接节点: %d\n", millis()/1000, nodeCount);
}

/**
 * @brief 打印网络状态报告实现
 * 显示当前连接的节点列表、节点总数以及WiFi信号强度
 */
void MeshNode::printNetworkStatus() {
    std::list<uint32_t> nodeList = getNodeList();
    
    Serial.println("\n=== 网络状态报告 ===");
    Serial.printf("节点总数: %d\n", nodeList.size() + 1); // +1 包括自己
    
    if (nodeList.size() > 0) {
        Serial.print("已连接节点ID: ");
        for (uint32_t nodeId : nodeList) {
            Serial.printf("%u ", nodeId);
        }
        Serial.println();
    } else {
        Serial.println("未连接到任何节点 - 正在搜索网络...");
    }
    
    // WiFi 信号质量
    Serial.printf("WiFi RSSI: %d dBm (", getRSSI());
    if (getRSSI() >= -50) Serial.print("优秀");
    else if (getRSSI() >= -60) Serial.print("良好");
    else if (getRSSI() >= -70) Serial.print("一般");
    else Serial.print("差");
    Serial.println(")");
}



// ========== 回调函数 ==========
/**
 * @brief 收到消息时的回调函数实现
 * @param from 发送消息的节点ID
 * @param msg 收到的消息内容
 * 处理收到的消息，如果是心跳消息则发送确认回复
 *7B 7B 09 10 03 01 00 00 00 00 0F 7D 7D
 *                 addr   cmd
 */
void MeshNode::receivedCallback(uint32_t from, String &msg) {
    // 1. 先做长度校验，避免字节数不足8个时访问越界（必须加，防止程序崩溃）
    if (msg.length() < 13) {
        return;
    }
    // instance->slave_addr = static_cast<uint8_t>(msg.charAt(6));//addr
    // instance->slave_cmd = static_cast<uint8_t>(msg.charAt(8));//cmd

}

/**
 * @brief 新节点连接时的回调函数实现
 * @param nodeId 新连接的节点ID
 * 输出新节点连接信息并发送欢迎消息
 */
void MeshNode::newConnectionCallback(uint32_t nodeId) {
    if(instance != nullptr) {
        Serial.printf("[%lu] +++ 新节点连接: %u\n", millis()/1000, nodeId);
        
        // 发送欢迎消息
        String welcome = "WELCOME_" + String(instance->mesh.getNodeId());
        instance->mesh.sendSingle(nodeId, welcome);
    }
}

/**
 * @brief 网络拓扑变化时的回调函数实现
 * 输出网络拓扑变化信息并打印当前网络状态
 */
void MeshNode::changedConnectionCallback() {
    if(instance != nullptr) {
        Serial.printf("[%lu] 网络拓扑发生变化\n", millis()/1000);
        instance->printNetworkStatus();
    }
}

/**
 * @brief 节点断开连接时的回调函数实现
 * @param nodeId 断开连接的节点ID
 * 输出断开连接信息并提示自动重路由
 */
void MeshNode::droppedConnectionCallback(uint32_t nodeId) {
    if(instance != nullptr) {
        Serial.printf("[%lu] --- 节点断开连接: %u\n", millis()/1000, nodeId);
        
        // 断开后，Mesh会自动尝试重新连接或重新路由
        Serial.println("网络将自动尝试重新路由...");
    }
}

/**
 * @brief 根据RSSI值获取信号质量描述实现
 * @param rssi RSSI值（dBm）
 * @return 返回信号质量的中文描述（优秀、良好、一般、差）
 */
String MeshNode::getSignalQuality(int rssi) {
    if (rssi >= -50) return "优秀";
    else if (rssi >= -60) return "良好";
    else if (rssi >= -70) return "一般";
    else return "差";
}

/**
 * @brief 发送广播消息实现
 * @param msg 要广播的消息内容
 * @return 返回发送是否成功
 */
bool MeshNode::sendBroadcast(String msg) {
    return mesh.sendBroadcast(msg);
}

/**
 * @brief 向指定节点发送单播消息实现
 * @param nodeId 目标节点ID
 * @param msg 要发送的消息内容
 * @return 返回发送是否成功
 */
bool MeshNode::sendSingle(uint32_t nodeId, String msg) {
    return mesh.sendSingle(nodeId, msg);
}

/**
 * @brief 获取当前节点ID实现
 * @return 返回当前节点的唯一标识符
 */
uint32_t MeshNode::getNodeId() {
    return mesh.getNodeId();
}

/**
 * @brief 获取网络中所有节点列表实现
 * @return 返回包含所有已知节点ID的列表
 */
std::list<uint32_t> MeshNode::getNodeList() {
    return mesh.getNodeList();
}

/**
 * @brief 获取WiFi信号强度实现
 * @return 返回当前WiFi的RSSI值（dBm）
 */
int8_t MeshNode::getRSSI() {
    return WiFi.RSSI();
}

/*
 * @Author: chengyang chengyangkj@outlook.com
 * @Date: 2023-07-27 09:58:26
 * @LastEditors: chengyangkj chengyangkj@qq.com
 * @LastEditTime: 2023-10-06 12:29:09
 * @FilePath: /ros_qt5_gui_app/include/channel/base/comm_instance.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <any>
#include <boost/dll/import.hpp>
#include <boost/filesystem.hpp>
#include "msg/msg_info.h"
#include "../virtual_channel_node.h"

class ChannelManager {
private:
    VirtualChannelNode *channel_ptr_{nullptr};
    boost::dll::shared_library *library_channel_;

public:
    static ChannelManager* instance();
    ~ChannelManager();

    bool OpenChannel(const std::string &name);                      ///< 传入channel so路径，打开对应的通信channel
    bool OpenChannelAuto();                                         ///< 自动查找当前可执行程序路径下的lib目录中的channel并打开
    void CloseChannel();                                            ///< 关闭通信channel
    std::vector<std::string> DiscoveryAllChannel();                 ///< 查找lib路径下所有channel
    void SendMessage(const MsgId &msg_id, const std::any &msg);     ///< 调用通道发送消息

    VirtualChannelNode *GetChannel();

    // 接收到通道数据时调用 func
    void RegisterOnDataCallback(std::function<void(const MsgId &id, const std::any &data)> &&func)
    {
        if (channel_ptr_ != nullptr)
        {
            channel_ptr_->RegisterOnDataCallback(std::move(func));
        }
    }

private:
    explicit ChannelManager();
};

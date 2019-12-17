
#ifndef _HTTPCONNECTION_H
#define _HTTPCONNECTION_H

#include "../base/noncopyable.h"

#include <memory>
#include <functional>

// FOR TEST
#include <string>

namespace net
{
  class EventLoop;
  class Channel;
}

namespace Http
{

  class HttpData;

  class HttpConnection: public Base::noncopyable,
                        public std::enable_shared_from_this<HttpConnection>
  {
    using CallBack = std::function<void()>;

  public:
    HttpConnection(net::EventLoop* loop, int sockFd);
    ~HttpConnection();

    // setCloseCallBack 只能由HttpServer调用
    void setCloseCallBack(const CallBack& cb) { closeCallBack_ = cb; }
    // handleClose由HttpData调用
    void handleClose() { closeCallBack_(); }

    // void handleError()? 

    // 新连接建立时由HttpServer调用,每个连接只能调用一次
    void connectionEstablish();
    // 连接断开时由HttpServer调用,每个连接只能调用一次
    void connectionDestroy();

    // 重新注册写事件
    void updateWrite();
    // 重新注册读事件
    void updateRead();

  private:
    net::EventLoop* loop_;
    // HttpServer的ConnectionMap以sockFd_为键
    int sockFd_;
    std::shared_ptr<HttpData> httpData_;
    std::unique_ptr<net::Channel> channel_;

    // closeCallBack必须为Server注册的回调函数
    // closeCallBack使Server清除本次连接信息
    CallBack closeCallBack_;

    // FOR TEST
    std::string buffer;
  };

}

#endif  // _HTTPCONNECTION_H
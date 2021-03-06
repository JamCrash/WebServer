
#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include "../base/noncopyable.h"

#include <stdint.h>
#include <map>
#include <memory>

namespace net
{
  class EventLoop;
  class Channel;
  class EventLoopThreadPool;
}

namespace Http
{

  class HttpConnection;

  class HttpServer: public Base::noncopyable
  {
    using HttpConnectionPtr = std::shared_ptr<HttpConnection>;

  public:
    HttpServer(net::EventLoop* loop, uint16_t port, int threadNum = 0);
    ~HttpServer();

    void start();

  private:
    void handleNewConnection();
    void handleDisconnecting(int sockFd);
    void removeConnection(int sockFd);

  private:
    net::EventLoop* baseLoop_;
    uint16_t port_;
    int acceptFd_;
    std::unique_ptr<net::Channel> acceptChannel_;
    bool start_;
    std::unique_ptr<net::EventLoopThreadPool> pool_;
    
    std::map<int, HttpConnectionPtr> connections_;
  };

}

#endif  // _HTTP_SERVER_H

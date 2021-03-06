
#ifndef _HTTP_DATA_H
#define _HTTP_DATA_H

#include "../base/noncopyable.h"

#include <memory>
#include <string>
#include <map>
#include <functional>

namespace Http
{

  enum ProcessState
  {
    STATE_PARSE_URI = 1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_PARSE_FINISH
  };

  enum URIState
  {
    PARSE_URI_SUCCESS = 1,
    //PARSE_URI_AGAIN,
    PARSE_URI_ERROR
  };

  enum HeaderState
  {
    PARSE_HEADER_SUCCESS = 1,
    //PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR
  };

  enum Parse_Header_State
  {
    H_START,
    H_KEY,
    H_COLON,
    H_VALUE_START,
    H_VALUE
  };

/*   enum ConnectionState
  {
    H_CONNECTED, H_DISCONNECTED
  };
 */
  enum AnalysisState
  {
    ANALYSIS_SUCCESS, ANALYSIS_ERROR
  };

  // 支持的请求方法
  enum Method
  {
    HTTP_GET, HTTP_POST, HTTP_HEAD
  };

  class HttpConnection;

  class HttpData: public Base::noncopyable
  {
    using CallBack = std::function<void()>;

  public:
    HttpData(HttpConnection* connection, int sockFd);
    ~HttpData();

    void handleRead();
    void handleWrite();

    /* void setCloseCallBack(const CallBack& cb) { closeCallBack_ = cb; } */

  private:
    URIState parseURI();
    HeaderState parseHeader();

    // analysis向writeBuffer_中写入正确响应后的数据
    AnalysisState analysis();

    void handleError(std::string errorCode, std::string errorMsg);

    /* // 调用handleClose使服务端清除本次连接信息,并断开本次连接
    void handleClose()
    {
      if(closeCallBack_)
        closeCallBack_();
    } */

  private:
    HttpConnection* connection_;
    int sockFd_;
    bool error_;

    //ConnectionState connectionState_;
    ProcessState processState_;
    Parse_Header_State headerState_;

    std::string readBuffer_;
    std::string writeBuffer_;

    static std::map<std::string, std::string> mimeType_;
    
    // 存储请求头数据
    std::map<std::string, std::string> headers_;
    // HTTP版本
    std::string version_;
    // 请求的文件
    std::string filename_;
    // 请求方法
    Method method_;

    /* // closeCallBack;
    // closeCallBack使Server清除本次连接信息
    CallBack closeCallBack_; */
  };

}

#endif  // _HTTP_DATA_H

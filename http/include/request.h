#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_
#include "httpbase.h"
namespace keno::http {
// 请求头
struct Request {
  //   请求方法
  std::string method;
  // 请求路径
  std::string path;
  //   请求头
  Headers headers;
  //   请求体
  std::string body;
  // 远端地址
  std::string remote_addr;
  //   远端端口
  int remote_port = -1;
  // for server
  //   http 版本
  std::string version;
  //   目标
  std::string target;
  //   参数
  Params params;
  //   文件
  MultipartFormDataMap files;
  //   范围
  Ranges ranges;
  //   正则
  Match matches;

  // for client
  //   响应函数
  ResponseHandler response_handler;
  //   接受上下文
  ContentReceiverWithProgress content_receiver;
  //   处理程序
  Progress progress;

#ifdef HTTP_OPENSSL_SUPPORT
  const SSL *ssl = nullptr;
#endif
  // 有请求头吗
  bool has_header(const std::string &key) const;
  // 通过key 获取请求头
  std::string get_header_value(const std::string &key, size_t id = 0) const;
  //   通过key 获取请求头
  template <typename T>
  T get_header_value(const std::string &key, size_t id = 0) const;
  size_t get_header_value_count(const std::string &key) const;
  //   设置请求头
  void set_header(const std::string &key, const std::string &val);
  // 时候存在某个key的参数
  bool has_param(const std::string &key) const;
  //   获取通过key请求参数
  std::string get_param_value(const std::string &key, size_t id = 0) const;
  //   通过key获取参数长度
  size_t get_param_value_count(const std::string &key) const;
  // 是否有文件参数
  bool is_multipart_form_data() const;
  // 是否是一个文件
  bool has_file(const std::string &key) const;
  //   通过指定key获取文件参数
  MultipartFormData get_file_value(const std::string &key) const;

  // private members...
  //   重定向
  size_t redirect_count_ = HTTP_REDIRECT_MAX_COUNT;
  //   上下文长度
  size_t content_length_ = 0;
  //   提供上下文
  ContentProvider content_provider_;
  bool is_chunked_content_provider_ = false;
  //   认证信息长度
  size_t authorization_count_ = 0;
  // 日志函数
  
};

} // namespace keno::http
#endif // HTTP_REQUEST_H_
#ifndef HTTP_STREAM_H
#define HTTP_STREAM_H
#include "httpbase.h"
namespace keno::http {
class Stream {
public:
  virtual ~Stream() = default;
  // 可读接口
  virtual bool is_readable() const = 0;
  // 可写接口
  virtual bool is_writable() const = 0;
  // 读接口
  virtual ssize_t read(char *ptr, size_t size) = 0;
  //  写接口
  virtual ssize_t write(const char *ptr, size_t size) = 0;
  //   获取远端ip和端口
  virtual void get_remote_ip_and_port(std::string &ip, int &port) const = 0;
  virtual socket_t socket() const = 0;

  template <typename... Args>
  //   写返回数据
  ssize_t write_format(const char *fmt, const Args &...args);
  ssize_t write(const char *ptr);
  ssize_t write(const std::string &s);
};
} // namespace keno::http
#endif // HTTP_STREAM_H
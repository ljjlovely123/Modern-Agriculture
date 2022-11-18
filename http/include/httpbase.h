#ifndef HTTP_BASE_H
#define HTTP_BASE_H
#include "config.h"
#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif //_CRT_SECURE_NO_WARNINGS

#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif //_CRT_NONSTDC_NO_DEPRECATE

#if defined(_MSC_VER)
#if _MSC_VER < 1900
#error Sorry, Visual Studio versions prior to 2015 are not supported
#endif

#pragma comment(lib, "ws2_32.lib")

#ifdef _WIN64
using ssize_t = __int64;
#else
using ssize_t = long;
#endif
#endif // _MSC_VER

#ifndef S_ISREG
#define S_ISREG(m) (((m)&S_IFREG) == S_IFREG)
#endif // S_ISREG

#ifndef S_ISDIR
#define S_ISDIR(m) (((m)&S_IFDIR) == S_IFDIR)
#endif // S_ISDIR

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#include <io.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef WSA_FLAG_NO_HANDLE_INHERIT
#define WSA_FLAG_NO_HANDLE_INHERIT 0x80
#endif

#ifndef strcasecmp
#define strcasecmp _stricmp
#endif // strcasecmp

using socket_t = SOCKET;
#ifdef CPPHTTPLIB_USE_POLL
#define poll(fds, nfds, timeout) WSAPoll(fds, nfds, timeout)
#endif

#else // not _WIN32

#include <arpa/inet.h>
#ifndef _AIX
#include <ifaddrs.h>
#endif
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#ifdef __linux__
#include <resolv.h>
#endif
#include <netinet/tcp.h>
#ifdef HTTP_USE_POLL
#include <poll.h>
#endif
#include <csignal>
#include <pthread.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using socket_t = int;
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif
#endif //_WIN32

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <cctype>
#include <climits>
#include <condition_variable>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <thread>

#ifdef HTTP_OPENSSL_SUPPORT
#ifdef _WIN32
#include <wincrypt.h>

// these are defined in wincrypt.h and it breaks compilation if BoringSSL is
// used
#undef X509_NAME
#undef X509_CERT_PAIR
#undef X509_EXTENSIONS
#undef PKCS7_SIGNER_INFO

#ifdef _MSC_VER
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "cryptui.lib")
#endif
#endif //_WIN32

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>

#if defined(_WIN32) && defined(OPENSSL_USE_APPLINK)
#include <openssl/applink.c>
#endif

#include <iostream>
#include <sstream>

#if OPENSSL_VERSION_NUMBER < 0x1010100fL
#error Sorry, OpenSSL versions prior to 1.1.1 are not supported
#endif

#if OPENSSL_VERSION_NUMBER < 0x10100000L
#include <openssl/crypto.h>
inline const unsigned char *ASN1_STRING_get0_data(const ASN1_STRING *asn1) {
  return M_ASN1_STRING_data(asn1);
}
#endif
#endif

#ifdef HTTP_ZLIB_SUPPORT
#include <zlib.h>
#endif

#ifdef HTTP_BROTLI_SUPPORT
#include <brotli/decode.h>
#include <brotli/encode.h>
#endif
namespace keno::http {
struct ci {
  bool operator()(const std::string &s1, const std::string &s2) const {
    return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(),
                                        s2.end(),
                                        [](unsigned char c1, unsigned char c2) {
                                          return ::tolower(c1) < ::tolower(c2);
                                        });
  }
};

using Headers = std::multimap<std::string, std::string, keno::http::ci>;
// 要支持json 不能用map
using Params = std::multimap<std::string, std::string>;
// 正则表达式
using Match = std::smatch;
// 处理程序
using Progress = std::function<bool(uint64_t current, uint64_t total)>;

struct Response;
// 响应操作函数
using ResponseHandler = std::function<bool(const Response &response)>;

// 文件类型参数结构体用来存放文件参数信息
struct MultipartFormData {
  std::string name;
  std::string content;
  std::string filename;
  std::string content_type;
};
// 支持多个文件参数
using MultipartFormDataItems = std::vector<MultipartFormData>;
// 多媒体参数map
using MultipartFormDataMap = std::multimap<std::string, MultipartFormData>;
// 数据集 对象定义里面嵌套对象定义

class DataSink {
public:
  DataSink() : os(&sb_), sb_(*this) {}

  DataSink(const DataSink &) = delete;
  DataSink &operator=(const DataSink &) = delete;
  DataSink(DataSink &&) = delete;
  DataSink &operator=(DataSink &&) = delete;

  std::function<bool(const char *data, size_t data_len)> write;
  std::function<void()> done;
  std::function<bool()> is_writable;
  std::ostream os;
  // std::streambuf 是流媒体文件对象
private:
  class data_sink_streambuf : public std::streambuf {
  public:
    explicit data_sink_streambuf(DataSink &sink) : sink_(sink) {}

  protected:
    std::streamsize xsputn(const char *s, std::streamsize n) {
      sink_.write(s, static_cast<size_t>(n));
      return n;
    }

  private:
    DataSink &sink_;
  };

  data_sink_streambuf sb_;
};

// 上下文提供者
using ContentProvider =
    std::function<bool(size_t offset, size_t length, DataSink &sink)>;
// 不带长度的上下文提供者
using ContentProviderWithoutLength =
    std::function<bool(size_t offset, DataSink &sink)>;
// 提供者资源释放者上下文
using ContentProviderResourceReleaser = std::function<void(bool success)>;
// 有进展的内容接收者
using ContentReceiverWithProgress =
    std::function<bool(const char *data, size_t data_length, uint64_t offset,
                       uint64_t total_length)>;
// 上下文接收者
using ContentReceiver =
    std::function<bool(const char *data, size_t data_length)>;
// 多内容头上下文接收者
using MultipartContentHeader =
    std::function<bool(const MultipartFormData &file)>;
// 上下文读取

// 定义范围
using Range = std::pair<ssize_t, ssize_t>;
// 多个范围
using Ranges = std::vector<Range>;

// 流对象

// socket 操作函数
using SocketOptions = std::function<void(socket_t sock)>;
// 默认操作函数
void default_socket_options(socket_t sock);

template <class T, class... Args>
typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(Args &&...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
typename std::enable_if<std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(std::size_t n) {
  typedef typename std::remove_extent<T>::type RT;
  return std::unique_ptr<T>(new RT[n]);
}

inline bool has_header(const Headers &headers, const std::string &key) {
  return headers.find(key) != headers.end();
}
} // namespace keno::http
#endif // HTTP_BASE_H
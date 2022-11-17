#include "contentreader.h"
#include "request.h"
#include "response.h"
#include "stream.h"
// 客户端
namespace keno::http {
class Logger;
class Client {
public:
  // Universal interface
  explicit Client(const std::string &scheme_host_port);

  explicit Client(const std::string &scheme_host_port,
                  const std::string &client_cert_path,
                  const std::string &client_key_path);

  // HTTP only interface
  explicit Client(const std::string &host, int port);

  explicit Client(const std::string &host, int port,
                  const std::string &client_cert_path,
                  const std::string &client_key_path);

  Client(Client &&) = default;

  ~Client();

  bool is_valid() const;

  Result Get(const std::string &path);
  Result Get(const std::string &path, const Headers &headers);
  Result Get(const std::string &path, Progress progress);
  Result Get(const std::string &path, const Headers &headers,
             Progress progress);
  Result Get(const std::string &path, ContentReceiver content_receiver);
  Result Get(const std::string &path, const Headers &headers,
             ContentReceiver content_receiver);
  Result Get(const std::string &path, ContentReceiver content_receiver,
             Progress progress);
  Result Get(const std::string &path, const Headers &headers,
             ContentReceiver content_receiver, Progress progress);
  Result Get(const std::string &path, ResponseHandler response_handler,
             ContentReceiver content_receiver);
  Result Get(const std::string &path, const Headers &headers,
             ResponseHandler response_handler,
             ContentReceiver content_receiver);
  Result Get(const std::string &path, const Headers &headers,
             ResponseHandler response_handler, ContentReceiver content_receiver,
             Progress progress);
  Result Get(const std::string &path, ResponseHandler response_handler,
             ContentReceiver content_receiver, Progress progress);

  Result Get(const std::string &path, const Params &params,
             const Headers &headers, Progress progress = nullptr);
  Result Get(const std::string &path, const Params &params,
             const Headers &headers, ContentReceiver content_receiver,
             Progress progress = nullptr);
  Result Get(const std::string &path, const Params &params,
             const Headers &headers, ResponseHandler response_handler,
             ContentReceiver content_receiver, Progress progress = nullptr);

  Result Head(const std::string &path);
  Result Head(const std::string &path, const Headers &headers);

  Result Post(const std::string &path);
  Result Post(const std::string &path, const char *body, size_t content_length,
              const std::string &content_type);
  Result Post(const std::string &path, const Headers &headers, const char *body,
              size_t content_length, const std::string &content_type);
  Result Post(const std::string &path, const std::string &body,
              const std::string &content_type);
  Result Post(const std::string &path, const Headers &headers,
              const std::string &body, const std::string &content_type);
  Result Post(const std::string &path, size_t content_length,
              ContentProvider content_provider,
              const std::string &content_type);
  Result Post(const std::string &path,
              ContentProviderWithoutLength content_provider,
              const std::string &content_type);
  Result Post(const std::string &path, const Headers &headers,
              size_t content_length, ContentProvider content_provider,
              const std::string &content_type);
  Result Post(const std::string &path, const Headers &headers,
              ContentProviderWithoutLength content_provider,
              const std::string &content_type);
  Result Post(const std::string &path, const Params &params);
  Result Post(const std::string &path, const Headers &headers,
              const Params &params);
  Result Post(const std::string &path, const MultipartFormDataItems &items);
  Result Post(const std::string &path, const Headers &headers,
              const MultipartFormDataItems &items);
  Result Post(const std::string &path, const Headers &headers,
              const MultipartFormDataItems &items, const std::string &boundary);
  Result Put(const std::string &path);
  Result Put(const std::string &path, const char *body, size_t content_length,
             const std::string &content_type);
  Result Put(const std::string &path, const Headers &headers, const char *body,
             size_t content_length, const std::string &content_type);
  Result Put(const std::string &path, const std::string &body,
             const std::string &content_type);
  Result Put(const std::string &path, const Headers &headers,
             const std::string &body, const std::string &content_type);
  Result Put(const std::string &path, size_t content_length,
             ContentProvider content_provider, const std::string &content_type);
  Result Put(const std::string &path,
             ContentProviderWithoutLength content_provider,
             const std::string &content_type);
  Result Put(const std::string &path, const Headers &headers,
             size_t content_length, ContentProvider content_provider,
             const std::string &content_type);
  Result Put(const std::string &path, const Headers &headers,
             ContentProviderWithoutLength content_provider,
             const std::string &content_type);
  Result Put(const std::string &path, const Params &params);
  Result Put(const std::string &path, const Headers &headers,
             const Params &params);
  Result Put(const std::string &path, const MultipartFormDataItems &items);
  Result Put(const std::string &path, const Headers &headers,
             const MultipartFormDataItems &items);
  Result Put(const std::string &path, const Headers &headers,
             const MultipartFormDataItems &items, const std::string &boundary);
  Result Patch(const std::string &path);
  Result Patch(const std::string &path, const char *body, size_t content_length,
               const std::string &content_type);
  Result Patch(const std::string &path, const Headers &headers,
               const char *body, size_t content_length,
               const std::string &content_type);
  Result Patch(const std::string &path, const std::string &body,
               const std::string &content_type);
  Result Patch(const std::string &path, const Headers &headers,
               const std::string &body, const std::string &content_type);
  Result Patch(const std::string &path, size_t content_length,
               ContentProvider content_provider,
               const std::string &content_type);
  Result Patch(const std::string &path,
               ContentProviderWithoutLength content_provider,
               const std::string &content_type);
  Result Patch(const std::string &path, const Headers &headers,
               size_t content_length, ContentProvider content_provider,
               const std::string &content_type);
  Result Patch(const std::string &path, const Headers &headers,
               ContentProviderWithoutLength content_provider,
               const std::string &content_type);

  Result Delete(const std::string &path);
  Result Delete(const std::string &path, const Headers &headers);
  Result Delete(const std::string &path, const char *body,
                size_t content_length, const std::string &content_type);
  Result Delete(const std::string &path, const Headers &headers,
                const char *body, size_t content_length,
                const std::string &content_type);
  Result Delete(const std::string &path, const std::string &body,
                const std::string &content_type);
  Result Delete(const std::string &path, const Headers &headers,
                const std::string &body, const std::string &content_type);

  Result Options(const std::string &path);
  Result Options(const std::string &path, const Headers &headers);

  bool send(Request &req, Response &res, Error &error);
  Result send(const Request &req);

  size_t is_socket_open() const;

  socket_t socket() const;

  void stop();

  void set_hostname_addr_map(std::map<std::string, std::string> addr_map);

  void set_default_headers(Headers headers);

  void set_address_family(int family);
  void set_tcp_nodelay(bool on);
  void set_socket_options(SocketOptions socket_options);

  void set_connection_timeout(time_t sec, time_t usec = 0);
  template <class Rep, class Period>
  void
  set_connection_timeout(const std::chrono::duration<Rep, Period> &duration);

  void set_read_timeout(time_t sec, time_t usec = 0);
  template <class Rep, class Period>
  void set_read_timeout(const std::chrono::duration<Rep, Period> &duration);

  void set_write_timeout(time_t sec, time_t usec = 0);
  template <class Rep, class Period>
  void set_write_timeout(const std::chrono::duration<Rep, Period> &duration);

  void set_basic_auth(const std::string &username, const std::string &password);
  void set_bearer_token_auth(const std::string &token);
#ifdef HTTP_OPENSSL_SUPPORT
  void set_digest_auth(const std::string &username,
                       const std::string &password);
#endif

  void set_keep_alive(bool on);
  void set_follow_location(bool on);

  void set_url_encode(bool on);

  void set_compress(bool on);

  void set_decompress(bool on);

  void set_interface(const std::string &intf);

  void set_proxy(const std::string &host, int port);
  void set_proxy_basic_auth(const std::string &username,
                            const std::string &password);
  void set_proxy_bearer_token_auth(const std::string &token);
#ifdef HTTP_OPENSSL_SUPPORT
  void set_proxy_digest_auth(const std::string &username,
                             const std::string &password);
#endif

#ifdef HTTP_OPENSSL_SUPPORT
  void enable_server_certificate_verification(bool enabled);
#endif

  void set_logger(Logger logger);

  // SSL
#ifdef HTTP_OPENSSL_SUPPORT
  void set_ca_cert_path(const std::string &ca_cert_file_path,
                        const std::string &ca_cert_dir_path = std::string());

  void set_ca_cert_store(X509_STORE *ca_cert_store);

  long get_openssl_verify_result() const;

  SSL_CTX *ssl_context() const;
#endif

private:
  std::unique_ptr<ClientImpl> cli_;

#ifdef HTTP_OPENSSL_SUPPORT
  bool is_ssl_ = false;
#endif
};



} // namespace keno::http

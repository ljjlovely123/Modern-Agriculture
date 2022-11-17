#include "contentreader.h"
#include "httpbase.h"
#include "request.h"
#include "response.h"
namespace keno::http {
// 定义服务
class Server {
public:
  using Handler = std::function<void(const Request &, Response &)>;

  using ExceptionHandler =
      std::function<void(const Request &, Response &, std::exception_ptr ep)>;

  enum class HandlerResponse {
    Handled,
    Unhandled,
  };
  //   响应处理函数
  using HandlerWithResponse =
      std::function<HandlerResponse(const Request &, Response &)>;
  // 带上下文处理函数
  using HandlerWithContentReader = std::function<void(
      const Request &, Response &, const ContentReader &content_reader)>;

  using Expect100ContinueHandler =
      std::function<int(const Request &, Response &)>;
  //
  Server();
  virtual ~Server();

  virtual bool is_valid() const;
  // get
  Server &Get(const std::string &pattern, Handler handler);
  Server &Post(const std::string &pattern, Handler handler);
  Server &Post(const std::string &pattern, HandlerWithContentReader handler);
  Server &Put(const std::string &pattern, Handler handler);
  Server &Put(const std::string &pattern, HandlerWithContentReader handler);
  Server &Patch(const std::string &pattern, Handler handler);
  Server &Patch(const std::string &pattern, HandlerWithContentReader handler);
  Server &Delete(const std::string &pattern, Handler handler);
  Server &Delete(const std::string &pattern, HandlerWithContentReader handler);
  Server &Options(const std::string &pattern, Handler handler);
  // 设置基本路径
  bool set_base_dir(const std::string &dir,
                    const std::string &mount_point = std::string());
  // 设置挂载点
  bool set_mount_point(const std::string &mount_point, const std::string &dir,
                       Headers headers = Headers());
  //   移除挂载点
  bool remove_mount_point(const std::string &mount_point);

  Server &set_file_extension_and_mimetype_mapping(const std::string &ext,
                                                  const std::string &mime);
  //   设置请求参数中文件的处理函数
  Server &set_file_request_handler(Handler handler);
  // 设置错误处理
  Server &set_error_handler(HandlerWithResponse handler);
  // 设置错误处理
  Server &set_error_handler(Handler handler);
  // 设置异常处理
  Server &set_exception_handler(ExceptionHandler handler);
  //   设置预处理
  Server &set_pre_routing_handler(HandlerWithResponse handler);
  //   后置处理
  Server &set_post_routing_handler(Handler handler);

  Server &set_expect_100_continue_handler(Expect100ContinueHandler handler);
  // 设置日志函数
  Server &set_logger(Logger logger);
  // 设置网络地址协议
  Server &set_address_family(int family);
  //   设置tcp延迟
  Server &set_tcp_nodelay(bool on);
  //   设置socket操作函数
  Server &set_socket_options(SocketOptions socket_options);
  // 设置默认请求头
  Server &set_default_headers(Headers headers);
  // 设置保持连接最大数
  Server &set_keep_alive_max_count(size_t count);
  //   设置保持连接超时时间
  Server &set_keep_alive_timeout(time_t sec);
  // 设置读取数据超时时间
  Server &set_read_timeout(time_t sec, time_t usec = 0);
  // 设置读取数据超时时间
  template <class Rep, class Period>
  Server &set_read_timeout(const std::chrono::duration<Rep, Period> &duration);
  // 设置写出数据超时时间
  Server &set_write_timeout(time_t sec, time_t usec = 0);
  // 设置写出数据超时时间
  template <class Rep, class Period>
  Server &set_write_timeout(const std::chrono::duration<Rep, Period> &duration);
  // 空闲检查时长
  Server &set_idle_interval(time_t sec, time_t usec = 0);

  template <class Rep, class Period>
  Server &set_idle_interval(const std::chrono::duration<Rep, Period> &duration);
  // 负载最大长度
  Server &set_payload_max_length(size_t length);
  // 绑定端口
  bool bind_to_port(const std::string &host, int port, int socket_flags = 0);
  // 绑定任意端口
  int bind_to_any_port(const std::string &host, int socket_flags = 0);
  // 监听后绑定
  bool listen_after_bind();
  // 监听
  bool listen(const std::string &host, int port, int socket_flags = 0);
  // 是否在运行
  bool is_running() const;
  //   停止
  void stop();
  // 任务队列
  std::function<TaskQueue *(void)> new_task_queue;

protected:
  //   处理请求
  bool process_request(Stream &strm, bool close_connection,
                       bool &connection_closed,
                       const std::function<void(Request &)> &setup_request);

  std::atomic<socket_t> svr_sock_;
  size_t keep_alive_max_count_ = HTTP_KEEPALIVE_MAX_COUNT;
  time_t keep_alive_timeout_sec_ = HTTP_KEEPALIVE_TIMEOUT_SECOND;
  time_t read_timeout_sec_ = HTTP_READ_TIMEOUT_SECOND;
  time_t read_timeout_usec_ = HTTP_READ_TIMEOUT_USECOND;
  time_t write_timeout_sec_ = HTTP_WRITE_TIMEOUT_SECOND;
  time_t write_timeout_usec_ = HTTP_WRITE_TIMEOUT_USECOND;
  time_t idle_interval_sec_ = HTTP_IDLE_INTERVAL_SECOND;
  time_t idle_interval_usec_ = HTTP_IDLE_INTERVAL_USECOND;
  size_t payload_max_length_ = HTTP_PAYLOAD_MAX_LENGTH;

private:
  using Handlers = std::vector<std::pair<std::regex, Handler>>;
  using HandlersForContentReader =
      std::vector<std::pair<std::regex, HandlerWithContentReader>>;

  socket_t create_server_socket(const std::string &host, int port,
                                int socket_flags,
                                SocketOptions socket_options) const;
  int bind_internal(const std::string &host, int port, int socket_flags);
  bool listen_internal();

  bool routing(Request &req, Response &res, Stream &strm);
  bool handle_file_request(const Request &req, Response &res,
                           bool head = false);
  bool dispatch_request(Request &req, Response &res, const Handlers &handlers);
  bool
  dispatch_request_for_content_reader(Request &req, Response &res,
                                      ContentReader content_reader,
                                      const HandlersForContentReader &handlers);

  bool parse_request_line(const char *s, Request &req);
  void apply_ranges(const Request &req, Response &res,
                    std::string &content_type, std::string &boundary);
  bool write_response(Stream &strm, bool close_connection, const Request &req,
                      Response &res);
  bool write_response_with_content(Stream &strm, bool close_connection,
                                   const Request &req, Response &res);
  bool write_response_core(Stream &strm, bool close_connection,
                           const Request &req, Response &res,
                           bool need_apply_ranges);
  bool write_content_with_provider(Stream &strm, const Request &req,
                                   Response &res, const std::string &boundary,
                                   const std::string &content_type);
  bool read_content(Stream &strm, Request &req, Response &res);
  bool
  read_content_with_content_receiver(Stream &strm, Request &req, Response &res,
                                     ContentReceiver receiver,
                                     MultipartContentHeader multipart_header,
                                     ContentReceiver multipart_receiver);
  bool read_content_core(Stream &strm, Request &req, Response &res,
                         ContentReceiver receiver,
                         MultipartContentHeader mulitpart_header,
                         ContentReceiver multipart_receiver);

  virtual bool process_and_close_socket(socket_t sock);

  struct MountPointEntry {
    std::string mount_point;
    std::string base_dir;
    Headers headers;
  };
  std::vector<MountPointEntry> base_dirs_;

  std::atomic<bool> is_running_;
  std::map<std::string, std::string> file_extension_and_mimetype_map_;

  Handler file_request_handler_;

  Handlers get_handlers_;

  Handlers post_handlers_;

  HandlersForContentReader post_handlers_for_content_reader_;

  Handlers put_handlers_;

  HandlersForContentReader put_handlers_for_content_reader_;

  Handlers patch_handlers_;

  HandlersForContentReader patch_handlers_for_content_reader_;

  Handlers delete_handlers_;

  HandlersForContentReader delete_handlers_for_content_reader_;

  Handlers options_handlers_;

  HandlerWithResponse error_handler_;

  ExceptionHandler exception_handler_;

  HandlerWithResponse pre_routing_handler_;

  Handler post_routing_handler_;

  Logger logger_;

  Expect100ContinueHandler expect_100_continue_handler_;

  int address_family_ = AF_UNSPEC;
  bool tcp_nodelay_ = HTTP_TCP_NODELAY;
  SocketOptions socket_options_ = default_socket_options;

  Headers default_headers_;
};

std::string to_string(const Error error);

std::ostream &operator<<(std::ostream &os, const Error &obj);


}

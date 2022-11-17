#include "error.h"
#include "httpbase.h"
namespace keno ::http {

class Result {
public:
  Result(std::unique_ptr<Response> &&res, Error err,
         Headers &&request_headers = Headers{})
      : res_(std::move(res)), err_(err),
        request_headers_(std::move(request_headers)) {}
  // Response
  operator bool() const { return res_ != nullptr; }
  bool operator==(std::nullptr_t) const { return res_ == nullptr; }
  bool operator!=(std::nullptr_t) const { return res_ != nullptr; }
  const Response &value() const { return *res_; }
  Response &value() { return *res_; }
  const Response &operator*() const { return *res_; }
  Response &operator*() { return *res_; }
  const Response *operator->() const { return res_.get(); }
  Response *operator->() { return res_.get(); }

  // Error
  Error error() const { return err_; }

  // Request Headers
  bool has_request_header(const std::string &key) const;
  std::string get_request_header_value(const std::string &key,
                                       size_t id = 0) const;
  template <typename T>
  T get_request_header_value(const std::string &key, size_t id = 0) const;
  size_t get_request_header_value_count(const std::string &key) const;

private:
  std::unique_ptr<Response> res_;
  Error err_;
  Headers request_headers_;
};
struct Response {
  //   版本号
  std::string version;
  //   状态码
  int status = -1;
  //   原因
  std::string reason;
  //   响应头
  Headers headers;
  //   响应体
  std::string body;
  //   重定向后路径
  std::string location; // Redirect location

  bool has_header(const std::string &key) const;

  std::string get_header_value(const std::string &key, size_t id = 0) const;

  template <typename T>
  T get_header_value(const std::string &key, size_t id = 0) const;

  size_t get_header_value_count(const std::string &key) const;
  void set_header(const std::string &key, const std::string &val);
  // 设置重定向路径
  void set_redirect(const std::string &url, int status = 302);
  //   设置上下文
  void set_content(const char *s, size_t n, const std::string &content_type);

  void set_content(const std::string &s, const std::string &content_type);
  // 设置上下文提供者
  void set_content_provider(
      size_t length, const std::string &content_type, ContentProvider provider,
      ContentProviderResourceReleaser resource_releaser = nullptr);

  void set_content_provider(
      const std::string &content_type, ContentProviderWithoutLength provider,
      ContentProviderResourceReleaser resource_releaser = nullptr);

  void set_chunked_content_provider(
      const std::string &content_type, ContentProviderWithoutLength provider,
      ContentProviderResourceReleaser resource_releaser = nullptr);

  Response() = default;
  Response(const Response &) = default;
  Response &operator=(const Response &) = default;
  Response(Response &&) = default;
  Response &operator=(Response &&) = default;
  ~Response() {
    if (content_provider_resource_releaser_) {
      content_provider_resource_releaser_(content_provider_success_);
    }
  }

  // private members...
  //   上下文长度
  size_t content_length_ = 0;
  ContentProvider content_provider_;
  ContentProviderResourceReleaser content_provider_resource_releaser_;
  bool is_chunked_content_provider_ = false;
  bool content_provider_success_ = false;
};
} // namespace keno::http

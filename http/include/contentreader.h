#include "httpbase.h"
namespace keno::http {
class ContentReader {
public:
  //   读取函数
  using Reader = std::function<bool(ContentReceiver receiver)>;
  //   多头部分读取函数
  using MultipartReader = std::function<bool(MultipartContentHeader header,
                                             ContentReceiver receiver)>;

  ContentReader(Reader reader, MultipartReader multipart_reader)
      : reader_(std::move(reader)),
        multipart_reader_(std::move(multipart_reader)) {}

  bool operator()(MultipartContentHeader header,
                  ContentReceiver receiver) const {
    return multipart_reader_(std::move(header), std::move(receiver));
  }

  bool operator()(ContentReceiver receiver) const {
    return reader_(std::move(receiver));
  }

  Reader reader_;
  MultipartReader multipart_reader_;
};
} // namespace keno::http
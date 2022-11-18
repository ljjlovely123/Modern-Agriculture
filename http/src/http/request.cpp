#include "../../include/request.h"
namespace keno::http {

inline bool Request::has_header(const std::string &key) const {
  return headers.find(key) != headers.end();
}
template <typename T>
inline T get_header_value(const Headers & /*headers*/,
                          const std::string & /*key*/, size_t /*id*/ = 0,
                          uint64_t /*def*/ = 0) {}

template <>
inline uint64_t get_header_value<uint64_t>(const Headers &headers,
                                           const std::string &key, size_t id,
                                           uint64_t def) {
  auto rng = headers.equal_range(key);
  auto it = rng.first;
  std::advance(it, static_cast<ssize_t>(id));
  if (it != rng.second) {
    return std::strtoull(it->second.data(), nullptr, 10);
  }
  return def;
}
} // namespace keno::http

#ifndef HTTP_ERROR_H
#define HTTP_ERROR_H
namespace keno::http {
enum class Error {
  Success = 0,
  Unknown,
  Connection,
  BindIPAddress,
  Read,
  Write,
  ExceedRedirectCount,
  Canceled,
  SSLConnection,
  SSLLoadingCerts,
  SSLServerVerification,
  UnsupportedMultipartBoundaryChars,
  Compression,
  ConnectionTimeout,
};
}
#endif // HTTP_ERROR_H
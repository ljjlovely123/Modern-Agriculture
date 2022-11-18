//
//  config.h
//
//  Copyright (c) 2022 liujiajia. All rights reserved.
//  MIT License
//

#ifndef HTTP_CONFIG_H
#define HTTP_CONFIG_H
/**
 *
 */

#ifndef HTTP_KEEPALIVE_TIMEOUT_SECOND
// 保持连接超时时间 5
#define HTTP_KEEPALIVE_TIMEOUT_SECOND 5
#endif

#ifndef HTTP_KEEPALIVE_MAX_COUNT
// 保持最大连接数
#define HTTP_KEEPALIVE_MAX_COUNT 5
#endif

#ifndef HTTP_CONNECTION_TIMEOUT_SECOND
// 连接超时时间 300s
#define HTTP_CONNECTION_TIMEOUT_SECOND 300
#endif

#ifndef HTTP_CONNECTION_TIMEOUT_USECOND
// 连接超时毫秒
#define HTTP_CONNECTION_TIMEOUT_USECOND 0
#endif

#ifndef HTTP_READ_TIMEOUT_SECOND
// 读取超时 5s
#define HTTP_READ_TIMEOUT_SECOND 5
#endif

#ifndef HTTP_READ_TIMEOUT_USECOND
#define HTTP_READ_TIMEOUT_USECOND 0
#endif

#ifndef HTTP_WRITE_TIMEOUT_SECOND
// 写入超时 5s
#define HTTP_WRITE_TIMEOUT_SECOND 5
#endif

#ifndef HTTP_WRITE_TIMEOUT_USECOND
// 写入超时 5s
#define HTTP_WRITE_TIMEOUT_USECOND 0
#endif

#ifndef HTTP_IDLE_INTERVAL_SECOND
#define HTTP_IDLE_INTERVAL_SECOND 0
#endif

#ifndef HTTP_IDLE_INTERVAL_USECOND
#ifdef _WIN32
#define HTTP_IDLE_INTERVAL_USECOND 10000
#else
#define HTTP_IDLE_INTERVAL_USECOND 0
#endif
#endif

#ifndef HTTP_REQUEST_URI_MAX_LENGTH
#define HTTP_REQUEST_URI_MAX_LENGTH 8192
#endif

#ifndef HTTP_HEADER_MAX_LENGTH
#define HTTP_HEADER_MAX_LENGTH 8192
#endif

#ifndef HTTP_REDIRECT_MAX_COUNT
#define HTTP_REDIRECT_MAX_COUNT 3
#endif

#ifndef HTTP_MULTIPART_FORM_DATA_FILE_MAX_COUNT
#define HTTP_MULTIPART_FORM_DATA_FILE_MAX_COUNT 1024
#endif

#ifndef HTTP_PAYLOAD_MAX_LENGTH
// 负载最大长度
#define HTTP_PAYLOAD_MAX_LENGTH ((std::numeric_limits<size_t>::max)())
#endif

#ifndef HTTP_FORM_URL_ENCODED_PAYLOAD_MAX_LENGTH
//
#define HTTP_FORM_URL_ENCODED_PAYLOAD_MAX_LENGTH 8192
#endif

#ifndef HTTP_TCP_NODELAY
#define HTTP_TCP_NODELAY 0
#endif

#ifndef HTTP_RECV_BUFSIZ
#define HTTP_RECV_BUFSIZ size_t(4096u)
#endif

#ifndef HTTP_COMPRESSION_BUFSIZ
#define HTTP_COMPRESSION_BUFSIZ size_t(16384u)
#endif

#ifndef HTTP_THREAD_POOL_COUNT
#define HTTP_THREAD_POOL_COUNT                                                 \
  ((std::max)(8u, std::thread::hardware_concurrency() > 0                      \
                      ? std::thread::hardware_concurrency() - 1                \
                      : 0))
#endif

#ifndef HTTP_RECV_FLAGS
#define HTTP_RECV_FLAGS 0
#endif

#ifndef HTTP_SEND_FLAGS
#define HTTP_SEND_FLAGS 0
#endif

#ifndef HTTP_LISTEN_BACKLOG
#define HTTP_LISTEN_BACKLOG 5
#endif

#endif
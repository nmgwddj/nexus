#pragma once

#include <nexus/quic/stream.hpp>
#include <nexus/h3/fields.hpp>

namespace nexus::h3 {

class client_connection;
class server_connection;

/// a bidirectional HTTP/3 stream that can send and receive HTTP headers, and
/// meets the type requirements of asio's AsyncRead/WriteStream and
/// SyncRead/WriteStream for transferring the HTTP message body
class stream : public quic::stream {
  friend class client_connection;
  friend class server_connection;
 public:
  /// read headers from the stream. this must either be called before reading
  /// body via read_some()/async_read_some(), or after reading returns an
  /// end_of_stream error to read the optional trailer section
  template <typename CompletionToken> // void(error_code)
  decltype(auto) async_read_headers(fields& f, CompletionToken&& token) {
    return state->async_read_headers(f, std::forward<CompletionToken>(token));
  }

  /// read headers from the stream. this must either be called before reading
  /// body via read_some()/async_read_some(), or after reading returns an
  /// end_of_stream error to read the optional trailer section
  void read_headers(fields& f, error_code& ec);
  /// read headers from the stream. this must either be called before reading
  /// body via read_some()/async_read_some(), or after reading returns an
  /// end_of_stream error to read the optional trailer section
  void read_headers(fields& f);

  /// write headers to the stream. if this is called after another call to
  /// write_headers()/async_write_headers() or after writing body via
  /// write_some()/async_write_some(), this represents the trailer section
  /// and implies the end of the message body
  template <typename CompletionToken> // void(error_code)
  decltype(auto) async_write_headers(const fields& f, CompletionToken&& token) {
    return state->async_write_headers(f, std::forward<CompletionToken>(token));
  }

  /// write headers to the stream. if this is called after another call to
  /// write_headers()/async_write_headers() or after writing body via
  /// write_some()/async_write_some(), this represents the trailer section
  /// and implies the end of the message body
  void write_headers(const fields& f, error_code& ec);
  /// write headers to the stream. if this is called after another call to
  /// write_headers()/async_write_headers() or after writing body via
  /// write_some()/async_write_some(), this represents the trailer section
  /// and implies the end of the message body
  void write_headers(const fields& f);
};

} // namespace nexus::h3

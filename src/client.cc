#include <nexus/h3/client.hpp>
#include <nexus/h3/stream.hpp>
#include <nexus/quic/connection.hpp>
#include <lsquic.h>

namespace nexus {
namespace quic {

client::client(const executor_type& ex, const udp::endpoint& endpoint,
               asio::ssl::context& ctx)
    : state(ex, &socket, nullptr, 0),
      socket(state, endpoint, false, ctx)
{
}

client::client(const executor_type& ex, const udp::endpoint& endpoint,
               asio::ssl::context& ctx, const settings& s)
    : state(ex, &socket, &s, 0),
      socket(state, endpoint, false, ctx)
{
}

client::client(udp::socket&& socket, asio::ssl::context& ctx)
    : state(socket.get_executor(), &this->socket, nullptr, 0),
      socket(state, std::move(socket), ctx)
{
}

client::client(udp::socket&& socket, asio::ssl::context& ctx, const settings& s)
    : state(socket.get_executor(), &this->socket, &s, 0),
      socket(state, std::move(socket), ctx)
{
}

client::executor_type client::get_executor() const
{
  return state.get_executor();
}

udp::endpoint client::local_endpoint() const
{
  return socket.local_endpoint();
}

void client::connect(connection& conn,
                     const udp::endpoint& endpoint,
                     const char* hostname)
{
  state.connect(conn.state, endpoint, hostname);
}

void client::close()
{
  state.close();
  socket.close();
}

} // namespace quic

namespace h3 {

client::client(const executor_type& ex, const udp::endpoint& endpoint,
               asio::ssl::context& ctx)
    : state(ex, &socket, nullptr, LSENG_HTTP),
      socket(state, endpoint, false, ctx)
{
}

client::client(const executor_type& ex, const udp::endpoint& endpoint,
               asio::ssl::context& ctx, const quic::settings& s)
    : state(ex, &socket, &s, LSENG_HTTP),
      socket(state, endpoint, false, ctx)
{
}

client::client(udp::socket&& socket, asio::ssl::context& ctx)
    : state(socket.get_executor(), &this->socket, nullptr, LSENG_HTTP),
      socket(state, std::move(socket), ctx)
{
}

client::client(udp::socket&& socket, asio::ssl::context& ctx,
               const quic::settings& s)
    : state(socket.get_executor(), &this->socket, &s, LSENG_HTTP),
      socket(state, std::move(socket), ctx)
{
}

client::executor_type client::get_executor() const
{
  return state.get_executor();
}

udp::endpoint client::local_endpoint() const
{
  return socket.local_endpoint();
}

void client::connect(client_connection& conn,
                     const udp::endpoint& endpoint,
                     const char* hostname)
{
  state.connect(conn.state, endpoint, hostname);
}

void client::close()
{
  state.close();
  socket.close();
}

udp::endpoint client_connection::remote_endpoint()
{
  return state.remote_endpoint();
}

void client_connection::connect(stream& s, error_code& ec)
{
  auto op = quic::detail::stream_connect_sync{s.state};
  state.connect(op);
  op.wait();
  ec = *op.ec;
}

void client_connection::connect(stream& s)
{
  error_code ec;
  connect(s, ec);
  if (ec) {
    throw system_error(ec);
  }
}

void client_connection::close(error_code& ec)
{
  state.close(ec);
}

void client_connection::close()
{
  error_code ec;
  close(ec);
  if (ec) {
    throw system_error(ec);
  }
}

} // namespace h3
} // namespace nexus

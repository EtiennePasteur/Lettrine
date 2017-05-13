#pragma once
#ifndef NBT_HPP_
#define NBT_HPP_

# include <boost/asio.hpp>
namespace io = boost::asio;

enum class TAG : unsigned char
  {
    Short = 2,
    Byte_Array = 7,
    String = 8,
    List = 9
  };

class Socc
{
  io::io_service io_service;
  io::ip::tcp::socket s;
  io::ip::tcp::resolver resolver;

  inline void _dropdatag(TAG const tag)
  {
    io::write(s, io::buffer((char const *)&tag, sizeof(tag)));
  }

public:
  explicit Socc(char const * const port)
    : s(io_service), resolver(io_service)
  {
    io::connect(s, resolver.resolve({ "localhost", port }));
  }

  void send(char const * const val)
  {
    _dropdatag(TAG::String);
    short len = std::strlen(val);
    uint16_t net = htons(len);
    io::write(s, io::buffer((char const *)&net, sizeof(net)));
    io::write(s, io::buffer(val, sizeof(char) * len));
  }
};

#endif /* !NBT_HPP_ */

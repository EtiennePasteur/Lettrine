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

  void dropdapayload(short val)
  {
    uint16_t net = htons(val);
    io::write(s, io::buffer((char const *)&net, sizeof(uint16_t)));
  }

  void dropdapayload(char const *val)
  {
    io::write(s, io::buffer(val, std::strlen(val)));
  }

public:
  explicit Socc(char const *port)
    : s(io_service), resolver(io_service)
  {
    io::connect(s, resolver.resolve({ "localhost", port }));
  }

  void send(char const *val)
  {
    dropdapayload((short)std::strlen(val));
    dropdapayload(val);
  }
};

#endif /* !NBT_HPP_ */

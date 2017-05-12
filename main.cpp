// std/boost utils
#include <iostream>
#include <boost/format.hpp>
template<typename... Args> constexpr auto fmt(Args&&... args) {
  return (boost::format(std::forward<Args>(args)...));
}
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string/predicate.hpp>

// NBT payload
#include "nbt.hpp"

static char const *_version = "0.1.0";
static char const *_usage =
  R"(Usage: %s SOURCE

	SOURCE		url or directory


Lettrine %s - Open Library Hackathon 2017)";

static inline int usage(char const *name)
{
  std::cout << fmt(_usage) % name % _version << std::endl;
  return (1);
}

int main(int ac, char **av)
{
  if (ac != 2)
    return (usage(av[0]));

  fs::path p(av[1]);
  if (fs::is_directory(p))
    {
      Socc s("4242");
      char const *request;

      std::cout << fmt("Entering %s directory...") % av[1] << std::endl;
      for(auto&& entry : boost::make_iterator_range(fs::directory_iterator(p), {}))
	{
	  request = entry.path().c_str();
	  if (boost::algorithm::ends_with(request, ".cpp"))
	    {
	      std::cout << entry << std::endl;
	      s.send(request);
	    }
	}
    }
  return (0);
}

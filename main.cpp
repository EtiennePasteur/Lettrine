// std/boost utils
#include <iostream>
#include <boost/format.hpp>
template<typename... Args> constexpr auto fmt(Args&&... args) {
  return (boost::format(std::forward<Args>(args)...));
}
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
namespace fs = boost::filesystem;
#include <boost/lexical_cast.hpp>

// NBT payload
#include "nbt.hpp"

static char const * const _version = "0.1.0";
static char const * const _usage =
  R"(Usage: %s SOURCE

	SOURCE		url or directory


Lettrine %s - Open Library Hackathon 2017)";

Socc socc("4242");

static inline int usage(char const * const name)
{
  std::cout << fmt(_usage) % name % _version << std::endl;
  return (1);
}

int main(int ac, char **av)
{
  if (ac != 2)
    return (usage(av[0]));

  fs::path const p(av[1]);
  if (fs::is_directory(p))
    {
      fs::path dir;
      std::string file;
      unsigned page;

      std::cout << fmt("Entering %s directory...") % av[1] << std::endl;
      for(auto&& entry : boost::make_iterator_range(fs::directory_iterator(p), {}))
	{
	  dir = entry.path();
	  file = (fmt("%s/metadata/%s.json") % dir.string() % dir.filename().string()).str();
	  if (!fs::is_directory(dir) || !fs::is_regular_file(file))
	    continue;
	  std::cout << fmt("loading %s ...") % file << std::endl;
	  socc.send(file);
	  dir += "/img";
	  if (fs::is_directory(dir))
	    for(auto&& entry : boost::make_iterator_range(fs::directory_iterator(dir), {}))
	      {
	        file = entry.path().string();
	        page = file.find("_") + 1;
		page = boost::lexical_cast<int>(file.substr(page, file.find(".") - page));
		std::cout << "This is page " << page << std::endl;
	      }
	}
    }
  return (0);
}

#include "host_info.hpp"

#include <random>
#include <sstream>

namespace Cnm {

HostInfo HostInfo::generate(std::string name, uint8_t f, uint8_t s, uint8_t t,
                            uint8_t fo) {
  std::stringstream ss;

  // something is definitely wrong with  C++ and it's uint8_t
  // without this ugly type conversion stringstream treats uint8_t as
  // characters, not numbers.
  ss << (int)f << "." << (int)s << "." << (int)t << "." << (int)fo;

  std::string address = std::move(ss).str();

  return HostInfo(name, address);
}

HostInfo HostInfo::generate(std::string name) {
  std::random_device rd;
  std::default_random_engine re(rd());
  std::uniform_int_distribution<int> d(0, 255);
  return generate(name, d(re), d(re), d(re), d(re));
}

}  // namespace Cnm

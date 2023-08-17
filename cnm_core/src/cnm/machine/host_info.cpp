#include "host_info.hpp"

#include <random>
#include <sstream>

namespace Cnm {

HostInfo HostInfo::generate(std::string_view name, uint8_t f, uint8_t s,
                            uint8_t t, uint8_t fo) {
  std::ostringstream ss;
  ss << f << "." << s << "." << t << "." << fo;

  std::string address = std::move(ss).str();

  return HostInfo(name, address);
}

HostInfo HostInfo::generate(std::string_view name) {
  std::random_device rd;
  std::default_random_engine re(rd());
  std::uniform_int_distribution<uint8_t> d(0, 255);
  return generate(name, d(re), d(re), d(re), d(re));
}

}  // namespace Cnm

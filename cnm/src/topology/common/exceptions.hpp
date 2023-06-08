#ifndef HPP_CNM_LIB_TOPOLOGY_COMMON_EXCEPTIONS_HPP
#define HPP_CNM_LIB_TOPOLOGY_COMMON_EXCEPTIONS_HPP

#include <exception>
#include <sstream>
#include <string_view>

namespace Cnm::Topology::Exceptions {

class AddressNotFound : public std::exception {
 public:
  AddressNotFound(std::string_view topology, std::string_view address)
      : searched_topology_{topology}, searched_address_{address} {
    std::stringstream ss;

    ss << "The address " << searched_address_
       << " is not found in the topology " << searched_topology_ << "!";

    msg_ = ss.str();
  }

  [[nodiscard]] const char* what() const noexcept override {
    return msg_.c_str();
  }

  [[nodiscard]] std::string_view topology() const noexcept {
    return searched_topology_;
  }

  [[nodiscard]] std::string_view address() const noexcept {
    return searched_address_;
  }

 private:
  std::string_view searched_topology_;
  std::string_view searched_address_;

  std::string msg_;
};

}  // namespace Cnm::Topology::Exceptions

#endif  // HPP_CNM_LIB_TOPOLOGY_COMMON_EXCEPTIONS_HPP

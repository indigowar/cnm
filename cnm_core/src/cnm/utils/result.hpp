#ifndef HPP_CNM_CORE_UTILS_RESULT_HPP
#define HPP_CNM_CORE_UTILS_RESULT_HPP

#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace Utils {

// Template class for return type.
// It is used to minimize the usage of exceptions, because they do not provide
// a clear and robust way to deal with errors.
template <typename Value, typename Error>
class Result final {
 public:
  static constexpr Result<Value, Error> Ok(Value &&v) {
    return Result(ValueObj(std::move(v)));
  }

  static constexpr Result<Value, Error> Err(Error &&err) {
    return Result(ErrorObj(std::move(err)));
  }

  constexpr bool isOk() const noexcept {
    return std::holds_alternative<ValueObj>(value_);
  }

  constexpr bool isErr() const noexcept {
    return std::holds_alternative<ErrorObj>(value_);
  }

  constexpr Value &&unwrap() { return std::move(std::get<ValueObj>(value_).v); }

  constexpr Error &&unwrapErr() {
    return std::move(std::get<ErrorObj>(value_).err);
  }

 private:
  struct ValueObj {
    Value v;
    ValueObj(Value v) : v{std::move(v)} {}
  };

  struct ErrorObj {
    Error err;
    ErrorObj(Error &&e) : err{std::move(e)} {}
  };

  using VariantT = std::variant<ValueObj, ErrorObj>;

  constexpr Result(ValueObj v) : value_{std::move(v)} {}
  constexpr Result(ErrorObj e) : value_{std::move(e)} {}

  VariantT value_;
};

}  // namespace Utils

// This is alias to Result<T, std::string_view>,
// here string_view is used as Error.
template <typename T>
using result_t = Utils::Result<T, std::string_view>;

#endif  // HPP_CNM_CORE_UTILS_RESULT_HPP

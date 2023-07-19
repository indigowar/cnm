#ifndef HPP_CNM_CORE_UTILS_RESULT_HPP
#define HPP_CNM_CORE_UTILS_RESULT_HPP

#include <utility>
#include <variant>

template <typename Value, typename Error>
class Result final {
 public:
  static constexpr Result<Value, Error> Ok(Value&& v) {
    return Result(ValueObj(std::move(v)));
  }

  static constexpr Result<Value, Error> Err(Error&& err) {
    return Result(ErrorObj(std::move(err)));
  }

  constexpr bool is_ok() const noexcept {
    return std::holds_alternative<ValueObj>(value_);
  }

  constexpr bool is_err() const noexcept {
    return std::holds_alternative<ErrorObj>(value_);
  }

  constexpr Value&& unwrap() { return std::move(std::get<ValueObj>(value_).v); }

  constexpr Error&& unwrap_err() {
    return std::move(std::get<ErrorObj>(value_).err);
  }

 private:
  struct ValueObj {
    Value v;
    ValueObj(Value v) : v{std::move(v)} {}
  };

  struct ErrorObj {
    Error err;
    ErrorObj(Error&& e) : err{std::move(e)} {}
  };

  using VariantT = std::variant<ValueObj, ErrorObj>;

  constexpr Result(ValueObj v) : value_{std::move(v)} {}
  constexpr Result(ErrorObj e) : value_{std::move(e)} {}

  VariantT value_;
};

#endif  // HPP_CNM_CORE_UTILS_RESULT_HPP

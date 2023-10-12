#ifndef HPP_CNM_GUI_LIB_RENDER_LIST_CHOOSER_HPP
#define HPP_CNM_GUI_LIB_RENDER_LIST_CHOOSER_HPP

#include <imgui.h>

#include <array>
#include <optional>
#include <string_view>

template <size_t SIZE>
class ListChooser final {
 public:
  explicit ListChooser(std::array<std::string_view, SIZE> array)
      : variants{std::move(array)}, current{std::nullopt} {}

  [[nodiscard]] constexpr bool has_selected() const noexcept {
    return current.has_value();
  }

  [[nodiscard]] constexpr size_t get() const noexcept {
    return current.value();
  }

  void operator()() {
    for (size_t i{}; i != variants.size(); i++) {
      auto is_selected = current.has_value();
      if (is_selected) {
        is_selected = i == current.value();
      }
      if (ImGui::Selectable(variants.at(i).data(), is_selected)) {
        current = i;
      }
    }
  }

 private:
  std::array<std::string_view, SIZE> variants;
  std::optional<size_t> current;
};

#endif  // HPP_CNM_GUI_LIB_RENDER_LIST_CHOOSER_HPP

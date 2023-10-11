#ifndef HPP_CNM_GUI_LIB_RENDER_READERS_HPP
#define HPP_CNM_GUI_LIB_RENDER_READERS_HPP

#include <imgui.h>

// StringReader is a wrapper on ImGui::InputText for better readability.
template <size_t BUFF_SIZE>
class StringReader final {
 public:
  void operator()(const char* name, bool with_name = true,
                  ImGuiInputTextFlags_ flags = ImGuiInputTextFlags_None) {
    ImGui::PushID(name);
    ImGui::InputText(with_name ? name : "", buffer, BUFF_SIZE, flags);
    ImGui::PopID();
  }

  char buffer[BUFF_SIZE] = {};
};

// NumberReader - a StringReader that reads only numbers in certain BASE.
template <size_t BASE, size_t NUMBER_SIZE>
class NumberReader final {
 public:
  void operator()(const char* name, bool with_name = true) {
    if constexpr (BASE > 10) {
      string_reader(name, with_name, ImGuiInputTextFlags_CharsHexadecimal);
    } else {
      string_reader(name, with_name, ImGuiInputTextFlags_CharsDecimal);
    }
  }

 private:
  StringReader<NUMBER_SIZE + 1> string_reader;
};

#endif  // HPP_CNM_GUI_LIB_RENDER_READERS_HPP

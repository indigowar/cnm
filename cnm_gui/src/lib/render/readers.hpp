#ifndef HPP_CNM_GUI_LIB_RENDER_READERS_HPP
#define HPP_CNM_GUI_LIB_RENDER_READERS_HPP

#include <imgui.h>

#include <algorithm>
#include <map>

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
template <typename NUMERIC, size_t BASE, size_t NUMBER_SIZE>
  requires std::integral<NUMERIC>
class NumberReader final {
 public:
  void operator()(const char* name, bool with_name = true) {
    if constexpr (BASE > 10) {
      string_reader(name, with_name, ImGuiInputTextFlags_CharsHexadecimal);
    } else {
      string_reader(name, with_name, ImGuiInputTextFlags_CharsDecimal);
    }
  }

  NUMERIC value() const noexcept {
    return std::strtol(string_reader.buffer, nullptr, BASE);
  }

 private:
  StringReader<NUMBER_SIZE + 1> string_reader;
};

class MapReader final {
  struct Entity {
    void operator()(
        size_t number,
        ImGuiInputTextFlags_ flagsForKeyField = ImGuiInputTextFlags_None,
        ImGuiInputTextFlags_ flagsForValueField = ImGuiInputTextFlags_None) {
      ImGui::PushItemWidth(60.0f);
      char id[16] = {};
      sprintf(id, "key%zu", number);
      key(id, false, flagsForKeyField);
      ImGui::PopItemWidth();

      ImGui::SameLine();

      ImGui::PushItemWidth(60.0f);
      char id2[16] = {};
      sprintf(id2, "val%zu", number);
      value(id2, false, flagsForValueField);
      ImGui::PopItemWidth();
    }

    StringReader<32> key;
    StringReader<128> value;
  };

 public:
  void operator()(
      const char* name,
      ImGuiInputTextFlags_ flagsForKeyField = ImGuiInputTextFlags_None,
      ImGuiInputTextFlags_ flagsForValueField = ImGuiInputTextFlags_None) {
    ImGui::Text("##%s", name);
    for (size_t i{}; i != values.size(); i++) {
      ImGui::Text("#%zu", i);
      ImGui::SameLine();
      values.at(i)(i);
      ImGui::SameLine();
      if (ImGui::Button("-")) {
        values.erase(values.begin() + i);
        return;
      }
    }
    if (ImGui::Button("+")) {
      values.emplace_back();
    }
  }

  [[nodiscard]] std::map<std::string, std::string> get() const noexcept {
    std::map<std::string, std::string> result{};
    for (auto it : values) {
      std::string key{it.key.buffer};
      std::string value{it.value.buffer};
      result.emplace(key, value);
    }
    return result;
  }

 private:
  std::vector<Entity> values;
};

#endif  // HPP_CNM_GUI_LIB_RENDER_READERS_HPP

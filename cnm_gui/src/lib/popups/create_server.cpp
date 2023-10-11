#include "create_server.hpp"

#include <imgui.h>
#include <imgui_internal.h>

using namespace Popup;

void readSmallNumber(const char* id, int* value) {
  ImGui::PushID(id);
  ImGui::InputInt(
      "", value, 0, 0,
      ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_CharsDecimal);
  ImGui::PopID();
}

template <size_t BUFF_SIZE>
struct StringReader final {
  void operator()(const char* name, bool with_name = true) {
    ImGui::PushID(name);
    ImGui::InputText(with_name ? name : "", buffer, 256);
    ImGui::PopID();
  }

  char buffer[BUFF_SIZE] = {};
};

void CreateServerPopup::render(bool* is_open) {
  if (is_open) {
    if (!*is_open) {
      return;
    }
  }

  auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
               ImGuiWindowFlags_NoDocking;

  ImGui::SetNextWindowSize(ImVec2(600, 400));
  ImGui::Begin("create_server_window", nullptr, flags);

  // header
  ImGui::TextColored(ImVec4(1, 1, 1, 1), "Create a new server");
  ImGui::Separator();

  // read the name of the server
  static StringReader<256> name_reader{};

  name_reader("name", false);
  ImGui::Separator();

  // The Address of the Server in IPv4
  ImGui::Text("The Address of the Server");
  ImGui::SameLine();
  int first{}, second{}, third{}, fourth{};
  ImGui::PushItemWidth(20.0f);
  readSmallNumber("first", &first);
  ImGui::SameLine();
  readSmallNumber("second", &second);
  ImGui::SameLine();
  readSmallNumber("third", &third);
  ImGui::SameLine();
  readSmallNumber("fourth", &fourth);
  ImGui::PopItemWidth();
  ImGui::Separator();

  static const char* items[] = {"File Server", "Plain", "KYS"};
  static int selected_item = -1;

  ImGui::Text("## Select the Type of the Server:");

  for (int i = 0; i < 3; i++) {
    if (ImGui::Selectable(items[i], selected_item == i)) {
      selected_item = i;
    }
  }

  if (selected_item == 0) {
    // specific for file server settings
  }

  ImGui::Separator();

  if (ImGui::Button("Close")) {
    *is_open = false;
    on_close(info);
  }
  ImGui::SameLine();

  if (ImGui::Button("Save")) {
    *is_open = false;
    on_save(info);
  }
  ImGui::End();
}

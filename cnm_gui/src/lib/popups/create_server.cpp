#include "create_server.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "lib/render/list_chooser.hpp"
#include "lib/render/readers.hpp"

using namespace Popup;

void renderIPv4Reader(NumberReader<uint8_t, 10, 3>& f,
                      NumberReader<uint8_t, 10, 3>& s,
                      NumberReader<uint8_t, 10, 3>& t,
                      NumberReader<uint8_t, 10, 3>& fo) {
  ImGui::PushItemWidth(25.0f);
  f("first of ipv4", false);
  ImGui::SameLine();
  s("second of ipv4", false);
  ImGui::SameLine();
  t("third of ipv4", false);
  ImGui::SameLine();
  fo("fourth of ipv4", false);
  ImGui::PopItemWidth();
  ImGui::Separator();
}

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
  static NumberReader<uint8_t, 10, 3> first_reader{};
  static NumberReader<uint8_t, 10, 3> second_reader{};
  static NumberReader<uint8_t, 10, 3> third_reader{};
  static NumberReader<uint8_t, 10, 3> fourth_reader{};
  renderIPv4Reader(first_reader, second_reader, third_reader, fourth_reader);

  // The Type of the Server
  ImGui::Text("## Select the Type of the Server:");
  static auto type_chooser = ListChooser<3>({"File Server", "Plain", "KYS"});
  type_chooser();

  if (type_chooser.has_selected()) {
    if (type_chooser.get() == 0) {
      static MapReader file_reader;

      file_reader("Files");
    }
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

#include "create_office_equipment.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "lib/render/list_chooser.hpp"
#include "lib/render/readers.hpp"

using namespace Popup;

inline void renderIPv4Reader(NumberReader<10, 3>& f, NumberReader<10, 3>& s,
                             NumberReader<10, 3>& t, NumberReader<10, 3>& fo) {
  ImGui::PushItemWidth(25.0f);
  f("first of ipv4 for oe", false);
  ImGui::SameLine();
  s("second of ipv4 for oe", false);
  ImGui::SameLine();
  t("third of ipv4 for oe", false);
  ImGui::SameLine();
  fo("fourth of ipv4 for oe", false);
  ImGui::PopItemWidth();
  ImGui::Separator();
}

void CreateOfficeEquipmentPopup::render(bool* is_open) {
  if (is_open) {
    if (!*is_open) {
      return;
    }
  }

  auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
               ImGuiWindowFlags_NoDocking;

  ImGui::SetNextWindowSize(ImVec2(600, 400));
  ImGui::Begin("create_office_equipment_window", nullptr, flags);

  ImGui::TextColored(ImVec4(1, 1, 1, 1), "Create a new Office Equipment");
  ImGui::Separator();

  // read the name of the server
  static StringReader<256> name_reader{};
  name_reader("office_equipment_name", false);
  ImGui::Separator();

  // The Address of the Server in IPv4
  ImGui::Text("The Address of the Office Equipment");
  static NumberReader<10, 3> first_reader{};
  static NumberReader<10, 3> second_reader{};
  static NumberReader<10, 3> third_reader{};
  static NumberReader<10, 3> fourth_reader{};
  renderIPv4Reader(first_reader, second_reader, third_reader, fourth_reader);
  ImGui::Separator();

  // Render the List of Office Equipment Types
  ImGui::Text("## Select the Type of the Office Equipment:");
  static auto type_chooser = ListChooser<2>({"Printer", "Scanner"});
  type_chooser();

  // TODO: add specific settings for different office equipment types.

  if (ImGui::Button("Close")) {
    *is_open = false;
    on_close(info);
  }
  ImGui::SameLine();

  // TODO: make Button Save disabled, when the input is invalid.
  if (ImGui::Button("Save")) {
    *is_open = false;
    on_save(info);
  }

  ImGui::End();
}

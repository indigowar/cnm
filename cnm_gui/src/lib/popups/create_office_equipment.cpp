#include "create_office_equipment.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>

#include "lib/render/list_chooser.hpp"
#include "lib/render/readers.hpp"

using namespace Popup;

inline void renderIPv4Reader(NumberReader<uint8_t, 10, 3>& f,
                             NumberReader<uint8_t, 10, 3>& s,
                             NumberReader<uint8_t, 10, 3>& t,
                             NumberReader<uint8_t, 10, 3>& fo) {
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
  name_reader("office_equipment_name", false);
  ImGui::Separator();

  // The Address of the Server in IPv4
  ImGui::Text("The Address of the Office Equipment");
  renderIPv4Reader(first_reader, second_reader, third_reader, fourth_reader);
  ImGui::Separator();

  // Render the List of Office Equipment Types
  ImGui::Text("## Select the Type of the Office Equipment:");
  type_chooser();

  ImGui::Text("## Write the directory:");
  base_dir("base_directory", false);

  if (ImGui::Button("Close")) {
    *is_open = false;
    reset();
  }
  ImGui::SameLine();

  auto info = makeInfo();

  if (validate(info)) {
    if (ImGui::Button("Save")) {
      *is_open = false;
      on_save(info);
      reset();
    }
  } else {
    ImGui::Button("Can not save, data is invalid");
  }

  ImGui::End();
}

CreateOfficeEquipmentPopup::Info CreateOfficeEquipmentPopup::makeInfo() {
  Info info{};
  info.name = std::string{name_reader.buffer};
  info.base_dir = std::string{base_dir.buffer};
  info.ip.f = first_reader.value();
  info.ip.s = second_reader.value();
  info.ip.t = third_reader.value();
  info.ip.fo = fourth_reader.value();
  info.type = !type_chooser.has_selected()
                  ? Type::NotSelected
                  : (type_chooser.get() == 0 ? Type::Printer : Type::Scanner);
  return info;
}

bool CreateOfficeEquipmentPopup::validate(
    CreateOfficeEquipmentPopup::Info info) {
  if (info.name.size() == 0) {
    return false;
  }

  if (info.base_dir.size() == 0) {
    return false;
  }

  if (!std::filesystem::exists(info.base_dir)) {
    return false;
  }

  if (info.type == Type::NotSelected) {
    return false;
  }

  return true;
}

void CreateOfficeEquipmentPopup::reset() {
  memset(name_reader.buffer, 0, sizeof(name_reader.buffer));
  memset(base_dir.buffer, 0, sizeof(base_dir.buffer));
  first_reader = NumberReader<uint8_t, 10, 3>();
  second_reader = NumberReader<uint8_t, 10, 3>();
  third_reader = NumberReader<uint8_t, 10, 3>();
  fourth_reader = NumberReader<uint8_t, 10, 3>();
  type_chooser = ListChooser<2>({"Printer", "Scanner"});
}

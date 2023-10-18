#include "create_personal_computer.hpp"

using namespace Popup;

void renderIP(NumberReader<uint8_t, 10, 3>& f, NumberReader<uint8_t, 10, 3>& s,
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

CreatePC::CreatePC() : type_chooser({"Randomized", "Controlled"}) {}

void CreatePC::render(bool* is_open) {
  if (is_open) {
    if (!*is_open) {
      return;
    }
  }

  auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
               ImGuiWindowFlags_NoDocking;

  ImGui::SetNextWindowSize(ImVec2(600, 400));
  ImGui::Begin("create_pc_window", nullptr, flags);

  // header
  ImGui::TextColored(ImVec4(1, 1, 1, 1), "Create a new Personal Computer.");
  ImGui::Separator();

  name_reader("name", false);
  ImGui::Separator();

  ImGui::Text("The Address of The Computer");
  renderIP(first_ip, second_ip, third_ip, fourth_ip);

  ImGui::Text("Select the Type Of Personal Computer");
  type_chooser();

  ImGui::Separator();

  if (ImGui::Button("Close")) {
    *is_open = false;
  }
  ImGui::SameLine();

  auto info = makeInfo();
  auto is_valid = validate(info);

  if (is_valid) {
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

CreatePC::Info CreatePC::makeInfo() {
  Info info{};
  info.name = std::string{name_reader.buffer};
  info.ip.f = first_ip.value();
  info.ip.s = second_ip.value();
  info.ip.t = third_ip.value();
  info.ip.fo = fourth_ip.value();
  info.type =
      !type_chooser.has_selected()
          ? Type::NotSpecified
          : (type_chooser.get() == 0 ? Type::Randomized : Type::Controlled);
  return info;
}

void CreatePC::reset() {
  memset(name_reader.buffer, 0, sizeof(name_reader.buffer));
  type_chooser = ListChooser<2>({"Randomized", "Controlled"});
  first_ip = NumberReader<uint8_t, 10, 3>();
  second_ip = NumberReader<uint8_t, 10, 3>();
  third_ip = NumberReader<uint8_t, 10, 3>();
  fourth_ip = NumberReader<uint8_t, 10, 3>();
}

bool CreatePC::validate(const CreatePC::Info& info) {
  if (info.name.empty()) {
    return false;
  }

  if (info.type == Type::NotSpecified) {
    return false;
  }

  return true;
}

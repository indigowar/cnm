#include "create_server.hpp"

#include <imgui.h>
#include <imgui_internal.h>

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

CreateServerPopup::CreateServerPopup()
    : type_chooser({"FileServer", "Proxy"}) {}

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
  name_reader("name", false);
  ImGui::Separator();

  // The Address of the Server in IPv4
  ImGui::Text("The Address of the Server");
  renderIPv4Reader(first_ip, second_ip, third_ip, fourth_ip);

  ImGui::Text("Choose the limit of concurrent serving");
  ImGui::PushItemWidth(60.0f);
  limit_reader("worker_limit", false);
  ImGui::PopItemWidth();

  // The Type of the Server
  ImGui::Text("## Select the Type of the Server:");
  type_chooser();

  if (type_chooser.has_selected()) {
    if (type_chooser.get() == 0) {
      file_reader("Files");
    }
  }

  ImGui::Separator();

  if (ImGui::Button("Close")) {
    *is_open = false;
    reset();
  }
  ImGui::SameLine();

  auto info = makeInfo();

  auto info_is_valid = validate(info);

  if (info_is_valid) {
    if (ImGui::Button("Save")) {
      *is_open = false;
      on_save(info);
      reset();
    }
  } else {
    ImGui::Button("Can not save, data is invalid");
  }

  //  ImGui::BeginDisabled(info_is_valid);
  //  if (ImGui::Button("Save")) {
  //    *is_open = false;
  //    on_save(info);
  //    reset();
  //  }
  //  ImGui::EndDisabled();
  ImGui::End();
}

CreateServerPopup::Info CreateServerPopup::makeInfo() {
  Info info{};
  info.name = std::string{name_reader.buffer};
  info.limit = limit_reader.value();
  info.ip.f = first_ip.value();
  info.ip.s = second_ip.value();
  info.ip.t = third_ip.value();
  info.ip.fo = fourth_ip.value();
  info.type = ServerType::FileServer;
  info.files = file_reader.get();
  return info;
}

void CreateServerPopup::reset() {
  memset(name_reader.buffer, '\0', sizeof(name_reader.buffer));
  limit_reader = NumberReader<size_t, 10, 100>();
  first_ip = NumberReader<uint8_t, 10, 3>();
  second_ip = NumberReader<uint8_t, 10, 3>();
  third_ip = NumberReader<uint8_t, 10, 3>();
  fourth_ip = NumberReader<uint8_t, 10, 3>();
  type_chooser = ListChooser<2>({"FileServer", "Proxy"});
  file_reader = MapReader();
}

bool CreateServerPopup::validate(CreateServerPopup::Info info) {
  if (info.name.size() == 0) {
    return false;
  }

  if (info.limit == 0) {
    return 0;
  }

  if (info.type == ServerType::FileServer && !info.files.has_value()) {
    return false;
  }

  if (info.type == ServerType::FileServer) {
    if (info.files->size() == 0) {
      return false;
    }
  }

  return true;
}

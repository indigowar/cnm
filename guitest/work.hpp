#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct coords_t {
  size_t x;
  size_t y;
};

struct window_source final {
  std::string name;

  ImVec2 pos;
  ImVec2 size;

  window_source* next;
};

struct connection {
  window_source* first;
  window_source* second;
};

ImVec2 calculate_center(ImVec2 pos, ImVec2 size) {
  return ImVec2(pos.x + (size.x / 2), pos.y + (size.y / 2));
}

void draw_line(window_source& ws, window_source& sw) {
  ImVec2 point1 = calculate_center(ws.pos, ws.size);

  ImVec2 point2 = calculate_center(sw.pos, sw.size);

  ImGui::GetForegroundDrawList()->AddLine(point1, point2, IM_COL32_WHITE);
}

void render_window_source(window_source& ws) {
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

  ImGui::Begin(ws.name.c_str(), nullptr);

  ws.pos = ImGui::GetWindowPos();
  ws.size = ImGui::GetWindowSize();

  if (ws.next) {
    draw_line(ws, *ws.next);
  }

  ImGui::End();
  ImGui::PopStyleColor();  // Restore default background color
}

void render_connection(connection& con) {
  if (con.first && con.second) {
    ImVec2 point1 = calculate_center(con.first->pos, con.first->size);
    ImVec2 point2 = calculate_center(con.second->pos, con.second->size);
    ImGui::GetForegroundDrawList()->AddLine(point1, point2, IM_COL32_WHITE);
  }
}

class work final {
 public:
  explicit work() = default;

  // start() - called on startup of application
  void start() {
    windows = std::vector<window_source>{
        window_source{"Say my name!"},
        window_source{"I don't know your name"},
        window_source{"You do know my name, say it."},
        window_source{"You are... you are the Heisenberg"},
        window_source{"You are goddamn it right."},
    };

    connection con{&windows.at(0), &windows.at(2)};
    connection con1{&windows.at(2), &windows.at(1)};

    connections.push_back(con);
    connections.push_back(con1);
  }

  // update() - called on every update before render
  void update() {}

  // render() - called on render
  void render() {
    std::for_each(connections.begin(), connections.end(), render_connection);
    std::for_each(windows.begin(), windows.end(), render_window_source);
  }

 private:
  std::vector<window_source> windows;
  std::vector<connection> connections;
};

#pragma once

#include <bits/stdc++.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

template <class T>
concept RenderableT = requires(T a) { a.render(); };

template <RenderableT T>
void render(T& obj) {
  obj.render();
}

struct win_source_t final {
  std::string title;
  ImVec2 pos;
  ImVec2 size;

  win_source_t(std::string s) : title(s) {}

  void render() {
    ImGui::Begin(title.c_str());

    pos = ImGui::GetWindowPos();
    size = ImGui::GetWindowSize();

    ImGui::End();
  }

  ImVec2 center() const noexcept {
    return ImVec2(pos.x + (size.x / 2), pos.y + (size.y / 2));
  }
};

struct con_t final {
  win_source_t& first;
  win_source_t& second;
  bool is_red;

  con_t(win_source_t& f, win_source_t& s) : first{f}, second{s}, is_red{} {}

  void render() {
    ImVec2 p1 = first.center();
    ImVec2 p2 = second.center();
    auto draw_list = ImGui::GetBackgroundDrawList();
    draw_list->AddLine(p1, p2,
                       is_red ? IM_COL32(255, 0, 0, 1) : IM_COL32_WHITE);

    // Check if the mouse cursor is hovering over the line
    if (ImGui::IsMouseHoveringRect(p1, p2)) {
      // Check if the left mouse button is pressed
      if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        // Handle the line click event
        // ...
        std::cout << "click!" << std::endl;
      }
    }
  }
};

class work final {
 public:
  void init() {
    wins.push_back(win_source_t("first"));
    wins.push_back(win_source_t("second"));
    wins.push_back(win_source_t("third"));
    wins.push_back(win_source_t("fourth"));
    wins.push_back(win_source_t("fifth"));

    cons.push_back(con_t(wins.at(0), wins.at(1)));
    cons.push_back(con_t(wins.at(0), wins.at(2)));
    cons.push_back(con_t(wins.at(0), wins.at(3)));
    cons.push_back(con_t(wins.at(0), wins.at(4)));
  }

  void cycle() {
    std::for_each(cons.begin(), cons.end(), render<con_t>);
    std::for_each(wins.begin(), wins.end(), render<win_source_t>);
  }

  std::vector<win_source_t> wins;
  std::vector<con_t> cons;
};

bool IsMouseOverLine(ImVec2 p1, ImVec2 p2, float lineWidth, float threshold) {
  ImVec2 mousePos = ImGui::GetMousePos();

  // Calculate the distance between the line and the mouse position
  float distance = abs((p2.y - p1.y) * mousePos.x - (p2.x - p1.x) * mousePos.y +
                       p2.x * p1.y - p2.y * p1.x) /
                   sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));

  return distance <= threshold &&
         mousePos.x >= std::min(p1.x, p2.x) - lineWidth &&
         mousePos.x <= std::max(p1.x, p2.x) + lineWidth &&
         mousePos.y >= std::min(p1.y, p2.y) - lineWidth &&
         mousePos.y <= std::max(p1.y, p2.y) + lineWidth;
}

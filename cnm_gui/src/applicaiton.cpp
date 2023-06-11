// x
#include "application.hpp"
/// x

#include <imgui.h>
#include <spdlog/spdlog.h>

#include <topology/ring/node.hpp>

#include "helpers/fps_window.hpp"
#include "helpers/menu.hpp"
#include "nodes/path.hpp"

Application::Application()
    : view_{std::make_shared<Cnm::Topology::Ring::Node>(nullptr, nullptr,
                                                        nullptr, "127.0.0.1")},
      view1_{std::make_shared<Cnm::Topology::Ring::Node>(
          nullptr, nullptr, nullptr, "654.211.123")},
      menu_{makeMenuBar()} {}

Application::~Application() { spdlog::info("application destr"); }

void Application::render() {
  helpers::renderFPSWindow();
  menu_.render();
  view_.render();
  view1_.render();

  NodePath path(view_, view1_);
  path.render();
}

helpers::Menu Application::makeMenuBar() {
  /**
   * TODO: Add the actual logic for every MenuField
   */

  auto test = [](std::string_view first, std::string_view second) {
    spdlog::info("{}->{} clicked!", first, second);
  };

  auto system = helpers::MenuItem(
      "System",
      {helpers::MenuField("Exit", std::bind(test, "System", "Exit")),
       helpers::MenuField("Clear", std::bind(test, "System", "Clear"))});

  auto topology = helpers::MenuItem(
      "Topology",
      {helpers::MenuField("Ring", std::bind(test, "Topology", "Ring")),
       helpers::MenuField("Star", std::bind(test, "Topology", "Star")),
       helpers::MenuField("Mesh", std::bind(test, "Topology", "Mesh"))});

  auto view = helpers::MenuItem(
      "View", {helpers::MenuField("Test", std::bind(test, "View", "Test"))});

  auto machine = helpers::MenuItem(
      "Machine",
      {helpers::MenuField("PC", std::bind(test, "Machine", "PC")),
       helpers::MenuField("Server", std::bind(test, "Machine", "Server")),
       helpers::MenuField("Office Equipment",
                          std::bind(test, "Machine", "Office Equipment"))});

  auto menu = helpers::Menu({system, view, topology, machine});

  return menu;
}

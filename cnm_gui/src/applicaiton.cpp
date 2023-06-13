#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include <topology/ring/node.hpp>

#include "application.hpp"
#include "helpers/fps_window.hpp"
#include "helpers/menu.hpp"

Application::Application() : menu_{makeMenuBar()}, close_{} {}

Application::~Application() { spdlog::info("application destr"); }

void Application::render() {
  helpers::renderFPSWindow();
  menu_.render();
}

bool Application::shouldClose() const noexcept { return close_; }

helpers::Menu Application::makeMenuBar() {
  /**
   * TODO: Add the actual logic for every MenuField
   */

  auto test = [](std::string_view first, std::string_view second) {
    spdlog::info("{}->{} clicked!", first, second);
  };

  auto app = helpers::MenuItem(
      "Applicaition",
      {helpers::MenuField("Exit",
                          std::bind(&Application::handleAppClose, this)),
       helpers::MenuField("Clear", std::bind(test, "Application", "Clear"))});

  auto view = helpers::MenuItem(
      "View", {helpers::MenuField("Test", std::bind(test, "View", "Test"))});

  auto topology = helpers::MenuItem(
      "Topology",
      {helpers::MenuField("Ring", std::bind(test, "Topology", "Ring")),
       helpers::MenuField("Star", std::bind(test, "Topology", "Star")),
       helpers::MenuField("Mesh", std::bind(test, "Topology", "Mesh"))});

  auto machine = helpers::MenuItem(
      "Machine",
      {helpers::MenuField("PC", std::bind(test, "Machine", "PC")),
       helpers::MenuField("Server", std::bind(test, "Machine", "Server")),
       helpers::MenuField("Office Equipment",
                          std::bind(test, "Machine", "Office Equipment"))});

  return helpers::Menu({app, view, topology, machine});
}

void Application::handleAppClose() {
  spdlog::info("handleAppClose called");
  close_ = true;
}
void Application::handleAppClear() {}

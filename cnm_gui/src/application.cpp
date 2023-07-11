#include "application.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <random>

#include "helpers/fps_window.hpp"
#include "helpers/menu.hpp"

bool Application::shouldClose() const noexcept { return false; }

void Application::render() {
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

  ImGui::Begin("hello, world");
  ImGui::Text("hello, world");
  ImGui::End();

  ImGui::Begin("not hello world");
  ImGui::Button("aaaa!");
  ImGui::End();

  helpers::renderFPSWindow();
}

// static size_t rand_size_t() {
//   std::random_device rd;
//   std::mt19937 mt(rd());
//   std::uniform_int_distribution<size_t> distr(0, 10000);
//   return distr(mt);
// }
//
// Application::Application() : menu_{makeMenuBar()}, close_{}, warnings_{} {
//   makeMsgWindow("hello, world!");
// }
//
// Application::~Application() { spdlog::info("application destr"); }
//
// void Application::render() {
//   helpers::renderFPSWindow();
//
//   for (auto& i : warnings_) {
//     i.render();
//   }
//
//   menu_.render();
// }
//
// bool Application::shouldClose() const noexcept { return close_; }
//
// helpers::Menu Application::makeMenuBar() {
//   /**
//    * TODO: Add the actual logic for every MenuField
//    */
//
//   const static auto test = [](std::string_view first, std::string_view
//   second) {
//     spdlog::info("{}->{} clicked!", first, second);
//   };
//
//   auto app = helpers::MenuItem(
//       "Applicaition",
//       {helpers::MenuField("Exit",
//                           std::bind(&Application::handleAppClose, this)),
//        helpers::MenuField("Clear", std::bind(test, "Application",
//        "Clear"))});
//
//   auto view = helpers::MenuItem(
//       "View", {helpers::MenuField("Test", std::bind(test, "View", "Test"))});
//
//   auto topology = helpers::MenuItem(
//       "Topology",
//       {helpers::MenuField("Ring", std::bind(test, "Topology", "Ring")),
//        helpers::MenuField("Star", std::bind(test, "Topology", "Star")),
//        helpers::MenuField("Mesh", std::bind(test, "Topology", "Mesh"))});
//
//   auto machine = helpers::MenuItem(
//       "Machine",
//       {helpers::MenuField("PC", std::bind(test, "Machine", "PC")),
//        helpers::MenuField("Server", std::bind(test, "Machine", "Server")),
//        helpers::MenuField("Office Equipment",
//                           std::bind(test, "Machine", "Office Equipment"))});
//
//   return helpers::Menu({app, view, topology, machine});
// }
//
// void Application::handleAppClose() {
//   spdlog::info("handleAppClose called");
//   close_ = true;
// }
//
// void Application::handleAppClear() {}
//
// void Application::removeMsgWindow(size_t msg_id) {
//   auto res = std::remove_if(
//       warnings_.begin(), warnings_.end(),
//       [msg_id](const auto& i) -> bool { return i.id() == msg_id; });
//   warnings_.erase(res);
// }
//
// void Application::makeMsgWindow(std::string_view view) {
//   warnings_.push_back(MessageWindow(*this, view, rand_size_t()));
// }
//
// MessageWindow::MessageWindow(Application& app, std::string_view view, size_t
// id)
//     : owner_{app}, msg_{view}, id_{id} {}
//
// MessageWindow& MessageWindow::operator=(const MessageWindow& other) {
//   id_ = other.id_;
//   owner_ = other.owner_;
//   msg_ = other.msg_;
//   return *this;
// }
//
// void MessageWindow::render() {
//   ImGui::Begin("Warning");
//   ImGui::Text("%s", msg_.data());
//   if (ImGui::Button("Close")) {
//     owner_.removeMsgWindow(id_);
//   }
//   ImGui::End();
// }
//
// size_t MessageWindow::id() const noexcept { return id_; }

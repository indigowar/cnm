#ifndef HPP_CNM_GUI_SCENES_RING_SCENE_HPP
#define HPP_CNM_GUI_SCENES_RING_SCENE_HPP

#include <cnm/topology/star/star.hpp>
#include <map>
#include <memory>

#include "lib/menu/menu.hpp"
#include "lib/popups/create_office_equipment.hpp"
#include "lib/popups/create_personal_computer.hpp"
#include "lib/popups/create_server.hpp"
#include "lib/scenes/exiter.hpp"
#include "lib/scenes/scene.hpp"
#include "lib/scenes/switcher.hpp"

class RingScene final : public Scenes::Scene {
 public:
  RingScene(Scenes::Switcher* switcher = nullptr,
            Scenes::Exiter* exitter = nullptr);
  void start() override;

  void update() override;

  void render() override;

  void postRender() override;

  void cleanup() override;

  void freeze() override;

  void invoke() override;

 private:
  void renderEditor();

  Menu::Menu makeMenuBar();

  std::unique_ptr<Menu::Menu> menu;

  std::unique_ptr<Cnm::Star::Star> topology;

  bool open_server_creation_window;
  bool open_office_equipment_creation_window;
  bool open_pc_creation_window;

  Popup::CreateServerPopup create_server_popup{};
  Popup::CreateOfficeEquipmentPopup create_office_eq_popup{};
  Popup::CreatePC create_pc_popup{};
};

#endif  // HPP_CNM_GUI_SCENES_RING_SCENE_HPP

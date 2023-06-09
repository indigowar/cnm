#ifndef HPP_CNM_GUI_APPLICATION_HPP
#define HPP_CNM_GUI_APPLICATION_HPP

class Application {
 public:
  explicit Application();

  ~Application();

  void render();

 private:
  void makeMenuBar();
};

#endif  // HPP_CNM_GUI_APPLICATION_HPP

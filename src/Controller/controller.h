#ifndef SRC_CONTROLLER_CONTROLLER_H_
#define SRC_CONTROLLER_CONTROLLER_H_

#include "../Model/glwidget.h"
#include "../View/view.h"

namespace s21 {
class Controller {
 public:
  static Controller& getInstance() {
    static Controller controller;
    return controller;
  }

  //   void setFilepath(const std::string& filepath) {
  //     model_.setFilepath(filepath);
  //   }
  //   const GLObject& getObject() const { return model_.getObject(); }
  //   void changeObjectState(const TransformParams& tp) {
  //     model_.changeObjState(tp);
  //   }

 private:
  Controller() {}
  Controller(const Controller&) = delete;
  Controller(Controller&&) = delete;
  ~Controller() {}
  Controller& operator=(const Controller&) = delete;
  Controller& operator=(Controller&&) = delete;

  //   Model& model_ = Model::getInstance();
};
}  // namespace s21

#endif  //  SRC_CONTROLLER_CONTROLLER_H_
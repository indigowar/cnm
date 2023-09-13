#ifndef HPP_CNM_CORE_CORE_OBJECT_HPP
#define HPP_CNM_CORE_CORE_OBJECT_HPP

namespace Cnm {

// Object is the interface to many objects in the program.
// every object that runs by itself in the CNM should implement this class.
class Object {
 public:
  virtual ~Object() = default;

  // start() executes on the creation of object,
  // after constructor.
  virtual void start() = 0;

  // stop() is killing the object completely,
  // after this call the object can not work.
  virtual void stop() = 0;

  // invoke() is used to start the object again,
  // if previously it was stopped by freeze().
  virtual void invoke() = 0;

  // freeze() is used to hold the execution of the object for some time.
  // it does not completely kill the Object like stop(),
  // Object can run again after calling invoke().
  virtual void freeze() = 0;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CORE_OBJECT_HPP

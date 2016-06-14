#pragma once

// C/C++
#include <memory>

namespace tensy {
class Component {
public:
  virtual ~Component() {}
};  // component

typedef std::unique_ptr<Component> ComponentPtr;

} // tensy

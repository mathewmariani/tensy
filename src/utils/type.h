#pragma once

// C/C++
#include <map>
#include <typeindex>

namespace tensy {
template<class T>
class Type {
public:
  static Type<T>& getType(std::type_index t) {
    if (s_types.count(t) <= 0) {
      s_types.emplace(t, Type<T>(t));
    }

    auto it = s_types.find(t);
    return it->second;
  }

  template<class E>
  static unsigned int getIndex() {
    return Type::getType(std::type_index(typeid(E))).m_index;
  }

private:
  static unsigned int s_index;
  static std::map<std::type_index, Type<T>> s_types;

  unsigned int m_index;
  std::type_index m_type;

  Type(std::type_index &type) :
    m_index(s_index++),
    m_type(type) {

  }
};  // type

template<class T>
unsigned int Type<T>::s_index = 0;

template<class T>
std::map<std::type_index, Type<T>> Type<T>::s_types;

} // tensy

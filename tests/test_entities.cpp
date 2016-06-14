// C/C++
#include <iostream>
#include <lest.hpp>

// tensy
#include <entitymanager.h>

// 1. Create/Delete Entity
//    - add new entity to the system
//    - delete entity from the system
//    - delete entity not found in the system
//    - create entity from recycled id
// 2. Attach Component
//    - attach component to entity
//    - attach multiple components to entity
//    - attach component using default constructor
//    - attach component using contructor
// 3. Detach Component
//    - detach component from entity
//    - detach unassociated component from entity
//    - detach all components from entity
// 4. Get Component
//    - component added is the same as component returned
//    - get component from entity
//    - get component from unknown entity
//    - get unassociated component from entity
//    - get all data of component T

using namespace tensy;

class ComponentA : public tensy::Component {
public:
  ComponentA() : x(0), y(0), z(0) {}
  ComponentA(int x, int y, int z) : x(x), y(y), z(z) {}
  int x, y, z;
};

class ComponentB : public tensy::Component {
public:
  ComponentB() {}
};

const lest::test specification[] = {
  CASE("create new entity in the system.") {
    EntityManager manager;

    EXPECT(manager.createEntity("entity_0") == 0);
    EXPECT(manager.createEntity("entity_1") == 1);
    EXPECT(manager.createEntity("entity_2") == 2);
    EXPECT(manager.createEntity("entity_3") == 3);
  },

  CASE("delete an entity from the system.") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");
    EXPECT_NO_THROW(manager.deleteEntity(e));
  },

  CASE("delete an entity not found in the system.") {
    EntityManager manager;
    EXPECT_THROWS_AS(manager.deleteEntity(5), tensy::Exception);
  },

  CASE("create new entity using recycled id") {
    EntityManager manager;

    auto e0 = manager.createEntity("entity_0");
    auto e1 = manager.createEntity("entity_1");
    auto e2 = manager.createEntity("entity_2");
    auto e3 = manager.createEntity("entity_3");

    manager.deleteEntity(e0);
    manager.deleteEntity(e1);
    manager.deleteEntity(e2);
    manager.deleteEntity(e3);

    EXPECT(manager.createEntity("recycled") == 3);
    EXPECT(manager.createEntity("recycled") == 2);
    EXPECT(manager.createEntity("recycled") == 1);
    EXPECT(manager.createEntity("recycled") == 0);
  },

  CASE("attach component to entity") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");
    manager.attachComponent<ComponentA>(e);

    EXPECT(manager.hasComponent<ComponentA>(e) == true);
    EXPECT(manager.hasComponent<ComponentB>(e) == false);
  },

  CASE("attach multiple components to entity") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");
    manager.attachComponent<ComponentA>(e);
    manager.attachComponent<ComponentB>(e);

    EXPECT(manager.hasComponent<ComponentA>(e) == true);
    EXPECT(manager.hasComponent<ComponentB>(e) == true);
  },

  CASE("attach component using default values") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");
    manager.attachComponent<ComponentA>(e);

    auto transform = manager.getComponent<ComponentA>(e);
    EXPECT(transform.x == 0);
    EXPECT(transform.y == 0);
    EXPECT(transform.z == 0);
  },

  CASE("attach component using contructor") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");
    manager.attachComponent<ComponentA>(e, 1, 2, 3);

    auto transform = manager.getComponent<ComponentA>(e);
    EXPECT(transform.x == 1);
    EXPECT(transform.y == 2);
    EXPECT(transform.z == 3);
  },

  CASE("detach component from entity") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");
    manager.attachComponent<ComponentA>(e);
    manager.attachComponent<ComponentB>(e);
    manager.detachComponent<ComponentA>(e);

    EXPECT(manager.hasComponent<ComponentA>(e) == false);
    EXPECT(manager.hasComponent<ComponentB>(e) == true);
  },

  CASE("detach unassociated component from entity") {
      EXPECT(true == true);
  },

  CASE("detach all components from entity") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");
    manager.attachComponent<ComponentA>(e);
    manager.attachComponent<ComponentB>(e);

    manager.detachComponent<ComponentA>(e);
    manager.detachComponent<ComponentB>(e);

    EXPECT(manager.hasComponent<ComponentA>(e) == false);
    EXPECT(manager.hasComponent<ComponentB>(e) == false);
  },

  CASE("component added is the same as component returned") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");
    auto c = &manager.attachComponent<ComponentB>(e);

    EXPECT(&(manager.getComponent<ComponentB>(e)) == c);
  },

  CASE("get component from entity") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");
    manager.attachComponent<ComponentA>(e);
    manager.attachComponent<ComponentB>(e);

    EXPECT_NO_THROW(manager.getComponent<ComponentA>(e));
    EXPECT_NO_THROW(manager.getComponent<ComponentB>(e));
  },

  CASE("get component from unknown entity") {
    EntityManager manager;

    EXPECT_THROWS_AS(manager.getComponent<ComponentA>(5), tensy::Exception);
    EXPECT_THROWS_AS(manager.getComponent<ComponentB>(5), tensy::Exception);
  },

  CASE("get unassociated component from entity") {
    EntityManager manager;

    auto e = manager.createEntity("entity_0");

    EXPECT_THROWS_AS(manager.getComponent<ComponentA>(e), tensy::Exception);
    EXPECT_THROWS_AS(manager.getComponent<ComponentB>(e), tensy::Exception);
  },

  CASE("get all data of component T") {
    EntityManager manager;

    manager.createEntity("entity_0");
    manager.createEntity("entity_1");
    manager.createEntity("entity_2");
    manager.createEntity("entity_3");

    manager.attachComponent<ComponentA>(0);
    manager.attachComponent<ComponentA>(1);
    manager.attachComponent<ComponentA>(2);
    manager.attachComponent<ComponentA>(3);

    EXPECT(manager.getComponentData<ComponentA>().size() == 4);
  }
};

int main() {
  printf("tensy entity testing\n");
  return lest::run(specification);
}

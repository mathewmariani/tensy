// C/C++
#include <iostream>
#include <lest.hpp>

// tensy
#include <tensy.h>

// tests
// 1. Adding/Removing a process
//    - adding a process
//    - adding a process that already exsits
//    - removing a process
//    - removing a process that doesn't exist
// 2. Getting a process

using namespace tensy;

class ComponentA : public tensy::Component {
public:
  ComponentA(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}
  int x, y, z;
};

class ComponentB : public tensy::Component {
public:
  ComponentB() {}
};

class ProcessorA : public tensy::Processor {
public:
	ProcessorA() {}
	~ProcessorA() {}

	void entityCreated(unsigned int id) { printf("Entity (%u) created!\n", id); }
	void entityUpdated(unsigned int id) { printf("Entity (%u) updated!\n", id); }
	void entityDeleted(unsigned int id) { printf("Entity (%u) deleted!\n", id); }
	void entityEnabled(unsigned int id) { printf("Entity (%u) enabled!\n", id); }
	void entityDisabled(unsigned int id) { printf("Entity (%u) disabled!\n", id); }
	void update(float dt) {}
};

class ProcessorB : public tensy::Processor {
public:
	ProcessorB() {}
	~ProcessorB() {}

	void entityCreated(unsigned int id) { printf("Entity (%u) created!\n", id); }
	void entityUpdated(unsigned int id) { printf("Entity (%u) updated!\n", id); }
	void entityDeleted(unsigned int id) { printf("Entity (%u) deleted!\n", id); }
	void entityEnabled(unsigned int id) { printf("Entity (%u) enabled!\n", id); }
	void entityDisabled(unsigned int id) { printf("Entity (%u) disabled!\n", id); }
	void update(float dt) {}
};

const lest::test specification[] = {
  CASE("adding a process.") {
    EntityManager manager;

    EXPECT_NO_THROW(manager.addProcessor<ProcessorA>());
    EXPECT_NO_THROW(manager.addProcessor<ProcessorB>());
    EXPECT(manager.processorExists<ProcessorA>() == true);
    EXPECT(manager.processorExists<ProcessorB>() == true);
  },

  CASE("Removing a process.") {
    EntityManager manager;
    manager.addProcessor<ProcessorA>();
    manager.addProcessor<ProcessorB>();

    EXPECT_NO_THROW(manager.removeProcessor<ProcessorA>());
    EXPECT_NO_THROW(manager.removeProcessor<ProcessorB>());

    EXPECT(manager.processorExists<ProcessorA>() == false);
    EXPECT(manager.processorExists<ProcessorB>() == false);
  },

  CASE("Removing process that doesn't exist.") {
    EntityManager manager;

    EXPECT_THROWS_AS(manager.removeProcessor<ProcessorA>(), tensy::Exception);
    EXPECT_THROWS_AS(manager.removeProcessor<ProcessorB>(), tensy::Exception);

    EXPECT(manager.processorExists<ProcessorA>() == false);
    EXPECT(manager.processorExists<ProcessorB>() == false);
  },

  CASE("getting a process.") {
    EntityManager manager;

    auto pa = &manager.addProcessor<ProcessorA>();
    auto pb = &manager.addProcessor<ProcessorB>();

    EXPECT_NO_THROW(manager.getProcessor<ProcessorA>());
    EXPECT_NO_THROW(manager.getProcessor<ProcessorB>());

    EXPECT(&(manager.getProcessor<ProcessorA>()) == pa);
    EXPECT(&(manager.getProcessor<ProcessorB>()) == pb);
  }

};

int main() {
  printf("tensy processor testing\n");
  return lest::run(specification);
}

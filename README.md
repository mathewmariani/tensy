# Templated Entity System
tensy is an open source C++ implementation of a Data Oriented Entity System model.

# Documentation
There are three systems for tensy.

## Entity
An Entity is just an identifier, used to describe an object, and provides no functionality.
- Once an entity is deleted `deleteEntity(entity);` its identifier is pooled to be reused.
- Every time an entity is created, or deleted `entityCreated(id)` or `entityDeleted(id)` is called, respectively.

```cpp
EntityManager manager;
auto entity = manager.createEntity("tag");
// ...
manager.deleteEntity(entity);
```

## Component
A component is the data used to describe an entity. 

- Components should always contain simple data and never functions.
- Components should be implemented using [Flyweights](http://gameprogrammingpatterns.com/flyweight.html) for memory-management and efficiency. 
- Components can be attached/detached to entities. Every time a component/detached is attached to an entity `entityUpdated(id)` is called.

```cpp
class Transform : public tensy::Component {
public:
  Transform(int x, int y, int z) : x(x), y(y), z(z) {}
  int x, y, z;
};

manager.attachComponent<Transform>(entity);
auto transform = manager.getComponent<Transform>(entity);
printf("(%u, %u, %u)", transform.x, transform.y, transform.z);
// > (0, 0 , 0)

manager.attachComponent<Transform>(entity, 1, 2, 3);
auto transform = manager.getComponent<Transform>(entity);
printf("(%u, %u, %u)", transform.x, transform.y, transform.z);
// > (1, 2, 3)
```

## Process
Processors contain the logic and `update(float dt);` should be called at every update.
- Processors can be added or removed from a system.

```cpp
class MyProcessor : public tensy::Processor {
public:
  MyProcessor() {}
  ~MyProcessor() {}

  void entityCreated(unsigned int id) {
    // ...
  }
  void entityUpdated(unsigned int id) {
    // ...
  }
  void entityDeleted(unsigned int id) {
    // ...
  }
  void update(float dt) {
    // process data
  }
};

manager.addProcess<MyProcessor>();
```
### References
Adam Martin [Entity Systems are the future of MMOG development](http://t-machine.org/index.php/2007/09/03/entity-systems-are-the-future-of-mmog-development-part-1/)

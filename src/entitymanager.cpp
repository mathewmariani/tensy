#include "entitymanager.h"
namespace tensy {
EntityManager::EntityManager() {
  _uid = 0;
}

EntityManager::~EntityManager() {
  m_entities.clear();
	m_entitiesComponentData.clear();
	m_processors.clear();
}

unsigned int EntityManager::createEntity(const std::string& name) {
  auto id = ((m_deleted.size() > 0) ? m_deleted.back() : _uid++);
  if (id >= m_entities.size()) {
    m_entities.emplace_back(name);
  } else {
    m_deleted.pop_back();
    m_entities[id] = name;
  }

  for (auto const& value : m_processors) {
    value.get()->entityCreated(id);
  }

  return id;
}

void EntityManager::deleteEntity(unsigned int id) {
  TENSY_ASSERT(
    (id < m_entities.size()),
    "Trying to use unknown entity."
  );

  detachAllComponents(id);
  m_deleted.emplace_back(id);

  for (auto const& value : m_processors) {
    value.get()->entityDeleted(id);
  }
}

void EntityManager::attachComponent(
  unsigned int entityId, unsigned int componentId, ComponentPtr *ptr) {
	// resize the vector so it holds the component index
	if (componentId >= m_entitiesComponentData.size()) {
		m_entitiesComponentData.resize(componentId * 3 / 2 + 1);
	}

	// resize the vector so it holds the entity id
	auto& entities = m_entitiesComponentData[componentId];
	if (entityId >= entities.size()) {
	  entities.resize(entityId * 3 / 2 + 1);
	}

  entities[entityId] = ComponentPtr(ptr->release());

	// update all processors
	for (auto const& value : m_processors) {
		value.get()->entityUpdated(entityId);
	}
}

void EntityManager::detachComponent(
  unsigned int entityId, unsigned int componentId) {

  TENSY_ASSERT(
  	(componentId < m_entitiesComponentData.size()),
  	"Trying to use unknown component."
  );

  auto& entities = m_entitiesComponentData[componentId];
  if (entityId <= entities.size()) {
  	entities[entityId] = nullptr;

  	// update all processors
  	for (auto const& value : m_processors) {
  		value.get()->entityUpdated(entityId);
  	}
  }
}

void EntityManager::detachAllComponents(unsigned int id) {
  for (int i = 0; i < m_entitiesComponentData.size(); ++i) {
    auto& entities = m_entitiesComponentData[id];
    if (id < entities.size()) {
      entities[id] = nullptr;
    }
  }
}

void EntityManager::update(float dt) {
  for (auto const& value : m_processors) {
    value.get()->update(dt);
  }
}
};  // tensy

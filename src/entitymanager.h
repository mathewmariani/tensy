#pragma once

// C/C++
#include <atomic>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

// tensy
#include "component.h"
#include "processor.h"
#include "utils/type.h"
#include "utils/assert.h"

namespace tensy {

typedef unsigned int Entity;

class EntityManager {
public:
	EntityManager();
	~EntityManager();

	// typedef for code readability
	typedef std::vector<std::string> EntityTable;
	typedef std::vector<std::vector<ComponentPtr>> EntitiesComponentDataTable;
	typedef std::vector<ProcessorPtr> ProcessorTable;

  /**
	 * Create a new entity in the system.
	 * @param  name Name of entity for debugging.
	 * @return      Unique identifier of the entity.
	 */
	unsigned int createEntity(const std::string& name);

	/**
	 * Remove an entity and its mapped component data from the system.
	 * @param id Unique identifier of the entity.
	 */
	void deleteEntity(unsigned int id);

	/**
	 * Associates an component with an entity.
	 * @param  id   Unique identifier of the entity.
	 * @param  args The arguments for the component constructor.
	 * @return      Returns a reference to the component data.
	 */
	template<class T, typename... Args>
	T& attachComponent(unsigned int id, Args&&... args);

	/**
	 * De-associates a component with an entity.
	 * @param id Unique identifier of the entity.
	 */
	template<class T>
	void detachComponent(unsigned int id);

	/**
	 * De-associates all components with an entity.
	 * @param id Unique identifier of the entity.
	 */
	void detachAllComponents(unsigned int id);

	/**
	 * Returns a reference to the component data for an existing entity.
	 * @param  id Unique identifier of the entity.
	 * @return    Component data of entity.
	 */
	template<class T>
	T& getComponent(unsigned int id);

	/**
	 * Returns a vector of references to all component data of component of type T.
	 * @return    vector of references to component data of type T.
	 */
	template<class T>
  std::vector<ComponentPtr*> getComponentData();

	/**
	 * Returns true if a component is associated with an existing entity.
	 * @param  id Unique identifier of the entity.
	 * @return    True if a component is associated with entity.
	 */
	template<class T>
	bool hasComponent(unsigned int id);

	/**
	 * Add a processor to the system.
	 * @return A reference to the processor added.
	 */
	template<class T>
	T& addProcessor();

	/**
	 * Removes a processor from the system.
	 */
	template<class T>
	void removeProcessor();

	/**
	 * Returns a reference to a processor.
	 * @return Returns a reference.
	 */
	template<class T>
	T& getProcessor();

	template<class T>
	/**
	 * Returns true if a processor is found within the system.
	 * @return True is processor is found.
	 */
	bool processorExists();

	/**
	 * Update all known processors.
	 * @param dt Delta time since the last update call.
	 */
	void update(float dt);

private:
	// use wrappers to avoid code bloating
	void attachComponent(unsigned int entityId, unsigned int componentId, ComponentPtr *ptr);
	void detachComponent(unsigned int entityId, unsigned int componentId);

private:
	std::atomic<unsigned int> _uid;
  std::vector<unsigned int> m_deleted;

  EntityTable m_entities;
	EntitiesComponentDataTable m_entitiesComponentData;
	ProcessorTable m_processors;
};	// entitymanager

template<class T, typename... Args>
T& EntityManager::attachComponent(unsigned int id, Args&&... args) {
	auto index = Type<Component>::getIndex<T>();
  ComponentPtr ptr(new T{std::forward<Args>(args)...});
	attachComponent(id, index, &ptr);

	return (T&) *m_entitiesComponentData[index][id].get();
}

template<class T>
void EntityManager::detachComponent(unsigned int id) {
	auto index = Type<Component>::getIndex<T>();
	detachComponent(id, index);
}

template<class T>
T& EntityManager::getComponent(unsigned int id) {
	auto index = Type<Component>::getIndex<T>();
	TENSY_ASSERT(
		(index < m_entitiesComponentData.size()),
		"Trying to use unknown component."
	);

	TENSY_ASSERT(
		(id < m_entitiesComponentData[index].size() || m_entitiesComponentData[index][id] != nullptr),
		"No reference for component data with entity."
	);

	return (T&) *m_entitiesComponentData[index][id].get();
}

template<class T>
std::vector<ComponentPtr*> EntityManager::getComponentData() {
	auto index = Type<Component>::getIndex<T>();
	TENSY_ASSERT(
		(index < m_entitiesComponentData.size()),
		"Trying to use unknown component."
	);

	std::vector<ComponentPtr*> data;

	for (int i = 0; i < m_entitiesComponentData[index].size(); ++i) {
		if (m_entitiesComponentData[index][i] != nullptr) {
            data.push_back(&m_entitiesComponentData[index][i]);
		}
	}

    return data;
}

template<class T>
bool EntityManager::hasComponent(unsigned int id) {
	auto index = Type<Component>::getIndex<T>();
	return (
		index < m_entitiesComponentData.size() &&
		id < m_entitiesComponentData[index].size() &&
		m_entitiesComponentData[index][id] != nullptr
	);
}

template<class T>
T& EntityManager::addProcessor() {
	auto index = Type<Processor>::getIndex<T>();
	if (index >= m_processors.size()) {
		m_processors.resize(index * 3 / 2 + 1);
	}

	m_processors[index] = ProcessorPtr(new T);
	m_processors[index].get()->m_manager = this;
	return (T&)*m_processors[index].get();
}

template<class T>
void EntityManager::removeProcessor() {
	auto index = Type<Processor>::getIndex<T>();
	TENSY_ASSERT(
		(index < m_processors.size()),
		"Trying to use unknown processor."
	);

	m_processors[index] = nullptr;
}

template<class T>
T& EntityManager::getProcessor() {
	auto index = Type<Processor>::getIndex<T>();
	TENSY_ASSERT(
		(index < m_processors.size() || m_processors[index] != nullptr),
		"Trying to use unknown processor."
	);

	return (T&) *m_processors[index].get();
}

template<class T>
bool EntityManager::processorExists() {
	auto index = Type<Processor>::getIndex<T>();
	return (index < m_processors.size() && m_processors[index] != nullptr);
}

}	// tensy

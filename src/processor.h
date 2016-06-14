#pragma once

// C/C++
#include <memory>

namespace tensy {
class EntityManager;
class Processor {
friend EntityManager;
public:
	Processor() : m_manager(nullptr) {}
	virtual ~Processor() {}
	virtual void entityCreated(unsigned int id) = 0;
	virtual void entityUpdated(unsigned int id) = 0;
	virtual void entityDeleted(unsigned int id) = 0;
	virtual void update(float dt) = 0;

private:
	EntityManager* m_manager;
};	// processor

typedef std::unique_ptr<Processor> ProcessorPtr;

} // tensy

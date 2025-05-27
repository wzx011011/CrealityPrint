#include "softfuncmanager.h"

softFuncManager softFuncManager::s_Manager;
softFuncManager::softFuncManager(QObject* parent)
	:QObject(parent)
{
	m_SoftFunc = new SoftwareFunction(this);
}

softFuncManager& softFuncManager::instance()
{
	return s_Manager;
}

SoftwareFunction* softFuncManager::GetSoftFunc()
{
	return m_SoftFunc;
}
softFuncManager::~softFuncManager()
{
	if (m_SoftFunc != nullptr)
	{
		delete m_SoftFunc;
		m_SoftFunc = nullptr;
	}
}

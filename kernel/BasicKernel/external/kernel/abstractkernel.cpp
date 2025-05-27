#include "abstractkernel.h"


namespace creative_kernel
{

	AbstractKernelUI* AbstractKernelUI::m_self = nullptr;

	AbstractKernelUI* AbstractKernelUI::getSelf()
	{
        return m_self;
    }

    void AbstractKernelUI::registerUIVariables(QQmlApplicationEngine &engine)
    {
        m_engine = &engine;
    }

    QQmlApplicationEngine *AbstractKernelUI::getRegisterUIVariables()
    {
        return m_engine;
    }

}



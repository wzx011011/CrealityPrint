#ifndef CREATIVE_KERNEL_OPENGLCOMPUTE_1603846706957_H
#define CREATIVE_KERNEL_OPENGLCOMPUTE_1603846706957_H
#include "basickernelexport.h"
#include "computefunc.h"

namespace qtuser_qml
{
	class RawOGL;
}

namespace creative_kernel
{
	class OpenGLCompute : public QObject
	{
	public:
		OpenGLCompute(QObject* parent, qtuser_qml::RawOGL* ogl);
		~OpenGLCompute();

		void compute(computeFunc func);
	protected:
		QOpenGLContext* m_context;
		qtuser_qml::RawOGL* m_ogl;
	};
}

#endif // CREATIVE_KERNEL_OPENGLCOMPUTE_1603846706957_H
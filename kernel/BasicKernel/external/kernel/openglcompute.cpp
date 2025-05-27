#include "openglcompute.h"
#include "qtuserqml/gl/rawogl.h"

#include <QtGui/QOffscreenSurface>
namespace creative_kernel
{
	OpenGLCompute::OpenGLCompute(QObject* parent, qtuser_qml::RawOGL* ogl)
		:m_ogl(ogl)
	{
		QOpenGLContext* sharedContext = m_ogl->sharedContext();
		m_context = new QOpenGLContext();
		m_context->setFormat(sharedContext->format());
		m_context->create();
		m_context->setShareContext(sharedContext);
	}

	OpenGLCompute::~OpenGLCompute()
	{

	}

	void OpenGLCompute::compute(computeFunc func)
	{
		if (!func)
			return;

		m_context->makeCurrent(m_ogl->surface());
		func(m_ogl);
		m_context->doneCurrent();
	}
}
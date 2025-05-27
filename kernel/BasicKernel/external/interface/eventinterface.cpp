#include "eventinterface.h"
#include "external/kernel/rendercenter.h"
#include "qtuser3d/event/eventsubdivide.h"

using namespace qtuser_3d;

#define IMPL_NULL_CHECK(x)  void add##x(x* handler){\
									EventSubdivide* divide = getRenderCenter()->eventSubdivide();\
									if(divide) divide->add##x(handler);} \
void remove##x(x* handler) { \
		EventSubdivide* divide = getRenderCenter()->eventSubdivide(); \
		if (divide) divide->remove##x(handler); }\
void close##x##s(x* handler) { \
		EventSubdivide* divide = getRenderCenter()->eventSubdivide(); \
		if (divide) divide->close##x##s(); }

namespace creative_kernel
{						
	IMPL_NULL_CHECK(ResizeEventHandler)
	IMPL_NULL_CHECK(HoverEventHandler)
	IMPL_NULL_CHECK(WheelEventHandler)
	IMPL_NULL_CHECK(RightMouseEventHandler)
	IMPL_NULL_CHECK(MidMouseEventHandler)
	IMPL_NULL_CHECK(LeftMouseEventHandler)
	IMPL_NULL_CHECK(KeyEventHandler)
}
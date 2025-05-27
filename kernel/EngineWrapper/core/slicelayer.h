#ifndef _NULLSPACE_SLICELAYER_1590032647129_H
#define _NULLSPACE_SLICELAYER_1590032647129_H
#include "enginewrapperexport.h"
#include <QtCore/QList>
#include "sliceline.h"

class ENGINE_WRAPPER_API SliceLayer
{
public:
	SliceLayer();
	~SliceLayer();

	QList<SliceLine> m_sliceLines;
	float m_DeltaTime;//¿€º∆ ±º‰
	float thickness;//≤„∫Ò
};
#endif // _NULLSPACE_SLICELAYER_1590032647129_H

#ifndef _NULLSPACE_SLICEMODEL_1590033186701_H
#define _NULLSPACE_SLICEMODEL_1590033186701_H
#include "enginewrapperexport.h"
#include "slicelayer.h"

class ENGINE_WRAPPER_API SliceModel
{
public:
	SliceModel();
	~SliceModel();

	void save(const QString& name);
	std::vector<SliceLayer> m_sliceLayers;
	int colorType;
	bool isThirdparty;
};
#endif // _NULLSPACE_SLICEMODEL_1590033186701_H

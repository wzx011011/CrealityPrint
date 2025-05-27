#ifndef _NULLSPACE_SLICEMODELBUILDER_1590033290815_H
#define _NULLSPACE_SLICEMODELBUILDER_1590033290815_H
#include "enginewrapperexport.h"
#include <QtCore/QObject>
#include "core/sliceline.h"

class SliceModel;
namespace cxsw
{
	class SliceResult;
}
class ENGINE_WRAPPER_API SliceModelBuilder: public QObject
{
public:
	SliceModelBuilder(QObject* parent = nullptr);
	virtual ~SliceModelBuilder();

	static void buildSliceModel(SliceModel* sliceModel, cxsw::SliceResult* sliceResult, float default_height = 0);
	static void buildSliceModel(SliceModel* sliceModel, const std::vector<std::string>& layerCode, float default_height = 0, float beltOffset=0, float beltOffsetY=0,bool spiralFlag = false,int isbelt = 0);
	
};
#endif // _NULLSPACE_SLICEMODELBUILDER_1590033290815_H

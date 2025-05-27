#ifndef _PRINTERENTITY_USE_1588651416007_H
#define _PRINTERENTITY_USE_1588651416007_H
#include "creativekernelexport.h"
#include <Qt3DCore/QEntity>

#include "qtuser3d/math/box3d.h"
#include "qtuser3d/entity/imageentity.h"
#include "qtuser3d/camera/screencamera.h"
#include "qtuser3d/entity/printerentity.h"
#include "qtuser3d/entity/boxentity.h"
#include "qtuser3d/entity/printerskirtentity.h"
#include "qtuser3d/entity/printergrid.h"
#include "qtuser3d/entity/printertext.h"
#include "qtuser3d/entity/axisentity.h"
#include "qtuser3d/entity/faceentity.h"
#include "qtuser3d/entity/texfaces.h"
#include "qtuser3d/module/facepickable.h"
#include "qtuser3d/entity/hotbedentity.h"
using namespace qtuser_3d;


class CREATIVE_KERNEL_API PrinterEntityUse: public PrinterEntity
{
public:
	PrinterEntityUse(Qt3DCore::QNode* parent = nullptr);
	virtual ~PrinterEntityUse();

	void onCameraChanged(qtuser_3d::ScreenCamera* screenCamera) override;
	void updateBox(const qtuser_3d::Box3D& box) override;

	void onModelChanged(Box3D basebox, bool bleft, bool bright, bool bfront, bool bback, bool bup, bool bdown) override;

	void showPrinterEntity(bool isShow) override;

	void enableSkirt(bool enable);
	void visibleSubGrid(bool visible);

	void updateFace(Box3D& box,faceType type);
	void setVisibility(int type, bool visibility);

	void drawBedFaces(bedType _bedType) override;

	void onThemeChanged(int themeType) override;

	void visibleLogo(bool visible) override;

	void visibleBottomColor(int type=0) override;

	void onCheckBed(QList<Box3D>& boxes) override;
protected:
	BoxEntity* m_boxEntity;
	PrinterGrid* m_printerGrid;
	PrinterSkirtEntity* m_printerSkirt;
	PrinterText* m_printerText;
	AxisEntity* m_axisEntity;
	FaceEntity* m_faceEntity;
	TexFaces* m_bottom;
	bool m_bShowEntity;

	HotbedEntity* m_hotbed;
	//ImageEntity* m_imageEntity;


};

#endif // _PRINTERENTITY_USE_1588651416007_H

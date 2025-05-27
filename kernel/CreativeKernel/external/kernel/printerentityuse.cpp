#include "printerentityuse.h"
#include "interface/spaceinterface.h"
#include "interface/renderinterface.h"

PrinterEntityUse::PrinterEntityUse(Qt3DCore::QNode* parent)
	:PrinterEntity(parent)
{
	QVector4D blueColor = QVector4D(0.325f, 0.325f, 0.325f, 1.0f);
	m_boxEntity = new BoxEntity(this);
	m_boxEntity->setColor(blueColor);
	m_printerSkirt = new PrinterSkirtEntity(this);
	//m_printerText = new PrinterText(this);
	m_printerGrid = new PrinterGrid(this);
	m_printerGrid->setLineColor(QVector4D(0.325f, 0.325f, 0.325f, 1.0f));
	//m_printerGrid->setShowColor(QVector4D(1.0f, 1.0f, 1.0f, 1.0f));

	m_axisEntity = new AxisEntity(this, 1);
	m_faceEntity = new FaceEntity(this, 0x1f);
//	m_faceEntity->setColor(qtuser_3d::faceType::down, QVector4D(0.81f, 0.81f, 0.81f, 0.3f));

	m_bottom = new TexFaces(this);

	m_bShowEntity = true;

	//m_imageEntity = new ImageEntity(this);
	m_hotbed = new HotbedEntity(nullptr);
}

PrinterEntityUse::~PrinterEntityUse()
{
}

void PrinterEntityUse::onCameraChanged(qtuser_3d::ScreenCamera* screenCamera)
{
	qtuser_3d::Box3D bbox = creative_kernel::baseBoundingBox();
	Qt3DRender::QCamera* camera = screenCamera->camera();

	QVector3D cameraPosition = camera->position();
	QVector3D viewCenter = camera->viewCenter();
	QVector3D viewUp = camera->upVector();

	//skirt
	if(m_bShowEntity)
	{
		bool enable = cameraPosition.z() > bbox.min.z();
		enableSkirt(enable);
	}

	//grid
	{
		bool visible = false;
		if (cameraPosition.z() < bbox.min.z())
		{
			visible = qAbs(cameraPosition.z() - bbox.min.z()) < 250.0f;
		}
		else
		{
			visible = (qAbs(cameraPosition.z() - bbox.min.z()) < 250.0f) || ((cameraPosition - viewCenter).length() < 100.0f);
		}

		visibleSubGrid(true);
	}
}

void PrinterEntityUse::updateBox(const qtuser_3d::Box3D& box)
{
	qDebug() << "scope = " << box.min << box.max;
	qtuser_3d::Box3D globalBox = box;
	m_boxEntity->updateGlobal(globalBox, false);
	m_printerSkirt->updateBoundingBox(globalBox);
	m_printerGrid->updateBounding(globalBox,1);
	//m_printerText->updateLen(globalBox, 10.0f, 4);
	//m_faceEntity->drawFace(globalBox);

	//m_imageEntity->updateGlobal(globalBox);
	m_bottom->updateBox(box);
}

void PrinterEntityUse::onModelChanged(Box3D basebox, bool bleft, bool bright, bool bfront, bool bback, bool bup, bool bdown)
{
	if (bleft)
	{
		updateFace(basebox, qtuser_3d::faceType::left);
	}
	else
	{
		setVisibility((int)qtuser_3d::faceType::left, false);
	}
	if (bfront)
	{
		updateFace(basebox, qtuser_3d::faceType::front);
	}
	else
	{
		setVisibility((int)qtuser_3d::faceType::front, false);
	}
	//if (bdown)
	//{
	//	updateFace(basebox, qtuser_3d::faceType::down);
	//}
	//else
	//{
	//	setVisibility((int)qtuser_3d::faceType::down, false);
	//}
	if (bright)
	{
		updateFace(basebox, qtuser_3d::faceType::right);
	}
	else
	{
		setVisibility((int)qtuser_3d::faceType::right, false);
	}
	if (bback)
	{
		updateFace(basebox, qtuser_3d::faceType::back);
	}
	else
	{
		setVisibility((int)qtuser_3d::faceType::back, false);
	}
	if (bup)
	{
		updateFace(basebox, qtuser_3d::faceType::up);
	}
	else
	{
		setVisibility((int)qtuser_3d::faceType::up, false);
	}
	if (!m_bottom->isImageColor())
	{
		m_bottom->setImageVisible(creative_kernel::haveModels() ? false : true);
	}
}

void PrinterEntityUse::visibleLogo(bool visible)
{
	m_bottom->setImageVisible(visible);
}

void PrinterEntityUse::visibleBottomColor(int type)
{
	m_bottom->loadImage(type);
}

void PrinterEntityUse::onCheckBed(QList<Box3D>& boxes)
{
	m_hotbed->checkBed(boxes);
}

void PrinterEntityUse::enableSkirt(bool enable)
{
	m_printerSkirt->setEnabled(enable);
}

void PrinterEntityUse::visibleSubGrid(bool visible)
{
	m_printerGrid->visibleSubGrid(visible);
}

void PrinterEntityUse::updateFace(Box3D& box,faceType type)
{
	m_faceEntity->updateFace(box,type);
}

void PrinterEntityUse::setVisibility(int type, bool visibility)
{
	m_faceEntity->setVisibility(type, visibility);
}
	
void PrinterEntityUse::drawBedFaces(bedType _bedType)
{
	if (_bedType != bedType::None)
	{
		//if (m_hotbed->faceNum()>0)
		//{
		//	m_hotbed->clearData();
		//	m_hotbed->setParent((Qt3DCore::QNode*)nullptr);
		//}
		//m_hotbed = new HotbedEntity(nullptr, 0);
		//m_hotbed->drawFace(faceNum);
		//m_hotbed->setParent(this);
		//delete m_hotbed;
		m_hotbed->clearData();
		m_hotbed->setParent((Qt3DCore::QNode*)nullptr);
		m_hotbed = new HotbedEntity(nullptr);
		m_hotbed->drawFace(_bedType);
		m_hotbed->setParent(this);
	}
	else
	{
		m_hotbed->clearData();
		m_hotbed->setParent((Qt3DCore::QNode*)nullptr);
		//delete m_hotbed;
		//m_hotbed = nullptr;
	}
		
}

void PrinterEntityUse::showPrinterEntity(bool isShow)
{
	m_bShowEntity = isShow;
	m_boxEntity->setEnabled(isShow);//蓝色边框
	m_printerSkirt->setEnabled(isShow);//灰色边线
	//m_printerText->setEnabled(isShow);//刻度
	m_printerGrid->setEnabled(isShow);//网格线
	m_axisEntity->setEnabled(false);//坐标指示
	//m_imageEntity->setEnabled(isShow);//logo
	m_bottom->setEnabled(isShow);
}

void PrinterEntityUse::onThemeChanged(int themeType)
{
	if (themeType == 0)
	{
		//深色
		m_printerSkirt->setColor(QVector4D(0.15f, 0.15f, 0.15f, 1.0f));
		m_bottom->setColor(QVector4D(0.27f, 0.27f, 0.27f, 1.0f));
		m_bottom->setLogColor(QVector4D(0.2f, 0.2f, 0.2f, 0.8f));
		m_printerGrid->setLineColor(QVector4D(0.32f, 0.32f, 0.32f, 1.0));
		m_printerGrid->setShowColor(QVector4D(0.65f, 0.23f, 0.23f, 1.0), 0);
		m_printerGrid->setShowColor(QVector4D(0.20f, 0.40f, 0.15f, 1.0), 1);
		m_boxEntity->setColor(QVector4D(0.325f, 0.325f, 0.325f, 1.0f));
	}
	else
	{
		//浅色
		m_printerSkirt->setColor(QVector4D(0.81f, 0.81f, 0.81f, 1.0f));
		m_bottom->setColor(QVector4D(1.00f, 1.00f, 1.00f, 1.0f));
		m_bottom->setLogColor(QVector4D(0.89f, 0.89f, 0.89f, 1.0f));
		m_printerGrid->setLineColor(QVector4D(0.73f, 0.73f, 0.73f, 1.0));
		m_printerGrid->setShowColor(QVector4D(1.00f, 0.68f, 0.68f, 1.0), 0);
		m_printerGrid->setShowColor(QVector4D(0.25f, 1.0f, 0.25f, 1.0), 1);
		m_boxEntity->setColor(QVector4D(0.79f, 0.79f, 0.79f, 1.0f));
	}
	creative_kernel::renderOneFrame();
}

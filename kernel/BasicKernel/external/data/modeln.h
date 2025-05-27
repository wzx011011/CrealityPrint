#ifndef CREATIVE_KERNEL_MODELN_1592181954934_H
#define CREATIVE_KERNEL_MODELN_1592181954934_H
#include "basickernelexport.h"
#include "data/trimeshentity.h"
#include <vector>

namespace us
{
	class ModelSetting;
}

namespace qtuser_3d
{
	class ModelNEntity;
	class PureColorEntity;
	class BasicEntity;
}

namespace creative_kernel
{
	class FDMSupportGroup;
	class BASIC_KERNEL_API ModelN : public TrimeshEntity
	{
		Q_OBJECT
	public:
		ModelN(QObject* parent = nullptr);
		virtual ~ModelN();

		void setModelName(const QString& name);

		QVector3D& GetInitPosition();
		void SetInitPosition(QVector3D QPosition);

		us::ModelSetting* setting();
		void setsetting(us::ModelSetting* modelsetting);

		void setState(float state);
		void setErrorState(bool error);
		void setBoxState(int state);  // 0 hide,  1 show as select state
		void setVisibility(bool visibility);
		bool isVisible();
		qtuser_3d::ModelNEntity* getModelEntity();

		// 自定义颜色，当 state 值大于 5 时生效
		void setCustomColor(QColor color);
		QColor getCustomColor();

		// 透明度
		void setTransparency(float alpha);
		float getTransparency();

		// 是否叠加光照效果
		void setLightingEnable(bool enable);
		bool getLightingEnable();

		void setNozzle(int nozzle);
		int nozzle();


        void buildFDMSupport();
		void showModelFace();
		void hideModelFace();
		//void showModelLine();
		//void hideModelLine();

		void setShowType(int showtype);

		void enterSupportStatus();
		void leaveSupportStatus();
		void setSupportCos(float cos);

		void setNeedCheckScope(int checkscope);

		void setWaterPoint(QVector3D water, float zcha);
		void unSetWaterPoint();

		qtuser_3d::Box3D boxWithSup();

		bool hasFDMSupport();
		void setFDMSup(FDMSupportGroup* fdmSup);

        FDMSupportGroup* fdmSupport();

		void setFDMSuooprt(bool detectAdd);
		bool getFDMSuooprt();

		QVector3D localRotateAngle();
		QQuaternion rotateByStandardAngle(QVector3D axis, float angle);
		// redoFlag: false = undo， true = redo
		void updateDisplayRotation(bool redoFlag, int step = 1);
		void resetRotate();

		void setSupportLiftHeight(float value);
		float getSupportLiftHeight();
		

		int haveAuxiliary(int pos) const;

		int prepareHollow();

		int setAuxiliaryMesh(int pos, const trimesh::TriMesh& mesh, const trimesh::xform& xf);

		trimesh::TriMesh* getAuxiliaryMesh(int pos);
		TriMeshPtr getAuxiliaryMeshPtr(int pos);
		int numAuxiliaryMesh() const;


		void SetModelViewClrState(ControlState statevalue, bool boxshow);

		// 附属模型：从属于当前 ModelN，其矩阵变换会叠加上所属 ModelN 的全局变换
		void insertAttachModel(QString uid, ModelN* model);
		void removeAttachModel(QString uid);
		void clearAttachModel();
		ModelN* getAttachModel(QString uid);
		QList<QString> getAttachModelIDList();
		bool hasAttach();

		// withAttach 是否包含附属模型
		// attachUpdating 合并模型是否需要更新，若为 false 直接返回上一次的合并缓存结果；只有当 withAttach 为 true 时才能生效
		trimesh::TriMesh* mesh(bool withAttach = false, bool attachUpdating = false);  // don't store this raw pointer as shared pointer, and use it as temporary variable
		TriMeshPtr meshptr(bool withAttach = false, bool attachUpdating = false);
		
		int getErrorEdges();
        int getErrorNormals();
		int getErrorHoles();
		int getErrorIntersects();

        void setErrorEdges(int value);
        void setErrorNormals(int value);
        void setErrorHoles(int value);
        void setErrorIntersects(int value);

		void needDetectError();

		void setTexture();
	protected:
		void onGlobalMatrixChanged(const QMatrix4x4& globalMatrix) override;
		void onStateChanged(ControlState state) override;
		void meshChanged(qtuser_core::Progressor *progressor=nullptr) override;
		void faceBaseChanged(int faceBase) override;

		void meshFromGeometryData(GeometryData* faceGeoData = nullptr, GeometryData* lineGeoData = nullptr, GeometryData* edgeGeData = nullptr) override;

		void setSupportsVisibility(bool visibility);

		void mirror(const QMatrix4x4& matrix, bool apply = true) override;
	protected:
		qtuser_3d::ModelNEntity* m_entity;

		// 附属模型更新标识
		bool m_attachUpdating;
		// 带附属模型的网格合并缓存
		TriMeshPtr m_meshWithAttachCache;
		QMap<QString, ModelN*> m_qmapofAttachModel;

		QVector3D m_initPosition;
		us::ModelSetting* m_setting;

		FDMSupportGroup* m_fdmSupportGroup;

		int m_currentLocalDispalyAngle;
		QList<QVector3D> m_localAngleStack;

		int m_showType;      // 1 面， 2 线
		bool m_visibility;
		bool m_detectAdd; //for erase support;
		int m_nozzle;
		float m_supportLiftHeight;
		
		//show error
        int m_errorEdges;      //缺陷边
        int m_errorNormals;    //缺陷的法线
		int m_errorHoles;      //孔洞个数
		int m_errorIntersects; //非流面

		/*
		* 0 为原始模型
		* 1 为抽壳后的两层壁
		* 2 为抽壳后的填充物
		*/
		std::vector< TriMeshPtr> m_auxiliaryMeshs;

	};
}
#endif // CREATIVE_KERNEL_MODELN_1592181954934_H

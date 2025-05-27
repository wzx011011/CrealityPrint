#ifndef _creative_kernel_UNDOMATRIXCHANGE_1590115341614_H
#define _creative_kernel_UNDOMATRIXCHANGE_1590115341614_H
#include <QtGui/QVector3D>
#include <QtGui/QQuaternion>
#include <QtGui/QMatrix4x4>
#include <QtCore/QList>
#include "data/trimeshheader.h"

namespace creative_kernel
{
	class ModelN;

	struct V3Change
	{
		QVector3D start;
		QVector3D end;
	};

	struct QChanged
	{
		QQuaternion start;
		QQuaternion end;
	};

	struct NUnionChangedStruct
	{
		ModelN* model;

		V3Change posChange;
		bool posActive;

		V3Change scaleChange;
		bool scaleActive;

		QChanged rotateChange;
		bool rotateActive;

		NUnionChangedStruct()
			: model(nullptr)
		{
			posActive = false;
			scaleActive = false;
			rotateActive = false;
		}
	};

	struct NMirrorStruct
	{
		ModelN* model;
		int type;
		QMatrix4x4 start;
		QMatrix4x4 end;
	};

	struct MeshChange
	{
		ModelN* model;
		TriMeshPtr start;
		TriMeshPtr end;
		QString startName;
		QString endName;

		TriMeshPtr aux_start;
		TriMeshPtr aux_end;
		int aux_pos;

		MeshChange()
			: model(nullptr)
		{
			aux_pos = -1;
		}
	};
}

#endif // _creative_kernel_UNDOMATRIXCHANGE_1590115341614_H

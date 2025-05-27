#ifndef COMMAND_OPERATION_H
#define COMMAND_OPERATION_H
#include "basickernelexport.h"
#include <vector>
#include <QtGui/QVector3D>
#include <QPoint>

namespace trimesh
{
	class TriMesh;
}

namespace creative_kernel
{
	BASIC_KERNEL_API int cmdSaveAs(const QString& fileName);
	BASIC_KERNEL_API void cmdSaveSelectStl(const QString filePath);

	BASIC_KERNEL_API int cmdShowModelFace();
	BASIC_KERNEL_API int cmdShowModelLine();
	BASIC_KERNEL_API int cmdShowModelFaceLine();

	BASIC_KERNEL_API int cmdReset();

	BASIC_KERNEL_API int cmdResetAll();

	BASIC_KERNEL_API int cmdClone(int numb);

	BASIC_KERNEL_API int cmdSetNozzle(int nozzle);

	BASIC_KERNEL_API int cmdOneSideAsBottomFace(QPoint point);

	BASIC_KERNEL_API int cmdPlaceOnPrinterBed();

	BASIC_KERNEL_API int cmdMergeModel();
}





#endif //
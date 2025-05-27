#ifndef _NULLSPACE_NORMALCALCULATOR_1591105609079_H
#define _NULLSPACE_NORMALCALCULATOR_1591105609079_H
#include "basickernelexport.h"
#include <QtCore/QObject>
#include "trimesh2/TriMesh.h"
#include "qtusercore/module/progressor.h"

class BASIC_KERNEL_API NormalCalculator: public QObject
{
public:
	NormalCalculator(QObject* parent = nullptr);
	virtual ~NormalCalculator();

	static void calculate(trimesh::TriMesh* mesh, qtuser_core::Progressor* progressor);
};
#endif // _NULLSPACE_NORMALCALCULATOR_1591105609079_H

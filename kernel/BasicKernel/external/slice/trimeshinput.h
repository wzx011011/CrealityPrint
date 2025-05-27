#ifndef _NULLSPACE_TRIMESHINPUT_1589465815718_H
#define _NULLSPACE_TRIMESHINPUT_1589465815718_H
#include "basickernelexport.h"
#include "us/modelinput.h"
#include "trimesh2/TriMesh.h"
#include "trimesh2/XForm.h"

namespace creative_kernel
{
	class ModelN;
}

class BASIC_KERNEL_API TrimeshInput: public us::ModelInput
{
public:
	TrimeshInput(QObject* parent = nullptr);
	virtual ~TrimeshInput();

	void setModel(creative_kernel::ModelN* model);
	void build();
	void beltSet();
	void tiltSliceSet(trimesh::vec axis, float angle);
	us::ModelSetting* modelSetting() override;
	const us::ModelSetting* modelSetting() const override;
	void setBeltOffset(const trimesh::vec3& offset);
	trimesh::TriMesh* generateBeltSupport(trimesh::TriMesh* mesh,float angle);
	trimesh::TriMesh* generateSlopeSupport(trimesh::TriMesh* mesh, float rotation_angle, trimesh::vec axis, float support_angle, bool bottomBigger);
//protected:
	int vertexNum() const override;
	float* position() const override;
	float* normal() const override;
	float* offset() const override;

	int triangleNum() const override;
	int* indices() const override;
protected:
	creative_kernel::ModelN* m_model;
	trimesh::TriMesh* m_mesh;
	trimesh::vec3 m_offset;
};
#endif // _NULLSPACE_TRIMESHINPUT_1589465815718_H

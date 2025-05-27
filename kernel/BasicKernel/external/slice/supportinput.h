#ifndef _SUPPORTINPUT_1596784817294_H
#define _SUPPORTINPUT_1596784817294_H
#include "basickernelexport.h"
#include "us/modelinput.h"
#include "trimesh2/TriMesh.h"
#include "trimesh2/XForm.h"

namespace creative_kernel
{
	class ModelN;
}

class BASIC_KERNEL_API SupportInput : public us::ModelInput
{
	Q_OBJECT
public:
	SupportInput(QObject* parent = nullptr);
	virtual ~SupportInput();

	void setModel(creative_kernel::ModelN* model);
	void setTrimesh(trimesh::TriMesh* mesh);
	void build();
	void buildFDM();
	void tiltSliceSet(trimesh::vec axis, float angle);

	void beltSet(trimesh::vec3 offset);
	void setBeltOffset(const trimesh::vec3& offset);

	us::ModelSetting* modelSetting() override;
	const us::ModelSetting* modelSetting() const override;
protected:
	int vertexNum() const override;
	float* position() const override;
	float* normal() const override;

	int triangleNum() const override;
	int* indices() const override;

protected:
	creative_kernel::ModelN* m_model;
	trimesh::TriMesh* m_mesh;
	us::ModelSetting* m_setting;
};

#endif // _SUPPORTINPUT_1596784817294_H
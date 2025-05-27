#ifndef _ExtendMeshGenJOB_1595582868614_H
#define _ExtendMeshGenJOB_1595582868614_H
#include "qtusercore/module/job.h"
#include "job/meshloadinfo.h"
#include "photo2mesh.h"
namespace creative_kernel
{
	class ModelN;
}

class ExtendMeshGenJob : public qtuser_core::Job
{

public:
	MeshLoadInfo* m_meshLoadInfo;
	Photo2Mesh m_p2m;
public:
	ExtendMeshGenJob(QObject* parent = nullptr);
	virtual ~ExtendMeshGenJob();

	void setModel(creative_kernel::ModelN* model);
	void setName(QString filename);

protected:
	QString name() override;
	QString description() override;
	void work(qtuser_core::Progressor* progressor) override;
	void failed() override;
	void successed(qtuser_core::Progressor* progressor) override;
signals:
	void ExtendMeshGened();
private:
	creative_kernel::ModelN* m_model;

	QString m_fileName;

};

#endif // _ExtendMeshGenJOB_1595582868614_H
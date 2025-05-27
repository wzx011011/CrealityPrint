#pragma once
#include "qtusercore/module/job.h"
#include "crealitycloudui.h"

#include "crealitycloud/cxnetworkmanager.h"

class LoginJob : public qtuser_core::Job
{
public:
	LoginJob(CrealityCloudUI* theOp = nullptr,QObject* parent = nullptr);
	virtual ~LoginJob();

	void SetIdenticalStr(QString identicalStr);
	void SetExpireTimeStr(QString expireTimeStr);
	void SetStopState(bool isStop);

protected:
	QString name() override;
	QString description() override;
	void work(qtuser_core::Progressor* progressor) override;
	void failed() override;
	void successed(qtuser_core::Progressor* progressor) override;
	void funcQrLoginStateFromCloud(const QByteArray& response);

private:
	QString m_strIdentical; 
	int m_nExpireTime;
	CrealityCloudUI* m_Op;
	//creative_kernel::loginUserInfo m_userinfo;
	bool m_stop;
	int m_rst;
};
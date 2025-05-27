#ifndef REMOTE_PRINTER_SESSION_H
#define REMOTE_PRINTER_SESSION_H

#include <QObject>
#include "RemotePrinterConstant.h"

struct RemotePrinterSession
{
	std::string uniqueId;//�豸Ψһ��ʶ����mac��ַ����mac��ַ������octoprint�ͼ�Ⱥ��˵�Ƿ�������ַ
	RemotePrinerType type;//Զ�̴�ӡ��������
	std::string ipAddress;//һ�Զ�Ҳ�Ǳ����������ַ
	std::string macAddress;
	std::string token;//token��api-key
	std::string previewImg;
	bool connected;
	time_t tmLastActive;
	time_t tmLastGetFileList;

	RemotePrinterSession()
		:type(RemotePrinerType::REMOTE_PRINTER_TYPE_NONE),
		connected(false),
		tmLastActive(0),
		tmLastGetFileList(0)
	{
	}
};

#endif // REMOTE_PRINTER_SESSION_H



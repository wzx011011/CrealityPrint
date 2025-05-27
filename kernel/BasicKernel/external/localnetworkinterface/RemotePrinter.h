#ifndef REMOTE_PRINTER_H
#define REMOTE_PRINTER_H

#include <QtCore/QString>
#include <QObject>
#include "RemotePrinterConstant.h"

struct RemotePrinter
{
	QString uniqueId;//�豸Ψһ��ʶ����mac��ַ����mac��ַ������octoprint�ͼ�Ⱥ��˵�Ƿ�������ַ
	RemotePrinerType type;//Զ�̴�ӡ��������
	QString ipAddress;
	QString macAddress;
	QString token;//����octoprint
	QString deviceName;//�豸�����ӡ���ݮ�ɣ�����
	QString productKey;//�豸ID
	QString printerName;//��ӡ������
	QString printFileName; //��ӡ�ļ�����
	QString curPosition; //��ǰλ�� ����X50 Y25.3 Z1.2
	int printerStatus;//��ӡ��״̬ 0������ 1������ 2: �����쳣
	int TFCardStatus; //TF��״̬ 0������ 1������
	int printState;//��ӡ״̬ 0������ 1����ӡ�� 2����ӡ��� 3����ӡʧ�� 4��ֹͣ��ӡ 5����ӡ��ͣ
	int fanSwitchState; //���ȿ���״̬ 0���ر� 1������
	int printProgress; //��ӡ����
	float nozzleTemp; //���쵱ǰ�¶�
	float nozzleTempTarget; //����Ŀ���¶�
	float bedTemp; //��ӡƽ̨��ǰ�¶�
	float begTempTarget; //��ӡƽ̨Ŀ���¶�
	int printSpeed; //��ӡ�ٶ�
	int printJobTime; //��ӡʱ��
	int printLeftTime; //��ӡʣ��ʱ��
	int devConnectType; //1:�Զ�����  2:�ֶ�����
	int autoHomeState; //1:���״̬  2:ִ����״̬
	int errorType;
	int video; //0: ����Ƶ����  1:����Ƶ����
	int ledState; //0: off  1: on
	int layer;
	int totalLayer;
	int printMode;//0:local 1:lan 2:cloud

	RemotePrinter()
	{
		uniqueId = "";
		type = RemotePrinerType::REMOTE_PRINTER_TYPE_NONE;
		ipAddress = "";
		macAddress = "";
		deviceName = "";
		productKey = "";
		printerName = "";
		printFileName = "";
		curPosition = "";
		printerStatus = 0;
		TFCardStatus = 0;
		printState = 0;
		fanSwitchState = 0;
		printProgress = 0;
		nozzleTemp = 0;
		nozzleTempTarget = 0;
		bedTemp = 0;
		begTempTarget = 0;
		printSpeed = 0;
		printJobTime = 0;
		printLeftTime = 0;
		devConnectType = 0;
		autoHomeState = 0;
		errorType = 1;
		video = 0;
		ledState = 0;
		layer = 0;
		totalLayer = 0;
		printMode = 0;
	}
};

Q_DECLARE_METATYPE(RemotePrinter)

#endif // REMOTE_PRINTER_H



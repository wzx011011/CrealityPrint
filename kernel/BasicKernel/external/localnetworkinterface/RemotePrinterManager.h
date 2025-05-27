#ifndef REMOTE_PRINTER_MANAGER_H
#define REMOTE_PRINTER_MANAGER_H

#include "basickernelexport.h"
#include "qtusercore/module/singleton.h"
#include "cxmdns/cxmdns.h"
#include "RemotePrinter.h"
#include "RemotePrinterSession.h"
#include <functional>
#include <mutex>
#include <condition_variable>
#include "LanPrinterInterface.h"
#include "OctoPrintInterface.h"

//�������Ƕ����ûص��ķ�ʽ��֪ͨUI���Ʋ㣬����Qt�ź���۵����������ں�����ֲ

namespace creative_kernel
{
	using FuncSearchCb = std::function<void(std::string, std::string)>;
	using FuncGetPrinterInfoCb = std::function<void(RemotePrinter)>;
	using FuncProgressCb = std::function<void(float progress, void*)>;
	using FuncGetFileListCb = std::function<void(std::string, std::string)>;
	using FuncGetPreviewCb = std::function<void(std::string, std::string, std::string)>;//mac address, image type, image data

	class BASIC_KERNEL_API RemotePrinterManager
	{
		SINGLETON_DECLARE(RemotePrinterManager)
	public:
		virtual ~RemotePrinterManager();
	public:
		void Init();
		void searchDevices();
		void searchLanDeviceThread();
		void refreshStateThread();
		
		void addPrinter(const RemotePrinter& printer);
		void deletePrinter(const RemotePrinter& printer);
		void getPrinterInfo(RemotePrinter& printer);

		void getFileList(const RemotePrinter& printer, const std::string& strDir);
		void pushFile(const RemotePrinter& printer, const std::string& filePath, std::function<void(std::string, float)> callback, std::function<void(std::string, std::string)> errorCallback=nullptr);
		void pushFile2AllPrinters(const std::string& filePath);
		void getPreviewImg(const RemotePrinter& printer, const std::string& filePath);
		void deleteFile(const RemotePrinter& printer, const std::string& filePath);
		void deleteFileFromAllPrinters(const std::string& filePath);

		void getCurrentJob(RemotePrinter& printer);
		void controlJob(RemotePrinter& printer, const int& controlType);//0:start 1:cancel 2:restart 3:pause 4:resume

		void controlPrinter(const RemotePrinter& printer, const PrintControlType& cmdType, const std::string& value = "");
		void transparentCommand(const RemotePrinter& printer, const std::string& value);

	private:
		void updateSessitonActive(const std::string& uuid, const time_t& tmActive = 0);

	private:
		//�źŸ�Ϊ�ص�
		FuncSearchCb m_pfnSearchCb = nullptr;
		FuncGetPrinterInfoCb m_pfnGetInfoCb = nullptr;
		FuncGetFileListCb m_pfnGetFileListCb = nullptr;
		FuncGetPreviewCb m_pfnGetPreviewCb = nullptr;

	public:
		void setSearchCb(FuncSearchCb callback) { m_pfnSearchCb = callback; }
		void setGetInfoCb(FuncGetPrinterInfoCb callback) { m_pfnGetInfoCb = callback; }
		void setGetFileListCb(FuncGetFileListCb callback) { m_pfnGetFileListCb = callback; }
		void setGetPreviewCb(FuncGetPreviewCb callback) { m_pfnGetPreviewCb = callback; }

	protected:

	private:
		bool m_bExit = false;
		std::condition_variable m_condition;
		std::mutex m_mutex;
		std::map<std::string, RemotePrinterSession> m_mapID2Printer;
		LanPrinterInterface* m_pLanPrinterInterface;
		OctoPrintInterface* m_pOctoPrinterInterface;
	};
	SINGLETON_EXPORT(BASIC_KERNEL_API, RemotePrinterManager)
}

#endif // REMOTE_PRINTER_MANAGER_H



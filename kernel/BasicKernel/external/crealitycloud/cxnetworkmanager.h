#ifndef _cxNetworkManager_H
#define _cxNetworkManager_H
#include <QtCore/QObject>
#include "basickernelexport.h"
#include <functional>
#include "qtusercore/module/singleton.h"
#include "networkmanagerthread.h"
#include <cpr/cpr.h>

namespace creative_kernel
{
	using FuncDeviceListCb = std::function<void(std::string)>;
	using FuncTaskListCb = std::function<void(std::string)>;

	struct loginInfo
	{
		int loginState;  //0: 未登录   1: 已登录
		QString userID;
		QString token;

		loginInfo() {
			loginState = 0;
			userID = "";
			token = "";
		}
	};

	struct userInfo
	{
		QString nickName;
		QString avatar;
		double maxStorageSpace;
		double usedStorageSpace;

		userInfo() {
			nickName = "";
			avatar = "";
			maxStorageSpace = 0;
			usedStorageSpace = 0;
		}
	};

	class BASIC_KERNEL_API cxNetworkManager : public QObject
	{
		SINGLETON_DECLARE(cxNetworkManager)
	public:
		//cxNetworkManager(QObject* parent = nullptr);
		virtual ~cxNetworkManager();

		/*
		* init 初始化，启动任务轮询线程
		*/
		void init();
		void setThreadPool(QThreadPool* pool);


		/*
		* uninit 反初始化，停止相关线程，程序关闭前调用
		*/
		void uninit();

		/*
		* type = 1: cxnet; type = 2:oss; type = 3: sensors
		*  eg: type = 1
		*      "__REQUEST_URl__": "www.baidu.com"
		*      "__REQUEST_TYPE__": "1"  ==> 1: post; 2: get; 3: get no base url
		*      "__REQUEST_BODY__": "body"
		*      "__REQUEST_TOKEN__": "token"
		*      "__REQUEST_USER_ID__": "user_id"
		*      "__REQUEST_TIME_OUT__": "5000" default 5000ms,please change this value when you want change timeout value
		*    call_back : func ret
		*     
		*/
		bool addRequestTask(int type, const QMap<QString, QString>& data, const std::function<void(QByteArray)>& call_back = NULL, const std::function<void(QByteArray)>& call_back1 = NULL);

		void setloginInfo(int status, QString token, QString userID) {
			m_loginInfo.loginState = status;
			m_loginInfo.token = token;
			m_loginInfo.userID = userID;
		};
		void setUserInfo(QString nickName, QString avatar, double maxStorageSpace, double usedStorageSpace) {
			m_userInfo.nickName = nickName;
			m_userInfo.avatar = avatar;
			m_userInfo.maxStorageSpace = maxStorageSpace;
			m_userInfo.usedStorageSpace = usedStorageSpace;
		};

		int getLoginInfoLoginState() { return m_loginInfo.loginState; }
		QString getLoginInfoUserID() { return m_loginInfo.userID; }
		QString getLoginInfoToken() { return m_loginInfo.token; }
		QString getLoginInfoNickName() { return m_userInfo.nickName; }
		QString getLoginInfoAvatar() { return m_userInfo.avatar; }
		double getLoginInfoMaxStorageSpace() { return m_userInfo.maxStorageSpace; }
		double getLoginInfoUsedStorageSpace() { return m_userInfo.usedStorageSpace; }

		//先用异步实现集控的接口，后面再整理代码
		const std::string cDeviceListSuffix = "/api/rest/print/cluster/deviceList";
		const std::string cTaskListSuffix = "/api/rest/print/cluster/taskList";
		const std::string cConfirmDeviceIdleSuffix = "/api/rest/print/cluster/confirm/";
		const std::string cCancelTaskSuffix = "/api/rest/print/cluster/cancel/";
		const std::string cAddTaskSuffix = "/api/rest/print/cluster/addTask";
		const std::string cGetGcodeListSuffix = "/api/cxy/v2/gcode/ownerList";
		const std::string cGetPrintReportSuffix = "/api/rest/print/cluster/report/";
		const std::string cGetGcodeSliceTypeListSuffix = "/api/cxy/v2/device/printerTypeList";
		const std::string cSetDeviceDesiredProperty_NewIOTControlPrintGcode = "/api/rest/iotrouter/rpc/oneway/";

		const std::string cStartPrintGcodeSuffix = "/api/cxy/v2/print/startPrint";;
		const std::string cGetOnwerDeviceList = "/api/cxy/v2/device/onwerDeviceList";
		const std::string cQueryDevicePropertyStatus_NewIOT = "/api/rest/iotrouter/plugins/telemetry/";
		const std::string cQueryDevicePropertyStatus_NewIOT_RealTime = "/api/rest/iotrouter/";
		const std::string cQueryDevicePropertyStatus_NewIOT_Position = "/api/rest/iotrouter/rpc/twoway/";

		//creality cloud
		const std::string cGetQrLoginInfo = "/api/cxy/account/v2/qrLogin";
		const std::string cQueryQrLoginInfo = "/api/cxy/account/v2/qrQuery";
		const std::string cUploadGcode = "/api/cxy/v2/gcode/uploadGcode";
		const std::string cModelGroupCreate = "/api/cxy/v3/model/modelGroupCreate";
		const std::string cGetAliyunInfo = "/api/cxy/account/v2/getAliyunInfo";
		const std::string cGetAliyunOssBucket = "/api/cxy/account/v2/getOssInfo";
		const std::string cImportMyModel = "/api/cxy/v3/model/filesDownload";
		const std::string cImportDetailItem = "/api/cxy/v3/model/fileDownload";
		const std::string cGetModelFileList = "/api/cxy/v3/model/fileListPage";
		const std::string cGetModelGroupDetail = "/api/cxy/v3/model/fileList";
		const std::string cGetRecommendModel = "/api/cxy/v3/model/recommend"; 
		const std::string cGetCategoryList = "/api/cxy/v2/common/categoryList"; 
		const std::string cGetModelLibraryList = "/api/cxy/v3/model/listCategory"; 
		const std::string cSearchModelList = "/api/cxy/search/model"; 
		const std::string cGetModelGroupInfomation = "/api/cxy/v3/model/modelGroupInfo";
		const std::string cGetSSOToken = "/api/cxy/account/v2/getSSOToken";
		const std::string cGetInfo = "/api/cxy/v2/user/getInfo";
		const std::string cQuickLoginByAccont = "/api/account/quickLogin";
		const std::string cGetVerificationCode = "/api/account/getVerifyCode";
		const std::string cLoginSuccess = "/api/cxy/v2/user/getInfo";
		const std::string cLoginByAccont = "/api/account/loginV2";
		const std::string cAddOrDeleteFavoriteModel = "/api/cxy/v3/model/modelGroupCollection";
		const std::string cGetGCodeList = "/api/cxy/v2/gcode/ownerList";
		const std::string cDeleteGcode = "/api/cxy/v2/gcode/deleteGcode";
		const std::string cGetModelList = "/api/cxy/v3/model/listUpload";
		const std::string cDeleteModel = "/api/cxy/v3/model/modelGroupDelete";
		const std::string cGetFavoritedList = "/api/cxy/v3/model/listCollect";
		//网络参数化
		const std::string cGetProfileList = "/api/cxy/v2/slice-profile/list";
		const std::string cGetPrinterTypeListNewResult = "/api/cxy/v2/device/printerTypeListNew";

	private:
		FuncDeviceListCb funcDeviceListCb = nullptr;
		FuncTaskListCb funcTaskListCb = nullptr;
		bool m_bExit = false;
		std::condition_variable m_condition;
		std::mutex m_mutex;
		time_t tmLastActive = 0;
		cpr::Header getHeader();
		std::string getDUID();
	public:
		std::string getCloudUrl();
		void setFuncDeviceListCb(FuncDeviceListCb func) { funcDeviceListCb = func; }
		void setFuncTaskListCb(FuncTaskListCb func) { funcTaskListCb = func; }
		void refreshStateThread();
		void updateActiveTime(const time_t& tm);
		void getDeviceList(std::function<void(std::string)> call_back);
		void getTaskList(std::function<void(std::string)> call_back);
		void confirmDeviceIdle(const std::string& deviceName, std::function<void(std::string)> call_back);
		void cancelTask(const std::string& deviceName, std::function<void(std::string)> call_back);
		void addTask(const std::string& strContent, std::function<void(bool)> call_back);
		void getGcodeList(const std::string& strContent, std::function<void(std::string)> call_back);
		void getPrintReport(const std::string& strContent, std::function<void(std::string)> call_back);
		void getGcodeSliceTypeListSuffix(std::function<void(std::string)> call_back);
		void setDeviceDesiredProperty_NewIOTControlPrintGcode(const std::string& urlSuffix, const std::string& strContent, std::function<void(std::string)> call_back);

		void startPrintGcode(const std::string& strContent, std::function<void(int, std::string)> call_back);
		void getPrinterTypeList(std::function<void(std::string)> call_back);
		void getOnwerDeviceList(const std::string& strContent, std::function<void(std::string)> call_back);
		void queryDevicePropertyStatus_NewIOT(const std::string& tbid, std::function<void(std::string)> call_back);
		void queryDevicePropertyStatus_NewIOT_RealTime(const std::string& tbid, std::function<void(std::string)> call_back);
		void queryDevicePropertyStatus_NewIOT_Position(const std::string& strContent, const std::string& tbid, std::function<void(std::string)> call_back);
		void queryDevicePropertyStatus(const std::string& productKey, const std::string& deviceName, const std::string& iotId, std::function<void(std::string)> call_back);
		void getDeviceStatus(const std::string& productKey, const std::string& deviceName, const std::string& iotId, std::function<void(std::string)> call_back);
		void setDeviceDesiredProperty_NewIOT(const std::string& strContent, const std::string& tbid, std::function<void(std::string)> call_back);
		void setDeviceDesiredProperty_NewIOTControlPrintTempSpeed(const std::string& strContent, const std::string& tbid, std::function<void(std::string)> call_back);

		//creality cloud
		void getQrLoginInfo(std::function<void(std::string, time_t)> call_back);//回调参数 identical,expireTime
		void startQueryQrLoginInfo(const std::string& strContent, std::function<void(int, std::string, std::string)> call_back);
		void queryQrLoginInfoThread(std::function<void(int, std::string, std::string)> call_back);
		void importMyModel(const std::string& strContent, std::function<void(int)> call_back, std::string downloadFilePath);
		void importDetailItem(const std::string& strContent, std::function<void(int)> call_back, std::string downloadFilePath);
		void getModelGroupDetail(const std::string& strContent, std::function<void(std::string)> call_back);
		void downloadAllModel(const std::string& strContent, std::function<void(int)> call_back, const std::string& filepath);
		void getRecommendModel(const std::string& strContent, std::function<void(std::string)> call_back);
		void getCategoryList(const std::string& strContent, std::function<void(std::string)> call_back);
		void getModelLibraryList(const std::string& strContent, std::function<void(std::string)> call_back);
		void searchModelList(const std::string& strContent, std::function<void(std::string)> call_back); 
		void getModelGroupInfomation(const std::string& strContent, std::function<void(std::string)> call_back);
		void getSSOToken(std::function<void(std::string)> call_back);
		void getInfo(std::function<void(std::string)> call_back);
		void quickLoginByAccont(const std::string& strContent, std::function<void(std::string)> call_back);
		void getVerificationCode(const std::string& strContent, std::function<void(std::string)> call_back);
		void loginSuccess(std::function<void(std::string)> call_back);
		void loginByAccont(const std::string& strContent, std::function<void(std::string)> call_back);

		void uploadGcode(const std::string& strContent, std::function<void(std::string)> call_back);
		void modelGroupCreate(const std::string& strContent, std::function<void(std::string)> call_back);
		//回调参数 bsuccess accessKeyId secretAccessKey sessionToken endpoint fileBucket prefixPath internalBucket internalPrefixPath
		void getAliyunOssKeyAndBucket(std::function<void(bool, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string)> call_back);
		void addOrDeleteFavoriteModel(const std::string& strContent, std::function<void(std::string)> call_back);
		void getGCodeList(const std::string& strContent, std::function<void(std::string)> call_back);
		void deleteGcode(const std::string& strContent, std::function<void(std::string)> call_back);
		void getModelList(const std::string& strContent, std::function<void(std::string)> call_back);
		void deleteModel(const std::string& strContent, std::function<void(std::string)> call_back);
		void getFavoritedList(const std::string& strContent, std::function<void(std::string)> call_back);

		//参数网络化
		void getProfileList(const std::string& printerName, const std::string& downloadPath, std::function<void(std::list<std::string>)> call_back);
		void getPrinterTypeListNewResult(const std::string& strContent, std::function<void(std::string)> call_back);
	private:
		void setQueryQrData(const std::string& strContent);
		std::string getQueryQrData();

	private:
		bool isInited;
		NetworkManagerThread managerThread;
		loginInfo m_loginInfo;
		userInfo m_userInfo;
		std::mutex m_mtxQueryQr;
		bool m_isQueryingQr = false;
		std::string m_queryQrData;
	};

	SINGLETON_EXPORT(BASIC_KERNEL_API, cxNetworkManager)

}

#endif // _cxNetworkManager_H
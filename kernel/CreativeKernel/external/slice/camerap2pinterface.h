#ifndef _CAMERAP2PIF_15916733167626_H
#define _CAMERAP2PIF_15916733167626_H
//#ifdef _PPCS_API___INC_
#include "camerap2pInfo.h"
class Camerap2pInterface
{
public:
	
	int xy_init();
	void* xy_client_thread(void* arg);

	int xy_write(int SessionID, unsigned char Channel, char* DataBuf, int DataSizeToWrite);
	int xy_read(int SessionID, unsigned char Channel, char* DataBuf, int ReadSize);
	int xy_read_n(int SessionID, unsigned char Channel, char* DataBuf, int ReadSize);
	int xy_connect(rtsp_session_t* p_sess);
	xy_para_t* get_xy_para(void);
	void set_xy_para(xy_para_t param);

private:
	const char* getP2PErrorCodeInfo(int err);
	void showNetwork(st_PPCS_NetInfo NetInfo);
	int isLANcmp(const char* IP1, const char* IP2);

private:
	char gThread_Exit = 0;
	char gThread_bRunning = 0;
	char gFlags_WakeupTest = 0;
	xy_para_t g_xy_para;
};




#endif // _CAMERAP2PIF_15916733167626_H

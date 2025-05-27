#define 	_PPCS_API___INC_
#define     TESTER_VERSION  "0.0.4.0"
#define 	PKT_TEST



#define TEST_WRITE_SIZE 		1004  // (251 * 4)
#define TOTAL_WRITE_SIZE 		(110*1024*TEST_WRITE_SIZE)
#define ST_TIME_USED	                       (int)((TimeEnd.TimeTick_mSec) - (TimeBegin.TimeTick_mSec))
#define ST_TIME_USED_MS(TimeBegin, TimeEnd)    (int)((TimeEnd.TimeTick_mSec) - (TimeBegin.TimeTick_mSec))
#define TIME_USED                              (int)((tEnd.TimeTick_mSec) - (tBegin.TimeTick_mSec))
#define TU_MS(tBegin, tEnd)                    (int)((tEnd.TimeTick_mSec) - (tBegin.TimeTick_mSec))

#include "camerap2pinterface.h"

const char* Camerap2pInterface::getP2PErrorCodeInfo(int err)
{
	switch (err)
	{
	case 0: return "ERROR_PPCS_SUCCESSFUL";
	case -1: return "ERROR_PPCS_NOT_INITIALIZED"; // API didn't initialized
	case -2: return "ERROR_PPCS_ALREADY_INITIALIZED";
	case -3: return "ERROR_PPCS_TIME_OUT";
	case -4: return "ERROR_PPCS_INVALID_ID";//Invalid Device ID !!
	case -5: return "ERROR_PPCS_INVALID_PARAMETER";
	case -6: return "ERROR_PPCS_DEVICE_NOT_ONLINE";
	case -7: return "ERROR_PPCS_FAIL_TO_RESOLVE_NAME";
	case -8: return "ERROR_PPCS_INVALID_PREFIX";//Prefix of Device ID is not accepted by Server !!
	case -9: return "ERROR_PPCS_ID_OUT_OF_DATE";
	case -10: return "ERROR_PPCS_NO_RELAY_SERVER_AVAILABLE";
	case -11: return "ERROR_PPCS_INVALID_SESSION_HANDLE";
	case -12: return "ERROR_PPCS_SESSION_CLOSED_REMOTE";
	case -13: return "ERROR_PPCS_SESSION_CLOSED_TIMEOUT";
	case -14: return "ERROR_PPCS_SESSION_CLOSED_CALLED";
	case -15: return "ERROR_PPCS_REMOTE_SITE_BUFFER_FULL";
	case -16: return "ERROR_PPCS_USER_LISTEN_BREAK";//Listen break is called !!
	case -17: return "ERROR_PPCS_MAX_SESSION";//Exceed max session !!
	case -18: return "ERROR_PPCS_UDP_PORT_BIND_FAILED";//The specified UDP port can not be binded !!
	case -19: return "ERROR_PPCS_USER_CONNECT_BREAK";
	case -20: return "ERROR_PPCS_SESSION_CLOSED_INSUFFICIENT_MEMORY";
	case -21: return "ERROR_PPCS_INVALID_APILICENSE";//API License code is not correct !!
	case -22: return "ERROR_PPCS_FAIL_TO_CREATE_THREAD";//Fail to Create Thread !!
	case -23: return "ERROR_PPCS_INVALID_DSK";
	default: return "Unknown, Not the error value of P2P!";
	}
} // getP2PErrorCodeInfo


void Camerap2pInterface::showNetwork(st_PPCS_NetInfo NetInfo)
{
	
	printf("-------------- NetInfo: -------------------\n");
	printf("Internet Reachable     : %s\n", (NetInfo.bFlagInternet == 1) ? "YES" : "NO");
	printf("P2P Server IP resolved : %s\n", (NetInfo.bFlagHostResolved == 1) ? "YES" : "NO");
	printf("P2P Server Hello Ack   : %s\n", (NetInfo.bFlagServerHello == 1) ? "YES" : "NO");
	switch (NetInfo.NAT_Type)
	{
	case 0: printf("Local NAT Type         : Unknow\n"); break;
	case 1: printf("Local NAT Type         : IP-Restricted Cone\n"); break;
	case 2: printf("Local NAT Type         : Port-Restricted Cone\n"); break;
	case 3: printf("Local NAT Type         : Symmetric\n"); break;
	case 4: printf("Local NAT Type         : Different Wan IP Detected!!\n"); break;
	}
	printf("My Wan IP : %s\n", NetInfo.MyWanIP);
	printf("My Lan IP : %s\n", NetInfo.MyLanIP);
	printf("-------------------------------------------\n");
	
}


// -1:invalid parameter,0:not the same LAN Addresses,1:Addresses belonging to the same LAN.
int Camerap2pInterface::isLANcmp(const char* IP1, const char* IP2)
{
#define   YES   1
#define   NO   0
	short Len_IP1 = strlen(IP1);
	short Len_IP2 = strlen(IP2);
	if (!IP1 || 7 > Len_IP1 || !IP2 || 7 > Len_IP2) return -1;
	if (0 == strcmp(IP1, IP2)) return YES;
	const char* pIndex = IP1 + Len_IP1 - 1;
	while (1)
	{
		if ('.' == *pIndex || pIndex == IP1) break;
		else pIndex--;
	}
	if (0 == strncmp(IP1, IP2, (int)(pIndex - IP1))) return YES;

	return NO;
}



xy_para_t* Camerap2pInterface::get_xy_para(void)
{
	return &g_xy_para;
}

void Camerap2pInterface::set_xy_para(xy_para_t param)
{
	g_xy_para = param;
}

int Camerap2pInterface::xy_connect(rtsp_session_t* p_sess)
{
	char DIDString[128];
	INT32 ret = -1;
	st_PPCS_NetInfo NetInfo;
	int SessionID = -99;
	int contimes = 0;

	sprintf(DIDString, "%s", g_xy_para.pDIDString);
	printf("DIDString=%s\n", DIDString);
	

	// 3. Network Detect
	ret = PPCS_NetworkDetect(&NetInfo, 0);
	if (0 > ret) {
		printf("PPCS_NetworkDetect failed: ret=%d\n", ret);
		
	}
	else {
		showNetwork(NetInfo);
		
	}


	// 4. Connect to Device.
	while ((p_sess->run))
	{
		CHAR	bEnableLanSearch = 1;//0x7e;//
		printf("PPCS_Connect(%s, %02x, 0)...%d times\n", DIDString, bEnableLanSearch, ++contimes);
		
		ret = PPCS_Connect(DIDString, bEnableLanSearch/*1*/, 0);
		if (0 > ret)
		{
			printf("Connect failed:[%s]\n", getP2PErrorCodeInfo(ret));

			break;
		}
		else
		{
			p_sess->p2pSessionHandle = SessionID = ret;
			// printf("[at %lu]Connect Success!! %d ms, Session=%d.\n", pthread_self(),TU_MS(t1,t2), SessionID);
			st_PPCS_Session Sinfo;
			ret = PPCS_Check(SessionID, &Sinfo);
			if (ERROR_PPCS_SUCCESSFUL == ret)
			{
                printf("----------- xy_connect success-----------\n");
				
			}
			break;
		}
	} // for

	return ret;
}


int Camerap2pInterface::xy_init()
{
	char InitString[SIZE_INITSTRING];
	INT32 ret = -1;
	int times = 0;

	sprintf(InitString, "%s", g_xy_para.pInitString);
	printf("InitString=%s\n", InitString);
	
	if (strlen(g_xy_para.pP2PKey) > 1) {
		sprintf(&InitString[strlen(InitString)], ":%s", g_xy_para.pP2PKey);
	}


	// 2. P2P Initialize
	while (1) {
		printf("PPCS_Initialize(%s) ...\n", InitString);
		
		ret = PPCS_Initialize((char*)InitString);
		if (ERROR_PPCS_SUCCESSFUL != ret && ERROR_PPCS_ALREADY_INITIALIZED != ret)
		{
			printf("PPCS_Initialize: ret=%d [%s] %d times\n", ret, getP2PErrorCodeInfo(ret), ++times);
			
			my_Sleep(200);
			continue;
		}
		else {
			printf("PPCS_Initialize done! ret=%d\n", ret);
			
			break;
		}
	}

	return ret;
}



#define CHECK_WRTIE_THRESHOLD_LEN ( 256*1024)
int Camerap2pInterface::xy_write(int SessionID, unsigned char Channel, char* DataBuf, int DataSizeToWrite)
{
	INT32 ret = 0;
	INT32 Check_ret = 0;
	ULONG TotalSize = 0;
	UINT32 WriteSize = 0;
	while (1)
	{
		// 在调用 PPCS_Write 之前一定要调用 PPCS_Check_Buffer 检测写缓存还有多少数据尚未发出去，需控制在一个合理范围，一般控制在 128KB/256KB 左右。
		Check_ret = PPCS_Check_Buffer(SessionID, Channel, &WriteSize, NULL);
		//       printf(" PPCS_Check_Buffer: Session=%d,CH=%d,WriteSize=%d,ret=%d %s [%d/%d]\n", SessionID, Channel, WriteSize, Check_ret, getP2PErrorCodeInfo(Check_ret),TotalSize,DataSizeToWrite);
		if (0 > Check_ret)
		{
			printf("  %s\n", getP2PErrorCodeInfo(Check_ret));
			
			break;
		}
		// 写缓存的数据大小超过128KB/256KB，则需考虑延时缓一缓。
		// 如果发现 wsize 越来越大，可能网络状态很差，需要考虑一下丢帧或将码率，这是一个动态调整策略，非常重要!!
		if (WriteSize < CHECK_WRTIE_THRESHOLD_LEN && TotalSize < DataSizeToWrite)
		{
			ret = PPCS_Write(SessionID, Channel, (CHAR*)&DataBuf[TotalSize], DataSizeToWrite);
			if (0 > ret)
			{
				if (ERROR_PPCS_REMOTE_SITE_BUFFER_FULL == ret)
				{
					continue;
				}
				if (ERROR_PPCS_SESSION_CLOSED_TIMEOUT == ret) {
					printf("Session=%d,CH=%d,ret=%d, Session Closed TimeOUT!!\n", SessionID, Channel, ret);
				}
				else if (ERROR_PPCS_SESSION_CLOSED_REMOTE == ret) {
					printf("Session=%d,CH=%d,ret=%d, Session Remote Closed!!\n", SessionID, Channel, ret);
					
				}
				else {
					printf("Session=%d,CH=%d,ret=%d %s\n", SessionID, Channel, ret, getP2PErrorCodeInfo(ret));
					
				}

				break; //errror
			}
			TotalSize += ret; // PPCS_Write return ret >=0: Number of byte wirten.
//		blk_times = 0;
		}
		//When PPCS_Check_Buffer return WriteSize equals 0, all the data in this channel is sent out
		else if (0 == WriteSize /*add*/ || TotalSize == DataSizeToWrite) {
			//			blk_times = 0;
			break;
		}
		else {
			{
				my_Sleep(1);// 2
			}
		}
	}

	return (DataSizeToWrite == TotalSize) ? DataSizeToWrite : -1;
}


int Camerap2pInterface::xy_read(int SessionID, unsigned char Channel, char* DataBuf, int ReadSize)
{
	INT32 ret = -1;
	int timeout = (ReadSize / 130);//1M band upload 130/ms
	int RSize = ReadSize;
	timeout = (timeout > 15) ? timeout : 15;

	ret = PPCS_Read(SessionID, Channel, (char*)DataBuf, &ReadSize, timeout);
	switch (ret) {
	case ERROR_PPCS_SUCCESSFUL:
	case ERROR_PPCS_TIME_OUT:
		return ReadSize;

	case ERROR_PPCS_NOT_INITIALIZED:
	case ERROR_PPCS_INVALID_PARAMETER:
	case ERROR_PPCS_INVALID_SESSION_HANDLE:
	default:
		printf("PPCS_Read: Session=%d,CH=%d,ReadSize=%d/%d,ret=%d [%s] timeout=%d\r\n",
			SessionID, Channel, ReadSize, RSize, ret, getP2PErrorCodeInfo(ret), timeout);
		
		return -1;
	}

}

int Camerap2pInterface::xy_read_n(int SessionID, unsigned char Channel, char* DataBuf, int ReadSize)
{
	INT32 ret = -1;
	int iRlen = 0;
	//	int retry = 0;

	while (iRlen < ReadSize) {
		if (0 < (ret = xy_read(SessionID, Channel, &DataBuf[iRlen], ReadSize - iRlen))) {
			iRlen += ret;
		}
		if (ret < 0) {
			return ret;
		}
	}
	return iRlen;
}

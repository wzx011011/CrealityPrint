#ifndef _CAMERAP2PINFO_H
#define _CAMERAP2PINFO_H
//#ifdef _PPCS_API___INC_

#include "PPCS_API.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#define CH_CMD				0
#define CH_DATA				1
#define SERVER_NUM			3	// WakeUp Server Number
#define SIZE_DID 			64	// Device ID Size
#define SIZE_APILICENSE 	24	// APILicense Size
#define SIZE_INITSTRING 	256	// InitString Size
#define SIZE_WAKEUP_KEY 	17	// WakeUp Key Size
#define NUMBER_OF_P2PCHANNEL    8


#if defined(WIN32DLL) || defined(WINDOWS)
//#pragma comment(lib, ".lib")
#pragma comment(lib,"ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <direct.h> 
#else
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <sched.h>
#include <stdarg.h>
#include <dirent.h>
#include <arpa/inet.h>
#endif
#if defined(WIN32DLL) || defined(WINDOWS)
//#pragma comment(lib, "PPCS_API.lib")
#include <time.h>
typedef     DWORD                   my_threadid;
typedef     HANDLE                  my_Thread_mutex_t;
#define     my_Mutex_Lock(mutex)    WaitForSingleObject(mutex, INFINITE)
#define     my_Mutex_UnLock(mutex)  ReleaseMutex(mutex)
#define     my_Mutex_Close(mutex)   CloseHandle(mutex)
#define     my_SocketClose(skt)     closesocket(skt)
#define     my_Thread_exit(a)       return(a)
#define     SNPRINTF                _snprintf
#define     LocalTime_R(a, b)       localtime_s(b, a)
#define     GmTime_R(a, b)          gmtime_s(b, a)
#define     my_Sleep(a)             Sleep(a)
#else
typedef     pthread_t               my_threadid;
typedef     pthread_mutex_t         my_Thread_mutex_t;
#define     my_Mutex_Lock(mutex)    pthread_mutex_lock(&mutex)
#define     my_Mutex_UnLock(mutex)  pthread_mutex_unlock(&mutex)
#define     my_Mutex_Close(mutex)   pthread_mutex_destroy(&mutex)
#define     my_SocketClose(skt)     close(skt)
#define     my_Thread_exit(a)       pthread_exit(a)
#define     SNPRINTF                snprintf
#define     LocalTime_R(a, b)       localtime_r(a, b)
#define     GmTime_R(a, b)          gmtime_r(a, b)
#define     my_Sleep(a)             sleep(a/1000)
#endif



#if defined(WIN32DLL) || defined(WINDOWS)
typedef     DWORD                   my_threadid;
typedef     HANDLE                  my_Thread_mutex_t;
#define     my_Mutex_Lock(mutex)    WaitForSingleObject(mutex, INFINITE)
#define     my_Mutex_UNLock(mutex)  ReleaseMutex(mutex)
#define     my_Mutex_Close(mutex)   CloseHandle(mutex)
#define     my_SocketClose(skt)     closesocket(skt)
#define     my_Thread_exit(a)       return(a)
#define     SNPRINTF                _snprintf
#define     LocalTime_R(a, b)       localtime_s(b, a)
#define     GmTime_R(a, b)          gmtime_s(b, a)
#else
typedef     pthread_t               my_threadid;
typedef     pthread_mutex_t         my_Thread_mutex_t;
#define     my_Mutex_Lock(mutex)    pthread_mutex_lock(&mutex)
#define     my_Mutex_UnLock(mutex)  pthread_mutex_unlock(&mutex)
#define     my_Mutex_Close(mutex)   pthread_mutex_destroy(&mutex)
#define     my_SocketClose(skt)     close(skt)
#define     my_Thread_exit(a)       pthread_exit(a)
#define     SNPRINTF                snprintf
#define     LocalTime_R(a, b)       localtime_r(a, b)
#define     GmTime_R(a, b)          gmtime_r(a, b)
#endif
typedef	int (*PPCS_Write_fun)(int SessionHandle, unsigned char Channel, char* DataBuf, int DataSizeToWrite);
typedef		int (*do_tcp_write_fun)(unsigned int fd, const char* buf, unsigned int len, unsigned int timeout_ms);
typedef int (*PPCS_ForceClose_fun)(int SessionHandle);

typedef struct {
	int type;
	union {
		struct tcp_ {
			do_tcp_write_fun do_tcp_write;
			int fd;
		}tcp;
		struct p2p_ {
			PPCS_Write_fun PPCS_Write;
			PPCS_ForceClose_fun PPCS_ForceClose;
			int SessionHandle;
			unsigned char Channel;
		}p2p;
	}fun;

}client_art_t;

typedef struct __rtsp_session_t {
	unsigned long long sess_id;         /* Session ID */
	int rtsp_sd;                        /* RTSP socket descriptor */
	char* recv_buf;                     /* Buffer for receiving client data */
	//struct rtp_rtcp rtp_rtcp[2];        /* RTP & RTCP information of video[0] & audio[1] */
	int p2pSessionHandle;
	int run;
	int count;
	int fault_type;/*0:tcp 1:p2p*/
	my_Thread_mutex_t mutex;
	struct timeval init_time;
	time_t frtime;
	int frame_num;

}rtsp_session_t;

typedef struct {
	char pDIDString[SIZE_DID];
	char pAPILicense[SIZE_APILICENSE];
	char pInitString[SIZE_INITSTRING];
	char pCRCKey[SIZE_APILICENSE];
	char pP2PKey[SIZE_APILICENSE];
}xy_para_t;

#endif // _CAMERAP2PINFO_H

#ifndef _GADATAINTERFACE_1592716998224_H
#define _GADATAINTERFACE_1592716998224_H
#include "basickernelexport.h"

//send operation data to Google Analytics
namespace creative_kernel
{
	BASIC_KERNEL_API void setStartFinish(bool start);
	BASIC_KERNEL_API bool sendDataToGA(QString eventType,QString eventAction, QString userID = "");
	BASIC_KERNEL_API bool initSASdk();
}
#endif // _GADATAINTERFACE_1592716998224_H
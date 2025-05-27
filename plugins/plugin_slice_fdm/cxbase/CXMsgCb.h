#pragma once
#include<iostream>

class CXMsgCbBase
{
public:
	CXMsgCbBase() {};
	~CXMsgCbBase() {};

	virtual void SendMsgCb(const std::string& sendMsg,const std::string& sendTime) {};
	virtual void RcvMsgCb(const std::string& rcvMsg,const std::string& rcvTime) {};

private:

};




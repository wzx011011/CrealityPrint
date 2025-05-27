#ifndef CXSW_GCODEHELPER_1603698936550_H
#define CXSW_GCODEHELPER_1603698936550_H
#include "enginewrapperexport.h"
#include <QtCore/QString>
#include <QtGui/QImage>

#include <vector>

//#define SLTHEADLEN 80
struct ImageInfo {
	char comment;//注释
	unsigned char identify[4];//标识位
	unsigned char len[4];//长度
	unsigned char imgType;//图片类型 JPG、PNG
	unsigned char resolution;//分辨率 36*36 48*48 64*64 96*96 144*144 200*200 300*300 400*400 600*600
	unsigned char modelLineS[2];//模型行起始位
	unsigned char modelLineE[2];//模型行结束位
	unsigned char modelHeight[2];//模型高度
};

namespace cxsw
{
	ENGINE_WRAPPER_API bool cxSaveGCode(const QString& fileName, const std::string& previewImageString, 
		const std::vector<std::string>& layerString, const std::vector<std::string>& prefixString, const std::string& tailCode);
	ENGINE_WRAPPER_API bool cxSaveGCode(const QString& fileName, const std::string& previewImageString,
		const std::vector<std::string>& layerString, const std::string& prefixString, const std::string& tailCode,const QImage* qimg=nullptr);

	ENGINE_WRAPPER_API bool cxLoadGCode(const QString& fileName,
		std::vector<std::string>& layerString, std::vector<std::string>& prefixString, std::vector<std::string>& tailCode);

	ENGINE_WRAPPER_API void image2String(const QImage& image, int width, int height, bool clear, std::string& outString);
	ENGINE_WRAPPER_API void changeImageBackcolor(QImage& image, int eWidth, int eHeight);
	void saveAllSTL(QString path);
	bool readFile(const char* path, unsigned int& fileSize, char* & fileBuf);
	ENGINE_WRAPPER_API QImage Hex565Str2Image888(const std::string& imgString, int width, int height);
	ENGINE_WRAPPER_API void writeImageInfo(const QString& fileName, const QImage& image, int eWidth, int eHeight, const QString& sType, int layers);
}

#endif // CXSW_GCODEHELPER_1603698936550_H
#ifndef _ImgProvider_H
#define _ImgProvider_H

#include "basickernelexport.h"
#include <QQuickImageProvider>

class BASIC_KERNEL_API ImgProvider : public QQuickImageProvider
{
public:
	ImgProvider();

public:
	void setImage(const QImage& pimg);
	void setImage(const QImage& pimg, QString printerID);
	QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize);
private:
	QImage img;
	QImage defaultIcon;
	QMap<QString, QImage> imageSourceMap;
};

#endif // !_ImgProvider_H
#ifndef _NULLSPACE_QrCodeImageProvider_15916733167626_H
#define _NULLSPACE_QrCodeImageProvider_15916733167626_H
#include <QtCore/QObject>
#include <QImage>
#include <QPainter>
#include <QQuickImageProvider>
#include "qtusercore/module/job.h"

class LoginJob;
class CrealityCloudUI;
class QrCodeImageProvider : public QQuickImageProvider
{
public:
    QrCodeImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
        m_job = nullptr;
        m_isStop = false;
    }
    QPixmap Url2QrCodeImage(QString urlStr, int imageWidth, int imageHeight);
    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);

    QString providerUrl() { return m_providerUrl; }
    void setProviderUrl(QString url) { m_providerUrl = url; }
    void setparameterUI(CrealityCloudUI* param);
    void setDlgClosed(bool isClosed);

    void setIdentical(QString identical);
    void setExpireTime(QString expireTime);

private:
    QString m_providerUrl;
    CrealityCloudUI* m_parameterUI;
    LoginJob* m_job;
    qtuser_core::JobPtr m_jobPtr;
    bool m_isStop;
    QString m_identical;
    QString m_expireTime;
};


#endif // _NULLSPACE_QrCodeImageProvider_15916733167626_H

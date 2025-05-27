#ifndef _NULLSPACE_INFOPLUGIN_1589979999085_H
#define _NULLSPACE_INFOPLUGIN_1589979999085_H
#include "creativeinterface.h"
#include <QtQml/QQmlComponent>

class InfoDispatch;
class ZSliderInfo;
class InfoPlugin: public QObject, public CreativeInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "creative.InfoPlugin")
	Q_INTERFACES(CreativeInterface)
public:
	InfoPlugin(QObject* parent = nullptr);
	virtual ~InfoPlugin();

	QString name() override;
	QString info() override;

	void initialize() override;
	void uninitialize() override;

protected:
	QObject* m_root;

    QQmlComponent* m_infocomponent;
    QQmlComponent* m_btncomponent;
    QQmlComponent* m_zSlidercomponent;

	QObject* m_infoUI;
    QObject* m_btnUI;
    QObject* m_zSliderUI;

	InfoDispatch* m_infoDispatch;
    ZSliderInfo *m_sliderInfo;

public slots:
	void slotLanguageChanged();
	void slotThemeChanged();

signals:
	void sigLanguageChanged(int langType);

signals:
	void sigThemeChanged(int themeType);

};
#endif // _NULLSPACE_INFOPLUGIN_1589979999085_H

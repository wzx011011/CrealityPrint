#pragma once
#include "creativeinterface.h"
#include "us/slicermanager.h"

class LetterCommand;
class LetterPlugin : public QObject, public CreativeInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "creative.LetterPlugin")
	Q_INTERFACES(CreativeInterface)

public:
	LetterPlugin(QObject* parent = nullptr);
	virtual ~LetterPlugin();

protected:
	QString name() override;
	QString info() override;

	void initialize() override;
	void uninitialize() override;

private slots:
	void slotLanguageChanged();
	void slotThemeChanged();

public slots:
	void visibleLeftBarButton(SlicerManager::SLICERTYPE type);
protected:
	LetterCommand* m_command;
};
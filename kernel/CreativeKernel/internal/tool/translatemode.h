#ifndef _NULLSPACE_TRANSLATEMODE_1589765331924_H
#define _NULLSPACE_TRANSLATEMODE_1589765331924_H
#include "qtuserqml/plugin/toolcommand.h"

class TranslateOp;
class TranslateMode : public ToolCommand
{
	Q_OBJECT
	Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
	Q_PROPERTY(bool message READ message WRITE setMessage NOTIFY supportMessage)
public:
	TranslateMode(QObject* parent = nullptr);
	virtual ~TranslateMode();

	void setMessage(bool isRemove);
	bool message();
	Q_INVOKABLE void accept();
	Q_INVOKABLE void cancel();

	Q_INVOKABLE void execute();
	Q_INVOKABLE void reset();
    Q_INVOKABLE void center();
    Q_INVOKABLE void bottom();
    Q_INVOKABLE bool checkSelect() override;

	QVector3D position();
	void setPosition(QVector3D position);
    bool selected();

    Q_INVOKABLE void blockSignalMoveChanged(bool block);

private  slots:
    void slotLanguageChanged();
	void slotMouseLeftClicked();
	void slotThemeChanged();
signals:
	void positionChanged();
	void supportMessage();
protected:
	TranslateOp* m_translateOp;
};
#endif // _NULLSPACE_TRANSLATEMODE_1589765331924_H

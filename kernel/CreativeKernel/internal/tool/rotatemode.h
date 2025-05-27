#ifndef _NULLSPACE_ROTATEMODE_1589765331924_H
#define _NULLSPACE_ROTATEMODE_1589765331924_H
#include "qtuserqml/plugin/toolcommand.h"

class RotateOp;
class RotateMode: public ToolCommand
{
	Q_OBJECT
	Q_PROPERTY(QVector3D rotate READ rotate WRITE setRotate NOTIFY rotateChanged)
	Q_PROPERTY(bool message READ message WRITE setMessage NOTIFY supportMessage)
public:
	RotateMode(QObject* parent = nullptr);
	virtual ~RotateMode();

	void setMessage(bool isRemove);
	bool message();

	Q_INVOKABLE void execute();
	Q_INVOKABLE void reset();
    Q_INVOKABLE void startRotate();
    Q_INVOKABLE bool checkSelect() override;
	Q_INVOKABLE void accept();
	Q_INVOKABLE void cancel();
    QVector3D rotate();
	void setRotate(QVector3D position);

    Q_INVOKABLE void blockSignalScaleChanged(bool block);
signals:
	void rotateChanged();
	void supportMessage();
private  slots:
    void slotLanguageChanged();
	void slotThemeChanged();
public slots:
	void testMessage();
public slots:
	void slotMouseLeftClicked();

protected:
	RotateOp* m_op;
};
#endif // _NULLSPACE_ROTATEMODE_1589765331924_H

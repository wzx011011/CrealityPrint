#ifndef _NULLSPACE_SPLITCOMMAND_1591235079966_H
#define _NULLSPACE_SPLITCOMMAND_1591235079966_H
#include "qtuserqml/plugin/toolcommand.h"
#include <QVector3D>

class SplitOp;
class SplitCommand: public ToolCommand
{
	Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPositon NOTIFY onPositionChanged)
    Q_PROPERTY(QVector3D dir READ dir WRITE setDir NOTIFY onRotateAngleChanged)
    Q_PROPERTY(QVector3D defauletPos READ defauletPos WRITE setDefauletPos)
    Q_PROPERTY(QVector3D defauletDir READ defauletDir WRITE setDefauletDir)
public:
	SplitCommand(QObject* parent = nullptr);
	virtual ~SplitCommand();

	Q_INVOKABLE void execute();
	Q_INVOKABLE void split();
    Q_INVOKABLE bool checkSelect() override;
    Q_INVOKABLE void undo();//by TCJ

    Q_INVOKABLE void blockSignalSplitChanged(bool block);

    Q_INVOKABLE void accept();
    Q_INVOKABLE void cancel();

    void setMessage(bool isRemove);
    bool message();

private  slots:
    void slotLanguageChanged();
    void slotRotateAngleChanged();
    void slotMouseLeftClicked();
    void slotThemeChanged();
protected:
	SplitOp* m_op;
    QVector3D position();
    QVector3D dir();
    QVector3D defauletPos();
    QVector3D defauletDir();
    QVector3D m_DefauletPos;
    QVector3D m_DefauletDir;
    void setPositon(QVector3D pos);
    void setDir(QVector3D d);
    void setDefauletPos(QVector3D pos);
    void setDefauletDir(QVector3D pos);

signals:
    void onPositionChanged();
    void onDirChanged();
    void onRotateAngleChanged();

    
};
#endif // _NULLSPACE_SPLITCOMMAND_1591235079966_H

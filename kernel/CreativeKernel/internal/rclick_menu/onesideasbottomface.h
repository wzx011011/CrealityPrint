#ifndef ONESIDEASBOTTOMFACE_H
#define ONESIDEASBOTTOMFACE_H

#include <QObject>
#include "menu/actioncommand.h"
#include <QPoint>

class OneSideAsBottomFace:public ActionCommand
{
    Q_OBJECT
public:
    OneSideAsBottomFace();
    ~OneSideAsBottomFace();

    Q_INVOKABLE void execute();
    bool enabled() override;
private slots:
    void slotLanguageChanged();

protected:
    QPoint m_point;
};

#endif // ONESIDEASBOTTOMFACE_H

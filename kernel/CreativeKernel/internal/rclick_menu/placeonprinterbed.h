#ifndef PLACEONPRINTERBED_H
#define PLACEONPRINTERBED_H

#include <QObject>
#include "menu/actioncommand.h"
#include <QPoint>

class TranslateOp;
class PlaceOnPrinterBed:public ActionCommand
{
    Q_OBJECT
public:
    PlaceOnPrinterBed();
    ~PlaceOnPrinterBed();

    Q_INVOKABLE void execute();
    bool enabled() override;
private slots:
    void slotLanguageChanged();
private:
    TranslateOp* m_translateOp;
};

#endif // PLACEONPRINTERBED_H

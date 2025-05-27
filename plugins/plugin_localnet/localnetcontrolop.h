#ifndef _LOCALNETCONTROLOP_H
#define _LOCALNETCONTROLOP_H
#include <QObject>
class LocalNetControlOp : public QObject
{
    Q_OBJECT
public:
    LocalNetControlOp(QObject* parent = nullptr);
    virtual ~LocalNetControlOp();

};

#endif // _LOCALNETCOMMAND_H

#ifndef _MIRRORACTIONCOMMAND_H
#define _MIRRORACTIONCOMMAND_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"
namespace creative_kernel
{
	class ModelN;
}
class MirrorCommand: public ActionCommand
{
	Q_OBJECT
public:
    MirrorCommand(QObject* parent = nullptr);
    MirrorCommand(EMirrorCommand eType,QObject* parent = nullptr);
    virtual ~MirrorCommand();

	Q_INVOKABLE void execute();
    void mirrorX();
    void mirrorY();
    void mirrorZ();
    void reset();

protected:
	creative_kernel::ModelN* get();
    int m_nType = 0;
private slots:
    void slotLanguageChanged();
};
#endif // _MIRRORACTIONCOMMAND_H

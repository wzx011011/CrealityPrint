#ifndef MERGEMODELACTION_H
#define MERGEMODELACTION_H

#include <QObject>
#include <QPoint>
#include "menu/actioncommand.h"
#include "trimesh2/TriMesh.h"

class MergeModelAction:public ActionCommand
{
    Q_OBJECT
public:
    MergeModelAction();
    ~MergeModelAction();

    Q_INVOKABLE void execute();
    bool enabled() override;
private:
    QString m_strFilePath;
private slots:
    void slotLanguageChanged();
};

#endif // MERGEMODELACTION_H

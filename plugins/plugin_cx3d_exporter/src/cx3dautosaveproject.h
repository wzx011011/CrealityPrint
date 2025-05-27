#ifndef CX3DAUTOSAVEPROJECT_H
#define CX3DAUTOSAVEPROJECT_H
#include <QTimer>
#include <QObject>
#include "modelspaceobserver.h"
class  Cx3dAutoSaveProject:public QObject/*,public SpaceTracer*/
{
    Q_OBJECT
public:
    Cx3dAutoSaveProject(QObject* parent = nullptr);
    virtual ~Cx3dAutoSaveProject();
    static Cx3dAutoSaveProject*instance();
    void createOpenProject(QString strFilePath = "", bool bOpen = true);
    void saveProject();
    void setSettingsData(QString file);
    QString getProjectPath();
    void stopTimer();
    void startTimer();

    Q_INVOKABLE void accept();
    Q_INVOKABLE void reject();
//protected :
//    void onBoxChanged(qtuser_3d::Box3D& box) override;
//    void onSceneChanged(qtuser_3d::Box3D& box) override;
public slots:
    void updateTime();
    void updateTmpTime();
    void slotMinutesChanged(float nMinute);

private:
    QTimer *m_timer;
    QTimer* tmp_timer;
    QString m_strFilePath;
    float m_fMinutes;


};

#endif // CX3DAUTOSAVEPROJECT_H

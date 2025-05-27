#ifndef _NULLSPACE_EDITPROFILE_15916733167626_H
#define _NULLSPACE_EDITPROFILE_15916733167626_H
#include <QtCore/QObject>

class Editprofile: public QObject
{
	Q_OBJECT
public:
    Editprofile();
    virtual ~Editprofile();

    void print_quality(int index);
    void settingconfig(QString type, QString name, QString value);
    void settingProfile(QString name, QString value);
    QString getValue(QString key);

    void setqmlUI(QObject* qmlUI);

    void getALLprofile(std::map<QString,QString>& value);

    void getALLprofile_dlp(std::map<QString, QString>& value_basic, std::map<QString, QString>& value_gcode);

    void getALLMachineprofile(std::map<QString,QString>& value);

    void getALLMachineprofile_DLP(std::map<QString,QString>& value);

    void setInfill_sparse_density(QString name, QString value);

    void getProfile_AdvancedSetting_Comprehensive(std::map<QString,QString>& value);

    void getProfile_AdvancedSetting_Shell(std::map<QString,QString>& value);

    void getProfile_AdvancedSetting_Support(std::map<QString,QString>& value);

    void getProfile_AdvancedSetting_Speed(std::map<QString,QString>& value);

    void getProfile_AdvancedSetting_SpecialMode(std::map<QString,QString>& value);

public slots:
    void editProfiled();

protected:


private:
};
#endif // _NULLSPACE_EDITPROFILE_15916733167626_H

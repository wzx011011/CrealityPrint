#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QString>
class Profile:public QObject
{

public:
    Profile();
    ~Profile() {}
    void setName(const QString name) { m_name = (name); }
    QString getName() const { if (m_name == nullptr) return nullptr; else return m_name; }

    QString getDescription()const {return m_description;}

    void setLayerHeight(const QString layer) { m_layerheight = (layer); }
    QString getLayerHeight() const { return m_layerheight; }

    void setQualityName(const QString& qName);
    QString getQualityName() {
        if (m_QualityName != QString())
            return m_QualityName;

        if (m_FullName.contains("high"))
        {
            return "high";
        }
        else if (m_FullName.contains("middle"))
        {
            return "middle";
        }
        else if (m_FullName.contains("low"))
        {
            return "low";
        }
        else if (m_FullName.contains("fast"))
        {
            return "fast";
        }
        else
        {
            return "middle";
        }
    }

    void setMaterialName(const QString& material);
    QString getMaterialName() const;

    QString translateName(QString name);

    QString machine();
    void setMachine(const QString& machine);

    QString fullName()
    {
        return m_FullName;
    }
    void setFullName(const QString& fullName);

    bool isDefaultProfile();
    void setIsDefaultProfile(bool isDefault);

private:
    QString m_FullName = QString();//配置文件全名 

    QString m_MaterialName;//隶属的材料名
    QString m_name = QString();//用户名，默认就是hight、low等
    QString m_QualityName = QString();//质量名

    QString m_machineName = QString();//配置隶属的机器
    bool m_IsDefaultFile = true;//是否默认配置
    QString m_layerheight = QString();//层高

    QString m_description = QString();//描述
};

#endif // PROFILE_H

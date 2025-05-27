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
    QString m_FullName = QString();//�����ļ�ȫ�� 

    QString m_MaterialName;//�����Ĳ�����
    QString m_name = QString();//�û�����Ĭ�Ͼ���hight��low��
    QString m_QualityName = QString();//������

    QString m_machineName = QString();//���������Ļ���
    bool m_IsDefaultFile = true;//�Ƿ�Ĭ������
    QString m_layerheight = QString();//���

    QString m_description = QString();//����
};

#endif // PROFILE_H

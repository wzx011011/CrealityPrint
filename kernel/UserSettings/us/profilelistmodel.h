#ifndef PROFILELISTMODEL_H
#define PROFILELISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "profile.h"
#include<QSettings>
#include <QFileInfo>
#include "usersettingsexport.h"
class USER_SETTINGS_API ProfileListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_PROPERTY(QStringList currentMetrials READ currentMetrials NOTIFY currentMetrialsChanged)

    explicit ProfileListModel(QObject *parent = nullptr);
    //设置当前选中项，返回是否是默认配置文件
    Q_INVOKABLE bool setSelectIndex(int index);
    Q_INVOKABLE QString getProfileLayerHeight(QString name="");
    Q_INVOKABLE void currentMetrial(const QString& curMaterial);
    Q_INVOKABLE void deleteCurFile();
    Q_INVOKABLE bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist = true);
    Q_INVOKABLE QString getCusFileFullName(QString name = QString());
    Q_INVOKABLE QStringList currentMetrials();
    Q_INVOKABLE QStringList profileList();
    void clean();
    void updateLayerHeigh();

    void addProfile(QString material, QString fileName, QString quality);
    Profile * const selectProfile()
    {
        return (m_CurMetrialProfileList.size()> 0 ? (m_selectIndex < m_CurMetrialProfileList.size() ? m_CurMetrialProfileList[m_selectIndex]: m_CurMetrialProfileList[0]):nullptr);
    }
    int rowCount(const QModelIndex& parent = QModelIndex()) const override
       {
        return m_CurMetrialProfileList.size();
       }

       QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
       bool setData(const QModelIndex &index, const QVariant &value, int role) override;
       virtual QHash<int, QByteArray> roleNames() const override;
       void setProfiles(QList<Profile*> list);
       void createProfiles(QFileInfoList files);
       void createMaterials(QStringList files);

   private:
       bool jugeIsDefaultFile(const QString& name);
       QList<Profile*> mData;
       QList<Profile*> m_CurMetrialProfileList;
       int m_selectIndex=0;
private:
    QStringList m_ProfileList;//配置文件名列表
    QString m_CurMaterial;//当前材料名
    QStringList m_CurMaterials;//当前材料列表

signals:
    void sigAddSuccess(int curIndex);
    void currentMetrialsChanged();

public slots:
    
};

#endif // PROFILELISTMODEL_H

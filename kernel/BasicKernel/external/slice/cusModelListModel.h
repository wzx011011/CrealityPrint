#ifndef CUSMODELLISTMODEL_H
#define CUSMODELLISTMODEL_H
#include "basickernelexport.h"
#include <QAbstractListModel>
#include <QObject>

class FileInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isChosed READ isChosed WRITE setIsChosed NOTIFY isChosedChanged)
public:

    QString fileName() const;
    void setFileName(const QString& fileName);

    bool visible() const;
    void setModelVisible(bool visible);

    FileInfo(QObject* parent = nullptr, QObject* item = nullptr);
    FileInfo(const FileInfo& fileInfo);
    ~FileInfo();

    bool isChosed() const;
    void setIsChosed(bool newIsChosed);

signals:
    void fileNameChanged();
    void isChosedChanged();

private:
    QString m_FileName;
    bool m_Visible;
    bool m_IsChosed;
};

class BASIC_KERNEL_API CusModelListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum FileInfoRoles {
        File_Name  = Qt::UserRole + 1,
        File_Visible,
        File_Checked,
        File_Size
    };

    CusModelListModel(QObject* parent = nullptr);
    CusModelListModel(const CusModelListModel& model);

    //add
    void addItem(QObject* item);

    //delete
    bool delItem(QObject* item);
    Q_INVOKABLE void visibleChanged(int index, bool visible);

    //select
    QObject* getItem(int row);
    Q_INVOKABLE int itemCount();
    Q_INVOKABLE bool isCheckedAll();
    Q_INVOKABLE void checkAll();
    Q_INVOKABLE void disCheckAll();
    QModelIndex getRowIndex(QObject* obj);

public slots:
    void slotModelSelectionChanged();
signals:
    void rowChanged(int row);
protected:
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
    void refreshItem(int row, int column); //刷新单项数据
    void addModelData(FileInfo* info);         //新增表数据
    bool delModel(QString modelName);
    void removeRow(int index);
    QList<QObject*> m_Items;
    QList<FileInfo*> m_FileInfoList;
};
#endif // CUSMODELLISTMODEL_H

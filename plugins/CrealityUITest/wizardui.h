#pragma once
#include <QObject>
#include <QVariant>
#include <QRect>
#include <QCursor>
class WizardUI : public QObject
{
    Q_OBJECT
public:
    explicit WizardUI(QObject *parent = nullptr);
    explicit WizardUI(QObject *rootObject, QObject *parent = nullptr);
    virtual ~WizardUI() override;
    Q_INVOKABLE void setRootObjet(QObject *rootObj);// { pRootObject = rootObj; }
    QObject *rootObject() const { return pRootObject; }
    void findRootByNode(QObject *nodeObject);

public slots:
    QObject *getObject(const QString &targetObjName) const;

    QVariant getObjectProperty(QObject *targetObj, const QString &propertyName) const;
    void setObjectProperty(QObject *targetObj, const QString &propertyName,
                           const QVariant &value) const;

    QVariant getObjectProperty(const QString &targetObjName, const QString &propertyName) const;
    void setObjectProperty(const QString &targetObjName, const QString &propertyName,
                           const QVariant &value) const;

    QRect getItemGeometryToScene(const QString &targetObjName) const;

    void setAppOverrideCursor(QCursor cursor);
    void restoreAppOverrideCursor();

    QPoint cursorGlobalPos() const;

private:
    QObject *pRootObject = nullptr;
};

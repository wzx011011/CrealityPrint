#ifndef _creative_kernel_KERNELUI_1589818161757_H
#define _creative_kernel_KERNELUI_1589818161757_H
#include "creativekernelexport.h"
#include "qtusercore/module/singleton.h"
#include <QtCore/QAbstractListModel>
#include <QTranslator>
#include<QUrl>
#include<QString>

#include "kernel/abstractkernel.h"
#include "kernel/appsetting.h"
#include "slice/sliceui.h"
#include "kernel/tablemodel.h"
namespace qtuser_qml
{
	class ToolCommandCenter;
    class TreeModel;
}

class ToolCommand;
class MeshLoaderWrapper;
class CCommandsData;
class CREATIVE_KERNEL_API KernelUI: public creative_kernel::AbstractKernelUI
{
	Q_OBJECT
	Q_PROPERTY(QObject* leftToolbar READ leftToolbar WRITE setLeftToolbar)
    Q_PROPERTY(QObject* footer READ footer WRITE setFooter)
    Q_PROPERTY(QObject* topbar READ topBar WRITE setTopBar)
	Q_PROPERTY(QObject* root READ root WRITE setRoot)
	Q_PROPERTY(QObject* appWindow READ appWindow WRITE setAppWindow)
	Q_PROPERTY(QAbstractListModel* lmodel READ lmodel CONSTANT)

    Q_PROPERTY(QObject* rightPanel READ rightPanel WRITE setRightPanel)
    Q_PROPERTY(QObject* glMainView READ glMainView WRITE setGLMainView)
    //by lisugui 2020/8/25 use to aboutUs menu getVersion
    Q_PROPERTY(QString currentVersion READ currentVersion WRITE setCurrentVersion)

	SINGLETON_DECLARE(KernelUI)
public:
	virtual ~KernelUI();

    void addModel(QObject* model, QObject* group) override;
    void delModel(QObject* model)  override;
    void addGroup(int index, QObject* group)  override;

    void switchPickMode()  override;
    void showAdaptShow(QObject* datafrom)  override;

    void switchPopupMode()  override;
	//lisugui 2021-1-30
	void switchShowPop(bool bVis = false)override;
	void refreshPickPanel() override;
	
    void executeCommand(QString cmdname, QObject* receiver, QString menuObjName) override;

    QObject* getUI(QString uiname) override;

    int addToolCommand(ToolCommand* command, QString key, int index = 0) override;
    int removeToolCommand(ToolCommand* command, QString key) override;

    int addActionCommand(ActionCommand* command, QString key) override;
    int removeActionlCommand(ActionCommand* command, QString key) override;

    qtuser_qml::TreeModel* treeModel() override;
    TableModel* tableModel()override;
    creative_kernel::AbstractAppSetting* getAppSettings() override;

    BasicSliceUI* createSliceUI() override;
    CusModelListModel* listModel() override;
    void registerEvent();

	QObject* leftToolbar();
	void setLeftToolbar(QObject* leftToolbar);
    QObject* footer();
    void setFooter(QObject* footer);
    QObject* topBar();
    void setTopBar(QObject* topbar);
	QObject* root();
	void setRoot(QObject* root);

    QObject* appWindow();
    void setAppWindow(QObject* appWindow);

    QObject* rightPanel();
    void setRightPanel(QObject* rightPanel);

    QObject* glMainView();
    void setGLMainView(QObject* glMainView);

    bool isSupportBtnChecked();
    //by lisugui 2020/8/25 use to aboutUs menu getVersion
    QString currentVersion();

    Q_INVOKABLE void setCurrentVersion(QString version);
    Q_INVOKABLE void openFileWithDrop(QList<QUrl> urls);

	QAbstractListModel* lmodel();

	qtuser_qml::ToolCommandCenter* lCenter();

	void visibleAll(bool visible);

	void requestOpenFileDialog(QObject* receiver);

    void requestMenuDialog(QObject* receiver,QString menuObjName);

    MeshLoaderWrapper *getMeshLoaderWrapper()const;

    Q_INVOKABLE void initLanguage();
    void requestUpdateLanguage(QString value, int languageType);
    void requestCloseAction();

    //lisugui 2020-10-30 qml closewindow，signal closewindow to closaCmd function
    Q_INVOKABLE void beforeCloseWindow();
    //end
    void changeTheme(int themeType);
    void changeMenuTheme();

public slots:
    void slotMeshLoadFiltersChanged(QStringList newExtensions);

signals:
    void sigChangeThemeColor();
    void sigLeftToolBarVisible(bool isVisible);


protected:
	QObject* m_root;
	QObject* m_appWindow;
	QObject* m_leftToolbar;
    QObject* m_footer;
    QObject* m_topbar;
    QObject* m_rightPanel;
    QObject* m_glMainView;
    //by lisugui 2020/8/25 use to aboutUs menu getVersion
    QString m_currentVersion = "";

	//left toolbar model list
	qtuser_qml::ToolCommandCenter* m_leftToolbarModelList;
	//top toolbar model list
    qtuser_qml::TreeModel* m_modelList;
    CusModelListModel* m_ModelList = nullptr;
    TableModel* m_tableModelList;
	MeshLoaderWrapper* m_meshLoaderWrapper;

    QStringList m_meshLoadTypes;

    creative_kernel::AppSetting* m_appSettings;

    SliceUI* m_sliceUI;

private:
    QTranslator *m_translator;
    bool m_isSupportChecked = false;
}; 

SINGLETON_EXPORT(CREATIVE_KERNEL_API, KernelUI)
#endif // _creative_kernel_KERNELUI_1589818161757_H

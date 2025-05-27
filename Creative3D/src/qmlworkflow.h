#ifndef QML_WORKFLOW_H
#define QML_WORKFLOW_H
#include <QtCore/QObject>
#include "qtuserqml/updater/UpdateManager.h"

class GLQuickItem;
namespace creative_kernel
{
	class Kernel;
	class RenderCenter;
}
class KernelUI;

class CXXGlobal;
class CreativePluginCenter;
class QMLWorkflow : public QObject
{
	Q_OBJECT
	Q_PROPERTY(GLQuickItem* glQuickItem READ glQuickItem WRITE setGLQuickItem)
	Q_PROPERTY(KernelUI* kernelUI READ kernelUI CONSTANT)
	Q_PROPERTY(QObject* tips READ tips CONSTANT)
	Q_PROPERTY(QObject* jobs READ jobs CONSTANT)
    Q_PROPERTY(QObject* updateManager READ updateManager CONSTANT)
public:
	QMLWorkflow(QObject* parent = nullptr);
	virtual ~QMLWorkflow();

	Q_INVOKABLE void initialize();
	Q_INVOKABLE bool checkUnsavedChanges();
	Q_INVOKABLE void uninitialize();
    Q_INVOKABLE QObject* getPluginByName(QString name);
	GLQuickItem* glQuickItem();
	void setGLQuickItem(GLQuickItem* quickItem);


	////when init over some auto task can run here
	Q_INVOKABLE void autoTask();

	KernelUI* kernelUI();
	QObject* tips();
	QObject* jobs();
    QObject* updateManager();

	Q_INVOKABLE void loadOver();
	Q_INVOKABLE void startWizard();

private:
	void __initKernelData();


private:
	CreativePluginCenter* m_creativePluginCenter;

	creative_kernel::Kernel* m_kernel;
	KernelUI* m_kernelUI;
	creative_kernel::RenderCenter* m_renderCenter;

    UpdateManager* m_pUpdateManager;
	CXXGlobal* m_cxxGlobal;

private  slots:
	void slotThemeChanged();
signals:
	void sigInitAdvanceParam();
};
#endif // QML_WORKFLOW_H

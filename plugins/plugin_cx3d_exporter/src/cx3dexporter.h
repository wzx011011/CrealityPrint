#ifndef _NULLSPACE_CX3D_1589849922902_H
#define _NULLSPACE_CX3D_1589849922902_H
#include "creativeinterface.h"
#include <QtCore/QUrl>

#include "inout/cxopenandsavefilemanager.h"

class CX3DImporterCommand;
class CX3DExporterCommand;
class CX3DExporter: public QObject, public CreativeInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "creative.CX3DExporter")
	Q_INTERFACES(CreativeInterface)
public:
    CX3DExporter(QObject* parent = nullptr);
     ~CX3DExporter();

    void setOpenedProjectPath(const QUrl& url);
	QUrl openedPath();
protected:
	QString name() override;
	QString info() override;

	void initialize() override;
	void uninitialize() override;
    void openDefualtProject();
public slots:
    void update();
protected:
	CX3DExporterCommand* m_exportcommand;
	CX3DImporterCommand* m_importcommand;
    QUrl m_openedUrl;
};
#endif // _NULLSPACE_CX3D_1589849922902_H

#ifndef _US_MODELINPUT_1589340410146_H
#define _US_MODELINPUT_1589340410146_H
#include "usersettingsexport.h"
#include "us/modelsetting.h"

namespace us
{
	class USER_SETTINGS_API ModelInput: public QObject
	{
	public:
		ModelInput(QObject* parent = nullptr);
		virtual ~ModelInput();

		virtual ModelSetting* modelSetting()=0;
		virtual const ModelSetting* modelSetting() const=0;

		int id() const;
		void setID(int id);

		const QString& name() const;
		void setName(const QString& name);

		virtual int vertexNum() const;
		virtual float* position() const;
		virtual float* normal() const;
		virtual float* offset() const;

		virtual int triangleNum() const;
		virtual int* indices() const;
		virtual void simplifyModel();
	protected:
		int m_id;
		QString m_name;
	};
}
#endif // _US_MODELINPUT_1589340410146_H

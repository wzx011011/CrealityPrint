#include "us/usettingsserializer.h"
#include "us/usettings.h"
#include <QtCore/QFile>
#include "qtusercore/string/stringtool.h"
namespace us
{
	USettingsSerializer::USettingsSerializer(QObject* parent)
		:QObject(parent)
	{
	}
	USettingsSerializer::~USettingsSerializer()
	{
	}

	void USettingsSerializer::load(USettings* settings, const QString& file)
	{
        std::vector<char*> vctDest;
        char* ch;
        QByteArray ba = file.toLatin1(); // must
        ch = ba.data();
        vctDest= qtuser_core::split_z(ch, ".");
        std::string str = vctDest.back();
        if (str =="ini" || str == "INI")
        {
            QFile qfile(file);
            bool opened = qfile.open(QIODevice::ReadOnly);
            QByteArray barray;
            if (opened == true)
            {
                char buffer[1];
                qint64 length = 0;
                int icount = 0;
                QByteArray b;
                QString qstrkey;
                QString qTemp;
                QString qstrvalue;
                while ((length = qfile.read(buffer, 1) != 0))
                {
                    if (buffer[0] != '\"' && buffer[0] != '=' && icount > 0)
                    {
                        qTemp += buffer[0];
                    }
                    if (buffer[0] == '\"')
                    {
                        icount++;
                    }
                    if (icount == 2 && buffer[0] == '=')
                    {
                        qstrkey = qTemp;
                        qTemp.clear();
                    }
                    if (4 == icount)
                    {
                        qstrvalue = qTemp;
                        icount = 0;
                        qTemp.clear();
                        //settings->insert(qstrkey, qstrvalue);
                    }
                }
            }
            qfile.close();
        }
        else if (str == "JSON" || str == "json")
        {

        }

        
	}

	void USettingsSerializer::loadFromIni()
	{

	}
}

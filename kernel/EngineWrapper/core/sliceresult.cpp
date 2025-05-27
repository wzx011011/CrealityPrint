#include "sliceresult.h"
#include "core/gcodehelper.h"
#include<QDebug>
#include<QStandardPaths>
#include<QDir>


#ifdef WIN32
#include <windows.h>
#endif

#include <random>

namespace cxsw
{
    SliceResult::SliceResult() 
        : m_strpath("")
        , m_outfile("")
    {
//#ifdef WIN32
//        char	strTmpPath[1024] = { 0 };
//        GetTempPath(sizeof(strTmpPath), strTmpPath);
//#else
//        char strTmpPath[] = "/tmp/";
//#endif
        QString binPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +"/tmp/";
         QDir tmpdir;
        if (!tmpdir.exists(binPath))
        {
            tmpdir.mkdir(binPath);
        }
         //std::default_random_engine random(time_t(NULL));
         //std::uniform_int_distribution<int> dis1(0);
         //binPath += QString::fromLocal8Bit(std::to_string(dis1(random)).c_str());
        binPath += "tmpgcodefile";
        QFile::remove(binPath);
        m_outfile.setFileName(binPath);

        if (!m_outfile.open(QIODevice::WriteOnly))
        {
            qDebug()<<"SliceResult::SliceResult() open gcode file failed ,m_outfile open failed,filePath=%s"<<m_strpath.c_str();
            return;
        }
        m_strpath = binPath.toLocal8Bit().data();
        qDebug() << "SliceResult::SliceResult() open gcode tmp file : " << m_strpath.c_str();
    }

    SliceResult::~SliceResult()
    {
    }

    std::string SliceResult::GCodeStdString()
    {
        std::string gcode;
        //load();
        for (auto val : m_data_gcodelayer)
        {
            gcode += val;
        }

        return gcode;
    }

    void SliceResult::SaveGCode(const QString& file_name, const std::string& previewImageStr)
    {
        cxsw::cxSaveGCode(file_name, previewImageStr, m_data_gcodelayer, m_gcodePrefix, m_gcodeTail);
        //cxsw::cxSaveGCode(file_name, previewImageStr, m_data_gcodeprefix, m_data_gcodelayer, m_data_gcodetail);
    }

    void SliceResult::SaveDLPGCode(const QString& file_name)
    {

    }

    void SliceResult::clear()
    {
        m_amount.clear();
        m_data_gcodelayer.clear();
        m_data_gcodeprefix.clear();
    }

    void SliceResult::save(const std::string& content)
    {    
        if (!content.empty())
        {
            m_outfile.write(content.c_str(), content.length());
        }
    }

    void SliceResult::close()
    {
        m_outfile.close();
    }

	void SliceResult::setPathName(const std::string path)
	{
        m_strpath = path;
	}

    std::string SliceResult::pathName()
    {
        return m_strpath;
    }

	void SliceResult::load(const std::string strpath)
    {
        if (!strpath.empty())
        {
            m_strpath = strpath;
        }

        QString fileName = QString::fromLocal8Bit(m_strpath.data());
        cxsw::cxLoadGCode(fileName, m_data_gcodelayer, m_data_gcodeprefix,m_data_gcodetail);

        if (m_data_gcodeprefix.size() > 0)
        {
            setGCodePrefix(m_data_gcodeprefix.at(0));
        }
        if (m_data_gcodetail.size() > 0)
        {
            setGCodeTail(m_data_gcodetail.at(0));
        }
    }
}

#ifndef SLICE_RESULT_H
#define SLICE_RESULT_H
#include "enginewrapperexport.h"
#include "slicemodel.h"
#include <QtCore/QFile>

namespace cxsw
{
    class ENGINE_WRAPPER_API SliceResult
    {
    public:
        SliceResult();
        ~SliceResult();

        void SaveGCode(const QString& file_name, const std::string& previewImageStr);
        void SaveDLPGCode(const QString& file_name);
        std::string GCodeStdString();
        std::vector<float> m_amount;//Progress
        std::vector<std::string> m_data_gcodelayer;//GCodeLayer
        std::vector<std::string> m_data_gcodeprefix; //GCodePrefix.
        
        std::vector<std::string> m_data_gcodetail; //GCodetail.
        SliceModel m_sliceModel;

        void clear();

        void save(const std::string& content);
        void load(const std::string strpath = "");
        void close();
        void setPathName(const std::string path);//for gcode preview
        std::string pathName();

        void setGCodePrefix(std::string prefix)
        { 
            m_gcodePrefix = prefix; 
        }
        void setGCodeTail(std::string tail)
        {
            m_gcodeTail = tail;
        }
        std::string getGCodePrefix() { return m_gcodePrefix; }
        std::string getGCodeTailfix() { return m_gcodeTail; }

    private:
        std::string m_strpath;
        QFile m_outfile;
        std::string m_gcodePrefix = "";
        std::string m_gcodeTail = "";
    };
}
#endif // SLICE_RESULT_H

#include "wrapperoutput.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>
#include "core/meshdatalist.h"
#include "core/meshdata.h"
#include "slice/trimeshinput.h"
#include "def/settingdef.h"
#include <map>
#include <random>
#include "trimesh2/TriMesh.h"
#include "trimesh2/Vec.h"
#include "slicemodel.h"
#include "slicemodelbuilder.h"
#include "layerlines.h"
#include "core/sliceresult.h"
#include "qtuser3d/math/box3d.h"
#include "sliceline.h"
#include"us/configurationmanager.h"
//#include "main/sliceattain.h"
#include <mutex>

#ifdef USE_ARCUS
#include "arcus/cloudslicer.h"

namespace cxsw
{
    class SliceWrapper;
    class SliceResult;
}
using namespace cxsw;

std::map<int, CloudSlicer*> g_all_wrapper;
std::mutex destroyMutex;
std::mutex jobMutex;
std::mutex settingMutex;
//std::mutex settingvalueMutex;
std::mutex openfileMutex;
#endif 

//typedef struct settingstu
//
//{
//    int job_id;
//    int modify_type;
//    QString key;
//    QString value;
//    int mesh_id;
//}setting_stu;
//std::vector<setting_stu> g_modify_setting;
//
////#ifdef ENGINE_WRAPPER_DLL
//namespace us
//{
//    class cinitialize
//    {
//    public:
//        cinitialize(){
//            QString configRoot = ConfigurationManager::instance().getConfigRoot();
//            us::SettingDef::instance().initialize(configRoot+"/default/fdm.def.json");
//        };
//        ~cinitialize() {};
//    };
//}
//us::cinitialize init;

int prepare_slice_job()//return job_id
{
    int job_id = -1;   

#ifdef USE_ARCUS
    //g_modify_setting.clear();
    int icount = 0;
    //SliceWrapper* slicewrapper = NULL;
    CloudSlicer* arcusslice = nullptr;
    while (1)
    {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        unsigned long count=(t2 - t1).count();//50545100

        if (count> 0)
        {
            job_id = 40000 + count%20001;
        }
        else
        {
            std::default_random_engine random(time(NULL));
            std::uniform_int_distribution<int> dis1(40000, 60000);
            job_id = dis1(random);
        }

        std::lock_guard<std::mutex> lock(jobMutex);
        auto jobvalue = g_all_wrapper.find(job_id);
        if (jobvalue == g_all_wrapper.end())
        {
            arcusslice = new CloudSlicer();
            g_all_wrapper.insert(std::make_pair(job_id, arcusslice));
            set_value(job_id, ModelSetting, "center_object", "true");
            icount++;
            break;
        }
        if (20000 == icount)
        {
            //full used
            job_id = -1;
            break;
        }
    }

#endif
    return job_id;
}

bool add_mesh(int job_id, const std::string& stl_path, float matrixs[16])//add mesh to job id
{
    if (-1 == job_id)
    {
        return false;
    }

#ifdef USE_ARCUS
    auto iterFind = g_all_wrapper.find(job_id);
    if (iterFind != g_all_wrapper.end())
    {
        CloudSlicer* pSlice = iterFind->second;
        trimesh::fxform trimatrix;
        for (int i = 0; i < 16; i++)
        {
            trimatrix[i] = matrixs[i];
        }

        trimesh::transpose(trimatrix);
        pSlice->AddMesh(job_id, stl_path, trimatrix);
    }
    else {
        return false;
    }
#endif
    return true;
}

#ifdef USE_ARCUS
void dealmodify(int job_id, std::shared_ptr<us::GlobalSetting> _globalSettings, std::shared_ptr<us::ModelListSetting>_modellistsetting, std::shared_ptr<us::ModelSetting>_modelsettings, std::shared_ptr<us::ExtruderSetting>_extrudersettings)
{
    std::lock_guard<std::mutex> lock(settingMutex);
    for (auto _value : g_modify_setting)
    {
        if (job_id != _value.job_id)
        {
            continue;
        }

		auto vh = _modellistsetting->settings().find(_value.key);
		if (vh != _modellistsetting->settings().end())
		{
			us::USetting* val = vh.value();
			val->setValue(_value.value);
		}

		auto vh_1 = _modelsettings->settings().find(_value.key);
		if (vh_1 != _modelsettings->settings().end())
		{
			us::USetting* val = vh_1.value();
			val->setValue(_value.value);
		}

		auto vh_2 = _extrudersettings->settings().find(_value.key);
		if (vh_2 != _extrudersettings->settings().end())
		{
			us::USetting* val = vh_2.value();
			val->setValue(_value.value);
		}

		auto vh_3 = _globalSettings->settings().find(_value.key);
		if (vh_3 != _globalSettings->settings().end())
		{
            us::USetting* val= vh_3.value();
            val->setValue(_value.value);
		}
    }
}

void get_data(stu_slice_result& _stu_slice_result, std::string& strdata_gcodeprefix)
{
    size_t ipos = strdata_gcodeprefix.find("\n");
    size_t _ipos = 0;
    double x1 = 0;
    double x2 = 0;
    double y1 = 0;
    double y2 = 0;
    double z1 = 0;
    double z2 = 0;
    while (ipos > 0 && ipos< strdata_gcodeprefix.length())
    {
        std::string strtemp = strdata_gcodeprefix.substr(0, ipos);
        strdata_gcodeprefix = strdata_gcodeprefix.substr(ipos+1, strdata_gcodeprefix.length());
        ipos = strdata_gcodeprefix.find("\n");

        _ipos = strtemp.find("TIME:");
        if (_ipos > 0 && _ipos < strtemp.length())
        {
            _stu_slice_result.print_time = std::stol(strtemp.substr(_ipos+5, strtemp.length()));
            continue;
        }
        _ipos = strtemp.find("Filament used:");
        if (_ipos > 0 && _ipos < strtemp.length())
        {
            _stu_slice_result.filament_len = std::stod(strtemp.substr(_ipos+14, strtemp.length()-1));
            continue;
        }
        _ipos = strtemp.find("MINX:");
        if (_ipos > 0 && _ipos < strtemp.length())
        {
            x1 = std::stod(strtemp.substr(_ipos+5, strtemp.length()));
            continue;
        }
        _ipos = strtemp.find("MAXX:");
        if (_ipos > 0 && _ipos < strtemp.length())
        {
            x2 = std::stod(strtemp.substr(_ipos+5, strtemp.length()));
            _stu_slice_result.x = x2 - x1;
            continue;
        }
        _ipos = strtemp.find("MINY:");
        if (_ipos > 0 && _ipos < strtemp.length())
        {
            y1 = std::stod(strtemp.substr(_ipos + 5, strtemp.length()));
            continue;
        }
        _ipos = strtemp.find("MAXY:");
        if (_ipos > 0 && _ipos < strtemp.length())
        {
            y2 = std::stod(strtemp.substr(_ipos + 5, strtemp.length()));
            _stu_slice_result.y = y2 - y1;
            continue;
        }
        _ipos = strtemp.find("MINZ:");
        if (_ipos > 0 && _ipos < strtemp.length())
        {
            z1 = std::stod(strtemp.substr(_ipos + 5, strtemp.length()));
            continue;
        }
        _ipos = strtemp.find("MAXZ:");
        if (_ipos > 0 && _ipos < strtemp.length())
        {
            z2 = std::stod(strtemp.substr(_ipos + 5, strtemp.length()));
            _stu_slice_result.z = z2 - z1;
            continue;
        }

    }
}

#endif

void slice(stu_slice_result& _stu_slice_result, int job_id,
                              const std::string& gcode_path, const std::string& engine_path, const std::string & engine_output_path,
                              const std::string& conversion_stl) //start slice job
{
    if (-1 == job_id)
    {
        return ;
    }

#ifdef USE_ARCUS
    auto iterFind = g_all_wrapper.find(job_id);
    if (iterFind != g_all_wrapper.end())
    {
        CloudSlicer* pslice = iterFind->second;
        std::shared_ptr<SliceResult>result;

        std::shared_ptr<us::GlobalSetting> _globalsettings(new us::GlobalSetting);
        if (nullptr == _globalsettings)
        {return;}
        _globalsettings->loadDefault(":/us/default/globalsettings_cloud.default");

        std::shared_ptr<us::ExtruderSetting> _extrudersettings(new us::ExtruderSetting);
        if (nullptr == _extrudersettings)
        {return;}

        std::shared_ptr<us::ModelListSetting> _modellistsettings(new us::ModelListSetting);
        if (nullptr == _modellistsettings)
        {return;}
        _modellistsettings->loadDefault(":/us/default/modellistsettings.default");

        std::shared_ptr<us::ModelSetting> _modelsettings(new us::ModelSetting);
        if (nullptr == _modellistsettings)
        { return;}
        _modelsettings->loadDefault(":/us/default/modelsettings.default");

        QList<const us::ModelGroupInput*> modelgroupinputs;
        us::ModelGroupInput* modelgroupinput = new us::ModelGroupInput();
        if (nullptr == modelgroupinput)
        {return ;}

        modelgroupinputs.push_back(modelgroupinput);
        std::vector<std::shared_ptr<MeshData>> _meshdatalist = pslice->getmeshdatalist()->GetMesheData();
        for (size_t i = 0; i < _meshdatalist.size(); i++)
        {
            std::shared_ptr<trimesh::TriMesh>_mesh =_meshdatalist[i]->GetMesh();
            TrimeshInput* meshInput = new TrimeshInput(modelgroupinput);
            if (nullptr == meshInput)
            {break ;}

            ((us::ModelInput*)meshInput)->setID(i++);
            ((us::ModelInput*)meshInput)->setName("");
            meshInput->setMesh(_mesh);
            if (!conversion_stl.empty())
            {
                _mesh->write(conversion_stl);
            }
            modelgroupinput->addModelInput(meshInput);
        }

        if (modelgroupinputs.size() > 0)
        {
            dealmodify(job_id,_globalsettings, _modellistsettings, _modelsettings, _extrudersettings);
            result = pslice->slice(modelgroupinputs, _globalsettings, _extrudersettings, _modellistsettings, _modelsettings, engine_path, engine_output_path);
        }

        for (auto _modelgroupinputs : modelgroupinputs)
        {
            delete _modelgroupinputs;
        }
        modelgroupinputs.clear();

        if (result != NULL)
        {
            if (result->m_data_gcodeprefix.size())
            {
                std::string strdata_gcodeprefix = result->m_data_gcodeprefix[0];
                get_data(_stu_slice_result, strdata_gcodeprefix);
            }
            result->SaveGCode(QString::fromStdString(gcode_path));
            _stu_slice_result.layer_count = result->m_data_gcodelayer.size()-3;           
        }

        std::lock_guard<std::mutex> lock(settingMutex);
        auto iter = g_modify_setting.begin();
        while (iter != g_modify_setting.end())
        {
            if (job_id == iter->job_id)
            {
                iter = g_modify_setting.erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }

#endif
    return ;

}

ENGINE_WRAPPER_API bool load_settings_file(int job_id, const std::string& fileName)
{
#ifdef USE_ARCUS
    std::lock_guard<std::mutex> lock(openfileMutex);
    QFile file(QString::fromStdString(fileName));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QString line = file.readLine();
            QStringList lists = line.split("=");
            if (lists.size() == 1)
            {
                //vctsetting.insert(std::make_pair(lists.at(0).trimmed(),""));
                file.close();
                return false;
            }
            else
            {
                set_value(job_id, GlobalSetting, lists.at(0).trimmed().toStdString(), lists.at(1).trimmed().toStdString());
            }
        }
    }
    else
    {
        return false;
    }
    file.close();
#endif
    return true;
}


ENGINE_WRAPPER_API void Gcode2Stl(cxsw::SliceResult* result, const std::string& file_name)
{
#ifdef USE_ARCUS
    SliceModel sliceModel;
    SliceModelBuilder::buildSliceModel(&sliceModel, result);

    int layers = (int)sliceModel.m_sliceLayers.size();
    if (layers == 0)
        return;

    std::vector<LayerPolyLine> slicePolys;
    int count = 0;
    float layer = 0.0f;
    //qtuser_3d::Box3D box;

    unsigned r = 4;

    for (const SliceLayer& sliceLayer : sliceModel.m_sliceLayers)
    {
        float step = 0.0f;

        slicePolys.push_back(LayerPolyLine());

        LayerPolyLine& lpl = slicePolys.back();
        lpl.m_layer = layer;
        lpl.m_thickness = sliceLayer.thickness;
        lpl.m_lineWidth = lpl.m_thickness;

        for (const SliceLine& sliceLine : sliceLayer.m_sliceLines)
        {
            if (sliceLine.type != SliceLineType::NoneType)
            {
                lpl.addLine(sliceLine, step);

                ++count;
                //box += sliceLine.start;
                //box += sliceLine.end;

                step += 1.0f;
            }
        }
        layer += 1.0f;

        lpl.extentedNPoly(r);
    }

    unsigned vertexNum = count * 2 * r;
    std::vector<trimesh::vec3> positions;
    positions.reserve(vertexNum);
    unsigned triangleNum = count * 3 * r;
    std::vector<trimesh::uivec3> indices;
    indices.reserve(triangleNum);

    unsigned vertexIndex = 0;

    float step = 0;

    for (LayerPolyLine& lpl : slicePolys)
    {
        for (int l = 0; l < lpl.m_lineAttributes.size(); l++)
        {
            std::vector< SliceLineAttributes>& slaSet = lpl.m_lineAttributes[l];

            for (int k = 0; k < slaSet.size(); k++)
                //for (SliceLineAttributes& sla : lpl.m_lineAttributes)
            {
                SliceLineAttributes& sla = slaSet[k];

                LinePoint* start = nullptr, * end = nullptr;
                lpl.getLineStartEnd(sla, start, end, l);

                trimesh::vec3* normal;
                trimesh::vec3* startPosition = nullptr, * endPosition = nullptr;

                lpl.getPointPositon(sla, startPosition, endPosition, l);
                lpl.getLineNormal(k, normal, l);

                for (int i = 0; i < r; i++)
                {
                    positions.push_back(*startPosition++);
                    positions.push_back(*endPosition++);
                }
                step = sla.step;

                for (unsigned i = 0; i < r; ++i)
                {
                    indices.push_back(trimesh::uivec3(vertexIndex + 2 * i, vertexIndex + 2 * i + 1, vertexIndex + ((2 * i + 3) % (2 * r))));
                    indices.push_back(trimesh::uivec3(vertexIndex + 2 * i, vertexIndex + ((2 * i + 3) % (2 * r)), vertexIndex + ((2 * i + 2) % (2 * r))));
                }

                if (start->m_type == LinePointType::StartPoint)
                {
                    indices.push_back(trimesh::uivec3(vertexIndex, vertexIndex + 2, vertexIndex + 4));
                    indices.push_back(trimesh::uivec3(vertexIndex, vertexIndex + 4, vertexIndex + 6));
                }
                if (end->m_type == LinePointType::EndPoint)
                {
                    indices.push_back(trimesh::uivec3(vertexIndex + 1, vertexIndex + 3, vertexIndex + 5));
                    indices.push_back(trimesh::uivec3(vertexIndex + 1, vertexIndex + 5, vertexIndex + 7));
                }

                vertexIndex += 2 * r;
            }

        }
        lpl.clearAll();
    }

    int vertexSize = positions.size();
    int triangleSize = indices.size();
    if (vertexSize && triangleSize)
    {
        trimesh::TriMesh* mesh = new trimesh::TriMesh();
        mesh->vertices.resize(vertexSize);
        mesh->faces.resize(triangleSize);

        memcpy(&mesh->vertices.at(0), &positions.at(0), 3 * vertexSize * sizeof(float));
        memcpy(&mesh->faces.at(0), &indices.at(0), 3 * triangleSize * sizeof(int));

        for (size_t i = 0; i < mesh->faces.size(); i++)
        {
            int temp = mesh->faces[i].at(1);
            mesh->faces[i].at(1) = mesh->faces[i].at(2);
            mesh->faces[i].at(2) = temp;
        }
        mesh->write(file_name);
        delete mesh;
    }
#endif
}

ENGINE_WRAPPER_API bool Gcode2Stl(const std::string& stl_path, const std::string& gcode_path)
{
    cxsw::SliceResult* result = new cxsw::SliceResult;
    result->load(gcode_path);
    if (result->m_data_gcodelayer.size())
    {
        Gcode2Stl(result, stl_path);
        return true;
    }
    return false;
}



ENGINE_WRAPPER_API bool set_value(int job_id,int modify_type, const std::string& key, const std::string& value, int mesh_id)
{
#ifdef USE_ARCUS
    std::lock_guard<std::mutex> lock(settingMutex);
    for (int i=0;i< g_modify_setting.size();i++)
    {
		if (job_id == g_modify_setting[i].job_id
			&& key == g_modify_setting[i].key.toStdString())
		{
            g_modify_setting[i].value = QString::fromStdString(value);
			return true;
		}
    }

    setting_stu _setting_stu;
    _setting_stu.job_id = job_id;
    _setting_stu.modify_type = modify_type;
    _setting_stu.key = QString::fromStdString(key);
    _setting_stu.value = QString::fromStdString(value);
    _setting_stu.mesh_id = mesh_id;
    g_modify_setting.push_back(_setting_stu);
#endif
    return true;
}

ENGINE_WRAPPER_API std::string get_value(int job_id, const std::string& key, int mesh_id)
{
#ifdef USE_ARCUS
    std::lock_guard<std::mutex> lock(settingMutex);
    auto iter = g_modify_setting.begin();
    while (iter != g_modify_setting.end())
    {
        if (job_id == iter->job_id)
        {
           if (QString::fromStdString(key) == iter->key)
           {
               return iter->value.toStdString();
           }
        }
        iter++;
    }
#endif
    return "";
}

ENGINE_WRAPPER_API void destroy(int job_id)//destroy job_id
{
    if (-1 == job_id)
    {
        return;
    }

#ifdef USE_ARCUS
    std::lock_guard<std::mutex> lock(destroyMutex);
    if (g_all_wrapper.size()>0)
    {
        auto iterFind = g_all_wrapper.find(job_id);
        if (iterFind != g_all_wrapper.end())
        {
            iterFind->second->close();
            delete iterFind->second;
            iterFind->second = nullptr;
            g_all_wrapper.erase(iterFind);
        }
    }
#endif
}

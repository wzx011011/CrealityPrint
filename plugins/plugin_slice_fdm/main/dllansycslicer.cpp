#include "dllansycslicer.h"
#include "interface/camerainterface.h"
#include "qtuser3d/camera/cameracontroller.h"
#include "cxutil/slicer/ExtruderTrain.h"
#include "cxss/sliceimpl.h"
#include "core/sliceresult.h"

#include "us/globalsetting.h"
#include "us/extrudersetting.h"
#include "us/modelgroupinput.h"
#include "us/modelinput.h"

#include "spdlog/cxlog_macro.h"
#include<QDebug>
#include <QStandardPaths>

DLLAnsycSlicer::DLLAnsycSlicer(QObject* parent)
	:AnsycSlicer(parent)
{
}

DLLAnsycSlicer::~DLLAnsycSlicer()
{
}

cxsw::SliceResult* DLLAnsycSlicer::doSlice(const QList<const us::ModelGroupInput*>& modelGroups, const us::GlobalSetting* globalSetting, QList<const us::ExtruderSetting*> extruderSettings,
		qtuser_core::Progressor* progressor)
{
    qInfo() << " DLLAnsycSlicer doSlice";
    //QString logPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Log/cxslice.log";
    //QByteArray cdata = logPath.toLocal8Bit();
    //std::string logStdPath(cdata);

    //cxlog::CXLog::Instance().InitCXLog(logStdPath);

    bool failed = false;
    auto pf = [&progressor](float r) {
        progressor->progress(r);
    };

    auto intf = [&progressor]()->bool {
        return progressor->interrupt();
    };

    auto ff = [&progressor, &failed](const char* msg)
    {
        failed = true;
        progressor->failed(msg);
    };

    cxsw::SliceResult* result = new cxsw::SliceResult();
    cxss::SliceImpl slice;

    cxutil::Scene& scene = slice.m_slice.scene;
    scene.fFunc = ff;
    scene.iFunc = intf;
    scene.pFunc = pf;

    //Global Settings
    bool r = scene.iFunc();
    const QHash<QString, us::USetting*>& G = globalSetting->settings();
    for (QHash<QString, us::USetting*>::const_iterator it = G.constBegin(); it != G.constEnd(); ++it)
    {
        scene.settings.add(it.key().toStdString(), it.value()->value().toString().toStdString());
    }

    if(scene.settings.get<bool>("machine_is_belt"))
        scene.settings.add("adhesion_type", "none");

    //Extruder Settings
    size_t extruder_count = scene.settings.get<size_t>("machine_extruder_count");
    if (extruder_count == 0)
        extruder_count = 1;
    for (size_t extruder_nr = 1; extruder_nr < extruder_count; extruder_nr++)
        scene.extruders.emplace_back(extruder_nr, &scene.settings);

    for (size_t i = 0; i < extruder_count; ++i)
    {
        int32_t extruder_nr = i;
        if (extruder_nr < 0 || extruder_nr >= static_cast<int32_t>(extruderSettings.size()))
        {
            ff("extruder out of index");
            failed = true;
            break;
        }

        cxutil::ExtruderTrain& extruder = scene.extruders[i];

        const QHash<QString, us::USetting*>& E = extruderSettings.at(i)->settings();
        for (QHash<QString, us::USetting*>::const_iterator it = E.constBegin(); it != E.constEnd(); ++it)
        {
            if (G.find(it.key()) != G.end())
            {
                extruder.settings->add(it.key().toStdString(), G.value(it.key())->value().toString().toStdString());
            }
            else
            {
                extruder.settings->add(it.key().toStdString(), it.value()->value().toString().toStdString());
            }
        }

    }

    //model groups
    int size = modelGroups.size();
    if (size > 0)
        scene.mesh_groups.resize(size);
    for (int i = 0; i < size; ++i)
    {
        const us::ModelGroupInput* modelGroupInput = modelGroups.at(i);
        const QList<us::ModelInput*>& modelInputs = modelGroupInput->modelInputs();

        if (modelInputs.size() <= 0)
        {
            continue; //Don't slice empty mesh groups.
        }

        cxutil::MeshGroup& meshGroup = scene.mesh_groups.at(i);
        const QHash<QString, us::USetting*>& MG = modelGroupInput->modelListSetting()->settings();
 
        //Load the settings in the mesh group.
        for (QHash<QString, us::USetting*>::const_iterator it = MG.constBegin(); it != MG.constEnd(); ++it)
        {
           // meshGroup.settings->add(it.key().toStdString(), it.value()->value().toString().toStdString());
            if (G.find(it.key()) != G.end())
            {
                meshGroup.settings->add(it.key().toStdString(), G.value(it.key())->value().toString().toStdString());
             }
            else
            {
                meshGroup.settings->add(it.key().toStdString(), it.value()->value().toString().toStdString());
             }
        }
        scene.machine_center_is_zero = meshGroup.settings->get<bool>("machine_center_is_zero");
        meshGroup.settings->add("machine_center_is_zero", "true");   //our system is always build in printer corner(origin), so set machine_center_is_zero == true, skip cura engine machine offset

        cxutil::AABB3D meshGroupBox;
        cxutil::FMatrix3x3 matrix;
        for (us::ModelInput* modelInput : modelInputs)
        {
            const QHash<QString, us::USetting*>& MS = modelInput->modelSetting()->settings();
            const size_t face_count = (size_t)modelInput->triangleNum();

            if (face_count <= 0)
            {
                failed = true;
                ff("Got an empty mesh. Ignoring it!");
                break;
            }

            cxutil::Mesh* mesh = new cxutil::Mesh();
            meshGroup.meshes.push_back(mesh);

            //Load the settings for the mesh.
            bool isBelt = G.value("machine_is_belt")->value().toBool();
            for (QHash<QString, us::USetting*>::const_iterator it = MS.constBegin(); it != MS.constEnd(); ++it)
            {
                if (it.key() == "support_mesh" || it.key() == "support_mesh_drop_down" || it.key() == "support_enable")
                {
                    mesh->settings->add(it.key().toStdString(), it.value()->value().toString().toStdString());
                }
                else if (G.find(it.key()) != G.end())
				{
					mesh->settings->add(it.key().toStdString(), G.value(it.key())->value().toString().toStdString());
				}
				else
                {
                    mesh->settings->add(it.key().toStdString(), it.value()->value().toString().toStdString());
                    qDebug() << "Msetting" << it.key() << " = " << it.value()->value().toString();
                }
            }
            if (isBelt)
            {
                mesh->settings->add("support_enable", "false");
            }

            float* p = modelInput->position();
            for (size_t face = 0; face < face_count; face++)
            {
                const cxutil::FPoint3* float_vertices = reinterpret_cast<const cxutil::FPoint3*>(p + 9 * face);

                cxutil::Point3 verts[3];
                verts[0] = matrix.apply(float_vertices[0]);
                verts[1] = matrix.apply(float_vertices[1]);
                verts[2] = matrix.apply(float_vertices[2]);
                mesh->addFace(verts[0], verts[1], verts[2]);
            }

            mesh->finish();
            meshGroupBox.include(mesh->getAABB());
        }

        if (scene.settings.get<size_t>("special_slope_slice_angle") != 0)
        {
            cxutil::Point3 need_offset(-meshGroupBox.min.x, -meshGroupBox.min.y, -meshGroupBox.min.z);
            for (cxutil::Mesh* amesh : meshGroup.meshes)
            {
                amesh->offset(need_offset);
                amesh->settings->add("support_enable", "false");
            }
            meshGroup.m_offset = need_offset;
        }

       
		size_t ibedType = scene.settings.get<size_t>("machine_hotzone_count");
        if (ibedType >0)
		{
			std::vector<cxutil::AABB> HotZone;
            if (13== ibedType)
            {
				for (size_t n = 0; n < 13; n++)
					scene.vctHotZone.emplace_back(false);

				signed long long unitX = 77500;
				signed long long unitY = 78750;
				HotZone.push_back(cxutil::AABB(cxutil::Point(unitX, unitY), cxutil::Point(unitX * 3, unitY * 3)));//D0
				HotZone.push_back(cxutil::AABB(cxutil::Point(unitX * 3, unitY * 2), cxutil::Point(unitX * 4, unitY * 3)));//D1
				HotZone.push_back(cxutil::AABB(cxutil::Point(unitX * 3, unitY * 1), cxutil::Point(unitX * 4, unitY * 2)));//D2
				HotZone.push_back(cxutil::AABB(cxutil::Point(unitX * 3, 0), cxutil::Point(unitX * 4, unitY * 1)));//D3
				HotZone.push_back(cxutil::AABB(cxutil::Point(unitX * 2, 0), cxutil::Point(unitX * 3, unitY * 1)));//D4
				HotZone.push_back(cxutil::AABB(cxutil::Point(unitX * 1, 0), cxutil::Point(unitX * 2, unitY * 1)));//D5
				HotZone.push_back(cxutil::AABB(cxutil::Point(0, 0), cxutil::Point(unitX * 1, unitY * 1)));//D6
				HotZone.push_back(cxutil::AABB(cxutil::Point(0, unitY * 1), cxutil::Point(unitX * 1, unitY * 2)));//D7
				HotZone.push_back(cxutil::AABB(cxutil::Point(0, unitY * 2), cxutil::Point(unitX * 1, unitY * 3)));//D8
				HotZone.push_back(cxutil::AABB(cxutil::Point(0, unitY * 3), cxutil::Point(unitX * 1, unitY * 4)));//D9
				HotZone.push_back(cxutil::AABB(cxutil::Point(unitX * 1, unitY * 3), cxutil::Point(unitX * 2, unitY * 4)));//D10
				HotZone.push_back(cxutil::AABB(cxutil::Point(unitX * 2, unitY * 3), cxutil::Point(unitX * 3, unitY * 4)));//D11
				HotZone.push_back(cxutil::AABB(cxutil::Point(unitX * 3, unitY * 3), cxutil::Point(unitX * 4, unitY * 4)));//D12
				for (cxutil::Mesh* amesh : meshGroup.meshes)
				{
					cxutil::AABB modelBox(cxutil::Point(amesh->getAABB().min.x, amesh->getAABB().min.y), cxutil::Point(amesh->getAABB().max.x, amesh->getAABB().max.y));
					for (int n = 0; n < HotZone.size(); n++)
					{
						if (HotZone[n].hit(modelBox))
						{
							scene.vctHotZone[n] = true;//加热区域
						}
					}
				}
            }
            else if (9== ibedType)
            {
				for (size_t n = 0; n < 9; n++)
					scene.vctHotZone.emplace_back(false);

				signed long long unit = 78300;
				signed long long small_unit = 70850;
                HotZone.push_back(cxutil::AABB(cxutil::Point(small_unit, small_unit), cxutil::Point(small_unit+unit, small_unit+unit )));//D0
                HotZone.push_back(cxutil::AABB(cxutil::Point(small_unit + unit, small_unit), cxutil::Point(small_unit*2 + unit, small_unit + unit)));//D1
                HotZone.push_back(cxutil::AABB(cxutil::Point(small_unit+unit, 0), cxutil::Point(small_unit*2 + unit, small_unit)));//D2
                HotZone.push_back(cxutil::AABB(cxutil::Point(small_unit, 0), cxutil::Point(small_unit + unit, small_unit)));//D3
                HotZone.push_back(cxutil::AABB(cxutil::Point(0, 0), cxutil::Point(small_unit, small_unit)));//D4
                HotZone.push_back(cxutil::AABB(cxutil::Point(0, small_unit), cxutil::Point(small_unit, small_unit + unit)));//D5
                HotZone.push_back(cxutil::AABB(cxutil::Point(0, small_unit+unit), cxutil::Point(small_unit, small_unit*2 + unit)));//D6
                HotZone.push_back(cxutil::AABB(cxutil::Point(small_unit, small_unit+unit), cxutil::Point(small_unit + unit, small_unit*2 + unit)));//D7
                HotZone.push_back(cxutil::AABB(cxutil::Point(small_unit + unit, small_unit + unit), cxutil::Point(small_unit * 2 + unit, small_unit * 2 + unit)));//D8
				for (cxutil::Mesh* amesh : meshGroup.meshes)
				{
					cxutil::AABB modelBox(cxutil::Point(amesh->getAABB().min.x, amesh->getAABB().min.y), cxutil::Point(amesh->getAABB().max.x, amesh->getAABB().max.y));
					for (int n = 0; n < HotZone.size(); n++)
					{
						if (HotZone[n].hit(modelBox))
						{
							scene.vctHotZone[n] = true;//加热区域
						}
					}
				}
            }
			else if (3 == ibedType)
			{
				for (size_t n = 0; n < 3; n++)
					scene.vctHotZone.emplace_back(false);

                signed long long unit = 17500;// 42500;
                signed long long unit2 = 47500;// 72500;
                
                HotZone.push_back(cxutil::AABB(cxutil::Point(0.0, 0.0), cxutil::Point(unit-1, 235000)));//D0
                HotZone.push_back(cxutil::AABB(cxutil::Point(unit-1, 0.0), cxutil::Point(235000, unit-1)));//D1
                HotZone.push_back(cxutil::AABB(cxutil::Point(235000 - unit-1, unit-1), cxutil::Point(235000, 235000)));//D2
                HotZone.push_back(cxutil::AABB(cxutil::Point(unit-1, 235000 - unit+1), cxutil::Point(235000 - unit+1, 235000)));//D3
                HotZone.push_back(cxutil::AABB(cxutil::Point(unit, unit), cxutil::Point(unit2-1, 235000 - unit)));//D4
                HotZone.push_back(cxutil::AABB(cxutil::Point(unit2-1, unit), cxutil::Point(235000 - unit, unit2-1)));//D5
                HotZone.push_back(cxutil::AABB(cxutil::Point(235000 - unit2+1, unit2-1), cxutil::Point(235000 - unit, 235000 - unit)));//D6
                HotZone.push_back(cxutil::AABB(cxutil::Point(unit2-1, 235000 - unit2+1), cxutil::Point(235000 - unit2+1, 235000 - unit)));//D7
                HotZone.push_back(cxutil::AABB(cxutil::Point(unit2, unit2), cxutil::Point(235000 -unit2, 235000 - unit2)));//D8
                for (cxutil::Mesh* amesh : meshGroup.meshes)
				{
					cxutil::AABB modelBox(cxutil::Point(amesh->getAABB().min.x, amesh->getAABB().min.y), cxutil::Point(amesh->getAABB().max.x, amesh->getAABB().max.y));
					for (int n = 0; n < HotZone.size(); n++)
					{
						if (HotZone[8].hit(modelBox))
						{
							scene.vctHotZone[0] = true;//加热区域
						}
                        if (HotZone[0].hit(modelBox) || HotZone[1].hit(modelBox) || HotZone[2].hit(modelBox) || HotZone[3].hit(modelBox))
                        {
                            scene.vctHotZone[2] = true;//加热区域
                        }
						if (HotZone[4].hit(modelBox) || HotZone[5].hit(modelBox) || HotZone[6].hit(modelBox) || HotZone[7].hit(modelBox))
						{
							scene.vctHotZone[1] = true;//加热区域
						}
					}
				}
            }
        }

        if (failed) break;
    }
    slice.slice(result->pathName());
    if (failed && result)
    {
        delete result;
        result = nullptr;
    }
    return result;
}
	 
//cxsw::SliceResult* DLLAnsycSlicer::doSlice(const QList<const us::ModelGroupInput*>& modelGroups, const us::GlobalSetting* globalSetting, QList<const us::ExtruderSetting*> extruderSettings,
//	qtuser_core::Progressor* progressor)
//{
//	qDebug() << "slice";
//
//	bool failed = false;
//	auto pf = [&progressor](float r) {
//		progressor->progress(r * 0.7f);
//	};
//
//	auto intf = [&progressor]()->bool {
//		return progressor->interrupt();
//	};
//
//	auto ff = [&progressor, &failed](const char* msg)
//	{
//		failed = true;
//		progressor->failed(msg);
//	};
//
//	//qDebug() << (globalSetting->settings().find("machine_center_is_zero").value()->value().toString());
//	cxsw::DllInvokerSlicer slicer;
//	cxsw::SliceResult* result = slicer.slice(modelGroups, globalSetting, extruderSettings.at(0), pf, intf, ff);
//
//	//cxsw::SliceResult* result = new cxsw::SliceResult();
//	//std::string filename = "E:\\model\\slice\\louding.gcode";
//	//result->load(filename);
//
//	if (failed && result)
//	{
//		delete result;
//		result = nullptr;
//	}
//
//	return result;
//}

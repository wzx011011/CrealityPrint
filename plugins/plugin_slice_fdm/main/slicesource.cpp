#include "slicesource.h"

#include "data/modeln.h"
#include "data/fdmsupportgroup.h"

#include "interface/spaceinterface.h"

#include "slice/trimeshinput.h"
#include "slice/modelgroupinputproxy.h"
#include "slice/supportinput.h"
#include "us/globalsetting.h"
#include "utils/matrixutil.h"
#include "mmesh/trimesh/trimeshutil.h"
#include"trimesh2/Vec.h"


using namespace creative_kernel;
SliceSource::SliceSource(QObject* parent)
	:QObject(parent)
{
}

SliceSource::~SliceSource()
{
}

bool SliceSource::needSlice()
{
	QList<ModelN*> models = modelns();
	return models.size() > 0;
}

QList<const us::ModelGroupInput*> SliceSource::createGroupInputs(us::GlobalSetting* globset)
{
	bool isBelt = globset->settings().value("machine_is_belt")->value().toBool();
	if (isBelt)
	{
		return createBeltGroupInputs(globset);
	}

	QList<const us::ModelGroupInput*> out;
	QList<ModelGroup*> groups = modelGroups();

	trimesh::vec3 offset;
	for (ModelGroup* group : groups)
	{
		ModelGroupInputProxy* modelGroupInput = new ModelGroupInputProxy();
		modelGroupInput->setGroup(group);
		{
			QList<ModelN*> models = group->models();
			int index = 0;
			for (ModelN* model : models)
			{
				TrimeshInput* meshInput = new TrimeshInput(modelGroupInput);
				model->mesh();
				meshInput->setID(index++);
				meshInput->setName(model->objectName());
				meshInput->setModel(model);
				meshInput->build();			
				modelGroupInput->addModelInput(meshInput);
                /*const us::USettings* uset = meshInput->modelSetting();
                TrimeshInput* meshInput2 = new TrimeshInput();
                meshInput->modelSetting();
                uset = meshInput2->modelSetting();
                if(meshInput->modelSetting()==nullptr)
                {
                    return out;
                }*/

				bool needAddSurpport = true;
				float angle = globset->settings().value("special_slope_slice_angle")->value().toInt();
				QString Axis = globset->settings().value("special_slope_slice_axis")->value().toString();
				trimesh::vec axis = trimesh::vec(0, 1, 0);
				if (Axis == "X") axis = trimesh::vec(1, 0, 0);
				if (angle != 0)
				{					
					meshInput->tiltSliceSet(axis, angle);
					if (globset->settings().value("support_enable")->value().toBool())
					{
						float support_angle = globset->settings().value("support_angle")->value().toFloat();
						trimesh::TriMesh* supportMesh = meshInput->generateSlopeSupport(model->mesh(), angle, axis, support_angle, false);
						//trimesh::TriMesh* supportMesh = meshInput->generateBeltSupport(model->mesh(), support_angle);
						if (supportMesh)
						{
							auto vh= meshInput->modelSetting()->settings().find("support_enable");
							if (vh != meshInput->modelSetting()->settings().end())
							{
								us::USetting* val = vh.value();
								QString value = "false";
								val->setValue(value);
							}

							SupportInput* supportInput = new SupportInput(modelGroupInput);
							supportInput->setID(index++);
							supportInput->setName(QString("%1").arg(1));
							supportInput->setModel(model);
							supportInput->setTrimesh(supportMesh);
							supportInput->build();
							supportInput->tiltSliceSet(axis, angle);

							vh = supportInput->modelSetting()->settings().find("support_mesh");
							if (vh != supportInput->modelSetting()->settings().end())
							{
								us::USetting* val = vh.value();
								QString value = "true";
								val->setValue(value);
							}
							vh = supportInput->modelSetting()->settings().find("support_mesh_drop_down");
							if (vh != supportInput->modelSetting()->settings().end())
							{
								us::USetting* val = vh.value();
								QString value = "false";
								val->setValue(value);
							}
							vh = supportInput->modelSetting()->settings().find("support_enable");
							if (vh != supportInput->modelSetting()->settings().end())
							{
								us::USetting* val = vh.value();
								QString value = "false";
								val->setValue(value);
							}
							modelGroupInput->addModelInput(supportInput);
							needAddSurpport = false;
						}
					}
				}

				if (needAddSurpport)
				{
					if (model->hasFDMSupport())
					{
						auto vh = meshInput->modelSetting()->settings().find("support_enable");
						if (vh != meshInput->modelSetting()->settings().end())
						{
							us::USetting* val = vh.value();
							QString value = "false";
							val->setValue(value);
						}
						if (model->getFDMSuooprt())
						{
							std::vector<trimesh::TriMesh*> supportMeshes;
							trimesh::TriMesh* supportMesh = model->fdmSupport()->createFDMSupportMesh();
							if (supportMesh)
								supportMeshes.push_back(supportMesh);
							for (size_t i = 0; i < supportMeshes.size(); i++)
							{
								SupportInput* supportInput = new SupportInput(modelGroupInput);
								supportInput->setID(index++);
								supportInput->setName(QString("%1").arg(i + 1));
								supportInput->setModel(model);
								supportInput->setTrimesh(supportMeshes[i]);
								supportInput->buildFDM();

								auto vh = supportInput->modelSetting()->settings().find("support_mesh");
								if (vh != supportInput->modelSetting()->settings().end())
								{
									us::USetting* val = vh.value();
									QString value = "true";
									val->setValue(value);
								}
								vh = supportInput->modelSetting()->settings().find("support_mesh_drop_down");
								if (vh != supportInput->modelSetting()->settings().end())
								{
									us::USetting* val = vh.value();
									QString value = "false";//根据bug#ID1014930要求改为false
									val->setValue(value);
								}

								if (angle != 0)
								{
									supportInput->tiltSliceSet(axis, angle);
									vh = supportInput->modelSetting()->settings().find("support_mesh_drop_down");
									if (vh != supportInput->modelSetting()->settings().end())
									{
										us::USetting* val = vh.value();
										QString value = "false";
										val->setValue(value);
									}
								}

								modelGroupInput->addModelInput(supportInput);
							}
						}
					}
					else
					{
						if (meshInput->modelSetting() == nullptr)
						{
							return out;
						}

						auto vh = meshInput->modelSetting()->settings().find("support_enable");
						if (vh != meshInput->modelSetting()->settings().end())
						{
							us::USetting* val = vh.value();
							QString value = globset->settings().value("support_enable")->value().toString();
							val->setValue(value);
						}
					}
				}
			}
		}
		out.push_back(modelGroupInput);
	}
	return out;
}

QList<const us::ModelGroupInput*> SliceSource::createBeltGroupInputs(us::GlobalSetting* globset)
{
	QList<const us::ModelGroupInput*> out;
	QList<ModelGroup*> groups = modelGroups();

	trimesh::vec3 offset;
	trimesh::point minZ(0, 0, 99999);
	for (ModelGroup* group : groups)
	{
		for (ModelN* model : group->models())
		{
			trimesh::point apoint = model->getMinYZ();
			if (minZ.z > apoint.z)
			{
				minZ = apoint;
			}
		}
	}
	offset = trimesh::vec3(0.0f, -minZ.y, 0.0f);
	QString str = QString::number(-minZ.y, 'f', 2);
	globset->settings().value("machine_belt_offset")->setValue(str);

	for (ModelGroup* group : groups)
	{
		ModelGroupInputProxy* modelGroupInput = new ModelGroupInputProxy();
		modelGroupInput->setGroup(group);
		{
			QList<ModelN*> models = group->models();
			int index = 0;

			//for (ModelN* model : models)
			for (int n=0;n<models.size();n++)
			{
				TrimeshInput* meshInput = new TrimeshInput(modelGroupInput);
				models[n] ->mesh();
				meshInput->setID(index++);
				meshInput->setName(models[n]->objectName());
				meshInput->setModel(models[n]);
				meshInput->build();

				float beltAngle = 45.0f * M_PIf / 180.0f;
				float layer_height = globset->settings().value("layer_height")->value().toFloat() / sinf(beltAngle);
				float layer_height0 = globset->settings().value("layer_height_0")->value().toFloat() / sinf(beltAngle);
				float support_xy_distance = globset->settings().value("support_xy_distance")->value().toFloat() / sinf(beltAngle);
				if (0==n)
				{
					QString str = QString::number(layer_height, 'f', 5);
					QString str0 = QString::number(layer_height0, 'f', 5);
					QString str1 = QString::number(support_xy_distance, 'f', 5);
					globset->settings().value("layer_height")->setValue(str);
					globset->settings().value("layer_height_0")->setValue(str0);
					globset->settings().value("support_xy_distance")->setValue(str1);
				}

				if (globset->settings().value("support_enable")->value().toBool())
				{
					float angle = globset->settings().value("support_angle")->value().toFloat();
					trimesh::vec axis = trimesh::vec(1, 0, 0);
					trimesh::TriMesh* supportMesh = meshInput->generateSlopeSupport(models[n]->mesh(), -45.0, axis, angle, true);
					//trimesh::TriMesh* supportMesh = meshInput->generateBeltSupport(models[n]->mesh(), angle);
					if (supportMesh)
					{
						//supportMesh->write("xxx.stl");
						SupportInput* supportInput = new SupportInput(modelGroupInput);
						supportInput->setID(index++);
						supportInput->setName(QString("%1").arg(1));
						supportInput->setModel(models[n]);
						supportInput->setTrimesh(supportMesh);
						supportInput->build();
						supportInput->beltSet(offset);

						auto vh = supportInput->modelSetting()->settings().find("support_mesh");
						if (vh != supportInput->modelSetting()->settings().end())
						{
							us::USetting* val = vh.value();
							QString value = "true";
							val->setValue(value);
						}
						vh = supportInput->modelSetting()->settings().find("support_mesh_drop_down");
						if (vh != supportInput->modelSetting()->settings().end())
						{
							us::USetting* val = vh.value();
							QString value = "false";
							val->setValue(value);
						}
						vh = supportInput->modelSetting()->settings().find("support_enable");
						if (vh != supportInput->modelSetting()->settings().end())
						{
							us::USetting* val = vh.value();
							QString value = "false";
							val->setValue(value);
						}
						modelGroupInput->addModelInput(supportInput);
					}
				}
				else if (models[n]->hasFDMSupport())
				{
					if (models[n]->getFDMSuooprt())
					{
						std::vector<trimesh::TriMesh*> supportMeshes;
						trimesh::TriMesh* supportMesh = models[n]->fdmSupport()->createFDMSupportMesh();
						supportMesh->need_bbox();
						trimesh::TriMesh::BBox box = supportMesh->bbox;
						//supportMesh->write("20220301.stl");
						if (supportMesh)
							supportMeshes.push_back(supportMesh);
						for (size_t i = 0; i < supportMeshes.size(); i++)
						{
							SupportInput* supportInput = new SupportInput(modelGroupInput);
							supportInput->setID(index++);
							supportInput->setName(QString("%1").arg(i + 1));
							supportInput->setModel(models[n]);
							supportInput->setTrimesh(supportMeshes[i]);
							supportInput->buildFDM();
							supportInput->beltSet(offset);

							auto vh = supportInput->modelSetting()->settings().find("support_mesh");
							if (vh != supportInput->modelSetting()->settings().end())
							{
								us::USetting* val = vh.value();
								QString value = "true";
								val->setValue(value);
							}
							vh = supportInput->modelSetting()->settings().find("support_mesh_drop_down");
							if (vh != supportInput->modelSetting()->settings().end())
							{
								us::USetting* val = vh.value();
								QString value = "false";
								val->setValue(value);
							}
							vh = supportInput->modelSetting()->settings().find("support_enable");
							if (vh != supportInput->modelSetting()->settings().end())
							{
								us::USetting* val = vh.value();
								QString value = "false";
								val->setValue(value);
							}
							modelGroupInput->addModelInput(supportInput);
						}
					}
				}
				meshInput->setBeltOffset(offset);
				meshInput->beltSet();

				auto vh = meshInput->modelSetting()->settings().find("support_enable");
				if (vh != meshInput->modelSetting()->settings().end())
				{
					us::USetting* val = vh.value();
					QString value = "false";
					val->setValue(value);
				}
				modelGroupInput->addModelInput(meshInput);
			}
		}
		out.push_back(modelGroupInput);
	}
	return out;

	//QList<const us::ModelGroupInput*> out;
	//QList<ModelGroup*> groups = modelGroups();

	//trimesh::vec3 offset;
	//trimesh::point minZ(0, 0, 99999);
	//for (ModelGroup* group : groups)
	//{
	//	for (ModelN* model : group->models())
	//	{
	//		trimesh::point apoint = model->getMinYZ();
	//		if (minZ.z > apoint.z)
	//		{
	//			minZ = apoint;
	//		}
	//	}
	//}
	//offset = trimesh::vec3(0.0f, -minZ.y, 0.0f);

	//QString str = QString::number(-minZ.y, 'f', 2);
	//globset->settings().value("machine_belt_offset")->setValue(str);

	//for (ModelGroup* group : groups)
	//{
	//	ModelGroupInputProxy* modelGroupInput = new ModelGroupInputProxy();
	//	modelGroupInput->setGroup(group);
	//	{
	//		QList<ModelN*> models = group->models();

	//		creative_kernel::ModelN* newModel = new creative_kernel::ModelN();
	//		std::vector<trimesh::TriMesh*> inMeshes;
	//		for (ModelN* model : models)
	//		{
	//			QMatrix4x4 m = model->globalMatrix();
	//			trimesh::fxform xf = qMatrix2Xform(m);
	//			//xf = trimesh::inv(xf);
	//			trimesh::TriMesh* inmesh = new trimesh::TriMesh();
	//			*inmesh = *model->mesh();
	//			trimesh::apply_xform(inmesh, trimesh::xform(xf));
	//			inMeshes.push_back(inmesh);
	//		}
	//		trimesh::TriMesh* outMesh = new trimesh::TriMesh();
	//		mmesh::mergeTriMesh(outMesh, inMeshes);
	//		TriMeshPtr meshptr(outMesh);
	//		//creative_kernel::cache(meshptr);
	//		newModel->setTrimesh(meshptr);
	//		newModel->updateMatrix();
	//		
	//		int index = 0;
	//		TrimeshInput* meshInput = new TrimeshInput(modelGroupInput);
	//		newModel->mesh();
	//		meshInput->setID(index++);
	//		meshInput->setName(newModel->objectName());
	//		meshInput->setModel(newModel);
	//		meshInput->build();

	//		float beltAngle = 45.0f * M_PIf / 180.0f;
	//		float layer_height = globset->settings().value("layer_height")->value().toFloat() / sinf(beltAngle);
	//		float layer_height0 = globset->settings().value("layer_height_0")->value().toFloat() / sinf(beltAngle);
	//		QString str = QString::number(layer_height, 'f', 5);
	//		QString str0 = QString::number(layer_height0, 'f', 5);
	//		globset->settings().value("layer_height")->setValue(str);
	//		globset->settings().value("layer_height_0")->setValue(str0);

	//		if (globset->settings().value("support_enable")->value().toBool())
	//		{
	//			float angle = globset->settings().value("support_angle")->value().toFloat();
	//			trimesh::TriMesh* supportMesh = meshInput->generateBeltSupport(newModel->mesh(), angle);
	//			if (supportMesh)
	//			{
	//				supportMesh->write("xxx.stl");
	//				SupportInput* supportInput = new SupportInput(modelGroupInput);
	//				supportInput->setID(index++);
	//				supportInput->setName(QString("%1").arg(1));
	//				supportInput->setModel(newModel);
	//				supportInput->setTrimesh(supportMesh);
	//				//supportInput->buildFDM();
	//				//supportInput->setBeltOffset(offset);
	//				supportInput->beltSet(offset);

	//				auto vh = supportInput->modelSetting()->settings().find("support_mesh");
	//				if (vh != supportInput->modelSetting()->settings().end())
	//				{
	//					us::USetting* val = vh.value();
	//					QString value = "true";
	//					val->setValue(value);
	//				}
	//				vh = supportInput->modelSetting()->settings().find("support_mesh_drop_down");
	//				if (vh != supportInput->modelSetting()->settings().end())
	//				{
	//					us::USetting* val = vh.value();
	//					QString value = "false";
	//					val->setValue(value);
	//				}
	//				vh = supportInput->modelSetting()->settings().find("support_enable");
	//				if (vh != supportInput->modelSetting()->settings().end())
	//				{
	//					us::USetting* val = vh.value();
	//					QString value = "false";
	//					val->setValue(value);
	//				}
	//				modelGroupInput->addModelInput(supportInput);
	//			}
	//		}

	//		meshInput->setBeltOffset(offset);
	//		meshInput->beltSet();

	//		auto vh = meshInput->modelSetting()->settings().find("support_enable");
	//		if (vh != meshInput->modelSetting()->settings().end())
	//		{
	//			us::USetting* val = vh.value();
	//			QString value = "false";
	//			val->setValue(value);
	//		}

	//		modelGroupInput->addModelInput(meshInput);
	//		if (meshInput->modelSetting() == nullptr)
	//		{
	//			return out;
	//		}
	//	}
	//	out.push_back(modelGroupInput);
	//}
	//return out;
}


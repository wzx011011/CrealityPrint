#include "modelpositioninitializer.h"

#include "external/data/modelspace.h"
#include "qtuser3d/math/space3d.h"
#include "qtuser3d/math/layoutalg.h"
#include <QtCore/QRectF>

#include "external/interface/spaceinterface.h"
#include "interface/modelinterface.h"
#include "data/softfuncmanager.h"
#include <nestplacer/nestplacer.h>
#include <libnest2d/libnest2d.hpp>
#include "us/slicermanager.h"

namespace creative_kernel
{
	ModelPositionInitializer::ModelPositionInitializer(QObject* parent)
		:QObject(parent)
	{
	}

	ModelPositionInitializer::~ModelPositionInitializer()
	{
	}

	void ModelPositionInitializer::initialize(ModelN* model)
	{
		ModelSpace* space = getModelSpace();

		qtuser_3d::Box3D initBox = model->localBox();
		qtuser_3d::Box3D baseBoundingBox = space->baseBoundingBox();

		QList<ModelN*> mns = space->modelns();
		QVector3D size = baseBoundingBox.size();
		QVector3D center = baseBoundingBox.center();
		center.setZ(0.0f);

		QVector3D initSize = initBox.size();
		QRectF initRect(center.x() - initSize.x() / 2.0f, center.y() + initSize.y() / 2.0f, initSize.x(), initSize.y());
		auto test = [&initRect, &mns](int x, int y)->bool {
			bool c = true;
			QRectF crect = initRect;
			crect.translate(QPointF((float)x * initRect.width() * 1.01f, (float)y * initRect.height() * 1.01f));
			for (ModelN* m : mns)
			{
				qtuser_3d::Box3D b = m->globalSpaceBox();
				QRectF rect(b.min.x(), b.max.y(), b.size().x(), b.size().y());

				if (crect.intersects(rect))
				{
					c = false;
					break;
				}
			}

			return c;
		};

		QVector3D offset = QVector3D(0.0f, 0.0f, 0.0f);


		int xx = 0;
		int yy = 0;
		int dir = 0;
		int step = 0;
		int stepcount = 0;
		int count = 0;

		int steps[20] = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10 };
		while (count < 100)
		{
			if (test(xx, yy))
			{
				break;
			}

			switch (dir)
			{
			case 0:
				--xx;
				break;
			case 1:
				--yy;
				break;
			case 2:
				++xx;
				break;
			case 3:
				++yy;
				break;
			default:
				break;
			}

			++stepcount;
			++count;

			if (stepcount == steps[step])
			{
				++dir;
				++step;
				stepcount = 0;
				if (dir >= 4) dir = 0;
			}
		}

		center += QVector3D(initSize.x() * (float)xx * 1.01f, initSize.y() * (float)yy * 1.01f, 0.0f);

		QVector3D oldCenter = initBox.center();
		QVector3D targetCenter = center + QVector3D(0.0f, 0.0f, initBox.size().z() / 2.0f);
		model->SetInitPosition(targetCenter - oldCenter);
		model->setLocalPosition(targetCenter - oldCenter, false);
	}

	void ModelPositionInitializer::initialize(ModelN* model, QList<ModelN*> models, const qtuser_3d::Box3D& baseBox)
	{

	}

	void ModelPositionInitializer::layout(ModelN* model, qtuser_core::Progressor* progressor, bool bAdaption)
	{
		using namespace qtuser_3d;

		Box3D baseBox = baseBoundingBox();
		S3DPrtRectF ptPlatform;

		ptPlatform.fXMin = baseBox.min.x();
		ptPlatform.fYMin = baseBox.min.y();
		ptPlatform.fXMax = baseBox.max.x();
		ptPlatform.fYMax = baseBox.max.y();

		
        if (softFuncManager::instance().GetSoftFunc()->modelAdaption() && bAdaption)
        {
            QVector3D newscale= QVector3D(1.0f, 1.0f, 1.0f);
            Box3D modelbox = model->globalSpaceBox();
            QVector3D oldscale = model->localScale();
            while (modelbox.size().x() > baseBox.size().x()||
                modelbox.size().y() > baseBox.size().y()||
                modelbox.size().z() > baseBox.size().z())
            {
                newscale = newscale * 0.9f;
                model->setLocalScale(newscale, true);
                modelbox = model->globalSpaceBox();

                qtuser_3d::Box3D box = model->globalSpaceBox();
                QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
                QVector3D oldLocalPosition = model->localPosition();
                QVector3D newLocalPosition = oldLocalPosition + zoffset;
                mixTSModel(model, oldLocalPosition, newLocalPosition, oldscale, newscale, false);
            }
        }



		auto InitializeModelInfor = [](ModelN* pModel, SModelPolygon& mdlInfor)
		{
			Box3D boxCur = pModel->globalSpaceBox();
			QVector3D ptGrobalCenter;

			ptGrobalCenter.setX((boxCur.min.x() + boxCur.max.x()) / 2);
			ptGrobalCenter.setY((boxCur.min.y() + boxCur.max.y()) / 2);
			mdlInfor.ptPolygon = pModel->convexPolygon().m_polygons;
			mdlInfor.ptGrobalCenter = ptGrobalCenter;

			Box3D baseGrobalBox = pModel->globalSpaceBox();
			mdlInfor.rcGrobalDst.fXMin = baseGrobalBox.min.x();
			mdlInfor.rcGrobalDst.fYMin = baseGrobalBox.min.y();
			mdlInfor.rcGrobalDst.fXMax = baseGrobalBox.max.x();
			mdlInfor.rcGrobalDst.fYMax = baseGrobalBox.max.y();
		};

		//initialize the source model information
		SModelPolygon modelInsert;
		//rcGroup = pModel->convexPolygon();
		InitializeModelInfor(model, modelInsert);

		//initialize all models that are in the platform
		QVector<SModelPolygon> plgGroup;
		QList<ModelN*> models = modelns();
		for (ModelN* m : models)
		{
			if (m != model)
			{
				SModelPolygon itModelInfor;
				InitializeModelInfor(m, itModelInfor);

				plgGroup.push_back(itModelInfor);
			}
		}

		S3DPrtPointF ptDst;

		CLayoutAlg layAlg;
		layAlg.InsertOnePolygon(ptPlatform, plgGroup, modelInsert, ptDst, 10, progressor);
		qDebug() << "load>>> layout";

		QVector3D ptValid;//
		ptValid.setX(ptDst.fX);
		ptValid.setY(ptDst.fY);

		QVector3D translate = ptValid - modelInsert.ptGrobalCenter + model->localPosition();
		model->SetInitPosition(translate);
		model->setLocalPosition(translate);

		checkModelRange();
		checkBedRange();
	}

	std::vector<trimesh::vec3> outLine(ModelN* model, trimesh::vec3& toffset)
	{
		QVector3D offset = model->localPosition();	
		std::vector<trimesh::vec3> lines;
		qtuser_3d::ConvexPolygon& convex = model->convexPolygon();
		if (offset == QVector3D())
		{
			float max_x = 0, min_x = FLT_MAX, max_y = 0, min_y = FLT_MAX;
			for (QVector3D& v : convex.m_polygons)
			{
				if (max_x < v.x()) max_x = v.x();
				if (min_x > v.x()) min_x = v.x();
				if (max_y < v.y()) max_y = v.y();
				if (min_y > v.y()) min_y = v.y();
			}
			offset.setX((max_x + min_x) / 2);
			offset.setY((max_y + min_y) / 2);
		}
		toffset = trimesh::vec3(offset.x(), offset.y(), offset.z());
		for (QVector3D& v : convex.m_polygons)
			lines.push_back(trimesh::vec3(v.x(), v.y(), v.z()) - toffset);
		return lines;
	}

	trimesh::vec3 getOldRT(ModelN* model)
	{
		QVector3D offset = model->localPosition();
		trimesh::vec3 toffset = trimesh::vec3(offset.x(), offset.y(), offset.z());
		return toffset;
	}

	int bOnTheEdge(Clipper3r::Path item_path, int width, int height)
	{
		Clipper3r::Path rect = {
			{0, 0},
			{width, 0},
			{width, height},
			{0, height}
		};
		Clipper3r::Paths result;
		Clipper3r::Clipper a;
		a.AddPath(rect, Clipper3r::ptSubject, true);
		a.AddPath(item_path, Clipper3r::ptClip, true);
		a.Execute(Clipper3r::ctIntersection, result, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
		if (result.empty())
			return 0;
		double inter_area = fabs(Clipper3r::Area(result[0]));
		double diff_area = fabs(inter_area - fabs(Clipper3r::Area(item_path)));
		if (inter_area > 0 && diff_area > 1000) return 1;
		if (diff_area < 1000) return 2;
		return 0;
	}

	bool nestAppend2d(QList <ModelN*> oldModels, ModelN* newModel, float width, float height, float delta, bool bAddModel)
	{
		const double Pi = 3.141592653589793238463;
		double scaleFactor = 1000.0;

		int imgW = (Clipper3r::cInt)(width * scaleFactor);   //???¨´??¨®¨°?¨ª
		int imgH = (Clipper3r::cInt)(height * scaleFactor);   //???¨´??¨®¨°??
		int dist = (Clipper3r::cInt)(delta * scaleFactor);     //???¨´???¨¤
		int offsetX = 0, offsetY = 0;

		if (1) //¨¨£¤¦Ì?¡À??¦Ì????
		{
			//offsetX = dist / 2, offsetY = dist / 2;
			int egde_dist = 100;//???¨´¦Ì?¡À??¦Ì?¨¤¨¤??a1??????
			if (egde_dist > dist) egde_dist = dist;
			offsetX += (dist - egde_dist) / 2;
			offsetY += (dist - egde_dist) / 2;
		}

		libnest2d::NestControl ctl;
		libnest2d::NestConfig<libnest2d::NfpPlacer, libnest2d::FirstFitSelection> cfg;
		cfg.placer_config.rotations.push_back(libnest2d::Radians(Pi / 4.0));//?¨¤¡À?D??¨¦¨®?Dy¡Áa??
		cfg.placer_config.rotations.push_back(libnest2d::Radians(Pi * 3 / 4.0));
		cfg.placer_config.rotations.push_back(libnest2d::Radians(Pi * 5 / 4.0));
		cfg.placer_config.rotations.push_back(libnest2d::Radians(Pi * 7 / 4.0));
		cfg.placer_config.alignment = libnest2d::NfpPlacer::Config::Alignment::DONT_ALIGN;

		auto convert = [&scaleFactor](Clipper3r::Path& oItem, ModelN* model) {
			std::vector<trimesh::vec3> lines;
			trimesh::vec3 offsetData;
			lines = outLine(model, offsetData);

			size_t size = lines.size();
			if (size > 0)
			{
				oItem.resize(size);
				for (size_t i = 0; i < size; ++i)
				{
					oItem.at(i).X = (Clipper3r::cInt)(lines.at(i).x * scaleFactor);
					oItem.at(i).Y = (Clipper3r::cInt)(lines.at(i).y * scaleFactor);
				}
			}
		};

		std::vector<libnest2d::Item> input;
		std::vector<libnest2d::Item> input_out_pack;
		size_t old_size = oldModels.size();
		for (int i = 0; i < old_size; i++)
		{
			ModelN* model = oldModels.at(i);
			Clipper3r::Path ItemPath;
			convert(ItemPath, model);

			if (Clipper3r::Orientation(ItemPath))
			{
				Clipper3r::ReversePath(ItemPath);
			}
			ItemPath = libnest2d::shapelike::convexHull(ItemPath);
			libnest2d::Item item = libnest2d::Item(ItemPath);
			trimesh::vec3 nest_data = getOldRT(model);
			item.translation({ Clipper3r::cInt(nest_data.x * scaleFactor), Clipper3r::cInt(nest_data.y * scaleFactor) });
			auto trans_item = item.transformedShape_s();
			if (bOnTheEdge(trans_item.Contour, imgW, imgH) == 2)
			{
				item.translation({ Clipper3r::cInt(nest_data.x * scaleFactor + offsetX), Clipper3r::cInt(nest_data.y * scaleFactor + offsetY) });
				input.push_back(item);
			}
			else
			{
				input_out_pack.push_back(item);
			}
		}

		Clipper3r::Path newItemPath;
		convert(newItemPath, newModel);
		if (Clipper3r::Orientation(newItemPath))
		{
			Clipper3r::ReversePath(newItemPath);
		}
		newItemPath = libnest2d::shapelike::convexHull(newItemPath);
		libnest2d::Item newItem = libnest2d::Item(newItemPath);


		if (bAddModel)
		{
			bool can_pack = false;
			newItem.translation({ imgW / 2, imgH / 2 });
			for (auto rot : cfg.placer_config.rotations) {
				newItem.rotation(rot);
				auto trans_item = newItem.transformedShape_s();
				if (bOnTheEdge(trans_item.Contour, imgW, imgH) == 2)
				{
					can_pack = true;
					break;
				}
			}
			if (!can_pack)
				return false;
			newItem.translation({ 0, 0 });
		}

		input.push_back(newItem);

		int imgW_dst = imgW; int imgH_dst = imgH;

		libnest2d::Box maxBox = libnest2d::Box(imgW_dst + 2 * offsetX, imgH_dst + 2 * offsetY, { imgW / 2 + offsetX, imgH / 2 + offsetY });
		std::size_t result = libnest2d::nest(input, maxBox, dist, cfg, ctl);

		int total_size = input.size();
		libnest2d::Item iitem = input.at(total_size - 1);
		if (iitem.rotation().toDegrees() == -90.)
		{
			iitem.rotation(0);
			iitem.translation({ 0, 0 });
		}
		else
		{
			libnest2d::Item new_item = input.at(total_size - 1);
			QQuaternion dq = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 1.0f), new_item.rotation().toDegrees());
			QQuaternion q = newModel->localQuaternion();
			QVector3D translate = QVector3D((new_item.translation().X - offsetX) / scaleFactor, (new_item.translation().Y - offsetY) / scaleFactor, 0.0f);
			translate.setZ(newModel->localPosition().z());
			newModel->SetInitPosition(translate);
			newModel->setLocalPosition(translate);
			setModelRotation(newModel, dq * q, true);
			return true;
		}

		return false;
	}

	bool ModelPositionInitializer::nestLayout(ModelN* model, bool bAddModel)
	{
		qtuser_3d::Box3D box = baseBoundingBox();
		QVector3D boxMinMax = box.size();
		QList <ModelN*> oldModels = modelns();
		bool can_pack = false;

		//can_pack = nestAppend2d(oldModels, model, boxMinMax.x(), boxMinMax.y(), 10.0, bAddModel);

		std::vector < std::vector<trimesh::vec3>> modelsData;
		std::vector<trimesh::vec3> transData;
		trimesh::vec3 RTdata;
		for (int m = 0; m < oldModels.size(); m++)
		{		
			std::vector<trimesh::vec3> oItem = creative_kernel::outLine(oldModels[m], RTdata);
			modelsData.push_back(oItem);
			transData.push_back(RTdata);
		}
		std::vector<trimesh::vec3> NewItem = creative_kernel::outLine(model, RTdata);
		trimesh::box3 workspaceBox(trimesh::vec3(box.min.x(), box.min.y(), box.min.z()), trimesh::vec3(box.max.x(), box.max.y(), box.max.z()));
		trimesh::vec3 offset(0, 0, 0);
		us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
		if (globalsetting->settings().value("machine_center_is_zero")->value().toBool())
		{
			offset = trimesh::vec3(boxMinMax.x() / 2, boxMinMax.y() / 2, 0);
		}

		std::function<void(trimesh::vec3)> modelPositionUpdateFunc_nest = [model, offset](trimesh::vec3 newBoxCenter) {
			QQuaternion dq = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 1.0f), newBoxCenter.z);
			QQuaternion q = model->localQuaternion();
			QVector3D translate = QVector3D(newBoxCenter.x - offset.x, newBoxCenter.y - offset.y, 0.0f);
			translate.setZ(model->localPosition().z());
			model->SetInitPosition(translate);
			model->setLocalPosition(translate);
			setModelRotation(model, dq * q, true);
		};

		nestplacer::NestParaFloat para = nestplacer::NestParaFloat(workspaceBox, 10.0f, nestplacer::PlaceType::NULLTYPE, true);
		can_pack = nestplacer::NestPlacer::layout_new_item(modelsData, transData, NewItem, para, modelPositionUpdateFunc_nest);

		checkModelRange();
		checkBedRange();
		return can_pack;
	}

	void ModelPositionInitializer::layoutBelt(ModelN* model, qtuser_core::Progressor* progressor, bool bAdaption)
	{
		Box3D baseBox = baseBoundingBox();

		QList<ModelN*> models = modelns();
		if (models.size())
		{
			QVector3D ptGrobalCenter;
			ptGrobalCenter.setX((baseBox.min.x() + baseBox.max.x()) / 2);
			float maxY = 0.0;
			for (ModelN* amodel : models)
			{
				if (maxY < amodel->globalSpaceBox().max.y())
				{
					maxY = amodel->globalSpaceBox().max.y();
				}
			}

			qtuser_3d::Box3D box = model->localBox();
			float localY = maxY+ 50+(box.max.y() - box.min.y()) * 0.5;
			ptGrobalCenter.setY(localY);
			QVector3D translate = ptGrobalCenter + model->localPosition();
			model->setLocalPosition(translate);
		}
		else
		{
			Box3D boxCur = model->globalSpaceBox();
			QVector3D ptGrobalCenter;
			ptGrobalCenter.setX((baseBox.min.x() + baseBox.max.x()) / 2);
			qtuser_3d::Box3D box = model->localBox();
			float localY = 10 + (box.max.y() - box.min.y()) * 0.5;
			ptGrobalCenter.setY(localY);
			QVector3D translate = ptGrobalCenter + model->localPosition();
			model->setLocalPosition(translate);
		}
		model->SetInitPosition(model->localPosition());
	}

	void ModelPositionInitializer::AdaptionModel(ModelN* model)
	{
		Box3D baseBox = baseBoundingBox();
		S3DPrtRectF ptPlatform;

		ptPlatform.fXMin = baseBox.min.x();
		ptPlatform.fYMin = baseBox.min.y();
		ptPlatform.fXMax = baseBox.max.x();
		ptPlatform.fYMax = baseBox.max.y();

		if (softFuncManager::instance().GetSoftFunc()->modelAdaption())
		{
			QVector3D newscale = QVector3D(1.0f, 1.0f, 1.0f);
			Box3D modelbox = model->globalSpaceBox();
			QVector3D oldscale = model->localScale();
			while (modelbox.size().x() > baseBox.size().x() ||
				modelbox.size().y() > baseBox.size().y() ||
				modelbox.size().z() > baseBox.size().z())
			{
				newscale = newscale * 0.9f;
				model->setLocalScale(newscale, true);
				modelbox = model->globalSpaceBox();

				qtuser_3d::Box3D box = model->globalSpaceBox();
				QVector3D zoffset = QVector3D(0.0f, 0.0f, -box.min.z());
				QVector3D oldLocalPosition = model->localPosition();
				QVector3D newLocalPosition = oldLocalPosition + zoffset;
				mixTSModel(model, oldLocalPosition, newLocalPosition, oldscale, newscale, false);
			}

			nestLayout(model, true);
			checkModelRange();
			checkBedRange();
		}
	}

	void EnlargeBox(Box3D& box, float d)
	{
		box.min += QVector3D(-d, -d, -d);
		box.max += QVector3D(d, d, d);
	};

	void ModelPositionInitializer::layout_all(QList<ModelN*>& models, qtuser_core::Progressor* progressor)
	{
		/* sort models by size */
		qSort(models.begin(), models.end(), [](ModelN* m1, ModelN* m2)
			{
				Box3D box1 = m1->globalSpaceBox();
				Box3D box2 = m2->globalSpaceBox();
				//if (box1.size().x() > box2.size().x()) { return true; }
				if (box1.size().y() > box2.size().y()) { return true; }
				//if (box1.size().z() > box2.size().z()) { return true; }
				return false;
			});

		auto IsBoundBoxHit = [](QVector3D pos1, Box3D box1, QVector3D pos2, Box3D box2, double safety_distance)
		{
			box1.translate(pos1 - box1.center());
			box2.translate(pos2 - box2.center());
			Box3D box1_copy(box1.min, box1.max);
			EnlargeBox(box1_copy, safety_distance);
			return box1_copy.intersects(box2);
		};

		Box3D basebox = baseBoundingBox();            // workspace w.r.t. wcs
		Box3D totalbox;                               // box contains all models
		QList<int> model_flags;                       // 0:not settled, 1:settled inside, 2:settled outside
		QList<QVector3D> positions;                   // position of boxes w.r.t. wcs
		float step_size = 5.0f;
		float model_safety_distance = 2.0;
		float boundary_safety_distance = basebox.size().x() * 0.3;
		QVector3D old_pos, new_pos, stepx(-step_size, 0.0f, 0.0f), stepy(0.0f, -step_size, 0.0f), total_offset;
		bool is_hit = false;
		bool is_last_stop = false;
		bool is_init_pos = true;
		int direction = 0;

		for (int i = 0; i < models.size(); i++)
		{
			Box3D gbox = models[i]->globalSpaceBox(); // model box w.r.t. model frame (DO NOT use local box cuz is not scaled)
			QVector3D box_size = gbox.size() * 0.5f;

			model_flags.push_back(0);
			positions.push_back(QVector3D());

			old_pos.setX(basebox.max.x() - boundary_safety_distance - box_size.x());
			old_pos.setY(basebox.max.y() - boundary_safety_distance - box_size.y());
			new_pos = old_pos;
			is_last_stop = false;
			is_init_pos = true;

			// sliding from right-top to left-bottom
			while (true)
			{
				is_hit = false;
				if (new_pos.x() < basebox.min.x() + boundary_safety_distance + box_size.x() ||
					new_pos.y() < basebox.min.y() + boundary_safety_distance + box_size.y())
				{
					is_hit = true;
				}
				else
				{
					for (size_t j = 0; j < i; j++)
					{
						if (model_flags[j] && IsBoundBoxHit(new_pos, gbox, positions[j], models[j]->globalSpaceBox(), model_safety_distance))
						{
							is_hit = true;
							break;
						}
					}
				}

				if (is_hit)
				{
					if (is_init_pos)
					{
						break;
					}
					if (is_last_stop)
					{
						model_flags[i] = 1;
						positions[i] = old_pos;
						break;
					}
					is_last_stop = true;
					direction++;
				}
				else
				{
					old_pos = new_pos;
					is_last_stop = false;
					is_init_pos = false;
				}
				new_pos = old_pos + ((direction % 2) ? stepx : stepy);
			}

			if (0 == model_flags[i])
			{
				if (boundary_safety_distance - 2.0f > 0.0f)
				{
					if (boundary_safety_distance - 2.0f * step_size - 2.0f > 0.0f)
					{
						boundary_safety_distance -= 2.0f * step_size;
					}
					else
					{
						boundary_safety_distance = 2.0f;
					}
					totalbox.clear();
					model_flags.clear();
					positions.clear();
					direction = 0;
					i = -1;
				}
				continue;  // if we cannot layout this model, adjust safety distance and try again
			}

			// expand totalbox
			if (!totalbox.valid)
			{
				totalbox = gbox;
				totalbox.translate(positions[i] - totalbox.center());
			}
			else
			{
				if (totalbox.min.x() > positions[i].x() - box_size.x())
				{
					totalbox.min.setX(positions[i].x() - box_size.x());
				}
				if (totalbox.max.x() < positions[i].x() + box_size.x())
				{
					totalbox.max.setX(positions[i].x() + box_size.x());
				}
				if (totalbox.min.y() > positions[i].y() - box_size.y())
				{
					totalbox.min.setY(positions[i].y() - box_size.y());
				}
				if (totalbox.max.y() < positions[i].y() + box_size.y())
				{
					totalbox.max.setY(positions[i].y() + box_size.y());
				}
			}
		}

		// settle models that can be settled inside
		total_offset = (basebox.size() - totalbox.size()) * 0.5 - QVector3D(boundary_safety_distance, boundary_safety_distance, 0.0f);
		for (size_t i = 0; i < models.size(); i++)
		{
			if (0 == model_flags[i])
			{
				continue;
			}
			Box3D curgbox = models[i]->globalSpaceBox();
			QVector3D curgboxcenter = (curgbox.max + curgbox.min) * 0.5;
			QVector3D translate = total_offset + positions[i] - curgboxcenter + models[i]->localPosition();
			translate.setZ(curgboxcenter.z() - models[i]->center().z() - curgbox.min.z());

			models[i]->SetInitPosition(translate);
			models[i]->setLocalPosition(translate);
		}

		// settle models that cannot be settled inside
		for (size_t i = 0; i < models.size(); i++)
		{
			if (1 == model_flags[i])
			{
				continue;
			}
			ModelPositionInitializer::layout_by_search(i, models, model_flags, step_size);
		}

		checkModelRange();
		checkBedRange();
	}

	void ModelPositionInitializer::layout_by_search(int model_index, QList<ModelN*>& models, QList<int>& model_flags, float step_size)
	{
		if (model_index >= models.size() || model_index >= model_flags.size() || 1 == model_flags[model_index])
		{
			return;
		}

		Box3D basebox = baseBoundingBox();
		Box3D gbox = models[model_index]->globalSpaceBox();
		QVector3D box_size = gbox.size() * 0.5f;
		QVector3D position(basebox.min.x() + box_size.x(), basebox.min.y() + box_size.y(), 0.0f);
		bool is_hit = false;

		// search inside (from left-bottom to right-top)
		EnlargeBox(basebox, -step_size);
		while (true)
		{
			is_hit = false;

			if (position.x() > basebox.max.x() - box_size.x() || position.y() > basebox.max.y() - box_size.y())
			{
				break;
			}

			for (size_t i = 0; i < models.size(); i++)
			{
				if (model_index == i || 1 != model_flags[i])
				{
					continue;
				}
				gbox.translate(position - gbox.center());
				
				if (gbox.intersects(models[i]->globalSpaceBox()))
				{
					is_hit = true;
					break;
				}
			}

			if (!is_hit)
			{
				Box3D curgbox = models[model_index]->globalSpaceBox();
				QVector3D curgboxcenter = (curgbox.max + curgbox.min) * 0.5;
				QVector3D translate = position - curgboxcenter + models[model_index]->localPosition();
				translate.setZ(-curgbox.min.z() / models[model_index]->localScale().z());

				models[model_index]->SetInitPosition(translate);
				models[model_index]->setLocalPosition(translate);
				model_flags[model_index] = 1;
				break;
			}

			if (position.x() + step_size > basebox.max.x() - box_size.x())
			{
				position.setX(basebox.min.x() + box_size.x());  // left
				position.setY(position.y() + step_size);
			}
			else
			{
				position.setX(position.x() + step_size);
			}
		}

		if (model_flags[model_index])  // settled inside
		{
			return;
		}

		/* search outside
		*              <-    6
		*        |     <-    2 |
		*      --|-------------|--
		*  v7 v3 |  Workspace  | 1^ 5^
		*      --|-------------|--
		*        |   0 ->      |
		*            4 ->
		*         ...
		*/
		EnlargeBox(basebox, 2.0f * step_size);
		position.setX(basebox.min.x() - step_size + box_size.x());
		position.setY(basebox.min.y() - step_size - box_size.y());
		int direction = 0;
		QVector3D outside_distance = box_size;
		while (true)
		{
			is_hit = false;

			for (size_t i = 0; i < models.size(); i++)
			{
				if (model_index == i || 2 != model_flags[i])
				{
					continue;
				}
				gbox.translate(position - gbox.center());
				if (gbox.intersects(models[i]->globalSpaceBox()))
				{
					is_hit = true;
					break;
				}
			}

			if (!is_hit)
			{
				Box3D curgbox = models[model_index]->globalSpaceBox();
				QVector3D curgboxcenter = (curgbox.max + curgbox.min) * 0.5;
				QVector3D translate = position - curgboxcenter + models[model_index]->localPosition();
				translate.setZ(-curgbox.min.z() / models[model_index]->localScale().z());

				models[model_index]->SetInitPosition(translate);
				models[model_index]->setLocalPosition(translate);
				model_flags[model_index] = 2;
				break;
			}

			switch (direction % 4)
			{
			case 0:
				if (position.x() > basebox.max.x() + outside_distance.x())
				{
					position.setY(position.y() + step_size);
					direction++;
				}
				else
				{
					position.setX(position.x() + step_size);
				}
				break;
			case 1:
				if (position.y() > basebox.max.y() + outside_distance.y())
				{
					position.setX(position.x() - step_size);
					direction++;
				}
				else
				{
					position.setY(position.y() + step_size);
				}
				break;
			case 2:
				if (position.x() < basebox.min.x() - outside_distance.x())
				{
					position.setY(position.y() - step_size);
					direction++;
				}
				else
				{
					position.setX(position.x() - step_size);
				}
				break;
			case 3:
				if (position.y() < basebox.min.y() - outside_distance.y())
				{
					position.setY(position.y() - step_size);
					outside_distance += QVector3D(step_size, step_size, 0.0);
					direction++;
				}
				else
				{
					position.setY(position.y() - step_size);
				}
				break;
			default:
				break;
			}
		}
	}
}

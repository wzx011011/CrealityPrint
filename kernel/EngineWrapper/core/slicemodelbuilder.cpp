#include "slicemodelbuilder.h"
#include "core/slicemodel.h"
#include "core/sliceresult.h"
#include "qtusercore/string/stringtool.h"
#include <qdebug.h>
#include "trimesh2/Vec.h"
#include "trimesh2/XForm.h"
#include "mmesh/trimesh/trimeshutil.h"

SliceModelBuilder::SliceModelBuilder(QObject* parent)
	:QObject(parent)
{
}
SliceModelBuilder::~SliceModelBuilder()
{
}


inline SliceLineType GetLineType(std::string& strLineType)
{
    if (strLineType.find(";TYPE:WALL-OUTER") != std::string::npos)
    {
        return SliceLineType::OuterWall;
    }
    else if (strLineType.find(";TYPE:WALL-INNER") != std::string::npos)
    {
        return SliceLineType::InnerWall;
    }
    else if (strLineType.find(";TYPE:SKIN") != std::string::npos)
    {
        return SliceLineType::Skin;
    }
    else if (strLineType.find(";TYPE:SUPPORT-INTERFACE") != std::string::npos)
    {
        return SliceLineType::SupportInterface;
    }
    else if (strLineType.find(";TYPE:SUPPORT-INFILL") != std::string::npos)
    {
        return SliceLineType::SupportInfill;
    }
    else if (strLineType.find(";TYPE:SUPPORT") != std::string::npos)
    {
        return SliceLineType::Support;
    }
    else if (strLineType.find(";TYPE:SKIRT") != std::string::npos)
    {
        return SliceLineType::SkirtBrim;
    }
    else if (strLineType.find(";TYPE:FILL") != std::string::npos)
    {
        return SliceLineType::Infill;
    }
    else if (strLineType.find(";TYPE:PRIME-TOWER") != std::string::npos)
    {
        return SliceLineType::PrimeTower;
    }
	else if (strLineType.find(";TYPE:Slow-Flow-Types") != std::string::npos)
	{
		return SliceLineType::FlowTravel;
	}
	else if (strLineType.find(";TYPE:Flow-In-Advance-Types") != std::string::npos)
	{
		return SliceLineType::AdvanceTravel;
	}
    else
    {
        return SliceLineType::NoneType;
    }
}

void SliceModelBuilder::buildSliceModel(SliceModel* sliceModel, const std::vector<std::string>& layerCode, float default_height,float beltOffset, float beltOffsetY, bool spiralFlag, int isbelt)
{
    trimesh::vec3 startPos(0.0, 0.0, 0.0), endPos(0.0, 0.0, 0.0);
    trimesh::vec3 lastPos(0.0, 0.0, 0.0);
	
    //belt 45¶È
    trimesh::vec3 offset(0, beltOffset, 0);
    trimesh::fxform xf4;
    if (1== isbelt)//creality print belt
    {
		trimesh::fxform xf = mmesh::beltXForm(offset, 45.0f);
		xf4 = trimesh::inv(xf);
    }
    else//creality slicer belt
    {
		trimesh::fxform xf = mmesh::beltXForm(offset, 45.0f,isbelt);
		xf4 = trimesh::inv(xf);
    }


    SliceLayer   SliceLayerTemp;
    SliceLineType  originLineType = SliceLineType::NoneType;

    SliceLineType  LastLineType = SliceLineType::NoneType;
    float speedTemp = 1200;
    float normalZ = 0.0;

    float prevLayerZ = 0;
    float curLayerZ = 0;
    float currentZ = 0;
	float prevE = 0;
	float currentE = 0;
    float tmpZ = 0;
    bool new_layer = false;
    bool new_Outwall_start = false;
    float prevThickness = default_height;
    float belt_Ymin = 0.0;

    std::vector<std::string> data_gcodelayer = layerCode;
    if (data_gcodelayer.size() < 2)
    {
        return;
    }
    int pos = 0;
    int nozzle = 0;

    std::vector<std::string>::iterator it_begin = data_gcodelayer.begin();
    //if (data_gcodelayer[0].find(";LAYER:0") >2)
    //{
    //    it_begin++;
    //}

    for (std::vector<std::string>::iterator it = it_begin; it < data_gcodelayer.end(); it++)
    {
        new_layer = true;
        curLayerZ = currentZ;
        std::vector<char*> vctDest;
        vctDest = qtuser_core::split_z((char*)(*it).data(), "\n");

        for (std::vector<char*>::iterator it2 = vctDest.begin(); it2 != vctDest.end(); it2++)
        {
            std::string strTemp = *it2;
            strTemp.erase(strTemp.find_last_not_of(" \n\r\t") + 1);

            if (strTemp.find(";TYPE:") != std::string::npos && strTemp.find(";TYPE:end") == std::string::npos)
            {
                originLineType = GetLineType(strTemp);

                if (originLineType != SliceLineType::FlowTravel && originLineType != SliceLineType::AdvanceTravel)
                {
                    LastLineType = originLineType;
                }
                
                if (originLineType == SliceLineType::OuterWall)
                {
                    new_Outwall_start = true;
                }
            }
            if (strTemp.find(";TYPE:end") != std::string::npos)
            {
                originLineType = LastLineType;
            }
            else if (strTemp.find("TIME_ELAPSED:") != std::string::npos)
            {
                int icount = strTemp.find(":") + 1;
                SliceLayerTemp.m_DeltaTime = qtuser_core::StrToInt(strTemp.substr(icount, strTemp.length()));
            }
            else  if (strTemp.substr(0, 1) == "T")
            {
                std::vector<char*> vctLine;
                vctLine = qtuser_core::split_z(*it2, " ");
                std::string strTemp2 = vctLine[0];
                nozzle = qtuser_core::StrToInt(strTemp2.substr(1, strTemp2.length()));
            }
            else if (strTemp[0] == 'G')
                //            else if (strTemp.find(" X") != std::string::npos && strTemp.find(" Y") != std::string::npos)
            {
                SliceLine SliceLineTemp;
                std::vector<char*> vctLine;//G1 Fxxx Xxxx Yxxx Exxx 
                vctLine = qtuser_core::split_z(*it2, " ");

                //bLineType = true;
                int xy_have = 0;
                int e_have = 0;

                for (std::vector<char*>::iterator it3 = vctLine.begin(); it3 != vctLine.end(); it3++)
                {
                    //it4 ==G1 / F4800 / X110.125 / Y106.709 /Z0.6 /E575.62352
                    std::string strTemp2 = *it3;

                    if (strTemp2.substr(0, 1) == "F")
                    {
                        speedTemp = qtuser_core::StrToInt(strTemp2.substr(1, strTemp2.length()));
                    }
                    else if (strTemp2.substr(0, 1) == "E" || strTemp2.substr(0, 1) == "P")
                    {
                        e_have = true;
                        if (new_layer)
                        {
                            currentZ = tmpZ;
                            new_layer = false;
                            prevLayerZ = curLayerZ;
                            curLayerZ = currentZ;
                        }
                        currentE = (qtuser_core::StrToFloat(strTemp2.substr(1, strTemp2.length())));
                        if ((currentE - prevE) < 0) ///ÓÐ»Ø³é
                        {
                            SliceLineTemp.reactE = prevE - currentE;
                        }
                        prevE = currentE;
                    }
                    else if (strTemp2.substr(0, 1) == "X")
                    {
                        endPos.at(0) = (qtuser_core::StrToFloat(strTemp2.substr(1, strTemp2.length())));
                        xy_have |= 1;
                    }
                    else if (strTemp2.substr(0, 1) == "Y")
                    {
                        endPos.at(1) = (qtuser_core::StrToFloat(strTemp2.substr(1, strTemp2.length())));
                        xy_have |= 2;
                    }
                    else if (strTemp2.substr(0, 1) == "Z")
                    {
                        endPos.at(2) = (qtuser_core::StrToFloat(strTemp2.substr(1, strTemp2.length())));
                        tmpZ = endPos.at(2);
                    }
                }

                //line
                if (endPos.at(2) < 0)
                {
                    endPos.at(2) = (startPos.at(2));
                }         
            
                SliceLineTemp.start = startPos;
                SliceLineTemp.end = endPos;

				if (isbelt)
				{
					SliceLineTemp.end = xf4 * SliceLineTemp.end;
					SliceLineTemp.end.y -= beltOffsetY;
				}

                startPos = SliceLineTemp.end;
                if (e_have)
                {
                    SliceLineTemp.type = originLineType;
                    if (strTemp[1] == '0')
                    {
                        SliceLineTemp.type = SliceLineType::Travel;
                    }
                    else if (SliceLineTemp.reactE > 0)
                    {
                        SliceLineTemp.type = SliceLineType::React;
                    }
                    else if (!spiralFlag && new_Outwall_start && xy_have)
                    {
                        new_Outwall_start = false;
                        SliceLineTemp.layerStartFlag = true;
                    }
                }
                else
                {
                    if (strTemp[1] == '0')
                    {
                        SliceLineTemp.type = SliceLineType::Travel;
                    }
                    else
                    {
                        SliceLineTemp.type = SliceLineType::NoneType;
                    }
                }

                SliceLineTemp.speed = speedTemp;
                SliceLineTemp.nozzle = nozzle;

                if (xy_have == 3)
                {
                    if (SliceLayerTemp.m_sliceLines.empty() && lastPos != trimesh::vec3(0.0, 0.0, 0.0))//fix bug #ID1014479
                    {
                        SliceLine travelTmp;
                        travelTmp.start = lastPos;
                        travelTmp.end = lastPos;
                        travelTmp.type = SliceLineType::Travel;
                        travelTmp.speed = SliceLineTemp.speed;
                        travelTmp.nozzle = SliceLineTemp.nozzle;
                        travelTmp.reactE = 0;
                        travelTmp.layerStartFlag = false;
                        SliceLayerTemp.m_sliceLines.push_back(travelTmp);
                    }
                    SliceLayerTemp.m_sliceLines.push_back(SliceLineTemp);
                    lastPos = SliceLineTemp.end;
                }
            }
        }

        if (SliceLayerTemp.m_sliceLines.size())
        {
            float thickness = curLayerZ - prevLayerZ;      // real thick height  
            if (thickness > 0)
            {
                if (default_height > 0)
                {
                    if (default_height > 0 && thickness < 2 * default_height && thickness > default_height / 2.0)
                    {
                        prevThickness = thickness;
                    }
                    else
                    {
                        thickness = prevThickness;
                    }
                }
                
            }
            else
            {
                thickness = prevThickness;
            }
            SliceLayerTemp.thickness = thickness;   
            sliceModel->m_sliceLayers.push_back(SliceLayerTemp);
            SliceLayerTemp.m_sliceLines.clear();
        }
    }
}

void SliceModelBuilder::buildSliceModel(SliceModel* sliceModel, cxsw::SliceResult* sliceResult, float default_height)
{
    sliceResult->load();

    buildSliceModel(sliceModel, sliceResult->m_data_gcodelayer, default_height);
}


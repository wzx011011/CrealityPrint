#ifndef GCODE_DATA_H_2
#define GCODE_DATA_H_2
#include "crslice2/header.h"

namespace crslice2
{
	class StepInfo
	{
	public:
		StepInfo() {}
		int moveId{ -1 };
		float speed{ -1 };
		float temperature{ -1 };
		float height{ -1 };	//高度
		float width{ -1 };	//宽度
		float fanSpeed{ -1 };	//风扇速度
		float volumetricRate{ -1 }; //流量
		float layerDuration{ -1 };	//层时间
		float acc{ -1 };	//加速度
		float position[3];
		char role{ -1 };
		char extruder{ -1 };

		bool equal(int moveId, float role, float extruder,
			float speed, float temperature, float height, float width, 
			float fanSpeed, float volumetricRate, float layerDuration, float acc)
		{
			return this->moveId == moveId &&
						this->role == role &&
						this->extruder == extruder &&
						this->speed == speed &&
						this->temperature == temperature &&
						this->height == height &&
						this->width == width &&
						this->fanSpeed == fanSpeed &&
						this->volumetricRate == volumetricRate &&
						this->layerDuration == layerDuration &&
						this->acc == acc;
		}

		bool valid()
		{
			return moveId >= 0;
		}
	};

	struct StepInfoPtr
	{
		int moveId;
		unsigned short span;
	};

	class GCodeRange
	{
	public:
		GCodeRange() {}
		GCodeRange(float min, float max) : max(max), min(min) {}

		float max;
		float min;

		float normalize(float value)
		{
			return (value - min) / (max - min);
		}

		float denormalize(float value)
		{
			return value * (max - min) + min;
		}

		int percent(float value)
		{
			return normalize(value) * 100 + 0.5; //四舍五入
		}

	};
	
	struct GCodeBlockRenderData
	{
		std::vector<std::vector<float>> verticeGroups;
		std::vector<std::vector<unsigned short>> indexGroups;
		std::vector<std::vector<StepInfoPtr>> stepInfoGroups;
	};

	struct GCodeLayerInfo
	{
		int stepCount;
		int first;
		int last;
	};

	class GCodePath
	{
	public:
		typedef std::vector<std::pair<int, std::vector<unsigned char>>> ImageData;

		class ExtruderColor
		{
		public:
			ExtruderColor() {}
			ExtruderColor(float r, float g, float b) : r(r), g(g), b(b) {}

			float r;
			float g;
			float b;
		};
		GCodePath() {}

		/* render */
		std::vector<GCodeBlockRenderData> blocks;
		/* render end */

		std::vector<GCodeLayerInfo> layers;

		GCodeRange speedRange;
		GCodeRange temperatureRange;
		GCodeRange heightRange;
		GCodeRange widthRange;
		GCodeRange fanSpeedRange;
		GCodeRange volumetricRateRange;
		GCodeRange layerDurationRange;
		GCodeRange accRange;

		std::vector<StepInfo> stepMap;

		/* gcode */
		std::vector<unsigned int> gcodeMap;	// moveid map to gcode line number
		std::vector<size_t> gcodeLinesEnd;
		/* gcode end */

		/* PrintEstimatedStatistics */
		float printTime;
		std::vector<std::pair<int, float>> rolesTime;
		std::vector<std::pair<int, float>> movesTime;
		std::vector<std::pair<int, float>> timeParts;
		std::vector<float> layersTime;
		float materialWeight;
		float materialLength;
		float materialCost;

		std::map<int, double> perFilamentUsedLength;	// model
		std::map<int, double> perFilamentUsedWeight;	// model
		std::map<int, double> flushPerFilamentUsedLength;	// flush
		std::map<int, double> flushPerFilamentUsedWeight;	// flush
		std::map<int, double> towerPerFilamentUsedLength;	// tower
		std::map<int, double> towerPerFilamentUsedWeight;	// tower
		/* PrintEstimatedStatistics end */

		/* attributes */
		std::vector<std::string> fliamentTypes;   //filament_type  
        std::vector<std::string> defaultFliamentColors;  //default_filament_colour
        std::vector<std::string> defaultFliamentColorsWithUnused;  //default_filament_colour
        std::vector<std::string> fliamentColors; //filament_colour
		float printableWidth{0};
		float printableLength{0};
		float printableHeight{0};
		std::vector<ExtruderColor> tools;
		std::string printerModel;
		float nozzleDiameter{0};
		trimesh::box3 boundingBox;
		ImageData imageData;
		/* attributes end */

		const StepInfo& getStepInfo(int moveId) { 
			if (moveId == -1)
				return stepMap.back();

			return stepMap.at(moveId); }
	};

}
#endif  // GCODE_DATA_H_2

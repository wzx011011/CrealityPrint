#include "modelgroupinput.h"
namespace us
{
	ModelGroupInput::ModelGroupInput(QObject* parent)
		:QObject(parent)
	{
	}
	ModelGroupInput::~ModelGroupInput()
	{
	}

	ModelListSetting* ModelGroupInput::modelListSetting()
	{
		return nullptr;
	}

	const ModelListSetting* ModelGroupInput::modelListSetting() const
	{
		return nullptr;
	}

	void ModelGroupInput::addModelInput(ModelInput* modelInput)
	{
		if (modelInput) m_modelInputs.push_back(modelInput);
	}

	const QList<ModelInput*>& ModelGroupInput::modelInputs() const
	{
		return m_modelInputs;
	}
}

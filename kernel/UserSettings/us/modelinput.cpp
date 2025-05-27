#include "us/modelinput.h"


namespace us
{
	ModelInput::ModelInput(QObject* parent)
		:QObject(parent)
	{
	}
	ModelInput::~ModelInput()
	{
	}

	/*ModelSetting* ModelInput::modelSetting()
	{
		return nullptr;
	}

	const ModelSetting* ModelInput::modelSetting() const
	{
		return nullptr;
	}
*/
	int ModelInput::id() const
	{
		return m_id;
	}

	void ModelInput::setID(int id)
	{
		m_id = id;
	}

	const QString& ModelInput::name() const
	{
		return m_name;
	}

	void ModelInput::setName(const QString& name)
	{
		m_name = name;
	}

	int ModelInput::vertexNum() const
	{
		return 0;
	}

	float* ModelInput::position() const 
	{
		return nullptr;
	}

	float* ModelInput::normal() const
	{
		return nullptr;
	}

	float* ModelInput::offset() const
	{
		return nullptr;
	}

	int ModelInput::triangleNum() const
	{
		return 0;
	}

	int* ModelInput::indices() const
	{
		return nullptr;
	}

	void ModelInput::simplifyModel()
	{

	}
}

#include "letterplugin.h"
#include "lettercommand.h"
#include "kernel/abstractkernel.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"

using namespace creative_kernel;

LetterPlugin::LetterPlugin(QObject* parent) : QObject(parent), m_command(nullptr)
{

}

LetterPlugin::~LetterPlugin()
{

}

QString LetterPlugin::name()
{
	return "LetterPlugin";
}

QString LetterPlugin::info()
{
	return "Lettering texts on the model surface.";
}

void LetterPlugin::initialize()
{
	m_command = new LetterCommand(this);
	m_command->setName(tr("Letter"));
	/*m_command->setEnabledIcon("qrc:/UI/photo/letter.png");
	m_command->setPressedIcon("qrc:/UI/photo/letter_d.png");
	m_command->setDisabledIcon("qrc:/UI/photo/letter_f.png");*/
	slotThemeChanged();

	m_command->setSource("qrc:/letter/letter/info.qml");

    AbstractKernelUI::getSelf()->addToolCommand(m_command, "left", 5);

	disconnect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));
	connect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));

	disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
	connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
}

void LetterPlugin::uninitialize()
{
	AbstractKernelUI::getSelf()->removeToolCommand(m_command, "left");
}

void LetterPlugin::slotLanguageChanged()
{
	m_command->setName(tr("Letter"));
}

void LetterPlugin::slotThemeChanged()
{
	QSettings setting;
	setting.beginGroup("themecolor_config");
	int nThemeType = setting.value("themecolor_config", 0).toInt();
	setting.endGroup();

	m_command->setEnabledIcon(nThemeType == 0 ? "qrc:/UI/photo/letter.png": "qrc:/UI/photo/letter2.png");
	m_command->setPressedIcon(nThemeType == 0 ? "qrc:/UI/photo/letter_d.png": "qrc:/UI/photo/letter_d.png");
	m_command->setDisabledIcon(nThemeType == 0 ? "qrc:/UI/photo/letter.png": "qrc:/UI/photo/letter.png");
}

void LetterPlugin::visibleLeftBarButton(SlicerManager::SLICERTYPE type)
{
	if (type == SlicerManager::SLICERTYPE::DLP && !m_command->enabled())
	{
		AbstractKernelUI::getSelf()->addToolCommand(m_command, "left");
		m_command->setEnabled(true);
	}
	else if (type == SlicerManager::SLICERTYPE::FDM && m_command->enabled())
	{
		AbstractKernelUI::getSelf()->removeToolCommand(m_command, "left");
		m_command->setEnabled(false);
	}
}

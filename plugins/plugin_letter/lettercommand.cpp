#include "lettercommand.h"
#include "letterop.h"

#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include "interface/modelinterface.h"
#include <QCoreApplication>
#include <QDirIterator>

#include "inout/cxopenandsavefilemanager.h"
#include <QStandardPaths>
#include "kernel/kernel.h"
#include "kernel/kernelui.h"
#include "kernel/abstractkernel.h"
#include "interface/gadatainterface.h"

LetterCommand::LetterCommand(QObject* parent) : ToolCommand(parent), m_pOp(nullptr)
{
	
}

LetterCommand::~LetterCommand()
{
	if (m_pOp)
	{
		delete m_pOp;
	}
}

void LetterCommand::slotMouseLeftClicked()
{
	message();
}

bool LetterCommand::message()
{
	if (m_pOp->getMessage())
	{
		getKernelUI()->requestMenuDialog(this, "deleteSupportDlg");
	}

	return true;
}

void LetterCommand::setMessage(bool isRemove)
{
	m_pOp->setMessage(isRemove);
}

void LetterCommand::execute()
{
	if (!m_pOp)
	{
		m_pOp = new LetterOp(this);
		connect(m_pOp, SIGNAL(mouseLeftClicked()), this, SLOT(slotMouseLeftClicked()));
	}

	// find fonts
	QStringList search_paths;
	search_paths.append(QCoreApplication::applicationDirPath());
	search_paths.append(QCoreApplication::applicationDirPath() + "/resources");
	int index = QCoreApplication::applicationDirPath().lastIndexOf("/");
	search_paths.append(QCoreApplication::applicationDirPath().left(index) + "/Resources/resources");
	search_paths.append(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/resources");

	m_listFonts.clear();

	for (QString& dir : search_paths)
	{
		QDirIterator iter(dir + "/fonts", QDirIterator::IteratorFlag::NoIteratorFlags);
		while (iter.hasNext())
		{
			iter.next();

			QFileInfo fileinfo = iter.fileInfo();
			if (fileinfo.isFile() && fileinfo.suffix() == "ttf")
			{
				m_listFonts.append(fileinfo.baseName());
			}
		}
	}
	m_listFonts = m_listFonts.toSet().toList();

	creative_kernel::setVisOperationMode(nullptr);
	creative_kernel::sendDataToGA("Model Editing & Layout", "Letter");
}

void LetterCommand::startLetter()
{
	creative_kernel::setVisOperationMode(m_pOp);
}

void LetterCommand::endLetter()
{
	creative_kernel::setVisOperationMode(nullptr);
	creative_kernel::AbstractKernelUI::getSelf()->switchPickMode();
}


bool LetterCommand::checkSelect()
{
	return creative_kernel::selectionms().size() > 0;
}


void LetterCommand::loadFont()
{
	CXFILE.open(this, supportFilters());
	QString fontPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/resources/fonts";
	QDir tempDir;
	if (!tempDir.exists(fontPath))
	{
		tempDir.mkpath(fontPath);
	}
}

bool LetterCommand::copyFileToPath(const QString& sourceDir, QString toDir)
{
	toDir.replace("\\", "/");
	if (sourceDir == toDir) {
		return true;
	}
	if (!QFile::exists(sourceDir)) {
		return false;
	}
	QDir createfile;
	bool exist = createfile.exists(toDir);
	if (exist)
		return false;

	QByteArray all;
	QFile sourcefile(sourceDir);
	if (sourcefile.open(QIODevice::ReadOnly))
	{
		all = sourcefile.readAll();
		sourcefile.close();
	}

	QFile dirfile(toDir);
	if (dirfile.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		dirfile.write(all);
		dirfile.close();
	}

	QFile(toDir).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
	return true;
}

QStringList LetterCommand::getFontList()
{
	return m_listFonts;
}

int LetterCommand::getCurFontIndex()
{
	int index = -1;

	for (size_t i = 0; i < m_listFonts.size(); i++)
	{
		if (m_pOp && m_pOp->GetFont() == m_listFonts[i])
		{
			index = i;
			break;
		}
	}

	return index;
}

void LetterCommand::setCurFontIndex(int cur_font_index)
{
	if (m_pOp && 0 < cur_font_index && cur_font_index < m_listFonts.size())
	{
		m_pOp->SetFont(m_listFonts[cur_font_index]);
	}
}

float LetterCommand::getHeight()
{
	if (m_pOp)
	{
		return m_pOp->GetHeight();
	}
	return 0.0f;
}

void LetterCommand::setHeight(float the_height)
{
	if (m_pOp)
	{
		m_pOp->SetHeight(the_height);
	}
}

float LetterCommand::getThickness()
{
	if (m_pOp)
	{
		return m_pOp->GetThickness();
	}
	return 0.0f;
}

void LetterCommand::setThickness(float the_thickness)
{
	if (m_pOp)
	{
		m_pOp->SetThickness(the_thickness);
	}
}

QString LetterCommand::getText()
{
	if (m_pOp)
	{
		return m_pOp->GetText();
	}
	return "";
}

void LetterCommand::setText(QString the_text)
{
	if (m_pOp)
	{
		m_pOp->SetText(the_text);
	}
}

int LetterCommand::getTextSide()
{
	if (m_pOp)
	{
		return m_pOp->GetTextSide();
	}
	return 0;
}

void LetterCommand::setTextSide(int the_text_side)
{
	if (m_pOp)
	{
		m_pOp->SetTextSide(the_text_side);
	}
}

QStringList LetterCommand::supportFilters()
{
	QStringList filters;
	filters.push_back("ttf");
	return filters;
}

void LetterCommand::handle(const QString& fileName)
{
	QString strTmp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QFileInfo fileInfo(fileName);
	copyFileToPath(fileName, strTmp+ "/resources/fonts/" + fileInfo.fileName());

	//update
	execute();
}

void LetterCommand::handle(const QStringList& fileNames)
{
	for (const QString fileName : fileNames)
	{
		handle(fileName);
	}
}

void LetterCommand::accept()
{
	setMessage(true);
}

void LetterCommand::cancel()
{
	setMessage(false);
	AbstractKernelUI::getSelf()->switchPickMode();
}

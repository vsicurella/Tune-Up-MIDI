/*
  ==============================================================================

    TuningFileParser.cpp
    Created: 23 Jul 2020 8:04:50pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuningFileParser.h"

TuningFileParser::TuningFileParser(const String& absoluteFilePath)
{
	loadNewFile(absoluteFilePath);
}

TuningFileParser::~TuningFileParser()
{
	tuningLoaded = nullptr;
}

void TuningFileParser::loadNewFile(const String& absoluteFilePath)
{
	filePath = absoluteFilePath;
	fileLoaded = File(filePath);
	DBG("File Size: " + String(fileLoaded.getSize()));

	type = TuningType(determineTuningType(fileLoaded));
	parseTuning();
}

void TuningFileParser::refreshTuning()
{
	tuningLoaded.reset(new Tuning(TuningDefinition::renderTuningDefinition(tuningLoadedDefinition)));
}

String TuningFileParser::getFilePath() const
{
	return filePath;
}

ValueTree TuningFileParser::getTuningDefinition() const
{
	return tuningLoadedDefinition.createCopy();
}

const Tuning& TuningFileParser::getTuning() const
{
	return *tuningLoaded.get();
}

TuningFileParser::TuningType TuningFileParser::getTuningType() const
{
	return type;
}

int TuningFileParser::determineTuningType(const File& tuningFileIn)
{
	int type = 0;

	// Check file extension

	String fileType = tuningFileIn.getFileExtension().toLowerCase();

	if (fileType == ".ftd")
		type = TuningType::FTD;

	else if (fileType == ".scl")
		type = TuningType::SCL;

	else if (fileType == ".tun")
		type = TuningType::TUN;

	else if (fileType == ".kbm")
		type = TuningType::KBM;

	else if (fileType == ".mnlgtuns" || fileType == ".mnlgtuno")
		type = TuningType::MNLGTUN;


	// TODO Double checking stuff

	return type;
}

ValueTree TuningFileParser::parseScalaFileDefinition(const File& scalaFile)
{
	std::string path = scalaFile.getFullPathName().toStdString();

	ValueTree tuningDefinition;
	TUN::CSCL_Import sclImport;

	sclImport.ReadSCL(path.c_str());
	
	TUN::CSingleScale tunSingleScale;
	sclImport.SetSingleScale(tunSingleScale);

	int baseNote = tunSingleScale.GetBaseNote();
	double baseFreq = tunSingleScale.GetBaseFreqHz();

	Array<double> centsTable;
	double periodCents = sclImport.GetLineInCents(sclImport.GetScaleSize() - 1);

	for (int i = 0; i <= sclImport.GetScaleSize(); i++)
	{
		centsTable.add(sclImport.GetLineInCents(i));
	}

	tuningDefinition = TuningDefinition::createStaticTuningDefinition(
		centsTable, 
		baseNote, 
		sclImport.GetTuningName(),
		sclImport.GetScaleDescription()
	);

	return tuningDefinition;
}

ValueTree TuningFileParser::parseTunFileDefinition(const File& tunFile)
{
	ValueTree tuningDefinition;

	TUN::CSingleScale tunSingleScale;
	std::string path = tunFile.getFullPathName().toStdString();

	if (tunSingleScale.Read(path.c_str()))
	{
		Array<double> centsTable;
		int baseNote = tunSingleScale.GetBaseNote();
		double baseFreq = tunSingleScale.GetBaseFreqHz();

		for (auto freq : tunSingleScale.GetNoteFrequenciesHz())
		{
			centsTable.add(ratioToCents(freq / baseFreq));
		}

		tuningDefinition = TuningDefinition::createStaticTuningDefinition(
			centsTable,
			baseNote,
			tunSingleScale.GetKeys()[TUN::CSingleScale::KEY_Name],
			tunSingleScale.GetKeys()[TUN::CSingleScale::KEY_Description]
		);

	}

	return tuningDefinition;
}

ValueTree TuningFileParser::parseMnlgtunDefinition(const File& mngltunFile)
{
	ValueTree tuningDefinition;
	MinilogueImporter mnlgTunImporter(mngltunFile.getFullPathName());
	
	if (mnlgTunImporter.getErrorMessage().length() == 0)
	{
		tuningDefinition = TuningDefinition::createStaticTuningDefinition(
			mnlgTunImporter.getCentsTable(),
			mnlgTunImporter.getBaseNote(),
			mnlgTunImporter.getFileName(),
			mnlgTunImporter.getComment()
		);
	}

	return tuningDefinition;
}

void TuningFileParser::parseTuning()
{
	switch (type)
	{
	case TuningType::FTD:
		tuningLoadedDefinition = ValueTree::fromXml(fileLoaded.loadFileAsString());
		break;

	case TuningType::SCL:
		tuningLoadedDefinition = parseScalaFileDefinition(fileLoaded);
		break;

	case TuningType::TUN:
		tuningLoadedDefinition = parseTunFileDefinition(fileLoaded);
		break;

	case TuningType::MNLGTUN:
		tuningLoadedDefinition = parseMnlgtunDefinition(fileLoaded);
		break;

	default: // DEFAULT TO STANDARD TUNING
		tuningLoadedDefinition = TuningDefinition::getStandardTuningDefinition();
		break;
	}

	refreshTuning();
}
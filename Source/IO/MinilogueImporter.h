/*
  ==============================================================================

    MinilogueImporter.h
    Created: 24 Jul 2020 12:01:29pm
    Author:  Vincenzo

	JUCE-based class to import tuning files exported by a 'logue Librarian

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MinilogueImporter
{


public:

	MinilogueImporter() {};
	MinilogueImporter(String absolutePathIn);
	~MinilogueImporter() {};

	bool readFile(String absolutePathIn);

	void setBaseNote(int baseMidiNote);

	const Array<double>& getCentsTable() const;

	String getErrorMessage() const;

	int getBaseNote() const;

	String getFileName() const;

	String getComment() const;

	String getAuthor() const;

private:

	/*
		Returns true if appears to be a valid 'logue tuning file
	*/
	bool unzipFileLoaded();
	
	void parseIntervalTable();

	void updateRootedTable();

public:

	const StringArray MnlgtunFileHeader = { "TunS_000TunS_", "TunO_000.TunO_" };
	const StringArray MnlgtunFileSuffix = { "bin", "info" };
	const String FileInformationXML = "FileInformation.xml";

	const int MNLGTUN_C60_CENTS = 6000;
	const int MNLGTUN_MAX_CENTS = 12800;

private:

	String filePath;
	File fileLoaded;
	String errorMessage;

	MemoryBlock tuningData;
	std::unique_ptr<XmlElement> tuningInformation;
	std::unique_ptr<XmlElement> fileInformation;

	bool isOctaveFile = false;
	
	Array<int> rawCentsTable;
	Array<double> rootedCentsTable;
	int baseNote = 60;
};	
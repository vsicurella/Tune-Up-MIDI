/*
  ==============================================================================

    MinilogueImporter.cpp
    Created: 24 Jul 2020 12:01:29pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "MinilogueImporter.h"

MinilogueImporter::MinilogueImporter(String absolutePathIn)
{
	readFile(absolutePathIn);
}

bool MinilogueImporter::readFile(String absolutePathIn)
{
	if (File::isAbsolutePath(absolutePathIn))
	{
		filePath = absolutePathIn;
		fileLoaded = File(filePath);

		if (unzipFileLoaded())
			parseIntervalTable();

		return true;
	}
	else
	{
		errorMessage = "File path is invalid.";
	}

	return false;
}

void MinilogueImporter::setBaseNote(int baseMidiNote)
{
	baseNote = baseMidiNote;
	updateRootedTable();
}

const Array<double>& MinilogueImporter::getCentsTable() const
{
	return rootedCentsTable;
}

String MinilogueImporter::getErrorMessage() const
{
	return errorMessage;
}

int MinilogueImporter::getBaseNote() const
{
	return baseNote;
}

String MinilogueImporter::getFileName() const
{
	return fileLoaded.getFileNameWithoutExtension();
}

String MinilogueImporter::getComment() const
{
	return tuningInformation->getAttributeValue(1);
}

String MinilogueImporter::getAuthor() const
{
	return tuningInformation->getAttributeValue(0);
}


bool MinilogueImporter::unzipFileLoaded()
{
	ZipFile zip(fileLoaded);
	
	if (zip.getNumEntries() == 3)
	{
		File tempFolder = File::getSpecialLocation(File::tempDirectory);
		
		for (int i = 0; i < 3; i++)
		{
			const ZipFile::ZipEntry* zipEntry = zip.getEntry(i);
			
			if (zip.uncompressEntry(i, tempFolder, true))
			{
				File tempFile = tempFolder.getChildFile(zipEntry->filename);

				if (i == 0)
				{
					tuningData.reset();
					tempFile.loadFileAsData(tuningData);
					if (tuningData.getSize() % 3 != 0)
					{
						errorMessage = "Tuning data is corrupted.";
						return false;
					}
				}
				else if (i == 1)
				{
					tuningInformation = XmlDocument::parse(tempFile);
					DBG("Loaded Tuning Information:\n" + tuningInformation->toString());
				}
				else if (i == 2)
				{
					fileInformation = XmlDocument::parse(tempFile);
					isOctaveFile = fileInformation->getChildByName("Contents")->getAttributeValue(3).getIntValue() > 0;
					DBG("Loaded File Information:\n" + fileInformation->toString());
				}
			}
			else
			{
				errorMessage = "Could not read data inside file.";
				return false;
			}
		}

		tempFolder.deleteRecursively();
		return true;
	}

	errorMessage = "File does not appear to be a valid mnlgtun file.";
	return false;
}

void MinilogueImporter::parseIntervalTable()
{
	MemoryInputStream mstream(tuningData, false);

	// convert binary data to cents
	rawCentsTable.clear();
	while (!mstream.isExhausted())
	{
		juce::uint16 hundreds = (juce::uint8)mstream.readByte() * 100;
		juce::uint16 tens = round(mstream.readShortBigEndian() / (double) 0x8000 * 100.0);
		double cents = hundreds + tens;
		rawCentsTable.add(cents);
	}
	
	if (isOctaveFile)
	{
		rawCentsTable.add(1200.0);
	}
	
	updateRootedTable();
	
	// TODO interpret tunings further
}
void MinilogueImporter::updateRootedTable()
{
	//if it's a scale tuning file, apply root note offset
	int rootCentsOffset = rawCentsTable[baseNote] * !isOctaveFile;
	rootedCentsTable.clear();
	for (auto cents : rawCentsTable)
		rootedCentsTable.add(cents - rootCentsOffset);	
}
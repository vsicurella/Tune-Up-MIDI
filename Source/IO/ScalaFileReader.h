/*
  ==============================================================================

    ScalaFileReader.h
    Created: 9 Jun 2020 12:22:20pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "TuningMath.h"

struct ScalaFile
{
	const Array<double> cents;
	const Array<double> ratios;
	
	const int numNotes;
	const double periodCents;
	const double periodRatio;

	const String fullText;
	const StringArray pitches;
	const StringArray pitchLabels;

	const String name;
	const String description;

	ScalaFile()
		: cents({}), ratios({}), numNotes(0), periodCents(0), periodRatio(0), fullText(""), pitches({}), pitchLabels({}), name(), description() {}

	ScalaFile(Array<double>& centsIn, Array<double>& ratiosIn, String nameIn, String descIn, String fullTextIn, StringArray pitchesIn, StringArray pitchLabelsIn)
		: cents(centsIn), ratios(ratiosIn),
		numNotes(centsIn.size()),
		periodCents(centsIn[numNotes - 1]),
		periodRatio(ratiosIn[numNotes - 1]),
		name(nameIn),
		description(descIn),
		fullText(fullTextIn),
		pitches(pitchesIn),
		pitchLabels(pitchLabelsIn)
	{}
};

class ScalaFileReader
{
	File fileInput;
	std::unique_ptr<FileInputStream> fstream;

	String fullText;
	StringArray tuningLines;
	StringArray tuningComments;

	String name;
	String description;
	int numNotes;

	Array<double> cents, ratios;

	ScalaFile scalaFile;

	bool read()
	{
		fstream.reset(new FileInputStream(fileInput));

		if (fstream->openedOk())
		{
			tuningLines.clear();
			tuningComments.clear();

			int descLine = -1;
			int sizeLine = -1;
			int tuningStart = -1;

			int lineNum = 0;
			int commentLines = 0;
			while (!fstream->isExhausted())
			{
				String line = fstream->readNextLine();
				String data, comment;

				data = line.upToFirstOccurrenceOf("!", false, true);
				comment = line.fromFirstOccurrenceOf("!", false, true);

				// Get Filename
				if (lineNum == 0)
				{
					name = comment;
					commentLines++;
				}

				else if (lineNum == 2 && !line.startsWithChar('!'))
				{
					description = line;
				}

				else if (lineNum == 3 && !line.startsWithChar('!'))
				{
					numNotes = data.getIntValue();
					commentLines++;
				}

				else if (line != "!" && commentLines >= 2)
				{
					// strip leading whitespace
					while (data[0] == ' ')
						data = data.fromFirstOccurrenceOf(" ", false, true);

					tuningLines.add(data);
					tuningComments.add(comment);
				}

				lineNum++;

				fullText += line + '\n';
			}

			DBG("SCALAFILEREADER| Read:\n" + fullText);

			// Setup cents and ratios
			cents.clear();
			ratios.clear();

			for (auto l : tuningLines)
			{
				IntervalType type = getIntervalType(l);
				double val;
				if (type == IntervalType::Cents)
				{
					val = l.getDoubleValue();
					cents.add(val);
					ratios.add(convertInterval(IntervalType::Cents, IntervalType::Ratio, l));
				}
				else if (type == IntervalType::Ratio)
				{
					val = parseRatio(l);
					ratios.add(val);
					cents.add(ratioToCents(val));
				}
				else
				{
					DBG("Invalid pitch value in file.");
					return false;
				}
			}

			if (!(ratios.size() == cents.size() && cents.size() == numNotes))
			{
				DBG("Tuning sizes do not match.");
				return false;
			}

			return true;
		}

		return false;
	}

public:
	
	bool open(File newFileIn)
	{
		fileInput = newFileIn;
		return read();
	}

	ScalaFile getScalaFile()
	{
		return ScalaFile(cents, ratios, name, description, fullText, tuningLines, tuningComments);
	}
};


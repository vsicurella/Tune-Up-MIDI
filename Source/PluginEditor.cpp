/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TuneupMidiAudioProcessorEditor::TuneupMidiAudioProcessorEditor (
	TuneupMidiAudioProcessor& p, 
	TuneUpMidiProcessor& mp, 
	TuneUpMidiState& ps
)
    : AudioProcessorEditor (&p), processor (p), midiProcessor(mp), pluginState(ps),
	grid(23)
{
	// CONTROL WINDOWS

	mainWindow.reset(new TuneUpWindow());
	addChildComponent(mainWindow.get());

	createTuningWindow.reset(new CreateTuningWindow(pluginState.getTuningDefinition()));
	addChildComponent(createTuningWindow.get());
	createTuningWindow->addChangeListener(this);

	generalOptionsWindow.reset(new GeneralOptionsWindow(ValueTree()));
	addChildComponent(generalOptionsWindow.get());
	generalOptionsWindow->addListener(this);

	controlWindows = {
		mainWindow.get(),
		createTuningWindow.get(),
		tuningBrowserWindow.get(),
		generalOptionsWindow.get(),
		toolboxOptionsWindow.get(),
		dynamicOptionsWindow.get()
	};

	// BUTTONS

	newTuningButton.reset(new TextButton("New"));
	addChildComponent(newTuningButton.get());
	newTuningButton->setButtonText(TRANS("New"));
	newTuningButton->addListener(this);

	loadTuningButton.reset(new TextButton("Load"));
	addChildComponent(loadTuningButton.get());
	loadTuningButton->setButtonText(TRANS("Load"));
	loadTuningButton->addListener(this);

	viewButton.reset(new TextButton("View"));
	addChildComponent(viewButton.get());
	viewButton->setButtonText(TRANS("View"));
	viewButton->addListener(this);

	optionsButton.reset(new TextButton("Options"));
	addChildComponent(optionsButton.get());
	optionsButton->setButtonText(TRANS("Options"));
	optionsButton->addListener(this);

	dynamicToggleButton.reset(new TextButton("Dynamic"));
	addChildComponent(dynamicToggleButton.get());
	dynamicToggleButton->setButtonText(TRANS("Dynamic"));
	dynamicToggleButton->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnRight);
	dynamicToggleButton->addListener(this);

	dynamicOptionsButton.reset(new TextButton("dynamicOptionsButton"));
	addChildComponent(dynamicOptionsButton.get());
	dynamicOptionsButton->setButtonText("O");
	dynamicOptionsButton->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);
	dynamicOptionsButton->addListener(this);

	backButton.reset(new TextButton(backTrans));
	addChildComponent(backButton.get());
	backButton->setButtonText(backTrans);
	backButton->addListener(this);

	saveButton.reset(new TextButton(saveTrans));
	addChildComponent(saveButton.get());
	saveButton->setButtonText(saveTrans);
	saveButton->addListener(this);

	generalOptionsButton.reset(new TextButton(generalTrans));
	addChildComponent(generalOptionsButton.get());
	generalOptionsButton->setButtonText(generalTrans);
	generalOptionsButton->addListener(this);

	toolboxButton.reset(new TextButton(toolsTrans));
	addChildComponent(toolboxButton.get());
	toolboxButton->setButtonText(toolsTrans);
	toolboxButton->addListener(this);

	newTuningModeBox.reset(new ComboBox("ModeBox"));
	addChildComponent(newTuningModeBox.get());
	newTuningModeBox->addItem("Equal Temperament", CreateTuningWindow::EqualTemperament);
	newTuningModeBox->addItem("Regular Temperament", CreateTuningWindow::RegularTemperament);
	newTuningModeBox->addListener(this);
	newTuningModeBox->setSelectedId(CreateTuningWindow::EqualTemperament);

	// BUTTON BARS

	buttonBarComponents = {
		newTuningButton.get(),
		loadTuningButton.get(),
		viewButton.get(),
		optionsButton.get(),
		dynamicToggleButton.get(),
		dynamicOptionsButton.get(),
		backButton.get(),
		saveButton.get(),
		newTuningModeBox.get()
	};

	buttonBars.resize(6);
	for (int i = 0; i < buttonBars.size(); i++)
	{
		ButtonBar& bar = buttonBars.getReference(i);
		bar.setGapSize(componentGap);

		switch (i)
		{
		case MainWindowMode:
			bar.addComponents(
				{ newTuningButton.get(), loadTuningButton.get(), viewButton.get(),
				  optionsButton.get(), dynamicToggleButton.get(), dynamicOptionsButton.get() },
				{ 0, 0, 0, 5, 5, 2 }
			);
			break;

		case NewTuningMode:
			bar.addComponents(
				{ backButton.get(), saveButton.get(), viewButton.get(), newTuningModeBox.get() },
				{ 0, 0, 0, 11 }
			);
			break;


		case LoadTuningMode:

			break;

		case DynamicOptions:

			break;

		default: // General and Toolbox Options
			bar.addComponents(
				{ backButton.get(), viewButton.get(), generalOptionsButton.get(), toolboxButton.get() }
			);
			break;

		}
	}

    setSize (500, 200);

	if (pluginState.getTuning())
	{
		mainWindow->loadTuning(pluginState.getTuning());
	}

	pluginState.addChangeListener(this);

	setControlMode(MainWindowMode);

	// TEMPORARY
	dynamicOptionsButton->setEnabled(false);
	dynamicToggleButton->setEnabled(false);
	viewButton->setEnabled(false);
	toolboxButton->setEnabled(false);
}

TuneupMidiAudioProcessorEditor::~TuneupMidiAudioProcessorEditor()
{
	// Revert to last saved settings if unsaved changes
	if (currentMode == NewTuningMode)
		loadTuning(lastTuningDefinition);

	mainWindow = nullptr;
	createTuningWindow = nullptr;
	tuningBrowserWindow = nullptr;
	generalOptionsWindow = nullptr;
	toolboxOptionsWindow = nullptr;
	dynamicOptionsWindow = nullptr;

	newTuningButton = nullptr;
	loadTuningButton = nullptr;
	viewButton = nullptr;
	optionsButton = nullptr;
	dynamicToggleButton = nullptr;
	dynamicOptionsButton = nullptr;
	backButton = nullptr;
	saveButton = nullptr;
	newTuningModeBox = nullptr;
}

//==============================================================================
void TuneupMidiAudioProcessorEditor::paint (Graphics& g)
{
	g.setColour(Colour(0xff323e44));
	g.fillAll();

	// toolbar background
	g.setColour(Colour(0xff323e44).darker(0.75f));
	g.fillRect(0, controlWindows[currentMode]->getBottom() + componentGap, getWidth(), barHeight);
}

void TuneupMidiAudioProcessorEditor::resized()
{
	grid.setSize(getHeight());
	barHeight = grid.getUnit(3);

	// Button Bar
	ButtonBar& bar = buttonBars.getReference(currentMode);
	Array<Component*> components = bar.getComponents();

	int barWidth = getWidth() - 2 * borderGap;
	switch (currentMode)
	{
	case MainWindowMode:
		barWidth -= 4 * componentGap;
		break;

	default:
		barWidth -= (components.size() - 1) * componentGap;
	}

	bar.setBounds(borderGap, borderGap, barWidth, barHeight);

	Component* c;
	for (int i = 0; i < components.size(); i++)
	{
		c = components[i];
		bool gap = true;

		if (currentMode == MainWindowMode && c->getName() == "dynamicOptionsButton")
			gap = false; // could put this in a ButtonBarComponent base class

		c->setBounds(bar.getComponentBounds(c, gap).toNearestInt());
	}
	

	// Control Windows
	BorderSize<int> controlBorder(bar.getBounds().getBottom() + componentGap, borderGap, barHeight + componentGap, borderGap);
	controlWindows[currentMode]->setBoundsInset(controlBorder);
}

void TuneupMidiAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &pluginState)
	{
		mainWindow->loadTuning(pluginState.getTuning());
	}

	else if (source == createTuningWindow.get())
	{
		loadTuning(createTuningWindow->getTuningDefinition());
	}
}

void TuneupMidiAudioProcessorEditor::buttonClicked(Button* buttonClicked)
{
	if (buttonClicked == backButton.get())
	{
		if (currentMode == NewTuningMode)
		{
			// Reset tuning
			loadTuning(lastTuningDefinition);
		}

		else
		{

		}

		setControlMode(MainWindowMode);
	}

	else if (buttonClicked == saveButton.get())
	{
		if (currentMode == NewTuningMode)
		{
			createTuningWindow->updateTuning();
		}

		setControlMode(MainWindowMode);
	}

	else if (buttonClicked == newTuningButton.get())
	{
		setControlMode(NewTuningMode);
	}

	else if (buttonClicked == loadTuningButton.get())
	{
		FileChooser chooser("Select your tuning file", File::getSpecialLocation(File::userDocumentsDirectory));
		chooser.browseForFileToOpen();
		loadedFile = chooser.getResult();

		onFileLoad();
	}

	else if (buttonClicked == optionsButton.get())
	{
		setControlMode(GeneralOptions);
	}
}

void TuneupMidiAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatChanged)
{
	if (comboBoxThatChanged == newTuningModeBox.get())
	{
		createTuningWindow->setMode(CreateTuningWindow::TuningMode(newTuningModeBox->getSelectedId()));
	}
}

void TuneupMidiAudioProcessorEditor::onFileLoad()
{
	bool success = scalaFileReader.open(loadedFile);

	if (success)
	{
		ScalaFile file = scalaFileReader.getScalaFile();
		loadTuning(TuningDefinition::createStaticTuningDefinition(file.cents, 60, file.name, file.description));
	}
	else
	{
		// TODO : Error Loading Scale Alert
	}

}

void TuneupMidiAudioProcessorEditor::loadTuning(ValueTree tuningDefinition)
{
	pluginState.setNewTuning(tuningDefinition);
	mainWindow->loadTuning(pluginState.getTuning());

	DBG("Tuning Loaded: \n" + tuningDefinition.toXmlString());
}

void TuneupMidiAudioProcessorEditor::setControlMode(ControlMode modeIn)
{
	for (auto c : buttonBars.getReference(currentMode).getComponents())
		c->setVisible(false);

	controlWindows[currentMode]->setVisible(false);

	currentMode = modeIn;

	// Functional stuff
	switch (currentMode)
	{
	case NewTuningMode:
	{
		midiProcessor.resetNotes();
		// backup current scale definition
		lastTuningDefinition = pluginState.getTuningDefinition().createCopy();

		createTuningWindow->updateTuning();
		break;
	}

	case GeneralOptions:
	{ 
		generalOptionsButton->setEnabled(false);
		toolboxButton->setEnabled(true);

		break;
	}

	case ToolbarOptions:
	{
		generalOptionsButton->setEnabled(true);
		toolboxButton->setEnabled(false);
	}

	default: // MainWindow
	{
		break;
	}
	}

	for (auto c : buttonBars.getReference(currentMode).getComponents())
		c->setVisible(true);

	controlWindows[currentMode]->setVisible(true);

	resized();
}
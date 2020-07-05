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
	grid(13)
{
	// CONTROL WINDOWS

	mainWindow.reset(new TuneUpWindow());
	addChildComponent(mainWindow.get());

	createScaleWindow.reset(new CreateScaleWindow(pluginState.getTuningDefinition()));
	addChildComponent(createScaleWindow.get());
	createScaleWindow->addChangeListener(this);

	controlWindows = {
		mainWindow.get(),
		createScaleWindow.get(),
		tuningBrowserWindow.get(),
		generalOptionsWindow.get(),
		toolboxOptionsWindow.get(),
		dynamicOptionsWindow.get()
	};

	// BUTTONS

	newScaleButton.reset(new TextButton("New"));
	addChildComponent(newScaleButton.get());
	newScaleButton->setButtonText(TRANS("New"));
	newScaleButton->addListener(this);

	loadScaleButton.reset(new TextButton("Load"));
	addChildComponent(loadScaleButton.get());
	loadScaleButton->setButtonText(TRANS("Load"));
	loadScaleButton->addListener(this);

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
	newTuningModeBox->addItem("Equal Temperament", CreateScaleWindow::EqualTemperament);
	newTuningModeBox->addItem("Regular Temperament", CreateScaleWindow::RegularTemperament);
	newTuningModeBox->addListener(this);
	newTuningModeBox->setSelectedId(CreateScaleWindow::EqualTemperament);

	// BUTTON BARS

	buttonBarComponents = {
		newScaleButton.get(),
		loadScaleButton.get(),
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
				{ newScaleButton.get(), loadScaleButton.get(), viewButton.get(),
				  optionsButton.get(), dynamicToggleButton.get(), dynamicOptionsButton.get() },
				{ 0, 0, 0, 5, 5, 2 }
			);
			break;

		case NewScaleMode:
			bar.addComponents(
				{ backButton.get(), saveButton.get(), viewButton.get(), newTuningModeBox.get() },
				{ 0, 0, 0, 11 }
			);
			break;


		case LoadScaleMode:

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
	if (currentMode == NewScaleMode)
		loadTuning(lastTuningDefinition);

	mainWindow = nullptr;
	createScaleWindow = nullptr;
	tuningBrowserWindow = nullptr;
	generalOptionsWindow = nullptr;
	toolboxOptionsWindow = nullptr;
	dynamicOptionsWindow = nullptr;

	newScaleButton = nullptr;
	loadScaleButton = nullptr;
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
	g.fillRect(0, grid.getUnit(11), getWidth(), grid.getUnit(2));
}

void TuneupMidiAudioProcessorEditor::resized()
{
	grid.setSize(getHeight());

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
		barWidth -= components.size() * componentGap;
	}

	bar.setBounds(borderGap, borderGap, barWidth, grid.getUnit(2));

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
	BorderSize<int> controlBorder(bar.getBounds().getBottom() + componentGap, borderGap, grid.getUnit(2) + componentGap, borderGap);
	controlWindows[currentMode]->setBoundsInset(controlBorder);
}

void TuneupMidiAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &pluginState)
	{
		mainWindow->loadTuning(pluginState.getTuning());
	}

	else if (source == createScaleWindow.get())
	{
		loadTuning(createScaleWindow->getTuningDefinition());
	}
}

void TuneupMidiAudioProcessorEditor::buttonClicked(Button* buttonClicked)
{
	if (buttonClicked == backButton.get())
	{
		if (currentMode == NewScaleMode)
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
		if (currentMode == NewScaleMode)
		{
			createScaleWindow->updateTuning();
		}

		setControlMode(MainWindowMode);
	}

	else if (buttonClicked == newScaleButton.get())
	{
		setControlMode(NewScaleMode);
	}

	else if (buttonClicked == loadScaleButton.get())
	{
		FileChooser chooser("Select your tuning file", File::getSpecialLocation(File::userDocumentsDirectory));
		chooser.browseForFileToOpen();
		loadedFile = chooser.getResult();

		onFileLoad();
	}

}

void TuneupMidiAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatChanged)
{
	if (comboBoxThatChanged == newTuningModeBox.get())
	{
		createScaleWindow->setMode(CreateScaleWindow::ScaleMode(newTuningModeBox->getSelectedId()));
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
	currentMode = modeIn;

	// Functional stuff
	switch (currentMode)
	{
	case NewScaleMode:
	{
		midiProcessor.resetNotes();
		// backup current scale definition
		lastTuningDefinition = pluginState.getTuningDefinition().createCopy();

		createScaleWindow->updateTuning();
		break;
	}

	case GeneralOptions:
	{
		break;
	}

	default: // MainWindow
	{
		break;
	}
	}

	// Button Bars
	for (auto c : buttonBarComponents)
		c->setVisible(false);

	for (auto c : buttonBars.getReference(currentMode).getComponents())
		c->setVisible(true);

	// GUI stuff
	for (auto window : controlWindows)
		if (window) // temporary while not all implemented
			window->setVisible(false);

	controlWindows[currentMode]->setVisible(true);

	resized();
}
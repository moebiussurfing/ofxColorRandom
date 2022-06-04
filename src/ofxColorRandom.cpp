#include "ofxColorRandom.h"


//--

// sorting map tools
// comparing colors to sorting methods
//--------------------------------------------------------------
bool compareName(const colorMapping_STRUCT& s1, const colorMapping_STRUCT& s2)
{
	return s1.name < s2.name;
}
bool compareBrightness(const colorMapping_STRUCT& s1, const colorMapping_STRUCT& s2)
{
	return s1.color.getBrightness() < s2.color.getBrightness();
}
bool compareHue(const colorMapping_STRUCT& s1, const colorMapping_STRUCT& s2)
{
	return s1.color.getHue() < s2.color.getHue();
}
bool compareSaturation(const colorMapping_STRUCT& s1, const colorMapping_STRUCT& s2)
{
	return s1.color.getSaturation() < s2.color.getSaturation();
}
bool comparePosition(const colorMapping_STRUCT& s1, const colorMapping_STRUCT& s2)
{
	return s1.position < s2.position;
}

//--

//--------------------------------------------------------------
ofxColorRandom::ofxColorRandom()
{
	ofxSurfingHelpers::setThemeDark_ofxGui();

	path_Global = "ofxColorRandom/";

	ofAddListener(ofEvents().keyPressed, this, &ofxColorRandom::keyPressed);
}

//--------------------------------------------------------------
ofxColorRandom::~ofxColorRandom()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxColorRandom::keyPressed);

	ofAddListener(params.parameterChangedE(), this, &ofxColorRandom::Changed_Params);

	exit();
}

//--------------------------------------------------------------
void ofxColorRandom::setupParams()
{
	params.setName("ofxColorRandom");

	params.add(index_Sorting);
	params.add(name_Sorting);

	params.add(index_CallOptions);
	params.add(name_CallOptions);

	// layout
	params_Layout.setName("Layout Gui Internal");
	params_Layout.add(amountColors);
	params_Layout.add(boxSize);
	params_Layout.add(boxPad);

	params_GuiPanels.setName("Panels");
	params_GuiPanels.add(bGui_Card);
	params_GuiPanels.add(position_Palette);

	params.add(params_GuiPanels);
	params.add(params_Layout);

	//params_Layout.add(bLockLayout);
	//params_Layout.add(bResetColorsBox);

	ofAddListener(params.parameterChangedE(), this, &ofxColorRandom::Changed_Params);

	//-

	// exclude
	name_Sorting.setSerializable(false);
	name_CallOptions.setSerializable(false);
	bGui_Card.setSerializable(false);
}

//--------------------------------------------------------------
void ofxColorRandom::setup()
{
	path_FileSettings = path_Global + "AppSettings.xml";
	ofxSurfingHelpers::CheckFolder(path_Global);
	
	setupParams();

	//--

	gui.setup("ofxColorRandom");
	gui.add(params);
	gui.setPosition(20, 20);

	//--

	//--------------------------------------------------------------
	listener_ModeSorting = index_Sorting.newListener([this](int& i)
		{
			ofLogNotice("index_Sorting: ") << i;

			buildMapFromPalette();

			if (index_Sorting == 5) index_Sorting = 0;
			index_Sorting = ofClamp(index_Sorting, 0, 4);

			if (index_Sorting == 0) { ofSort(colors_STRUCT, comparePosition); name_Sorting = "Original"; }
			else if (index_Sorting == 1) { ofSort(colors_STRUCT, compareName); name_Sorting = "Name"; }
			else if (index_Sorting == 2) { ofSort(colors_STRUCT, compareHue); name_Sorting = "Hue"; }
			else if (index_Sorting == 3) { ofSort(colors_STRUCT, compareBrightness); name_Sorting = "Brightness"; }
			else if (index_Sorting == 4) { ofSort(colors_STRUCT, compareSaturation); name_Sorting = "Saturation"; }

			buildPaletteFromMap();
		});

	// 1. default sorting
	// by name
	index_Sorting = SORTING_ORIGINAL;

	//--

	//--------------------------------------------------------------
	listener_Library = index_CallOptions.newListener([this](int& i)
		{
			ofLogNotice("index_CallOptions: ") << i;

			index_CallOptions = ofClamp(index_CallOptions, index_CallOptions.getMin(), index_CallOptions.getMax());

			switch (index_CallOptions)
			{
			case 0: name_CallOptions = "Deuteranopia"; break;
			case 1: name_CallOptions = "Normal"; break;
			case 2: name_CallOptions = "TEST"; break;
				
			//.. add more here
			}
		});

	//TODO: modify hard-coded when adding modes!
	index_CallOptions.setMin(0);
	index_CallOptions.setMax(2);

	//----

	// startup
	startup();
}

//--------------------------------------------------------------
void ofxColorRandom::startup() {

	position_Palette = glm::vec2(20, ofGetHeight() / 2);

	// settings
	ofxSurfingHelpers::loadGroup(params, path_FileSettings);

	getPaletteType();
	index_Sorting = index_Sorting;
}

//--------------------------------------------------------------
void ofxColorRandom::update() {
}

//--------------------------------------------------------------
void ofxColorRandom::draw() {

	drawCard();

	//--

	gui.draw();
}

//--------------------------------------------------------------
void ofxColorRandom::keyPressed(ofKeyEventArgs& eventArgs)
{
	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	ofLogNotice(__FUNCTION__) << " : " << key;

	//--

	if (0) {}

	else if (key == OF_KEY_TAB)
	{
		setNextSortType();
	}

	else if (key == '-')
	{
		amountColors--;
		getPaletteType();
	}
	else if (key == '+')
	{
		amountColors++;
		getPaletteType();
	}

	else if (key == ' ')
	{
		getPaletteType();
	}

	else if (key == '0')
	{
		getPaletteType(0);
	}

	else if (key == '1')
	{
		getPaletteType(1);
	}

	else if (key == '2')
	{
		getPaletteType(2);
	}

	//--

	/*
	{
		// Example: saves an html file with the colors
		std::ofstream myfile;
		myfile.open("example.html");
		auto g = colorgeneratornormal();

		palettecard.clear();

		for (int i = 0; i < amtcolorsincard; i++)
		{
			std::tuple<int, int, int> r = g();
			std::cout << std::get<0>(r) << "," << std::get<1>(r) << "," << std::get<2>(r) << std::endl;

			ofcolor c = ofcolor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			palettecard.push_back(c);

			myfile << r"(
		<h2 style = 'color:rgba(0,0,0,0);background-color:rgb()"
				<< std::get<0>(r) << "," << std::get<1>(r) << "," << std::get<2>(r) << r"()'>
				.
			</h2>
		)";
		}

		myfile.close();
	}
	*/
}

//--------------------------------------------------------------
void ofxColorRandom::exit() {
	ofxSurfingHelpers::saveGroup(params, path_FileSettings);
}

//--------------------------------------------------------------
void ofxColorRandom::drawCard()
{
	// 1 card of colors

	// draw selected card.

	if (bGui_Card)
	{
		int padding = 15;
		int labelSize = 25;

		int yBg = position_Palette.get().y - padding;
		int hBg = (boxSize + boxPad) + 2 * padding + labelSize;
		int ymaxBg = yBg + hBg;

		ofColor colorBgCards = ofColor(250);

		ofPushStyle();
		ofFill();

		for (int i = 0; i < paletteGenerated.size(); i++)
		{
			ofColor c = paletteGenerated[i];
			ofSetColor(c);
			float round = 2.5f;
			ofDrawRectRounded(
				glm::vec2(position_Palette.get().x + i * (boxSize + boxPad), position_Palette.get().y),
				boxSize, boxSize, round);
		}

		ofPopStyle();
	}
}

//--------------------------------------------------------------
int ofxColorRandom::getAmountcolors()
{
	ofLogNotice(__FUNCTION__) << colors_STRUCT.size();
	return colors_STRUCT.size();
}

//--------------------------------------------------------------
void ofxColorRandom::Changed_Params(ofAbstractParameter& e)
{
	//if (!DISABLE_Callbacks)
	{
		string name = e.getName();
		ofLogNotice(__FUNCTION__) << name << " : " << e;


		//-

		// gui panels

		if (0) {}

		//// reset box
		//else if (name == bResetColorsBox.getName() && bResetColorsBox)
		//{
		//	bResetColorsBox = false;
		//	//boxes
		//	boxSize = 30;
		//	boxPad = 0;
		//	// minimal card of colors
		//	amountColors = 7;
		//	amtCardsInRow = 4;
		//}
	}
}

//--------------------------------------------------------------
void ofxColorRandom::getPaletteType(int type)
{
	if (type == -1)//if no args use the last one!
	{
		type = index_CallOptions;
	}

	//--

	if (index_CallOptions != type) index_CallOptions.set(type);

	if (index_CallOptions == 0)
	{
		auto G = ColorGeneratorNormal();

		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}

	else if (index_CallOptions == 1)
	{
		auto G = ColorGeneratorDeuteranopia();

		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}

	else if (index_CallOptions == 2)
	{
		auto G = ColorGeneratorTEST();

		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
}

//--------------------------------------------------------------
void ofxColorRandom::buildMapFromPalette()
{
	colors_STRUCT.clear();

	std::string name = "";
	ofColor c;

	for (int i = 0; i < paletteGenerated.size(); i++)
	{
		c = paletteGenerated[i];
		name = name_CallOptions.get() + "-" + ofToString(i);
		{
			// 1. names map
			colors_NamesMAP[name] = c;

			// 2. struct
			colorMapping_STRUCT myColor;
			myColor.name = name;
			myColor.color = c;
			myColor.position = i;

			// 3. add color to vector
			colors_STRUCT.push_back(myColor);
		}
	}
}

//--------------------------------------------------------------
void ofxColorRandom::buildPaletteFromMap()
{
	paletteGenerated.clear();

	ofColor c;

	for (int i = 0; i < colors_STRUCT.size(); i++)
	{
		c = colors_STRUCT[i].color;
		paletteGenerated.push_back(c);
	}
}
//--------------------------------------------------------------
void ofxColorRandom::setNextSortType()
{
	index_Sorting++;
}
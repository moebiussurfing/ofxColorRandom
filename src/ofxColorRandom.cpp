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

	//params.add(bGui_Card);
	params.add(amountColors);
	params.add(hue);
	params.add(hue_Name);
	params.add(luminosity);
	params.add(luminosity_Name);
	params.add(sorting);
	params.add(sorting_Name);

	params_Layout.setName("Layout");
	params_Layout.add(boxSize);
	params_Layout.add(boxPad);
	params.add(params_Layout);

	ofAddListener(params.parameterChangedE(), this, &ofxColorRandom::Changed_Params);

	//--

	// exclude
	hue_Name.setSerializable(false);
	luminosity_Name.setSerializable(false);
	sorting_Name.setSerializable(false);

	//--

	gui.setup("ofxColorRandom");
	gui.add(params);
	gui.setPosition(20, 200);

	auto &g0 = gui.getGroup(params.getName());
	g0.getGroup(params_Layout.getName()).minimize();

	position_Palette = glm::vec2(20, 20);
}

//--------------------------------------------------------------
void ofxColorRandom::setup()
{
	path_FileSettings = path_Global + "AppSettings.xml";
	ofxSurfingHelpers::CheckFolder(path_Global);

	setupParams();

	//--

	//--------------------------------------------------------------
	listener_Hue = hue.newListener([this](int& i)
		{
			hue_Name = getHueName(i);

			ofLogNotice("hue: ") << i << " " << hue_Name;

			//getPaletteTypeHue();
			//doSortPalette();

			//--

			//TODO: improve passing vars to lambdas... I don't know how to do it...
			// 
			//RandomColor::HUENAMES _hue;
			//RandomColor::LUMINOSITY _luminosity;
			//switch (i)
			//{
			//case 0: return "UNSET"; break;
			//case 1: return "MONOCHROME"; break;
			//case 2: return "RED"; break;
			//case 3: return "ORANGE"; break;
			//case 4: return "YELLOW"; break;
			//case 5: return "GREEN"; break;
			//case 6: return "BLUE"; break;
			//case 7: return "PURPLE"; break;
			//case 8: return "PINK"; break;
			//default: return "UNKNOWN"; break;
			//}
		}
	);

	//--------------------------------------------------------------
	listener_Luminosity = luminosity.newListener([this](int& i)
		{
			luminosity_Name = getLuminosityName(i);

			ofLogNotice("luminosity: ") << i << " " << luminosity_Name;

			//getPaletteTypeLuminosity();
			//doSortPalette();
		}
	);

	//--------------------------------------------------------------
	listener_Sorting = sorting.newListener([this](int& i)
		{
			if (paletteGenerated.size() < 1) return;

			ofLogNotice("sorting: ") << i;

			doSortPalette();
		});

	// 1. default sorting
	// by name
	sorting = SORTING_ORIGINAL;

	//----

	// startup
	startup();
}

//--------------------------------------------------------------
void ofxColorRandom::doSortPalette() {

	ofLogNotice(__FUNCTION__);

	buildMapFromPalette();

	if (sorting == 5) sorting = 0;
	sorting = ofClamp(sorting, 0, 4);

	if (sorting == 0) { ofSort(colors_STRUCT, comparePosition); sorting_Name = "Original"; }
	else if (sorting == 1) { ofSort(colors_STRUCT, compareName); sorting_Name = "Name"; }
	else if (sorting == 2) { ofSort(colors_STRUCT, compareHue); sorting_Name = "Hue"; }
	else if (sorting == 3) { ofSort(colors_STRUCT, compareBrightness); sorting_Name = "Brightness"; }
	else if (sorting == 4) { ofSort(colors_STRUCT, compareSaturation); sorting_Name = "Saturation"; }

	buildPaletteFromMap();
}

//--------------------------------------------------------------
void ofxColorRandom::startup() {

	// settings
	ofxSurfingHelpers::loadGroup(params, path_FileSettings);

	if (hue < 2) hue = 2;//0 & 1 are broken..
	getPaletteTypeHue();
	doSortPalette();
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

	else if (key == OF_KEY_RIGHT)
	{
		int i = hue;
		i++;
		i = i % (hue.getMax() +1);
		if (i < 2)i = 2;
		hue = i;

		getPaletteTypeHue();
		doSortPalette();
	}

	else if (key == OF_KEY_TAB)
	{
		setNextSortType();
	}

	else if (key == '-')
	{
		amountColors--;
		getPaletteTypeHue();
	}
	else if (key == '+')
	{
		amountColors++;
		getPaletteTypeHue();
	}

	else if (key == ' ')
	{
		getPaletteTypeHue();
		doSortPalette();
	}

	else if (key == OF_KEY_RETURN)
	{
		getPaletteTypeLuminosity();
		doSortPalette();
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
	//if (bGui_Card)
	{
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
	////if (!DISABLE_Callbacks)
	//{
	//	string name = e.getName();
	//	ofLogNotice(__FUNCTION__) << name << " : " << e;

	//	//--

	//	// gui panels

	//	if (0) {}

	//	//// reset box
	//	//else if (name == bResetColorsBox.getName() && bResetColorsBox)
	//	//{
	//	//	bResetColorsBox = false;
	//	//	//boxes
	//	//	boxSize = 30;
	//	//	boxPad = 0;
	//	//	// minimal card of colors
	//	//	amountColors = 7;
	//	//	amtCardsInRow = 4;
	//	//}
	//}
}

//--------------------------------------------------------------
void ofxColorRandom::getPaletteTypeHue()
{
	switch (hue.get())
	{

	case 0:
	{
		break;//broken

		auto G = ColorGenerator_UNSET();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 1:
	{
		break;//broken

		auto G = ColorGenerator_MONOCHROME();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 2:
	{
		auto G = ColorGenerator_RED();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 3:
	{
		auto G = ColorGenerator_ORANGE();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 4:
	{
		auto G = ColorGenerator_YELLOW();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 5:
	{
		auto G = ColorGenerator_GREEN();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 6:
	{
		auto G = ColorGenerator_BLUE();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 7:
	{
		auto G = ColorGenerator_PURPLE();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 8:
	{
		auto G = ColorGenerator_PINK();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	}

}

//--------------------------------------------------------------
void ofxColorRandom::getPaletteTypeLuminosity()
{
	switch (luminosity.get())
	{

	case 0:
	{
		auto G = ColorGenerator_LUM_RANDOM();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 1:
	{
		auto G = ColorGenerator_LUM_BRIGHT();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 2:
	{
		auto G = ColorGenerator_LUM_LIGHT();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;

	case 3:
	{
		auto G = ColorGenerator_LUM_DARK();
		paletteGenerated.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteGenerated.push_back(c);
		}
	}
	break;
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

		name = ofToString(i);
		//name = name_CallOptions.get() + "-" + ofToString(i);

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
	sorting++;
}
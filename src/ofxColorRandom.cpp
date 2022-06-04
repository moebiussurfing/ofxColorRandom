#include "ofxColorRandom.h"


//--

// Sorting map tools
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

	//params.add(bGui);
	params.add(amountColors);
	params.add(hue);
	params.add(hue_Name);
	params.add(luminosity);
	params.add(luminosity_Name);
	params.add(sorting);
	params.add(sorting_Name);

	params_Layout.setName("LAYOUT");
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

	auto& g0 = gui.getGroup(params.getName());
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

			buildPalette();
		}
	);

	//--------------------------------------------------------------
	listener_Luminosity = luminosity.newListener([this](int& i)
		{
			luminosity_Name = getLuminosityName(i);

			ofLogNotice("luminosity: ") << i << " " << luminosity_Name;

			buildPalette();
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
ofColor ofxColorRandom::getRandomColor() {

	ofLogNotice(__FUNCTION__);

	ofColor c;

	//srand((int)time(NULL));

	RandomColor::Options o;

	o.seed = rand() % 1000;

	switch (hue)
	{
	case 0: o.hue_name = RandomColor::UNSET; break;
	case 1: o.hue_name = RandomColor::MONOCHROME; break;
	case 2: o.hue_name = RandomColor::RED; break;
	case 3: o.hue_name = RandomColor::ORANGE; break;
	case 4: o.hue_name = RandomColor::YELLOW; break;
	case 5: o.hue_name = RandomColor::GREEN; break;
	case 6: o.hue_name = RandomColor::BLUE; break;
	case 7: o.hue_name = RandomColor::PURPLE; break;
	case 8: o.hue_name = RandomColor::PINK; break;
	}

	switch (luminosity)
	{
	case 0: o.luminosity = RandomColor::LUMINOSITY::RANDOM; break;
	case 1: o.luminosity = RandomColor::LUMINOSITY::BRIGHT; break;
	case 2: o.luminosity = RandomColor::LUMINOSITY::LIGHT; break;
	case 3: o.luminosity = RandomColor::LUMINOSITY::DARK; break;
	}

	auto RG = RandomColor::RandomColorGenerator(o);
	auto r = RG.randomColorRGB();

	c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));

	ofLogNotice(__FUNCTION__) << " : " << c;

	return c;
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

	buildPalette();
}

//--------------------------------------------------------------
void ofxColorRandom::update() {
}

//--------------------------------------------------------------
void ofxColorRandom::draw() {
	ofBackground(colorBg);

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

	else if (key == ' ')
	{
		buildPalette();
	}

	else if (key == OF_KEY_F1)
	{
		colorBg = getRandomColor();
	}

	else if (key == 'H')
	{
		int i = hue;
		i++;
		i = i % (hue.getMax() + 1);
		hue = i;

		buildPalette();
	}

	else if (key == 'L')
	{
		int i = luminosity;
		i++;
		i = i % (luminosity.getMax() + 1);
		luminosity = i;

		buildPalette();
	}

	else if (key == OF_KEY_TAB)
	{
		setNextSortType();
	}

	else if (key == '-')
	{
		amountColors--;
	}
	else if (key == '+')
	{
		amountColors++;
	}
}

//--------------------------------------------------------------
void ofxColorRandom::exit() {
	ofxSurfingHelpers::saveGroup(params, path_FileSettings);
}

//--------------------------------------------------------------
void ofxColorRandom::drawCard()
{
	if (bGui)
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
	//if (DISABLE_Callbacks) return;

	string name = e.getName();
	ofLogNotice(__FUNCTION__) << name << " : " << e;

	//--

	if (0) {}

	else if (name == amountColors.getName())
	{
		buildPalette();
	}
}

//--------------------------------------------------------------
void ofxColorRandom::buildPalette(bool bSort)
{
	paletteGenerated.clear();
	for (size_t i = 0; i < amountColors; i++)
	{
		paletteGenerated.push_back(getRandomColor());
	}

	if (bSort) doSortPalette();
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
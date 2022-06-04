#include "ofxColorRandom.h"

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
}

//--------------------------------------------------------------
void ofxColorRandom::setup()
{
	path_FileSettings = path_Global + "AppSettings.xml";
	ofxSurfingHelpers::CheckFolder(path_Global);

	//-

	// exclude
	MODE_SORTING_name.setSerializable(false);
	name_CallOptions.setSerializable(false);
	bGui_Card.setSerializable(false);

	//--

	// params
	params.setName("ofxColorRandom");

	params.add(MODE_SORTING);
	params.add(MODE_SORTING_name);

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

	//--

	gui.setup("ofxColorRandom");
	gui.add(params);
	gui.setPosition(20, 20);

	//--

	//--------------------------------------------------------------
	listener_ModeSorting = MODE_SORTING.newListener([this](int& i)
		{
			//ofLogNotice("MODE_SORTING: ") << i;

			//if (MODE_SORTING == 5) MODE_SORTING = 0;
			//MODE_SORTING = ofClamp(MODE_SORTING, 0, 4);

			//if (MODE_SORTING == 0) { ofSort(colors_STRUCT, comparePosition); MODE_SORTING_name = "Original"; }
			//else if (MODE_SORTING == 1) { ofSort(colors_STRUCT, compareName); MODE_SORTING_name = "Name"; }
			//else if (MODE_SORTING == 2) { ofSort(colors_STRUCT, compareHue); MODE_SORTING_name = "Hue"; }
			//else if (MODE_SORTING == 3) { ofSort(colors_STRUCT, compareBrightness); MODE_SORTING_name = "Brightness"; }
			//else if (MODE_SORTING == 4) { ofSort(colors_STRUCT, compareSaturation); MODE_SORTING_name = "Saturation"; }

			////clearRectangles();
			////buildRectangles();
		});

	// 1. default sorting
	MODE_SORTING = SORTING_ORIGINAL;
	// by name, at the start

	//--

	index_CallOptions.setMax(2);

	//--------------------------------------------------------------
	listener_Library = index_CallOptions.newListener([this](int& i)
		{
			ofLogNotice("index_CallOptions: ") << i;

			index_CallOptions = ofClamp(index_CallOptions, index_CallOptions.getMin(), index_CallOptions.getMax());

			switch (index_CallOptions)
			{
			case 0: name_CallOptions = "ColorGeneratorDeuteranopia"; break;
			case 1: name_CallOptions = "ColorGeneratorNormal"; break;
			}
		});

	//----


	//-----

	// startup

	position_Palette = glm::vec2(20, ofGetHeight() / 2);

	// settings
	ofxSurfingHelpers::loadGroup(params, path_FileSettings);
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

	else if (key == ' ')
	{
		getPaletteType();
	}

	else if (key == '1')
	{
		getPaletteType(0);
	}

	else if (key == '2')
	{
		getPaletteType(1);
	}

	//--

	/*
	{
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

		float yBg = position_Palette.get().y - padding;
		float hBg = (boxSize + boxPad) + 2 * padding + labelSize;
		float ymaxBg = yBg + hBg;

		ofColor colorBgCards = ofColor(250);

		//if (colors_STRUCT.size() > 0)
		{
			ofPushStyle();
			ofFill();

			for (int i = 0; i < paletteCard.size(); i++)
			{
				ofColor c = paletteCard[i];
				ofSetColor(c);
				float round = 2.5f;
				ofDrawRectRounded(
					glm::vec2(position_Palette.get().x + i * (boxSize + boxPad), position_Palette.get().y),
					boxSize, boxSize, round);
			}

			//--

			ofPopStyle();
		}
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
	if (type == -1)
	{
		type = index_CallOptions;
	}

	//--

	if (index_CallOptions != type) index_CallOptions.set(type);

	if (index_CallOptions == 0)
	{
		auto G = ColorGeneratorNormal();

		paletteCard.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteCard.push_back(c);
		}
	}

	else if (index_CallOptions == 1)
	{
		auto G = ColorGeneratorDeuteranopia();

		paletteCard.clear();
		for (int i = 0; i < amountColors; i++)
		{
			std::tuple<int, int, int> r = G();
			ofColor c = ofColor(std::get<0>(r), std::get<1>(r), std::get<2>(r));
			paletteCard.push_back(c);
		}
	}
}

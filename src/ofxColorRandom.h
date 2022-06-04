#pragma once

#include "ofMain.h"

/*

WIP 

broken. luminosity breaks

*/

//----

// randomcolor.cpp : Defines the entry point for the console application.
// from: https://github.com/xuboying/randomcolor-cpp
// https://github.com/davidmerfield/randomColor
// https://randomcolor.lllllllllllllllll.com/

//#include "stdafx.h"
#include "randomcolor.h"
#include <iostream>
#include <fstream>
#include <time.h> /* time */
#include <functional>

#include "sample.cpp"

//--



//----


#include "ofxSurfingHelpers.h"
#include "ofxGui.h"
#include "ofxSurfing_ofxGui.h"

//--

// sorting modes
enum
{
	SORTING_ORIGINAL,
	SORTING_NAME,
	SORTING_HUE,
	SORTING_BRIGHTNESS,
	SORTING_SATURATION
};

// a color struct
typedef struct
{
	std::string name;
	ofColor color;
	int position; // original position vs name, hue, sat...etc

} 
colorMapping_STRUCT;


//--

class ofxColorRandom
{
private:

	ofxPanel gui;

	string path_Global;
	string path_FileSettings;

public:

	ofxColorRandom();
	~ofxColorRandom();

	void setup();
	void draw();

private:

	void startup();
	void setupParams();
	void update();
	void drawCard();
	void keyPressed(ofKeyEventArgs& eventArgs);
	void exit();

	ofParameter<int> amountColors{ "Card Colors", 7, 2, 30 };// minimal card of colors
	int getAmountcolors();

	ofParameter<int> boxSize{ "Box Size", 30, 0, 100 };//boxes
	ofParameter<int> boxPad{ "Pad", 5, 0, 10 };
	//ofParameter<bool> bGui_Card{ "Cards", true };
	ofParameter<glm::vec2> position_Palette{ "position_Cards",  glm::vec2(0), glm::vec2(0), glm::vec2(1920) };
	
	void doSortPalette();

	//--

	// Storage

private:

	map<std::string, ofColor> colors_NamesMAP;

	vector<colorMapping_STRUCT> colors_STRUCT;

	//--

private:

	vector<ofColor> paletteGenerated;

	//--

public:

	//--------------------------------------------------------------
	vector<ofColor> getPalette() {
		return paletteGenerated;
	}

private:

	ofParameterGroup params_Layout;
	ofParameterGroup params_GuiPanels;
	ofParameterGroup params;
	void Changed_Params(ofAbstractParameter& e);

	//--

	// 0:ORIGINAL, 1:NAME, 2:HUE, 3:BRIGHTNESS, 4:SATURATION, 5:NEXT
	ofParameter<int> sorting{ "Sorting Mode", 0, 0, 4 };
	ofParameter<std::string> sorting_Name{ "Sorting", "" };
	ofEventListener listener_Sorting;
	void setNextSortType();

	void getPaletteTypeHue();
	void getPaletteTypeLuminosity();
	
	void buildMapFromPalette();
	void buildPaletteFromMap();

	//--

	ofEventListener listener_Hue;
	ofParameter<int> hue{ "HUE", 0, 0, 8 };
	ofParameter<string> hue_Name{ "HUE ", "" };
	//--------------------------------------------------------------------------
	string getHueName(int i) {
		switch (i)
		{
		case 0: return "UNSET"; break;
		case 1: return "MONOCHROME"; break;
		case 2: return "RED"; break;
		case 3: return "ORANGE"; break;
		case 4: return "YELLOW"; break;
		case 5: return "GREEN"; break;
		case 6: return "BLUE"; break;
		case 7: return "PURPLE"; break;
		case 8: return "PINK"; break;
		default: return "UNKNOWN"; break;
		}
	}

	ofEventListener listener_Luminosity;
	ofParameter<int> luminosity{ "LUMINOSITY", 0, 0, 3 };
	ofParameter<string> luminosity_Name{ "LUMINOSITY ", "" };
	//--------------------------------------------------------------------------
	string getLuminosityName(int i) {
		switch (i)
		{
		case 0: return "RANDOM"; break;
		case 1: return "BRIGHT"; break;
		case 2: return "LIGHT"; break;
		case 3: return "DARK"; break;
		default: return "UNKNOWN"; break;
		}
	}
};


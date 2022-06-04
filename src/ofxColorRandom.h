#pragma once

#include "ofMain.h"

// randomcolor.cpp : Defines the entry point for the console application.
// from: https://github.com/xuboying/randomcolor-cpp

//#include "stdafx.h"
#include "randomcolor.h"
#include <iostream>
#include <fstream>
#include <time.h> /* time */
#include <functional>


//----


// Useful methods / snippets / Examples

//--------------------------------------------------------------------------
auto ColorGeneratorDeuteranopia = [=]() -> std::function<std::tuple<int, int, int>()> {
    srand((int)time(NULL));
    RandomColor::Options o;
    o.hue = 0;
    o.hue_name = RandomColor::UNSET;
    o.seed = rand() % 1000;
    auto RG = RandomColor::RandomColorGenerator(o);
    int  j = 3;
    int  i = 1;
    return [i, j, RG]() mutable {
        bool flag_done = false;
        while (!flag_done) {
            i++;
            if (i > 8) {
                i = 2;
                j++;
                if (j > 3) {
                    j = 1;
                }
            }
            flag_done = true;
            auto color_name = static_cast<RandomColor::HUENAMES>(i);
            if (color_name == RandomColor::HUENAMES::RED || color_name == RandomColor::HUENAMES::ORANGE || color_name == RandomColor::HUENAMES::GREEN || color_name == RandomColor::HUENAMES::PURPLE || color_name == RandomColor::HUENAMES::PINK) {
                flag_done = false;
            }
            RG.options.luminosity = static_cast<RandomColor::LUMINOSITY>(j);
            RG.options.hue_name = static_cast<RandomColor::HUENAMES>(i);
        }
        return RG.randomColorRGB();
    };
};

//--------------------------------------------------------------------------
auto ColorGeneratorNormal = [=]() -> std::function<std::tuple<int, int, int>()> {
    srand((int)time(NULL));
    RandomColor::Options o;
    o.hue = 0;
    o.hue_name = RandomColor::UNSET;
    o.seed = rand() % 1000;
    auto RG = RandomColor::RandomColorGenerator(o);
    int  j = 3;
    int  i = 1;
    return [i, j, RG]() mutable {
        bool flag_done = false;
        while (!flag_done) {
            i++;
            if (i > 8) {
                i = 2;
                j++;
                if (j > 3) {
                    j = 1;
                }
            }
            flag_done = true;
            auto color_name = static_cast<RandomColor::HUENAMES>(i);
            RG.options.luminosity = static_cast<RandomColor::LUMINOSITY>(j);
            RG.options.hue_name = static_cast<RandomColor::HUENAMES>(i);
        }
        return RG.randomColorRGB();
    };
};


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

} colorMapping_STRUCT;

//--
 
// color converters
// These string to hex conversions aren't trivial.
static int stringToHex(std::string hex)
{
    int aHex;
    stringstream convert(hex);
    convert >> std::hex >> aHex;

    return aHex;
}

static void hexToColor(ofColor& col, std::string hex)
{
    std::string r = hex.substr(0, 2);
    int ri = stringToHex(r);
    std::string g = hex.substr(2, 2);
    int gi = stringToHex(g);
    std::string b = hex.substr(4, 2);
    int bi = stringToHex(b);
    col.set(ri, gi, bi);
}

//--

class ofxColorRandom
{
public:

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

    ofxPanel gui;
    
    string path_Global;
    string path_FileSettings;

	ofxColorRandom();
	~ofxColorRandom();

	void setup();
	void update();
	void draw();
    void drawCard();
	void keyPressed(ofKeyEventArgs &eventArgs);
	void exit();
    
    ofParameter<glm::vec2> position_Palette{ "position_Cards",  glm::vec2(0), glm::vec2(0), glm::vec2(1920) };
    int getAmountcolors();

    ofParameter<int> boxSize{ "Box Size", 30, 0, 100 };//boxes
    ofParameter<int> boxPad{ "Pad", 5, 0, 10 };

    ofParameter<int> amountColors{ "Card Colors", 7, 2, 30 };// minimal card of colors

    ofParameter<bool> bGui_Card{ "Cards", true };
    //ofParameter<bool> bGui_Bg{ "Bg", true };


    //--

    // Storage

private:

    map<std::string, ofColor> colors_NamesMAP;

    vector<colorMapping_STRUCT> colors_STRUCT;

	//--

private:
    
    vector<ofColor> paletteCard;

    //--

public:

    vector<ofColor> getPaletteCard() {
        return paletteCard;
    }

private:

    ofParameterGroup params_Layout;
    ofParameterGroup params_GuiPanels;
    ofParameterGroup params;
    void Changed_Params(ofAbstractParameter& e);

    //--

    // 0:ORIGINAL, 1:NAME, 2:HUE, 3:BRIGHTNESS, 4:SATURATION, 5:NEXT
    ofParameter<int> MODE_SORTING{ "Sorting Mode", 0, 0, 4 };
    ofParameter<std::string> MODE_SORTING_name{ "Sorting", "" };
    ofEventListener listener_ModeSorting;

    ofParameter<int> index_CallOptions{ "Index Call", 0, 0, 0 };
    ofParameter<std::string> name_CallOptions{ "Name Call ", "" };
    ofEventListener listener_Library;

    void getPaletteType(int type = -1);
};


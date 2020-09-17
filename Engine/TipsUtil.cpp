#include "TipsUtil.h"
#include "Func.h"
#include "DebugOut.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <string>
#include "config.h"
using namespace std;

TipsUtil::TipsUtil()
{
    ifstream check("resources/graphics/ui/tips/tip_backgrounds.txt");
    bool exists = check.good();
    check.close();

    ///if config not exists -update: then dont create fresh one :)
    /*if(!exists)
    {
        ofstream conf("resources/data/sv1.p4sv");

        if(conf.is_open())
        {
            ///Safety warning
            conf << "# Take caution! The data below represents your save data! Don't edit it unless you know what you're doing, and if you must, PLEASE back it up somewhere else first <3 #";
            conf << '\n';
        }

        conf.close();
    }*/
}

void TipsUtil::LoadBackgrounds(Config& tconfig)
{

    //sf::Context context;
    config = &tconfig;
    debugOut = config->debugOut;

    ifstream conf("resources/graphics/ui/tips/tip_backgrounds.txt");
    if(conf.good())
    {
        string line;
        while(getline(conf, line))
        {
            ///ignore comments and empty lines
            if(line.find("#") == std::string::npos && line.find("//") == std::string::npos && line.length()>0)
            {
                ///Split the Key and Value
                backgroundFileNames.push_back(line);
            }
        }
    }
    else
    {
        cout << "ERROR! Could not load tips background file!" << endl;
    }
    conf.close();
    for (auto it = backgroundFileNames.begin(); it != backgroundFileNames.end(); ++it){
        ///             ####   BARRACKS MENU BACKGROUND
        PSprite ps_temp;
        ps_temp.loadFromFile("resources/graphics/ui/tips/"+*it,tconfig.GetInt("textureQuality"),1);
        ps_temp.setRepeated(true);
        ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
        ps_temp.setOrigin(0,0);
        ps_temp.setColor(sf::Color(255,255,255,255));

        sf::Vector2f tmpp;
        //float xRatio = (thisConfig->GetInt("resX")/1600.0);
        tmpp.x = 0;
        tmpp.y = 0;

        ps_temp.scaleX=1.0f;
        ps_temp.scaleY=1.0f;
        t_backgrounds.push_back(ps_temp);
    }
}

void TipsUtil::LoadIcons(Config& tconfig)
{

    //sf::Context context;
    config = &tconfig;
    debugOut = config->debugOut;
    ifstream conf("resources/graphics/ui/tips/tip_icons.txt");
    if(conf.good())
    {
        string line;
        while(getline(conf, line))
        {
            ///ignore comments and empty lines
            if(line.find("#") == std::string::npos && line.find("//") == std::string::npos && line.length()>0)
            {
                ///Split the Key and Value
                iconFileNames.push_back(line);
            }
        }
    }
    else
    {
        cout << "ERROR! Could not load tips icon file!" << endl;
    }
    conf.close();
    for (auto it = iconFileNames.begin(); it != iconFileNames.end(); ++it){
        ///             ####   BARRACKS MENU BACKGROUND
        PSprite ps_temp;
        ps_temp.loadFromFile("resources/graphics/ui/tips/"+*it,tconfig.GetInt("textureQuality"),1);
        ps_temp.setRepeated(true);
        ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
        ps_temp.setOrigin(0,0);
        ps_temp.setColor(sf::Color(255,255,255,255));

        sf::Vector2f tmpp;
        //float xRatio = (thisConfig->GetInt("resX")/1600.0);
        tmpp.x = 0;
        tmpp.y = 0;

        ps_temp.scaleX=1.0f;
        ps_temp.scaleY=1.0f;
        t_icons.push_back(ps_temp);
    }
}

void TipsUtil::LoadStrings(Config& tconfig)
{

    //sf::Context context;
    config = &tconfig;
    debugOut = config->debugOut;
    ifstream conf("resources/lang/tip_ENG.txt");
    if(conf.good())
    {
        string line;
        while(getline(conf, line))
        {
            ///ignore comments and empty lines
            if(line.find("#") == std::string::npos && line.find("//") == std::string::npos && line.length()>0)
            {
                ///Split the Key and Value
                cout<<"OUTPUTTING LINE FROM TIPS STRING FILE: "<<line<<endl;
                vector<string> key = Func::Split(line,'|');
                tipTitles.push_back(key[0]);
                tipTexts.push_back(key[1]);
            }
        }
    }
    else
    {
        cout << "ERROR! Could not load tips icon file!" << endl;
    }
    conf.close();
}

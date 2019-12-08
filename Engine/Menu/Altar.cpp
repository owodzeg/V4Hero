#include "Altar.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
#include "math.h"
#include <sstream>
AltarMenu::AltarMenu()
{
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    t_titlemenu.setFont(f_font);
    t_titlemenu.setCharacterSize(38);
    t_titlemenu.setFillColor(sf::Color::Black);

    t_itemtitle.setFont(f_font);
    t_itemtitle.setCharacterSize(28);
    t_itemtitle.setFillColor(sf::Color::Black);

    t_itemcategory.setFont(f_font);
    t_itemcategory.setCharacterSize(18);
    t_itemcategory.setFillColor(sf::Color::Black);




    isActive=false;
}
void AltarMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent, PatapolisMenu *curParentMenu){
    Scene::Initialise(thisConfigs,keymap,parent);
    parentMenu = curParentMenu;

    thisConfig->debugOut->DebugMessage("Altar menu image loaded: menurect.png");

    PSprite ps_temp;
    ps_temp.loadFromFile("resources/graphics/ui/menurect.png",1);
    ps_temp.setRepeated(false);
    ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(ps_temp.t.getSize().x,0);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(0,0);
    ps_temp.DoAutoScale = false;

    sf::Vector2f tmpp;
    tmpp.x = (thisConfig->GetInt("resX")*1920.0)/1920.0 - (100*thisConfig->GetInt("resX"))/1920.0;//(thisConfig->GetInt("resX")*1920.0)/1920.0-1200;
    tmpp.y = (thisConfig->GetInt("resY")*400.0)/1080.0;//(thisConfig->GetInt("resY")*400.0)/1080.0;

    s_menu_bkg = ps_temp;
    p_menu_bkg = tmpp;
    s_menu_bkg.scaleX=0.2f;
    s_menu_bkg.scaleY=0.15f;


    sf::Vector2f tmpp2;
    tmpp2.x = (thisConfig->GetInt("resX")*1920.0)/1920.0 - (100*thisConfig->GetInt("resX"))/1920.0;//(thisConfig->GetInt("resX")*1920.0)/1920.0-1200;
    tmpp2.y = (thisConfig->GetInt("resY")*200.0)/1080.0;//(thisConfig->GetInt("resY")*400.0)/1080.0;

    s_titlemenu_bkg = ps_temp;
    p_titlemenu_bkg = tmpp2;
    s_titlemenu_bkg.scaleX=0.2f;
    s_titlemenu_bkg.scaleY=0.06f;


    /// initialise text
    /*
    t_titlemenu
    t_itemtitle
    t_itemcategory
    t_itemdescription
    */
    Item* starting_item = v4core->savereader.invdata.GetItemByInvID(0).item;
    t_titlemenu.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"altar_title")));
    t_titlemenu.setOrigin(t_titlemenu.getGlobalBounds().width/2,t_titlemenu.getGlobalBounds().height/2);

    t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(starting_item->item_name)));
    t_itemtitle.setOrigin(t_itemtitle.getGlobalBounds().width/2,t_itemtitle.getGlobalBounds().height/2);
    switch(starting_item->category_id){
        case 0:
            t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_material")));
            break;
        case 1:
            t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_weapon")));
            break;
        case 2:
            t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_masks")));
            break;
    }
    t_itemcategory.setOrigin(t_itemcategory.getGlobalBounds().width/2,t_itemcategory.getGlobalBounds().height/2);


    /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
    std::vector<std::wstring> wordsinDesc = Func::Split(thisConfig->strRepo.GetUnicodeString(starting_item->item_description),' ');
    sf::String oldTotalString;
    sf::String currentTotalString;
    int maxWidth = ps_temp.t.getSize().x * 0.18;
    /// we split it into words, then go word by word testing the width of the string
    for (int i=0;i<wordsinDesc.size();i++){
        std::wstring currentWord = wordsinDesc[i];
        currentTotalString = currentTotalString + Func::ConvertToUtf8String(currentWord) + L" ";
        sf::Text t_newLine;
        t_newLine.setFont(f_font);
        t_newLine.setCharacterSize(16);
        t_newLine.setFillColor(sf::Color::Black);
        t_newLine.setString(currentTotalString);
        if (t_newLine.getGlobalBounds().width>maxWidth){
            /// when the string is too long, we go back to the last string and lock it in, then start a new line
            currentTotalString = oldTotalString;
            t_newLine.setString(currentTotalString);
            t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
            t_itemdescription.push_back(t_newLine);
            oldTotalString = currentWord+L" ";
            currentTotalString = currentWord+L" ";
        }
        oldTotalString = currentTotalString;
        /// if there are no more words, finish up the current line
        if (i+1==wordsinDesc.size()){
            currentTotalString = oldTotalString;
            t_newLine.setString(currentTotalString);
            t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
            t_itemdescription.push_back(t_newLine);
            oldTotalString = "";
            currentTotalString = "";
        }
    }
    //s_menu_bkg.t.getSize().x*0.2

    //(p_menu_bkg.y+(s_menu_bkg.t.getSize().y*0.15)) - p_titlemenu_bkg.y)
    if(thisConfig->GetInt("resX")>1920){
        numItemColumns=6;
    } else if(thisConfig->GetInt("resX")>1600){
        numItemColumns=5;
    }
    ShowCategory();
    mm_inventory_background.setSize(sf::Vector2f(p_titlemenu_bkg.x - (s_titlemenu_bkg.t.getSize().x*0.2) - (thisConfig->GetInt("resX")*150)/1920.0,(p_menu_bkg.y+(s_menu_bkg.t.getSize().y*0.15)) - p_titlemenu_bkg.y));
    mm_inventory_background.setFillColor(sf::Color(4,0,90));

    float singleTileWidth = mm_inventory_background.getSize().x/numItemColumns;
    float singleTileHeight = mm_inventory_background.getSize().y/5;//(thisConfig->GetInt("resX")*150)/1920.0;

    mm_highlighted_tile.setSize(sf::Vector2f(singleTileWidth,singleTileHeight));
    mm_highlighted_tile.setFillColor(sf::Color(255,255,255,80));

    mm_icon_example_tile.setSize(sf::Vector2f(singleTileWidth,singleTileHeight));
    mm_icon_example_tile.setFillColor(sf::Color(255,255,255,0));
    mm_icon_example_tile.setOutlineColor(sf::Color(255,255,255));
    mm_icon_example_tile.setOutlineThickness(2);


    mm_inventory_background.setSize(sf::Vector2f(mm_inventory_background.getSize().x+10,mm_inventory_background.getSize().y+10));

}
void AltarMenu::ShowCategory(){
    int totalItems = v4core->savereader.invdata.items.size();
    numItemRows = ceil(totalItems/(numItemColumns+0.0));
}
void AltarMenu::UpdateAltarDescriptions(){
    if (inventoryGridXPos+inventoryGridYPos*numItemColumns<v4core->savereader.invdata.items.size()){
        Item* starting_item = v4core->savereader.invdata.GetItemByInvID(inventoryGridXPos+inventoryGridYPos*numItemColumns).item;
        t_titlemenu.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"altar_title")));
        t_titlemenu.setOrigin(t_titlemenu.getGlobalBounds().width/2,t_titlemenu.getGlobalBounds().height/2);

        t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(starting_item->item_name)));
        t_itemtitle.setOrigin(t_itemtitle.getGlobalBounds().width/2,t_itemtitle.getGlobalBounds().height/2);
        switch(starting_item->category_id){
            case 0:
                t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_material")));
                break;
            case 1:
                t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_weapon")));
                break;
            case 2:
                t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_masks")));
                break;
        }
        t_itemcategory.setOrigin(t_itemcategory.getGlobalBounds().width/2,t_itemcategory.getGlobalBounds().height/2);


        /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
        std::vector<std::wstring> wordsinDesc = Func::Split(thisConfig->strRepo.GetUnicodeString(starting_item->item_description),' ');
        sf::String oldTotalString;
        sf::String currentTotalString;
        int maxWidth = s_titlemenu_bkg.t.getSize().x * 0.18;
        /// we split it into words, then go word by word testing the width of the string
        t_itemdescription.clear();
        for (int i=0;i<wordsinDesc.size();i++){
            std::wstring currentWord = wordsinDesc[i];
            currentTotalString = currentTotalString + Func::ConvertToUtf8String(currentWord) + L" ";
            sf::Text t_newLine;
            t_newLine.setFont(f_font);
            t_newLine.setCharacterSize(16);
            t_newLine.setFillColor(sf::Color::Black);
            t_newLine.setString(currentTotalString);
            if (t_newLine.getGlobalBounds().width>maxWidth){
                /// when the string is too long, we go back to the last string and lock it in, then start a new line
                currentTotalString = oldTotalString;
                t_newLine.setString(currentTotalString);
                t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
                t_itemdescription.push_back(t_newLine);
                oldTotalString = currentWord+L" ";
                currentTotalString = currentWord+L" ";
            }
            oldTotalString = currentTotalString;
            /// if there are no more words, finish up the current line
            if (i+1==wordsinDesc.size()){
                currentTotalString = oldTotalString;
                t_newLine.setString(currentTotalString);
                t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
                t_itemdescription.push_back(t_newLine);
                oldTotalString = "";
                currentTotalString = "";
            }
        }
        //s_menu_bkg.t.getSize().x*0.2
    } else {

        t_titlemenu.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"altar_title")));
        t_titlemenu.setOrigin(t_titlemenu.getGlobalBounds().width/2,t_titlemenu.getGlobalBounds().height/2);

        t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_none")));
        t_itemtitle.setOrigin(t_itemtitle.getGlobalBounds().width/2,t_itemtitle.getGlobalBounds().height/2);

        t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_material")));
        t_itemcategory.setOrigin(t_itemcategory.getGlobalBounds().width/2,t_itemcategory.getGlobalBounds().height/2);


        /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
        std::vector<std::wstring> wordsinDesc = Func::Split(thisConfig->strRepo.GetUnicodeString(L"desc_none"),' ');
        sf::String oldTotalString;
        sf::String currentTotalString;
        int maxWidth = s_titlemenu_bkg.t.getSize().x * 0.18;
        /// we split it into words, then go word by word testing the width of the string
        t_itemdescription.clear();
        for (int i=0;i<wordsinDesc.size();i++){
            std::wstring currentWord = wordsinDesc[i];
            currentTotalString = currentTotalString + Func::ConvertToUtf8String(currentWord) + L" ";
            sf::Text t_newLine;
            t_newLine.setFont(f_font);
            t_newLine.setCharacterSize(16);
            t_newLine.setFillColor(sf::Color::Black);
            t_newLine.setString(currentTotalString);
            if (t_newLine.getGlobalBounds().width>maxWidth){
                /// when the string is too long, we go back to the last string and lock it in, then start a new line
                currentTotalString = oldTotalString;
                t_newLine.setString(currentTotalString);
                t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
                t_itemdescription.push_back(t_newLine);
                oldTotalString = currentWord+L" ";
                currentTotalString = currentWord+L" ";
            }
            oldTotalString = currentTotalString;
            /// if there are no more words, finish up the current line
            if (i+1==wordsinDesc.size()){
                currentTotalString = oldTotalString;
                t_newLine.setString(currentTotalString);
                t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
                t_itemdescription.push_back(t_newLine);
                oldTotalString = "";
                currentTotalString = "";
            }
        }
    }
}
void AltarMenu::EventFired(sf::Event event){
    if(isActive){
        if(event.type == sf::Event::KeyPressed)
        {
            // do something here;
            if (event.key.code == sf::Keyboard::Num1){


            } else if (event.key.code == sf::Keyboard::Num2){



            } else if (event.key.code == sf::Keyboard::Num3){



            } else if (event.key.code == sf::Keyboard::Num4){



            } else if (event.key.code == sf::Keyboard::Num5){


            } else if (event.key.code == thisConfig->GetInt("keybindPon") || event.key.code == thisConfig->GetInt("secondaryKeybindPon")){

                /// we need to move to the right
                if (inventoryGridXPos<numItemColumns-1){
                    inventoryGridXPos+=1;
                } else {
                    inventoryGridXPos = 0;
                }

            } else if (event.key.code == thisConfig->GetInt("keybindPata") || event.key.code == thisConfig->GetInt("secondaryKeybindPata")){
                /// we need to move to the left
                if (inventoryGridXPos>0){
                    inventoryGridXPos-=1;
                } else {
                    inventoryGridXPos = numItemColumns-1;
                }

            } else if (event.key.code == thisConfig->GetInt("keybindChaka") || event.key.code == thisConfig->GetInt("secondaryKeybindChaka")){
                /// we need to move up
                if (inventoryGridYPos>0){

                    inventoryGridYPos-=1;
                    if (inventoryGridYPos - currentRow<0){
                            /// the position is above the top of the grid so we scroll up
                            currentRow-=1;
                    }
                } else {
                    inventoryGridYPos = numItemRows-1;

                    currentRow=inventoryGridYPos-4;
                }

            } else if (event.key.code == thisConfig->GetInt("keybindDon") || event.key.code == thisConfig->GetInt("secondaryKeybindDon")){
                /// we need to move down
                if (inventoryGridYPos<numItemRows-1){
                    inventoryGridYPos+=1;
                    if (inventoryGridYPos - currentRow>=5){
                        /// the position is below the bottom of the grid so we scroll down
                        currentRow+=1;
                    }
                } else {
                    inventoryGridYPos = 0;
                    currentRow=0;
                }

            } else if (event.key.code == thisConfig->GetInt("keybindBack")){
                this->Hide();
                this->isActive = false;
            }
            UpdateAltarDescriptions();

        } else if (event.type == sf::Event::MouseButtonReleased){
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
                std::ostringstream ss2;
                ss2 << numItemRows;
                std::string s2(ss2.str());
                thisConfig->debugOut->DebugMessage("Location: "+s2+"\n");
        }
    }
}

void AltarMenu::Update(sf::RenderWindow &window, float fps)
{
    if(isActive){
            s_menu_bkg.setPosition(p_menu_bkg.x,p_menu_bkg.y);
            s_menu_bkg.draw(window);
            s_titlemenu_bkg.setPosition(p_titlemenu_bkg.x,p_titlemenu_bkg.y);
            s_titlemenu_bkg.draw(window);

//-(s_titlemenu_bkg.t.getSize().x*0.2)/2
            /// draw altar menu text
            float smallOffset = (thisConfig->GetInt("resX")*100)/1920.0;
            float smallTextOffset = 0;
            t_titlemenu.setPosition(p_titlemenu_bkg.x+smallTextOffset-(s_titlemenu_bkg.t.getSize().x*0.2)/2,p_titlemenu_bkg.y+(s_titlemenu_bkg.t.getSize().y*0.06)*1/3);
            window.draw(t_titlemenu);

            t_itemtitle.setPosition(p_titlemenu_bkg.x+smallTextOffset-(s_menu_bkg.t.getSize().x*0.2)/2,p_menu_bkg.y+(s_menu_bkg.t.getSize().y*0.15)*1/8);
            window.draw(t_itemtitle);

            t_itemcategory.setPosition(p_titlemenu_bkg.x+smallTextOffset-(s_menu_bkg.t.getSize().x*0.2)/2,p_menu_bkg.y+((s_menu_bkg.t.getSize().y*0.15)*1/8)+42);
            window.draw(t_itemcategory);

            for (int i=0;i<t_itemdescription.size();i++){
                sf::Text currentLine = t_itemdescription[i];

                currentLine.setPosition(p_menu_bkg.x+smallTextOffset-(s_menu_bkg.t.getSize().x*0.2)/2,p_menu_bkg.y+((s_menu_bkg.t.getSize().y*0.15)*1/8)+74 + 22*i);

                window.draw(currentLine);
            }
            mm_inventory_background.setPosition(smallOffset-5,p_titlemenu_bkg.y-5);
            window.draw(mm_inventory_background);

            mm_highlighted_tile.setPosition(smallOffset+mm_highlighted_tile.getSize().x*inventoryGridXPos,p_titlemenu_bkg.y+mm_highlighted_tile.getSize().y*inventoryGridYPos-mm_highlighted_tile.getSize().y*currentRow);
            window.draw(mm_highlighted_tile);
            int ypos = 2-currentRow;
            if (ypos>=0){
                //mm_icon_example_tile.setPosition(smallOffset+mm_icon_example_tile.getSize().x*2,p_titlemenu_bkg.y+mm_icon_example_tile.getSize().y*2-mm_highlighted_tile.getSize().y*currentRow);
                //window.draw(mm_icon_example_tile);
            }


    }
}

void AltarMenu::UpdateButtons(){

}
void AltarMenu::OnExit(){

}
AltarMenu::~AltarMenu()
{
    //dtor
}

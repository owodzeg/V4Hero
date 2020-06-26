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

    int quality = thisConfigs->GetInt("textureQuality");

    switch(quality)
    {
        case 0: ///low
        {
            ratioX = thisConfigs->GetInt("resX") / float(640);
            ratioY = thisConfigs->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = thisConfigs->GetInt("resX") / float(1280);
            ratioY = thisConfigs->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = thisConfigs->GetInt("resX") / float(1920);
            ratioY = thisConfigs->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = thisConfigs->GetInt("resX") / float(3840);
            ratioY = thisConfigs->GetInt("resY") / float(2160);
            break;
        }
    }

    resRatioX = thisConfigs->GetInt("resX") / float(1920);
    resRatioY = thisConfigs->GetInt("resY") / float(1080);

    thisConfig->debugOut->DebugMessage("Altar menu image loaded: menurect.png");

    PSprite ps_temp;
    ps_temp.loadFromFile("resources/graphics/ui/menurect.png",quality,2);
    ps_temp.setOrigin(ps_temp.t.getSize().x,0);

    sf::Vector2f tmpp;
    tmpp.x = 1880.0;
    tmpp.y = 110.0;

    s_menu_bkg = ps_temp;
    p_menu_bkg = tmpp;
    s_menu_bkg.scaleX=(780.0*resRatioX)/s_menu_bkg.getGlobalBounds().width/ratioX;
    s_menu_bkg.scaleY=(160.0*resRatioY)/s_menu_bkg.getGlobalBounds().height/ratioY;


    sf::Vector2f tmpp2;
    tmpp2.x = 1880.0;
    tmpp2.y = 390.0;

    s_titlemenu_bkg = ps_temp;
    p_titlemenu_bkg = tmpp2;
    s_titlemenu_bkg.scaleX=(780.0*resRatioX)/s_menu_bkg.getGlobalBounds().width/ratioX;
    s_titlemenu_bkg.scaleY=(400.0*resRatioY)/s_menu_bkg.getGlobalBounds().height/ratioY;
    numItemColumns=5;

    ShowCategory();

    int inv_bg_x = 920+40;

    mm_inventory_background.setSize(sf::Vector2f(inv_bg_x*resRatioX, 675*resRatioY));
    mm_inventory_background.setFillColor(sf::Color(4,0,90));
    float singleTileWidth = ((inv_bg_x-(40*resRatioX))*resRatioX)/numItemColumns;
    float singleTileHeight = mm_inventory_background.getSize().y/5;//(thisConfig->GetInt("resX")*150)/1920.0;
    cout<<singleTileWidth<<" "<<singleTileHeight<<'\n';
    mm_highlighted_tile.setSize(sf::Vector2f(singleTileWidth,singleTileHeight));
    mm_highlighted_tile.setFillColor(sf::Color(255,255,255,80));

    mm_icon_example_tile.setSize(sf::Vector2f(singleTileWidth,singleTileHeight));
    mm_icon_example_tile.setFillColor(sf::Color(255,255,255,0));
    mm_icon_example_tile.setOutlineColor(sf::Color(255,255,255));
    mm_icon_example_tile.setOutlineThickness(2);

    ///             ####   MASK ITEM ICON
    mask_icon.loadFromFile("resources/graphics/item/mask_item.png",quality,2);
    mask_icon.scaleX=singleTileWidth/mask_icon.getGlobalBounds().width/ratioX;
    mask_icon.scaleY=singleTileHeight/mask_icon.getGlobalBounds().height/ratioY;


    ///             ####   SPEAR ITEM ICON
    spear_icon.loadFromFile("resources/graphics/item/spear_item.png",quality,2);
    spear_icon.scaleX=singleTileWidth/spear_icon.getGlobalBounds().width/ratioX;
    spear_icon.scaleY=singleTileHeight/spear_icon.getGlobalBounds().height/ratioY;

    ///             ####   MISC ITEM ICON
    misc_icon.loadFromFile("resources/graphics/item/misc_icon.png",quality,2);
    misc_icon.scaleX=singleTileWidth/misc_icon.getGlobalBounds().width/ratioX;
    misc_icon.scaleY=singleTileHeight/misc_icon.getGlobalBounds().height/ratioY;

    ///             ####   ARMOUR ITEM ICON
    armour_icon.loadFromFile("resources/graphics/item/armour_icon.png",quality,2);
    armour_icon.scaleX=singleTileWidth/armour_icon.getGlobalBounds().width/ratioX;
    armour_icon.scaleY=singleTileHeight/armour_icon.getGlobalBounds().height/ratioY;

    /// initialise text
    /*
    t_titlemenu
    t_itemtitle
    t_itemcategory
    t_itemdescription
    */
    //Item* starting_item = v4core->savereader.invdata.GetItemByInvID(0).item;
    t_titlemenu.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"altar_title")));
    t_titlemenu.setOrigin(t_titlemenu.getGlobalBounds().width/2,t_titlemenu.getGlobalBounds().height/2);

    t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"altar_item_title")));
    t_itemtitle.setOrigin(t_itemtitle.getGlobalBounds().width/2,t_itemtitle.getGlobalBounds().height/2);
    /*switch(starting_item->category_id){
        case 0:
            t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_material")));
            break;
        case 1:
            t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_weapon")));
            break;
        case 2:*/
            t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_masks")));
            //break;
    //}
    t_itemcategory.setOrigin(t_itemcategory.getGlobalBounds().width/2,t_itemcategory.getGlobalBounds().height/2);

    float menuSize = ((mm_inventory_background.getPosition().x/resRatioX)+(mm_inventory_background.getSize().x/resRatioX));

    /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
    std::vector<std::wstring> wordsinDesc = Func::Split(thisConfig->strRepo.GetUnicodeString(L"altar_item_description"),' ');
    sf::String oldTotalString;
    sf::String currentTotalString;
    float maxWidth = (1920-menuSize-140)*resRatioX;
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





    mm_inventory_background.setSize(sf::Vector2f(mm_inventory_background.getSize().x+20,mm_inventory_background.getSize().y+20));

}
void AltarMenu::ShowCategory(){
    int totalItems = v4core->savereader.invdata.items.size();
    numItemRows = ceil(totalItems/(numItemColumns+0.0));

}
void AltarMenu::ShowAltar(){
    ShowCategory();
    inventoryGridXPos=0;
    inventoryGridYPos=0;
    UpdateAltarDescriptions();
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

        float menuSize = ((mm_inventory_background.getPosition().x/resRatioX)+(mm_inventory_background.getSize().x/resRatioX));

        /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
        std::vector<std::wstring> wordsinDesc = Func::Split(thisConfig->strRepo.GetUnicodeString(starting_item->item_description),' ');
        sf::String oldTotalString;
        sf::String currentTotalString;
        float maxWidth = (1920-menuSize-140)*resRatioX;
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

        t_itemcategory.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_category_nothing")));
        t_itemcategory.setOrigin(t_itemcategory.getGlobalBounds().width/2,t_itemcategory.getGlobalBounds().height/2);

        float menuSize = ((mm_inventory_background.getPosition().x/resRatioX)+(mm_inventory_background.getSize().x/resRatioX));

        /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
        std::vector<std::wstring> wordsinDesc = Func::Split(thisConfig->strRepo.GetUnicodeString(L"desc_none"),' ');
        sf::String oldTotalString;
        sf::String currentTotalString;
        float maxWidth = (1920-menuSize-140)*resRatioX;
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
                    if (numItemRows<5){
                        currentRow=inventoryGridYPos-numItemRows+1;
                    } else {
                        currentRow=inventoryGridYPos-4;
                    }
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

//-(s_titleenu_bkg.t.getSize().x*0.2)/2
        /// draw altar menu text
        float smallOffset = 100;
        mm_inventory_background.setPosition(90*resRatioX,125*resRatioY);
        window.draw(mm_inventory_background);

        float menuSize = ((mm_inventory_background.getPosition().x/resRatioX)+(mm_inventory_background.getSize().x/resRatioX));


        s_menu_bkg.setPosition(p_menu_bkg.x,p_menu_bkg.y);
        s_menu_bkg.draw(window);
        s_titlemenu_bkg.setPosition(p_titlemenu_bkg.x,p_titlemenu_bkg.y);
        s_titlemenu_bkg.draw(window);

        float smallTextOffset = 0;
        t_titlemenu.setPosition((menuSize + ((1920 - menuSize) / 2)) * resRatioX,mm_inventory_background.getPosition().y+20);
        window.draw(t_titlemenu);

        int row_start = currentRow;
        int row_end = row_start+4;
        for (int y=0; y<numItemRows; y++)
        {
            for (int x=0; x<numItemColumns; x++)
            {
                int currentItemId = x+y*numItemColumns;
                if (currentItemId<v4core->savereader.invdata.items.size() && (currentItemId/numItemColumns)>=row_start && (currentItemId/numItemColumns)<=row_end)
                {
                    Item* starting_item = v4core->savereader.invdata.GetItemByInvID(currentItemId).item;
                    float xpos = 10+(mm_inventory_background.getPosition().x/resRatioX)+(mm_highlighted_tile.getSize().x/resRatioX+10)*x;
                    float ypos = 10+(mm_inventory_background.getPosition().y/resRatioY)+(mm_highlighted_tile.getSize().y/resRatioY+10)*y-(mm_highlighted_tile.getSize().y/resRatioY)*currentRow;

                    switch (starting_item->category_id)
                    {
                        case 1:
                            spear_icon.setPosition(xpos,ypos);
                            spear_icon.draw(window);
                            break;
                        case 2:
                            mask_icon.setPosition(xpos,ypos);
                            mask_icon.draw(window);
                            break;
                        case 3:
                            armour_icon.setPosition(xpos,ypos);
                            armour_icon.draw(window);
                            break;
                        case 0:
                        default:
                            misc_icon.setPosition(xpos,ypos);
                            misc_icon.draw(window);
                            break;
                    }
                }
            }
        }

        mm_highlighted_tile.setPosition((10+(mm_inventory_background.getPosition().x/resRatioX)+(mm_highlighted_tile.getSize().x/resRatioX+10)*inventoryGridXPos)*resRatioX,(10+(mm_inventory_background.getPosition().y/resRatioY)+(mm_highlighted_tile.getSize().y/resRatioY+10)*inventoryGridYPos-(mm_highlighted_tile.getSize().y/resRatioY)*currentRow)*resRatioY);
        window.draw(mm_highlighted_tile);

        t_itemtitle.setPosition((menuSize + ((1920 - menuSize) / 2)) * resRatioX, 430 * resRatioY);
        window.draw(t_itemtitle);

        t_itemcategory.setPosition((menuSize + ((1920 - menuSize) / 2)) * resRatioX, 470 * resRatioY);
        window.draw(t_itemcategory);

        for (int i=0;i<t_itemdescription.size();i++){
            sf::Text currentLine = t_itemdescription[i];

            currentLine.setPosition((menuSize + ((1920 - menuSize) / 2)) * resRatioX, (520 + 22*i) * resRatioY);

            window.draw(currentLine);
        }


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

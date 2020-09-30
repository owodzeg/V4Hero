#include "Altar.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
#include "math.h"


AltarMenu::AltarMenu()
{
    isActive=false;
}

void AltarMenu::Initialise(Config *thisConfigs, V4Core *parent, PatapolisMenu *curParentMenu)
{
    parent->SaveToDebugLog("Initializing Altar...");
    Scene::Initialise(thisConfigs, parent);
    parentMenu = curParentMenu;

    int quality = thisConfigs->GetInt("textureQuality");
    q = quality;

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

    resRatioX = thisConfigs->GetInt("resX") / float(1280);
    resRatioY = thisConfigs->GetInt("resY") / float(720);

    f_font.loadFromFile(thisConfigs->fontPath);

    altar_main.loadFromFile("resources/graphics/ui/altar/altar_main.png", quality, 1);

    altar_title.createText(f_font, 40, sf::Color(111, 71, 51, 255), Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"altar_title")), q, 1);
    altar_kaching.createText(f_font, 30, sf::Color(111, 71, 51, 255), "0 Ka-ching", q, 1);

    altar_item_title.createText(f_font, 24, sf::Color(111, 71, 51, 255), "", q, 1);
    altar_item_category.createText(f_font, 20, sf::Color(111, 71, 51, 255), "", q, 1);
    altar_item_desc.createText(f_font, 26, sf::Color(111, 71, 51, 255), "", q, 1);

    ctrlTips.create(54, f_font, 20, sf::String(L"Left/Right/Up/Down: Navigate      〇: Exit to Patapolis"), quality);

    parent->SaveToDebugLog("Initializing Altar finished.");
}
void AltarMenu::ShowCategory()
{

}

void AltarMenu::ShowAltar()
{

}

void AltarMenu::UpdateAltarDescriptions()
{
    int selItem = (gridSelY+gridOffsetY)*4 + gridSelX;

    if(selItem < inventory_boxes.size())
    {
        altar_item_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(inventory_boxes[selItem].data->item_name)));
        altar_item_category.setString(thisConfig->strRepo.GetUnicodeString("altar_category"+to_string(inventory_boxes[selItem].data->category_id)));
        altar_item_desc.setString(Func::ConvertToUtf8String(Func::wrap_text(thisConfig->strRepo.GetUnicodeString(inventory_boxes[selItem].data->item_description), 420, f_font, 26)));
    }
    else
    {
        altar_item_title.setString("");
        altar_item_category.setString("");
        altar_item_desc.setString("");
    }
}

void AltarMenu::EventFired(sf::Event event)
{
    if(isActive)
    {
        if(event.type == sf::Event::KeyPressed)
        {
            // do something here;
        }
    }
}

void AltarMenu::ReloadInventory()
{
    vector<InvBox> old_invboxes = inventory_boxes; ///for comparison and new item highlight

    inventory_boxes.clear();

    for(int i=0; i<v4core->savereader.invdata.items.size(); i++)
    {
        InventoryItem cur_invitem = v4core->savereader.invdata.GetItemByInvID(i);
        Item* cur_item = cur_invitem.item;

        bool ex = false;

        ///Check if InvBox with following item already exists
        for(int b=0; b<inventory_boxes.size(); b++)
        {
            if(inventory_boxes[b].data == cur_item)
            {
                ex = true;
                inventory_boxes[b].amount++;

                inventory_boxes[b].num.setString(Func::num_padding(inventory_boxes[b].amount, 3));
                inventory_boxes[b].num_shadow.setString(Func::num_padding(inventory_boxes[b].amount, 3));
            }
        }

        if(!ex)
        {
            InvBox tmp;
            tmp.data = cur_item;
            tmp.amount = 1;

            tmp.r_outer.setSize({104.0*resRatioX, 77.0*resRatioY});
            tmp.r_outer.setFillColor(sf::Color(102,102,102,255));

            tmp.r_inner.setSize({72.0*resRatioX, 72.0*resRatioY});
            tmp.r_inner.setFillColor(sf::Color(183,183,183,255));

            switch(cur_item->category_id)
            {
                case 0: ///materials
                {
                    tmp.r_inner.setFillColor(sf::Color(146,173,217,255));

                    ///look up material's icon
                    tmp.icon.loadFromFile("resources/graphics/ui/altar/materials/"+Func::num_padding(cur_item->spritesheet_id, 4)+".png", q, 1);
                    tmp.icon.setOrigin(tmp.icon.getLocalBounds().width/2, tmp.icon.getLocalBounds().height/2);

                    break;
                }

                case 1: ///spears
                {
                    tmp.r_inner.setFillColor(sf::Color(199,221,167,255));

                    ///look up material's icon
                    tmp.icon.loadFromFile("resources/graphics/ui/altar/equip/spear_1.png", q, 1);
                    tmp.icon.setOrigin(tmp.icon.getLocalBounds().width/2, tmp.icon.getLocalBounds().height/2);

                    break;
                }

                case 3: ///helms
                {
                    tmp.r_inner.setFillColor(sf::Color(199,221,167,255));

                    ///look up material's icon
                    tmp.icon.loadFromFile("resources/graphics/ui/altar/equip/helm_1.png", q, 1);
                    tmp.icon.setOrigin(tmp.icon.getLocalBounds().width/2, tmp.icon.getLocalBounds().height/2);

                    break;
                }

                case 4: ///key items
                {
                    tmp.r_inner.setFillColor(sf::Color(183,183,183,255));

                    ///look up material's icon
                    tmp.icon.loadFromFile("resources/graphics/ui/altar/materials/"+Func::num_padding(cur_item->spritesheet_id, 4)+".png", q, 1);
                    tmp.icon.setOrigin(tmp.icon.getLocalBounds().width/2, tmp.icon.getLocalBounds().height/2);

                    break;
                }
            }

            tmp.num.createText(f_font, 30, sf::Color::White, "001", q, 1);
            tmp.num_shadow.createText(f_font, 30, sf::Color(136,136,136,255), "001", q, 1);

            inventory_boxes.push_back(tmp);
        }
    }

    std::sort(inventory_boxes.begin(), inventory_boxes.end(),
              [](const InvBox& a, const InvBox& b)
                {
                    return a.data->altar_order_id < b.data->altar_order_id;
                });

    if((old_invboxes.size() > 0) || (!loadedSave)) ///if the invboxes are empty, don't highlight, unless it's a fresh save
    {
        ///Now check for new items
        for(int a=0; a<inventory_boxes.size(); a++)
        {
            ///get invbox item from current table
            bool found = false;
            bool highlight = false;

            for(int b=0; b<old_invboxes.size(); b++)
            {
                ///and compare it to every invbox item in the old table
                if(inventory_boxes[a].data->item_id == old_invboxes[b].data->item_id)
                {
                    found = true;

                    if(inventory_boxes[a].amount > old_invboxes[b].amount)
                    {
                        highlight = true;
                    }

                    if(old_invboxes[b].highlight)
                    highlight = false;
                }
            }

            cout << "Check invbox " << a << " found " << found << " highlight " << highlight << endl;

            if(!found)
            highlight = true;

            if(highlight)
            {
                inventory_boxes[a].r_highlight.setSize({104.0*resRatioX, 77.0*resRatioY});
                inventory_boxes[a].r_highlight.setFillColor(sf::Color::White);

                inventory_boxes[a].highlight = true;
            }
        }
    }

    UpdateAltarDescriptions();
}

void AltarMenu::Update(sf::RenderWindow &window, float fps, InputController& inputCtrl)
{
    if(isActive)
    {
        highlight_x += 7.0 / fps;

        ctrlTips.x = 0;
        ctrlTips.y = (720-ctrlTips.ySize);
        ctrlTips.draw(window);

        altar_main.setOrigin(altar_main.getLocalBounds().width/2, altar_main.getLocalBounds().height/2);
        altar_main.setPosition(340, 360);

        altar_main.draw(window);

        for(int i=0; i<24; i++)
        {
            if(gridOffsetY*4 + i < inventory_boxes.size())
            {
                int curItem = gridOffsetY*4 + i;

                int gridX = i % 4;
                int gridY = floor(i/4);

                float xpos = 72 + (gridX*118);
                float ypos = 64 + (gridY*88);

                inventory_boxes[curItem].r_outer.setPosition((40 + xpos)*resRatioX, (39 + ypos) * resRatioY);
                inventory_boxes[curItem].r_inner.setPosition((40 + xpos + 2.5)*resRatioX, (39 + ypos + 2.5) * resRatioY);
                window.draw(inventory_boxes[curItem].r_outer);
                window.draw(inventory_boxes[curItem].r_inner);

                //inventory_boxes[i].num.setOrigin(inventory_boxes[i].num.getLocalBounds().width,inventory_boxes[i].num.getLocalBounds().height);
                //inventory_boxes[i].num_shadow.setOrigin(inventory_boxes[i].num_shadow.getLocalBounds().width,inventory_boxes[i].num_shadow.getLocalBounds().height);

                if((inventory_boxes[curItem].data->category_id == 0) || (inventory_boxes[curItem].data->category_id == 4))
                inventory_boxes[curItem].icon.setScale(0.64,0.64);

                inventory_boxes[curItem].icon.setPosition(40 + xpos + 36 + 2.5, 39 + ypos + 36 + 2.5);
                inventory_boxes[curItem].icon.draw(window);

                inventory_boxes[curItem].num.setPosition(40+xpos+51-1, 39+ypos+45-2);
                inventory_boxes[curItem].num_shadow.setPosition(40+xpos+51, 39+ypos+45);

                inventory_boxes[curItem].num_shadow.draw(window);
                inventory_boxes[curItem].num.draw(window);

                if(inventory_boxes[curItem].highlight)
                {
                    inventory_boxes[curItem].r_highlight.setPosition((40 + xpos)*resRatioX, (39 + ypos) * resRatioY);
                    inventory_boxes[curItem].r_highlight.setFillColor(sf::Color(255,255,255,64+(sin(highlight_x)*64)));
                    window.draw(inventory_boxes[curItem].r_highlight);
                }
            }
            else
            {
                InvBox tmp_inv;

                int gridX = i % 4;
                int gridY = floor(i/4);

                float xpos = 72 + (gridX*118);
                float ypos = 64 + (gridY*88);

                tmp_inv.r_outer.setSize({104.0*resRatioX, 77.0*resRatioY});
                tmp_inv.r_outer.setFillColor(sf::Color(102,102,102,255));

                tmp_inv.r_inner.setSize({72.0*resRatioX, 72.0*resRatioY});
                tmp_inv.r_inner.setFillColor(sf::Color(183,183,183,255));

                tmp_inv.r_outer.setPosition((40 + xpos)*resRatioX, (39 + ypos) * resRatioY);
                tmp_inv.r_inner.setPosition((40 + xpos + 2.5)*resRatioX, (39 + ypos + 2.5) * resRatioY);
                window.draw(tmp_inv.r_outer);
                window.draw(tmp_inv.r_inner);
            }
        }

        r_sel.setSize({103.0*resRatioX, 77.0*resRatioY});
        r_sel.setFillColor(sf::Color::Transparent);
        r_sel.setOutlineThickness(3);
        r_sel.setOutlineColor(sf::Color(255,0,32,255));
        r_sel.setPosition((41 + 72 + (gridSelX*118))*resRatioX, (39 + 64 + (gridSelY*88))*resRatioY);

        window.draw(r_sel);

        rr_title_sh.Create(366+2, 100+2, 20, window.getSize().x / float(1280), sf::Color(0,0,0,96));
        rr_title_sh.x = 933-1;
        rr_title_sh.y = 141-1;
        rr_title_sh.setOrigin(sf::Vector2f((366+40)/2, (100+40)/2));
        rr_title_sh.Draw(window);

        rr_title.Create(366, 100, 20, window.getSize().x / float(1280));
        rr_title.x = 933;
        rr_title.y = 141;
        rr_title.setOrigin(sf::Vector2f((366+40)/2, (100+40)/2));
        rr_title.Draw(window);


        rr_desc_sh.Create(440+2, 385+2, 20, window.getSize().x / float(1280), sf::Color(0,0,0,96));
        rr_desc_sh.x = 933-1;
        rr_desc_sh.y = 436-1;
        rr_desc_sh.setOrigin(sf::Vector2f((440+40)/2, (385+40)/2));
        rr_desc_sh.Draw(window);

        rr_desc.Create(440, 385, 20, window.getSize().x / float(1280));
        rr_desc.x = 933;
        rr_desc.y = 436;
        rr_desc.setOrigin(sf::Vector2f((440+40)/2, (385+40)/2));
        rr_desc.Draw(window);

        altar_title.setOrigin(altar_title.getLocalBounds().width/2, altar_title.getLocalBounds().height/2);
        altar_kaching.setOrigin(altar_kaching.getLocalBounds().width/2, altar_kaching.getLocalBounds().height/2);

        altar_title.setPosition(933, 100);
        altar_kaching.setPosition(933, 170);

        altar_item_title.setPosition(933, 250);
        altar_item_category.setPosition(933, 280);
        altar_item_desc.setPosition(725, 330);

        altar_title.draw(window);
        altar_kaching.draw(window);

        altar_item_title.setOrigin(altar_item_title.getLocalBounds().width/2, altar_item_title.getLocalBounds().height/2);
        altar_item_category.setOrigin(altar_item_category.getLocalBounds().width/2, altar_item_category.getLocalBounds().height/2);
        altar_item_desc.setOrigin(0, 0);

        altar_item_title.draw(window);
        altar_item_category.draw(window);
        altar_item_desc.draw(window);

        if(inputCtrl.isKeyPressed(InputController::Keys::LEFT))
        {
            gridSelX--;

            if(gridSelX < 0)
            gridSelX = 3;

            UpdateAltarDescriptions();
        }
        if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
        {
            gridSelX++;

            if(gridSelX > 3)
            gridSelX = 0;

            UpdateAltarDescriptions();
        }
        if(inputCtrl.isKeyPressed(InputController::Keys::UP))
        {
            gridSelY--;

            if(gridSelY < 0)
            {
                if(gridOffsetY > 0)
                {
                    gridOffsetY--;
                    gridSelY = 0;
                }
                else
                {
                    gridOffsetY = ceil(inventory_boxes.size() / 4.0) - 6;

                    if(gridOffsetY < 0)
                    gridOffsetY = 0;

                    gridSelY = 5;
                }
            }

            UpdateAltarDescriptions();
        }
        if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
        {
            gridSelY++;

            if(gridSelY > 5)
            {
                if(inventory_boxes.size() > (6+gridOffsetY)*4)
                {
                    gridOffsetY++;
                    gridSelY = 5;
                }
                else
                {
                    gridSelY = 0;
                    gridOffsetY = 0;
                }
            }

            UpdateAltarDescriptions();
        }
        if(inputCtrl.isKeyPressed(InputController::Keys::CIRCLE))
        {
            this->Hide();
            this->isActive = false;
        }
    }
}

void AltarMenu::UpdateButtons()
{

}

void AltarMenu::OnExit()
{

}

AltarMenu::~AltarMenu()
{
    //dtor
}

#include "InventoryData.h"
#include "../SaveReader.h"
#include "Item.h"
#include "ItemRegistry.h"
#include <iostream>

/** Needs some rework **/

using namespace std;

InventoryData::InventoryData()
{
    cout << "InventoryData::InventoryData() constructor" << endl;
    itemReg.readItemFiles();
}

InventoryData::~InventoryData()
{
}

Item* InventoryData::getItemByItemID(vector<int> inv_id)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (inv_id == items[i].item->order_id)
        {
            return items[i].item;
        }
    }
}

int InventoryData::getItemCountByID(vector<int> item_id)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].item->order_id == item_id)
        {
            return items[i].item_count;
        }
    }
}

int InventoryData::getInvIDByItemID(vector<int> item_id) // Please for the love of- make this better
{
    for (int i = 0; i < items.size(); i++)
    {
        vector<int> cur_id = items[i].item->order_id;
        if (cur_id.size() != item_id.size())
        {
            continue;
        }

        if (cur_id[0] == item_id[0])
        {
            if (cur_id[1] == item_id[1])
            {
                if (item_id.size() > 2)
                {
                    if (cur_id[2] == item_id[2])
                    {
                        return i;
                    }
                } else
                {
                    return i;
                }
            }
        }
    }

    return 0;
}

bool InventoryData::checkItemObtained(vector<int> item_id)
{
    if (item_id.size() > 2)
    {
        cout << "InventoryData::checkItemObtained({" << item_id[0] << ", " << item_id[1] << ", " << item_id[2] << "})" << endl;
    } else
    {
        cout << "InventoryData::checkItemObtained({" << item_id[0] << ", " << item_id[1] << "})" << endl;
    }

    for (int i = 0; i < items.size(); i++)
    {
        InventoryData::InventoryItem current_item = items[i];

        if (current_item.item->order_id == item_id)
        {
            cout << "Found" << endl;
            return true;
        }
    }

    cout << "Not found" << endl;
    return false;
}

bool InventoryData::checkItemObtainedByName(string item_name)
{
    for (int i = 0; i < items.size(); i++)
    {
        InventoryData::InventoryItem current_item = items[i];

        if (current_item.item->item_name == item_name)
        {
            cout << "Found" << endl;
            return true;
        }
    }

    cout << "Not found" << endl;
    return false;
}

void InventoryData::addItem(vector<int> item_id, int count = 1)
{
    cout << "InventoryData::addItem({" << item_id[0];
    for (int i = 1; i < item_id.size(); i++)
    {
        cout << ", " << item_id[i];
    }
    cout << "});" << endl;

    if (checkItemObtained(item_id))
    {
        items[getInvIDByItemID(item_id)].item_count += count;
    } else
    {
        InventoryData::InventoryItem new_item;
        new_item.item = itemReg.getItemByID(item_id);
        new_item.item_count = count;
        items.push_back(new_item);
    }
}

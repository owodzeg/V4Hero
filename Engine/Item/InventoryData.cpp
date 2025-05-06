#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "InventoryData.h"
#include "../SaveReader.h"
#include "Item.h"
#include "ItemRegistry.h"
#include <iostream>
#include <spdlog/spdlog.h>

/** Needs some rework **/



InventoryData::InventoryData()
{
    SPDLOG_TRACE("InventoryData::InventoryData() constructor");
}

InventoryData::~InventoryData()
{
}

Item* InventoryData::getItemByItemID(std::vector<int> inv_id)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (inv_id == items[i].item->order_id)
        {
            return items[i].item;
        }
    }

    SPDLOG_ERROR("Error: Item not found in inventory");
    return nullptr;
}

int InventoryData::getItemCountByID(std::vector<int> item_id)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].item->order_id == item_id)
        {
            return items[i].item_count;
        }
    }

    SPDLOG_ERROR("Error: Item not found in inventory");
    return 0;
}

int InventoryData::getInvIDByItemID(std::vector<int> item_id) // Please for the love of- make this better
{
    for (int i = 0; i < items.size(); i++)
    {
        std::vector<int> cur_id = items[i].item->order_id;
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

bool InventoryData::checkItemObtained(std::vector<int> item_id)
{
    if (item_id.size() > 2)
    {
        SPDLOG_DEBUG("InventoryData::checkItemObtained({}, {}, {})", item_id[0], item_id[1], item_id[2]);
    } else
    {
        SPDLOG_DEBUG("InventoryData::checkItemObtained({}, {})", item_id[0], item_id[1]);
    }

    for (int i = 0; i < items.size(); i++)
    {
        InventoryData::InventoryItem current_item = items[i];

        if (current_item.item->order_id == item_id)
        {
            SPDLOG_TRACE("Found");
            return true;
        }
    }

    SPDLOG_TRACE("Not found");
    return false;
}

bool InventoryData::checkItemObtainedByName(std::string item_name)
{
    for (int i = 0; i < items.size(); i++)
    {
        InventoryData::InventoryItem current_item = items[i];

        if (current_item.item->item_name == item_name)
        {
            SPDLOG_TRACE("Found");
            return true;
        }
    }

    SPDLOG_TRACE("Not found");
    return false;
}

void InventoryData::addItem(std::vector<int> item_id, ItemRegistry& itemReg, int count)
{
    std::string str_item_id = std::to_string(item_id[0]);

    for (int i = 1; i < item_id.size(); i++)
    {
        str_item_id += "," + std::to_string(item_id[i]);
    }

    SPDLOG_INFO("Adding item with ID {}", str_item_id);

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

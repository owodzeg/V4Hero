#include "Squad.h"
#include "../CoreManager.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Squad::Squad(int id, std::string name, std::string image)
    : id_(id), name_(name), image_(image) {
    ResourceManager::getInstance().loadSprite("resources/graphics/"+image_+".png");
}

void Squad::addPon(Pon pon) {
    pons_.push_back(pon);
}

void Squad::removePon(int index) {
    pons_.erase(pons_.begin() + index);
}

int Squad::getId() const {
    return id_;
}

std::string Squad::getName() const {
    return CoreManager::getInstance().getStrRepo()->GetString(name_); // return localized name
}

std::string Squad::getImage() const {
    return image_;
}

std::vector<Pon> Squad::getPons() const {
    return pons_;
}

json Squad::toJson() const {
    json j;
    j["id"] = id_;
    j["name"] = name_;
    j["image"] = image_;
    return j;
}

Squad Squad::fromJson(json j) {
    int id = j["id"];
    std::string name = j["name"];
    std::string image = j["ui_image"];
    Squad squad(id, name, image);
    return squad;
}

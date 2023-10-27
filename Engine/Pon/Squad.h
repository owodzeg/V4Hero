#pragma once

#include <string>
#include <vector>
#include "Pon.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Squad {
public:
    Squad(int id, std::string name, std::string image);
    void addPon(Pon pon);
    void removePon(int index);
    int getId() const;
    std::string getName() const;
    std::string getImage() const;
    std::vector<Pon> getPons() const;
    json toJson() const;
    static Squad fromJson(json j);

private:
    int id_;
    std::string name_;
    std::string image_;
    std::vector<Pon> pons_;
};


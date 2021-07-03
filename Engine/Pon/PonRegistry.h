#ifndef PON_REGISTRY_H
#define PON_REGISTRY_H
#include "Pon.h"
#include <SFML/Graphics.hpp>
class PonRegistry
{
public:
    std::vector<Pon> pons;
    std::vector<int> squads_available;
    std::vector<int> army;
    PonRegistry();
    ~PonRegistry();
    Pon* GetPonByID(int id);
    void ReadPonFiles();
};

#endif // PON_REGISTRY_H

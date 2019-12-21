#ifndef PON_REGISTRY_H
#define PON_REGISTRY_H
#include <SFML/Graphics.hpp>
#include "Pon.h"
class PonRegistry
{
    public:
        std::vector<Pon> pons;
        PonRegistry();
        ~PonRegistry();
        Pon* GetPonByID(int id);
        void ReadPonFiles();
};

#endif // PON_REGISTRY_H

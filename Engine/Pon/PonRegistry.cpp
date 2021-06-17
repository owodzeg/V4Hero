
#include "PonRegistry.h"
#include "Pon.h"
#include "../Func.h"
#include <fstream>
#include<iostream>
using namespace std;

PonRegistry::PonRegistry()
{

}

void PonRegistry::ReadPonFiles()
{

}

PonRegistry::~PonRegistry()
{

}

Pon* PonRegistry::GetPonByID(int id)
{
    for(int i = 0; i < pons.size(); i++)
    {
        if(i == id)
        {
            return &pons[i];
        }
    }

    cout<< "PonRegistry could not find the appropriate Pon" << endl;
}


#include "PonRegistry.h"
#include "Pon.h"
#include "../Func.h"
#include <fstream>
#include<iostream>
using namespace std;

PonRegistry::PonRegistry(){

}
void PonRegistry::ReadPonFiles(){

}
PonRegistry::~PonRegistry(){

}
Pon* PonRegistry::GetPonByID(int id){

    for (int i=0;i<pons.size()-1;i++){
        Pon currentItem = pons[i];
        if (currentItem.pon_id==id)
            return &pons[i];
    }
    cout<<"A stupid happened"<<'\n';
}

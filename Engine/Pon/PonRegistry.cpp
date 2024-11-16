#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "PonRegistry.h"
#include "Pon.h"
#include <spdlog/spdlog.h>

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
    if(id < pons.size())
        return &pons[id];

    SPDLOG_ERROR("PonRegistry could not find the appropriate Pon for id {}. PonRegistry size: {}", id, pons.size());
    throw std::exception(); //TODO: a custom exception class perhaps?
}

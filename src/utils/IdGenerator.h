#ifndef FOOTBALL_MANAGEMENT_SYSTEM_ID_GENERATOR_H
#define FOOTBALL_MANAGEMENT_SYSTEM_ID_GENERATOR_H
#include <cstdint>


class IdGenerator {
    uint32_t nextId = 0;

    uint32_t generateId();
};

#endif
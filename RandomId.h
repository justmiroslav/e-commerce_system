#pragma once
#include <random>

vector<int> generatedIds;

int generateRandomId() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(10000, 99999);
    int id;
    do {
        id = dis(gen);
    } while (find(generatedIds.begin(), generatedIds.end(), id) != generatedIds.end());
    generatedIds.push_back(id);
    return id;
}

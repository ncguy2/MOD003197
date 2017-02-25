//
// Created by Guy on 21/02/2017.
//

#include <Entity.h>
#include <Defines.h>

Tree::Tree() : currentLife(MAX_TREE_LIFE), burning(false), ignited(false) {}

void Tree::Burn() {
    this->burning = true;
}

void Tree::Update() {
    if(this->IsBurning())
        this->currentLife--;
}

bool Tree::IsAlive() {
    return this->currentLife > 0;
}

bool Tree::IsBurning() {
    return this->burning && this->IsAlive();
}

bool Tree::IsIgnited() {
    return this->ignited;
}

void Tree::Ignite() {
    this->ignited = true;
}

void Tree::Clear() {
    this->ignited = false;
    this->burning = false;
}

void Tree::Kill() {
    this->currentLife = -1;
}

void Tree::Grow() {
    currentLife = MAX_TREE_LIFE;
    burning = false;
    ignited = false;
}

void Tree::Extinguish() {
    burning = false;
    ignited = false;
}

float Tree::GetCurrentLife() {
    return currentLife;
}

float Tree::GetCurrentLifePercentage() {
    if(currentLife <= 0)
        return 0;
    if(currentLife >= MAX_TREE_LIFE)
        return 1;
    float res = currentLife / MAX_TREE_LIFE;
    return res;
}

//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_BURNRULE_H
#define FIRESIM_BURNRULE_H

#include "Rules.h"

class BurnRule : public Rule {
public:
    BurnRule() : Rule(3) {}

    virtual void Execute(const Forest *forest, const Neighbours neighbours) {
        Tree* t = neighbours.self.tree;
        if(t == nullptr) return;
        if(t->IsBurning()) return;
        if(t->IsIgnited())
            t->Burn();
    }
};

#endif //FIRESIM_BURNRULE_H

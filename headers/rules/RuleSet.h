//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_RULESET_H
#define FIRESIM_RULESET_H

#include <vector>
#include <rules/Rules.h>

class RuleSet {
public:
    void AddRule(Rule* rule);

    /**
     * Moves the pointer to the next rule
     * @return The new focused rule
     */
    Rule* Next();
    /**
     * Resets the pointer to the first rule
     */
    Rule * Reset();

protected:
    int pointer;
    std::vector<Rule*> rules;
};

#endif //FIRESIM_RULESET_H

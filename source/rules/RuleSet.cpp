//
// Created by Guy on 21/02/2017.
//

#include <rules/Rules.h>
#include <rules/RuleSet.h>

void RuleSet::AddRule(Rule* rule) {
    rules.push_back(rule);
}

Rule* RuleSet::Next() {
    pointer++;
    if(pointer >= rules.size())
        return nullptr;
    return rules[pointer];
}

Rule* RuleSet::Reset() {
    pointer = 0;
    for(Rule* rule : rules)
        rule->Reset();
    return rules[pointer];
}

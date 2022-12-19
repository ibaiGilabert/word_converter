#ifndef STATETEXTVALUEFACTOR_H
#define STATETEXTVALUEFACTOR_H

#include "IStateTextValue.h"

class StateTextValueFactor : public IStateTextValue
{
public:
    static StateTextValueFactor& GetInstance();

    bool Parse(TextValue* textValue_, const std::string& word_) override;

private:
    StateTextValueFactor() = default;
    ~StateTextValueFactor() override = default;
    StateTextValueFactor(const StateTextValueFactor& other_) = delete;
    StateTextValueFactor& operator=(const StateTextValueFactor& other) = delete;
};

#endif // TEXTVALUESTATEFACTOR_H

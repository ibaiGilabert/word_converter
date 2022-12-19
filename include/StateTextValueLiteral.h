#ifndef STATETEXTVALUELITERAL_H
#define STATETEXTVALUELITERAL_H

#include "IStateTextValue.h"

class StateTextValueLiteral : public IStateTextValue
{
public:
    static StateTextValueLiteral& GetInstance();

    bool Parse(TextValue* textValue_, const std::string& word_) override;

private:
    StateTextValueLiteral() = default;
    ~StateTextValueLiteral() override = default;
    StateTextValueLiteral(const StateTextValueLiteral& other_) = delete;
    StateTextValueLiteral& operator=(const StateTextValueLiteral& other) = delete;
};

#endif // STATETEXTVALUELITERAL_H

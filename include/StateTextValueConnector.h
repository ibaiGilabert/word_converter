#ifndef STATETEXTVALUECONNECTOR_H
#define STATETEXTVALUECONNECTOR_H

#include "IStateTextValue.h"

class StateTextValueConnector : public IStateTextValue
{
public:
    static StateTextValueConnector& GetInstance();

    bool Parse(TextValue* textValue_, const std::string& word_) override;

private:
    StateTextValueConnector() = default;
    ~StateTextValueConnector() override = default;
    StateTextValueConnector(const StateTextValueConnector& other_) = delete;
    StateTextValueConnector& operator=(const StateTextValueConnector& other) = delete;
};

#endif // STATETEXTVALUECONNECTOR_H

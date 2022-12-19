#include "StateTextValueLiteral.h"
#include "StateTextValueFactor.h"
#include "TextValue.h"

StateTextValueLiteral& StateTextValueLiteral::GetInstance()
{
    static StateTextValueLiteral singleton;
    return singleton;
}

bool StateTextValueLiteral::Parse(TextValue* textValue_, const std::string& word_)
{
    unsigned int value(0);
    if (textValue_->IsLiteral(word_, value)) {
        // Update accumulated value
        textValue_->UpdateAccum(value, TextValue::kOperator::kOperatorAdd);
        // Transition: literal read, only accepts a multiplier factor
        textValue_->SetState(StateTextValueFactor::GetInstance());
        return true;
    }
    return false;
}

#include "StateTextValueFactor.h"
#include "StateTextValueConnector.h"
#include "TextValue.h"

StateTextValueFactor& StateTextValueFactor::GetInstance()
{
    static StateTextValueFactor singleton;
    return singleton;
}

bool StateTextValueFactor::Parse(TextValue* textValue_, const std::string& word_)
{
    unsigned int value(0);
    if (textValue_->IsFactor(word_, value)) {
        // Update accumulated value
        textValue_->UpdateAccum(value, TextValue::kOperator::kOperatorMultiply);
        // always persist group
        textValue_->PersistValue();
        // Transition: factor read, accepts either a literal or a connector.
        textValue_->SetState(StateTextValueConnector::GetInstance());
        return true;
    }
    return false;
}

#include "StateTextValueConnector.h"
#include "StateTextValueLiteral.h"
#include "TextValue.h"

StateTextValueConnector& StateTextValueConnector::GetInstance()
{
    static StateTextValueConnector singleton;
    return singleton;
}

bool StateTextValueConnector::Parse(TextValue* textValue_, const std::string& word_)
{ 
    // Extend StateTextValueLiteral parsing method
    if (!StateTextValueLiteral::GetInstance().Parse(textValue_, word_)) {
        if (textValue_->IsConnector(word_)) {
            // Transition: connector read, only accepts a new literal.
            textValue_->SetState(StateTextValueLiteral::GetInstance());
            return true;
        }
    }
    return false;
}

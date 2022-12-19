#include "IStateTextValue.h"
#include "TextValue.h"

void IStateTextValue::Enter(TextValue* textValue_)
{
    if (!textValue_->HasFinished()) {
        // it has not reached the end yet, so it is safe to dereference the pointer
        Parse(textValue_, *textValue_->GetCurrentPosition());
    }
}

void IStateTextValue::Exit(TextValue* textValue_)
{
    textValue_->Next();
}

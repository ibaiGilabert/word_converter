#ifndef ISTATETEXTVALUE_H
#define ISTATETEXTVALUE_H

#include <string>

// forward declaration to resolve circular dependency/include
class TextValue;

class IStateTextValue
{
public:
    virtual ~IStateTextValue() = default;

    virtual void Enter(TextValue*);
    virtual void Exit(TextValue*);

    virtual bool Parse(TextValue*, const std::string&) = 0;
};

#endif // ISTATETEXTVALUE_H

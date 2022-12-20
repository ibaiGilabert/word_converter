#ifndef TEXTVALUE_H
#define TEXTVALUE_H

#include <sstream>
#include <memory>
#include <regex>
#include <map>
#include <set>

// forward declaration to resolve circular dependency/include
class IStateTextValue;

class TextValue
{
public:
    enum class kOperator {
        kOperatorUndefined = -1,
        kOperatorAdd,
        kOperatorMultiply
    };
    static const std::map<std::string, unsigned int> kDictionaryLiteral;
    static const std::map<std::string, unsigned int> kDictionaryFactor;
    static const std::set<std::string> kDictionaryConnector;

    explicit TextValue();

    void SetState(IStateTextValue& newState_);
    void Parse(const std::string& dataIn_, std::string &dataOut_);
    void UpdateAccum(unsigned int value_, kOperator operator_);
    void PersistValue();
    void FlushBuffer(const std::string& data_, bool isLast_);

    bool IsLiteral(const std::string& word_) const;
    bool IsLiteral(const std::string& word_, unsigned int& value_/*, bool& isUnary_*/) const;
    bool IsFactor(const std::string& word_, unsigned int& value_) const;
    bool IsConnector(const std::string& word_) const;

    std::string ToLower(std::string word_) const;

    inline std::sregex_token_iterator GetCurrentPosition() const { return mIterator; }
    inline void Next() { ++mIterator; }
    inline bool HasFinished() const { return (mIterator == std::sregex_token_iterator()); }

private:
    IStateTextValue* mState;
    std::sregex_token_iterator mIterator;
    std::stringstream mBuffer;
    unsigned int mValue, mAccum;
};

#endif // TEXTVALUE_H

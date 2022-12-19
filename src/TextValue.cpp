#include "TextValue.h"
#include "StateTextValueLiteral.h"

//#include <iostream>

const std::map<std::string, unsigned int> TextValue::kDictionaryLiteral =
{
    // literals of arity-one
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
    {"ten", 10},

    // literals of arity-one
    {"eleven", 11},
    {"twelve", 12},
    {"thirteen", 13},
    {"fourteen", 14},
    {"fifteen", 15},
    {"sixteen", 16},
    {"seventeen", 17},
    {"eighteen", 18},
    {"nineteen", 19},

    // can be compound
    {"twenty", 20},
    {"thirty", 30},
    {"forty", 40},
    {"fifty", 50},
    {"sixty", 60},
    {"seventy", 70},
    {"eighty", 80},
    {"ninety", 90}
};

const std::map<std::string, unsigned int> TextValue::kDictionaryFactor =
{
    {"hundred", 100},
    {"thousand", 1000},
    {"million", 1000000},
    {"milliard", 1000000000}
};

const std::set<std::string> TextValue::kDictionaryConnector =
{
    {"and"}
};

TextValue::TextValue() :
    mValue(0),
    mAccum(0)
{
    // Set initial state
    mState = &StateTextValueLiteral::GetInstance();
    mIterator = std::sregex_token_iterator();
    mBuffer.clear();
}

void TextValue::SetState(IStateTextValue& newState_)
{
    mState->Exit(this);
    mState = &newState_;
    mState->Enter(this);
}

void TextValue::Parse(const std::string& dataIn_, std::string& dataOut_)
{
    // clear buffer
    mBuffer.clear();

    // initialize data's iterator
    std::regex rgx("[\\s\\t]+");
    mIterator = std::regex_token_iterator(dataIn_.begin(), dataIn_.end(), rgx, -1);
    std::sregex_token_iterator end;

    while (mIterator != end)
    {
        mValue = mAccum = 0;
        mState->Parse(this, *mIterator);

        // flush eventual accumulated values
        PersistValue();

        // let's see how far we are
        int dist = std::distance(mIterator, end);

        //TODO: take into account the 'zero' value
        if (mValue) {
            FlushBuffer(std::to_string(mValue), !dist);
        }

        if (dist) {
            // move on (if current value is not a valid none) and restore initial state
            if (!IsLiteral(*mIterator)) {
                FlushBuffer(*mIterator, dist == 1);
                Next();
            }
            mState = &StateTextValueLiteral::GetInstance();
        }
    }
    dataOut_ = mBuffer.str();
}

void TextValue::UpdateAccum(unsigned int value_, kOperator operator_)
{
    switch (operator_) {
        case kOperator::kOperatorAdd:
        mAccum += value_;
        break;
    case kOperator::kOperatorMultiply:
        mAccum *= value_;
        break;
    default:
        break;
    }
}

void TextValue::PersistValue()
{
    mValue += mAccum;
    mAccum = 0;
}

void TextValue::FlushBuffer(const std::string& data_, bool isLast_)
{
    mBuffer << data_ << (!isLast_ ? " " : "");
}

bool TextValue::IsLiteral(const std::string& word_) const
{
    unsigned int tmp(0);
    return IsLiteral(word_, tmp);
}

bool TextValue::IsLiteral(const std::string& word_, unsigned int& value_/*, bool &isUnary_*/) const
{
    // deal with word in lower case
    std::string wordLC = ToLower(word_);

    //https://www.grammarbook.com/numbers/numbers.asp
    // Rule 2a: look for hyphenate for values in range [21-99]
    std::regex rgx("^(\\w+)-(\\w+),?$");
    std::sregex_iterator itMatch{wordLC.begin(), wordLC.end(), rgx};
    if (std::distance(itMatch, std::sregex_iterator()) == 1) {
        const auto itLeft = kDictionaryLiteral.find(itMatch->str(1));
        // left side must be greater than 20
        if (itLeft != kDictionaryLiteral.end() && itLeft->second >= 20) {
            const auto itRight = kDictionaryLiteral.find(itMatch->str(2));
            // right side must be lower than 10
            if (itRight != kDictionaryLiteral.end() && itRight->second < 10) {
                value_ = itLeft->second + itRight->second;
//                isUnary_ = false;
                return true;
            }
        }
    }
    else {
        // look for literal straightforward
        const auto itValue = kDictionaryLiteral.find((wordLC.back() == ',') ? wordLC.substr(0, wordLC.size()-1) : wordLC);
        if (itValue != kDictionaryLiteral.end()) {
            value_ = itValue->second;
//            isUnary_ = value_ < 10;
            return true;
        }
    }
    return false;
}

bool TextValue::IsFactor(const std::string& word_, unsigned int& value_) const
{
    // deal with word in lower case
    std::string wordLC = ToLower(word_);

    // factor multiplier may be followed by a comma.
    const auto itValue = kDictionaryFactor.find((wordLC.back() == ',') ? wordLC.substr(0, wordLC.size()-1) : wordLC);
    if (itValue != kDictionaryFactor.end()) {
        value_ = itValue->second;
        return true;
    }
    return false;
}

bool TextValue::IsConnector(const std::string& word_) const
{
    return kDictionaryConnector.find(ToLower(word_)) != kDictionaryConnector.end();
}

std::string TextValue::ToLower(std::string word_) const
{
    std::transform(word_.begin(), word_.end(), word_.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return word_;
}

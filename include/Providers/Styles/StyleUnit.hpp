#pragma once

#include "IStyler.h"
#include "Feature.h"

using StyleCondition = std::function<bool(const Feature*)>;
using StyleConditionSet = std::vector<StyleCondition>;
using StyleInstruction = std::function<void(IStyler*&)>;
using StyleInstructionSet = std::vector<StyleInstruction>;

struct StyleUnit{
    StyleUnit(const StyleConditionSet &conditions, const StyleInstructionSet &instructions, std::string id="") :
        _conditions(conditions), _instructions(instructions), _id(!id.empty() ? id : "RANDOM_STRING_HERE") { }

        const std::string &id() const {
            return this->_id;
        }

        const std::vector<StyleCondition> conditions() const{
            return this->_conditions;
        }

        const std::vector<StyleInstruction> instructions() const {
            return this->_instructions;
        }

    private:
        std::string _id;
        std::vector<StyleCondition> _conditions;
        std::vector<StyleInstruction> _instructions;
    /// TODO: add visibility expressions
};
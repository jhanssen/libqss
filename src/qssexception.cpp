#include "../include/qssexception.h"

const std::unordered_map<int, std::string> qss::Exception::Messages {
    { Exception::HEADER_PARAM_INVALID, "Header param is invalid" },
    { Exception::BLOCK_PARAM_INVALID, "Block param is invalid" },
    { Exception::SELECTOR_INVALID, "Selector is invalid" },
    { Exception::BLOCK_BRACKETS_INVALID, "Block brackets invalid" },
    { Exception::MULTIPLE_IDS, "More than one id encountered" },
    { Exception::ILL_FORMED_HEADER_PARAM, "Header param is incomplete" }
};

std::string qss::Exception::what() const
{
    return Messages.at(m_code) + ": < " + m_details + " >";
}

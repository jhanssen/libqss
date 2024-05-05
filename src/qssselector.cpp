#include "../include/qssselector.h"

#include <ctre.hpp>

qss::Selector::Selector(const std::string & str)
{
    parse(str);
}

qss::Selector& qss::Selector::operator=(const Selector &selector)
{
    m_fragments = selector.m_fragments;
    return *this;
}

qss::Selector& qss::Selector::addChild(const SelectorElement &fragment)
{
    return append(fragment, SelectorElement::CHILD);
}

qss::Selector& qss::Selector::addChild(const std::string &fragment)
{
    return append(fragment, SelectorElement::CHILD);
}

qss::Selector& qss::Selector::addDescendant(const SelectorElement &fragment)
{
    return append(fragment, SelectorElement::DESCENDANT);
}

qss::Selector& qss::Selector::addDescendant(const std::string &fragment)
{
    return append(fragment, SelectorElement::DESCENDANT);
}

qss::Selector& qss::Selector::addGeneralSibling(const SelectorElement &fragment)
{
    return append(fragment, SelectorElement::GENERAL_SIBLING);
}

qss::Selector& qss::Selector::addGeneralSibling(const std::string &fragment)
{
    return append(fragment, SelectorElement::GENERAL_SIBLING);
}

qss::Selector& qss::Selector::addSibling(const SelectorElement &fragment)
{
    return append(fragment, SelectorElement::SIBLING);
}

qss::Selector& qss::Selector::addSibling(const std::string &fragment)
{
    return append(fragment, SelectorElement::SIBLING);
}

qss::Selector& qss::Selector::append(const std::string &fragment, SelectorElement::PositionType type)
{
    m_fragments.emplace_back(fragment);
    m_fragments.back().m_position = type;
    return *this;
}

qss::Selector& qss::Selector::append(const SelectorElement &fragment, SelectorElement::PositionType type)
{
    m_fragments.push_back(fragment);
    m_fragments.back().m_position = type;
    return *this;
}

static constexpr auto plusPattern = ctll::fixed_string{ "`+" };

void qss::Selector::parse(const std::string &selector)
{
    auto addFragment = [this](const QStringList& list, int index, SelectorElement& fragment, SelectorElement::PositionType pos)
    {
        fragment.parse(list[index]);
        fragment.m_position = pos;
        m_fragments.push_back(fragment);
    };

    auto str = TrimmedString(selector);

    if (str.size() > 0)
    {
        preProcess(str);

        auto rparts = ctre::split<plusPattern>(str);
        std::vector<std::string> parts;
        for (const auto& rpart : rparts) {
            std::string rpstr = rpart.to_string();
            if (!rpstr.empty()) {
                parts.push_back(std::move(rpstr));
            }
        }
        SelectorElement select;
        addFragment(parts, 0, select, SelectorElement::PARENT);

        for (int i = 1; i < parts.size();)
        {
            SelectorElement select;

            if (Combinators.count(parts[i]) != 0)
            {
                if (parts.size() > (i + 1))
                {
                    addFragment(parts, i + 1, select, Combinators.at(parts[i]));
                    i += 2;
                }
                else
                {
                    throw Exception{ Exception::SELECTOR_INVALID, parts[i] };
                }
            }
            else if (Delimiters.at(QSS_BLOCK_START_DELIMITER) == TrimmedString(parts[i]))
            {
                i++;
                continue;
            }
            else
            {
                addFragment(parts, i, select, SelectorElement::DESCENDANT);
                i++;
            }
        }
    }
}

std::string qss::Selector::toString() const
{
    std::string result;

    if (m_fragments.size() > 0)
    {
        result = m_fragments[0].toString();

        for (int i = 1; i < m_fragments.size(); ++i)
        {
            result += " " + m_fragments[i].toString();
        }
    }

    return result;
}

bool qss::operator==(const Selector &lhs, const Selector &rhs)
{
    return lhs.toString() == rhs.toString();
}

void qss::Selector::preProcess(std::string &str)
{
    int quotes = str[0] == '"';

    for (size_t i = 1; i < str.size(); ++i)
    {
        if (quotes % 2 == 0) {
            if (isspace(str[i]))
            {
                str[i] = PreProcessChar;
            }
            else if (i > 0)
            {
                switch (str[i]) {
                case '>':
                case '~':
                case '+':
                case ',': {
                    size_t add = 0;
                    if (str[i - 1] != PreProcessChar) {
                        str.insert(str.begin() + i, PreProcessChar);
                        add += 1;
                    }
                    if (i + add + 1 < str.size() && !isspace(str[i + add + 1])) {
                        str.insert(str.begin() + i + 1 + add, PreProcessChar);
                        add += 1;
                    }
                    i += add;
                    break; }
                }
            }
        }

        quotes += (str[i] == '"' && str[i - 1] != '\\');
    }
}

const std::unordered_map<std::string, qss::SelectorElement::PositionType> qss::Selector::Combinators {
    { ">", SelectorElement::CHILD }, { "~", SelectorElement::SIBLING },
    { "+", SelectorElement::GENERAL_SIBLING }, { " ", SelectorElement::DESCENDANT },
    { ",", SelectorElement::ADJACENT }
};

#include "../include/qssselectorelement.h"

namespace {
inline qss::SelectorElement::PseudoState pseudoStateFromString(std::string&& str)
{
    if (str.size() == 0) {
        return {
            std::string {},
            false
        };
    }
    if (str[0] == '!') {
        if (str.size() == 1) {
            return {
                std::string {},
                true
            };
        }
        return {
            qss::TrimmedString(str.substr(1)),
            true
        };
    }
    return {
        std::move(str),
        false
    };
}
}

qss::SelectorElement::SelectorElement(const std::string & str)
{
    parse(str);
}

qss::SelectorElement& qss::SelectorElement::operator=(const SelectorElement &fragment)
{
    m_pseudoStates = fragment.m_pseudoStates;
    m_name = fragment.m_name;
    m_params = fragment.m_params;
    m_id = fragment.m_id;
    m_position = fragment.m_position;
    return *this;
}

qss::SelectorElement& qss::SelectorElement::select(const std::string &sel)
{
    m_name = TrimmedString(sel);
    return *this;
}

qss::SelectorElement& qss::SelectorElement::on(const std::string &key, const std::string &value)
{
    m_params[TrimmedString(key)] = value;
    return *this;
}

qss::SelectorElement& qss::SelectorElement::on(const QStringPairs &params)
{
    for (const auto& param : params)
    {
        m_params[TrimmedString(param.first)] = TrimmedString(param.second);
    }
    return *this;
}

qss::SelectorElement& qss::SelectorElement::sub(const std::string &name)
{
    m_subControl = TrimmedString(name);
    return *this;
}

qss::SelectorElement& qss::SelectorElement::when(const std::string &pcl)
{
    m_pseudoStates.push_back(pseudoStateFromString(TrimmedString(pcl)));
    return *this;
}

qss::SelectorElement& qss::SelectorElement::id(const std::string &str)
{
    m_id = TrimmedString(str);
    return *this;
}

qss::SelectorElement& qss::SelectorElement::name(const std::string &str)
{
    m_name = TrimmedString(str);
    return *this;
}

qss::SelectorElement& qss::SelectorElement::clazz(const std::string& str)
{
    auto it = std::find(m_classes.begin(), m_classes.end(), str);
    if (it == m_classes.end()) {
        m_classes.push_back(str);
    }
    return *this;
}

qss::SelectorElement& qss::SelectorElement::noclazz(const std::string& str)
{
    auto it = std::find(m_classes.begin(), m_classes.end(), str);
    if (it != m_classes.end()) {
        m_classes.erase(it);
    }
    return *this;
}

void qss::SelectorElement::parse(const std::string &str)
{
    auto selector = TrimmedString(str);

    if (selector.size() != 0)
    {
        auto remaining = extractSubControlAndPseudoState(selector);
        remaining = extractParams(remaining);
        extractNameAndSelector(remaining);
    }
}

std::string qss::SelectorElement::toString() const
{
    std::string result;

    if (m_name.size() > 0)
    {
        result += m_position != PARENT ? Combinators.at(m_position) + " " + m_name : m_name;
    }

    if (m_id.size() > 0)
    {
        result += Delimiters.at(QSS_ID_DELIMITER) + m_id;
    }

    for (const auto& cl : m_classes)
    {
        result += Delimiters.at(QSS_CLASS_DELIMITER) + cl;
    }

    for (auto const& pair : m_params)
    {
        result += Delimiters.at(QSS_SELECT_PARAM_START_DELIMITER) +
                pair.first + Delimiters.at(QSS_PARAM_DELIMITER) +
                QuotedString(pair.second) + Delimiters.at(QSS_SELECT_PARAM_END_DELIMITER);
    }

    if (m_subControl.size() > 0)
    {
        result += Delimiters.at(QSS_SUB_CONTROL_DELIMITER) + m_subControl;
    }

    if (!m_pseudoStates.empty())
    {
        for (const auto& pseudo : m_pseudoStates) {
            if (pseudo.negated) {
                result += Delimiters.at(QSS_PSEUDO_CLASS_DELIMITER) + '!' + pseudo.name;
            } else {
                result += Delimiters.at(QSS_PSEUDO_CLASS_DELIMITER) + pseudo.name;
            }
        }
    }

    return result;
}

bool qss::SelectorElement::isGeneralizedFrom(const SelectorElement &fragment) const
{
    // The params of this should all be contained in fragment, since
    // params is generalized from fragment i.e. fragment is more specific
    for (const auto& pair : m_params)
    {
        auto itr = fragment.m_params.find(pair.first);

        if (itr == fragment.m_params.cend())
        {
            return false;
        }
        else
        {
            if (itr->second != pair.second)
            {
                return false;
            }
        }
    }

    for (const auto& cl : m_classes)
    {
        auto itr = std::find(fragment.m_classes.cbegin(), fragment.m_classes.cend(), cl);

        if (itr == fragment.m_classes.cend())
        {
            return false;
        }
    }

    if (!m_id.empty() && fragment.m_id != m_id)
    {
        return false;
    }

    if (!m_name.empty() && fragment.m_name != m_name)
    {
        return false;
    }

    // this's pseudo class should be empty or same as fragment
    for (const auto& ps : m_pseudoStates)
    {
        auto itr = std::find_if(fragment.m_pseudoStates.cbegin(), fragment.m_pseudoStates.cend(), [&ps](const auto& ops) -> bool {
            return ops.name == ps.name;
        });
        if (!ps.negated && itr == fragment.m_pseudoStates.end())
        {
            return false;
        }
        else if (ps.negated && itr != fragment.m_pseudoStates.end())
        {
            return false;
        }
    }

    if (!m_subControl.empty() && fragment.m_subControl != m_subControl)
    {
        return false;
    }

    return true;
}

bool qss::SelectorElement::isSpecificThan(const SelectorElement &fragment) const
{
    return fragment.isGeneralizedFrom(*this);
}

std::string qss::SelectorElement::value(const std::string & key) const
{
    auto itr = m_params.find(key);

    if (itr != m_params.cend())
    {
        return itr->second;
    }

    return std::string{};
}

std::string qss::SelectorElement::extractSubControlAndPseudoState(const std::string &str)
{
    auto parts = SplitString(str, Delimiters.at(QSS_PSEUDO_CLASS_DELIMITER));

    if (parts.size() > 1)
    {
        if (parts[1].size() == 0 && parts.size() > 2)
        {
            m_subControl = parts[2];
            for (size_t pos = 3; pos < parts.size(); ++pos) {
                m_pseudoStates.push_back(pseudoStateFromString(std::move(parts[pos])));
            }
        }
        else
        {
            for (size_t pos = 1; pos < parts.size(); ++pos) {
                m_pseudoStates.push_back(pseudoStateFromString(std::move(parts[pos])));
            }
        }
    }

    return parts[0];
}

std::string qss::SelectorElement::extractParams(const std::string &str)
{
    auto parts = SplitString(str, Delimiters.at(QSS_SELECT_PARAM_START_DELIMITER), false);

    if (parts.size() == 1 && str.find(Delimiters.at(QSS_SELECT_PARAM_START_DELIMITER)) != std::string::npos)
    {
        throw Exception{ Exception::ILL_FORMED_HEADER_PARAM, str };
    }

    if (parts.size() > 1)
    {
        for (int i = 1; i < parts.size(); ++i)
        {
            auto params = SplitString(parts[i], Delimiters.at(QSS_PARAM_DELIMITER));
            if (params.size() != 2)
            {
                throw Exception{ Exception::HEADER_PARAM_INVALID, parts[i] };
            }
            else
            {
                auto value = params[1].erase(0, 1);
                auto size = value.size();
                value = value.erase(size - 2, 2);
                m_params[params[0]] = value;
            }
        }
    }

    // TODO Is this legitimate or a temporary workaround?
    return parts.size() > 0 ? parts[0] : "";
}

void qss::SelectorElement::extractNameAndSelector(const std::string &str)
{
    auto select = SplitString(str, Delimiters.at(QSS_ID_DELIMITER));
    auto parts = SplitString(select[0], Delimiters.at(QSS_CLASS_DELIMITER), true);
    if (parts.size()) m_name = TrimmedString(parts[0]);

    for (auto i = 1; i < parts.size(); ++i)
    {
        m_classes.push_back(parts[i]);
    }

    if (select.size() == 2)
    {
        parts = SplitString(select[1], Delimiters.at(QSS_CLASS_DELIMITER), false);
        m_id = TrimmedString(parts[0]);

        for (auto i = 1; i < parts.size(); ++i)
        {
            m_classes.push_back(parts[i]);
        }
    }
    else if (select.size() > 2)
    {
        throw Exception{ Exception::MULTIPLE_IDS, str };
    }
}

bool qss::operator==(const SelectorElement &lhs, const SelectorElement &rhs)
{
    return lhs.toString() == rhs.toString();
}

const std::unordered_map<int, std::string> qss::SelectorElement::Combinators{
    { SelectorElement::CHILD, ">" }, { SelectorElement::DESCENDANT, "" },
    { SelectorElement::SIBLING, "~" }, { SelectorElement::GENERAL_SIBLING, "+" },
    { SelectorElement::PARENT, "" }, { SelectorElement::ADJACENT, "," }
};


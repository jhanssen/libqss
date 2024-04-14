#include "../include/qsspropertyblock.h"

qss::PropertyBlock::PropertyBlock(const std::string & str)
{
    parse(str);
}

qss::PropertyBlock& qss::PropertyBlock::operator=(const PropertyBlock &block)
{
    m_params = block.m_params;
    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::addParam(const std::string &key, const std::string &value)
{
    auto tkey = TrimmedString(key);
    m_params[tkey].first = TrimmedString(value);
    m_params[tkey].second = true;
    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::addParam(const QStringPairs &params)
{
    for (const auto& param : params)
    {
        auto tkey = TrimmedString(param.first);
        m_params[tkey].first = TrimmedString(param.second);
        m_params[tkey].second = true;
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::enableParam(const std::string &key, bool enable)
{
    auto tkey = TrimmedString(key);
    auto itr = m_params.find(tkey);

    if (itr != m_params.cend())
    {
        m_params[tkey].second = enable;
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::toggleParam(const std::string &key)
{
    auto tkey = TrimmedString(key);
    auto itr = m_params.find(tkey);

    if (itr != m_params.cend())
    {
        m_params[tkey].second = !itr->second.second;
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::remove(const std::string &key)
{
    if (m_params.count(key))
    {
        m_params.erase(key);
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::remove(const std::vector<std::string> &keys)
{
    for (auto const& key : keys)
    {
        remove(key);
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::operator+=(const PropertyBlock & block)
{
    for (const auto& pair : block.m_params)
    {
        m_params[pair.first] = pair.second;
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::operator+=(const std::string & block)
{
    return this->operator+=(PropertyBlock{ block });
}

void qss::PropertyBlock::parse(const std::string &input)
{
    auto str = TrimmedString(input);

    if (str.size() > 0)
    {
        auto parts = SplitString(str, Delimiters.at(QSS_STATEMENT_END_DELIMITER), false);

        for (auto const& part : parts)
        {
            auto line = SplitString(part, Delimiters.at(QSS_PSEUDO_CLASS_DELIMITER), false);

            // Allow more than 2 types in split, because the property value may
            // contain further delimiters inside quotes. This is not a solution,
            // but only a workaround: a full parsing of quoted content must be done instead.
            if (line.size() >= 2)
            {
                auto key = TrimmedString(line[0]);
                auto value = TrimmedString(line[1]);
                m_params[key].first = value;
                m_params[key].second = true;
            }
            else
            {
                throw Exception{ Exception::BLOCK_PARAM_INVALID, part };
            }
        }
    }
}

std::string qss::PropertyBlock::toString() const
{
    std::string result;

    for (auto const& pair : m_params)
    {
        if (pair.second.second)
        {
            result += "\t" + pair.first + Delimiters.at(QSS_PSEUDO_CLASS_DELIMITER);
            result += " " + pair.second.first + Delimiters.at(QSS_STATEMENT_END_DELIMITER) + "\n";
        }
    }

    return result;
}

std::size_t qss::PropertyBlock::size() const noexcept
{
    return m_params.size();
}

bool qss::operator==(const PropertyBlock & lhs, const PropertyBlock & rhs)
{
    return lhs.toString() == rhs.toString();
}

qss::PropertyBlock qss::operator+(const PropertyBlock & lhs, const PropertyBlock & rhs)
{
    PropertyBlock sum = lhs;
    sum += rhs;
    return sum;
}

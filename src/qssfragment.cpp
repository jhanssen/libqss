#include "../include/qssfragment.h"

qss::Fragment::Fragment(const std::string & input)
{
    parse(input);
}

qss::Fragment& qss::Fragment::operator=(const Fragment &fragment)
{
    m_selector = fragment.m_selector;
    m_block = fragment.m_block;
    return *this;
}

qss::Fragment& qss::Fragment::select(const Selector& selector)
{
    m_selector = selector;
    return *this;
}

qss::Fragment& qss::Fragment::select(const std::string &selector)
{
    m_selector.parse(selector);
    return *this;
}

qss::Fragment& qss::Fragment::addBlock(const PropertyBlock& block)
{
    m_block += block;
    return *this;
}

qss::Fragment& qss::Fragment::addBlock(const std::string &block)
{
    m_block += block;
    return *this;
}

qss::Fragment& qss::Fragment::addBlock(const QStringPairs &block)
{
    m_block.addParam(block);
    return *this;
}

qss::Fragment& qss::Fragment::addParam(const QStringPair &param)
{
    m_block.addParam(param.first, param.second);
    return *this;
}

qss::Fragment& qss::Fragment::addParam(const std::string &key, const std::string &val)
{
    m_block.addParam(key, val);
    return *this;
}

qss::Fragment& qss::Fragment::enableParam(const std::string &key, bool enable)
{
    m_block.enableParam(key, enable);
    return *this;
}

qss::Fragment& qss::Fragment::remove(const std::string &name)
{
    m_block.remove(name);
    return *this;
}

qss::Fragment& qss::Fragment::remove(const std::vector<std::string> &names)
{
    m_block.remove(names);
    return *this;
}

void qss::Fragment::parse(const std::string &input)
{
    auto str = TrimmedString(input);

    if (str.size() > 0)
    {
        auto start = str.find(Delimiters.at(QSS_BLOCK_START_DELIMITER));
        auto end = str.find(Delimiters.at(QSS_BLOCK_END_DELIMITER));

        if (start != std::string::npos && end != std::string::npos && (end - start) > 0)
        {
            std::string header = str, body = str;
            m_selector.parse(TrimmedString(header.erase(start, header.size())));
            body = body.substr(0, end);
            body.erase(0, start + 1);
            m_block.parse(TrimmedString(body));
        }
        else
        {
            throw Exception{ Exception::BLOCK_BRACKETS_INVALID, str };
        }
    }
}

std::string qss::Fragment::toString() const
{
    std::string result = m_selector.toString();
    result += " " + Delimiters.at(QSS_BLOCK_START_DELIMITER) + "\n";
    result += m_block.toString() + Delimiters.at(QSS_BLOCK_END_DELIMITER) + "\n";
    return result;
}

bool qss::operator==(const Fragment &lhs, const Fragment &rhs)
{
    return lhs.toString() == rhs.toString();
}

#include "../include/qssdocument.h"
#include <ctre.hpp>

#include <algorithm>

static constexpr auto commentPattern = ctll::fixed_string{ R"((//.*?$|/\*[\S\s]*?\*/)|(\\'(?:\\.|[^\\\\'])*\\'|"(?:\\.|[^\\"])*"))" };

qss::Document::Document(const std::string &str)
{
    std::string strNoComments;

    size_t left = 0;
    for (auto match : ctre::multiline_search_all<commentPattern>(str)) {
        auto off = std::distance(str.begin(), match.begin());
        auto end = std::distance(str.begin(), match.end());
        if (off > left) {
            strNoComments += str.substr(left, off - left);
        }
        left = end;
    }
    if (left < str.size()) {
        strNoComments += str.substr(left);
    }

    parse(strNoComments);
}

qss::Document& qss::Document::addFragment(const Fragment& fragment, bool enabled)
{
    auto selectorExists = false;

    for (auto& frags : m_fragments)
    {
        if (frags.first.selector() == fragment.selector())
        {
            frags.first.addBlock(fragment.block());
            selectorExists = true;
        }
    }

    if (!selectorExists)
    {
        m_fragments.push_back(std::make_pair(fragment, enabled));
    }

    return *this;
}

qss::Document& qss::Document::toggleFragment(int index)
{
    m_fragments[index].second = !m_fragments[index].second;
    return *this;
}

qss::Document& qss::Document::addFragment(const std::string& fragment, bool enabled)
{
    return addFragment(Fragment{ fragment }, enabled);
}

qss::Document& qss::Document::enableFragment(int index, bool enable)
{
    m_fragments[index].second = enable;
    return *this;
}

qss::Document& qss::Document::removeFragment(const std::string& fragment)
{
    auto it = std::remove_if(m_fragments.begin(), m_fragments.end(), [fragment](const QSSFragmentPair& existing){
        return existing.first.toString() == Fragment{ fragment }.toString();
    });
    (void)it;
    return *this;
}

qss::Document& qss::Document::removeFragment(int index)
{
    m_fragments.erase(m_fragments.begin() + index);
    return *this;
}

qss::Document& qss::Document::operator+=(const std::string& fragment)
{
    addFragment(fragment, true);
    return *this;
}

qss::Document& qss::Document::operator+=(const Document & qss)
{
    for (const auto& fragment : qss)
    {
        addFragment(fragment.first, true);
    }

    return *this;
}

qss::Document qss::Document::inheritable(const std::string & input) const
{
    Document qss;
    SelectorElement selector{ input };

    for (const auto& pair : m_fragments)
    {
        const auto& fragment = pair.first;
        const auto& fselect = fragment.selector();

        if (fselect.fragmentCount() == 1)
        {
            if (fselect[0].id() != selector.id())
            {
                continue;
            }

            Fragment frag;
            frag.parse(Delimiters.at(QSS_BLOCK_START_DELIMITER) + " " +
                fragment.block().toString() + " " + Delimiters.at(QSS_BLOCK_END_DELIMITER));
            qss.addFragment(fragment);
        }

        if (fragment.selector().fragmentCount() <= 2)
        {
            if (fragment.selector()[1].toString() == selector)
            {
                Fragment frag;
                frag.parse(Delimiters.at(QSS_BLOCK_START_DELIMITER) + " " +
                    fragment.block().toString() + " " + Delimiters.at(QSS_BLOCK_END_DELIMITER));
                qss.addFragment(fragment);
            }
        }
    }

    return qss;
}

void qss::Document::parse(const std::string& input)
{
    auto insideStr = false;
    std::string fragment;

    for (auto i = 0; i < input.size(); ++i)
    {
        fragment += input.at(i);

        if (input.at(i) == '"')
        {
            insideStr = !insideStr;
        }

        if (insideStr)
            continue;
        else
        {
            if (input.at(i) == Delimiters.at(QSS_BLOCK_END_DELIMITER)[0])
            {
                m_fragments.emplace_back(std::make_pair(fragment, true));
                fragment.clear();
            }
        }
    }
}

std::string qss::Document::toString() const
{
    std::string result;

    for (const auto& pair : m_fragments)
    {
        if (pair.second)
        {
            result += pair.first.toString() + "\n";
        }
    }

    return result;
}

std::size_t qss::Document::totalActiveFragments() const
{
    return std::count_if(m_fragments.cbegin(), m_fragments.cend(), [](const QSSFragmentPair& pair){
        return pair.second;
    });
}

qss::Document qss::operator+(const Document & lhs, const Document & rhs)
{
    Document sum = lhs;
    sum += rhs;
    return sum;
}

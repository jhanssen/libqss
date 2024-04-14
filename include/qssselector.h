#ifndef QSSSELECTOR_H
#define QSSSELECTOR_H

#include "qssselectorelement.h"

namespace qss
{
    class QSS_API Selector : public IParseable
    {
    public:

        typedef typename std::deque<SelectorElement>::const_iterator ConstItr;
        typedef typename std::deque<SelectorElement>::iterator Itr;

        Selector() {}
        Selector(const std::string& str);
        Selector& operator=(const Selector& selector);

        Selector& addChild(const SelectorElement& fragment);
        Selector& addChild(const std::string& fragment);
        Selector& addDescendant(const SelectorElement& fragment);
        Selector& addDescendant(const std::string& fragment);
        Selector& addGeneralSibling(const SelectorElement& fragment);
        Selector& addGeneralSibling(const std::string& fragment);
        Selector& addSibling(const SelectorElement& fragment);
        Selector& addSibling(const std::string& fragment);
        Selector& append(const std::string& fragment, SelectorElement::PositionType type);
        Selector& append(const SelectorElement& fragment, SelectorElement::PositionType type);

        void    parse(const std::string& input);
        std::string toString() const;
        std::size_t fragmentCount() const  noexcept { return m_fragments.size(); }

        ConstItr cbegin() const noexcept { return m_fragments.cbegin(); }
        ConstItr cend() const noexcept { return m_fragments.cend(); }
        ConstItr begin() const noexcept { return m_fragments.cbegin(); }
        ConstItr end() const noexcept { return m_fragments.cend(); }
        Itr      begin() noexcept { return m_fragments.begin(); }
        Itr      end() noexcept { return m_fragments.end(); }

        const SelectorElement& front() const { return m_fragments.front(); }
        const SelectorElement& back() const { return m_fragments.back(); }

        SelectorElement& front() { return m_fragments.front(); }
        SelectorElement& back() { return m_fragments.back(); }

        SelectorElement& operator[](int index) { return m_fragments[index]; }
        const SelectorElement& operator[](int index) const { return m_fragments[index]; }

        friend bool operator==(const Selector& lhs, const Selector& rhs);

    private:

        void preProcess(std::string& str);

        const static char PreProcessChar = '`';
        const static std::unordered_map<std::string, SelectorElement::PositionType> Combinators;

        std::deque<SelectorElement> m_fragments;
    };

    bool operator==(const Selector& lhs, const Selector& rhs);
}

#endif // QSSSELECTOR_H

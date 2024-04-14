#ifndef QSSDOCUMENT_H
#define QSSDOCUMENT_H

#include "qssfragment.h"

#include <algorithm>

namespace qss
{
    class QSS_API Document : public IParseable
    {
    public:

        typedef InvalidablePair<Fragment> QSSFragmentPair;
        typedef typename std::deque<QSSFragmentPair>::const_iterator ConstItr;
        typedef typename std::deque<QSSFragmentPair>::iterator Itr;

        Document() {}
        Document(const std::string& qss);
        virtual ~Document() {}

        Document& addFragment(const Fragment& fragment, bool enabled = true);
        Document& addFragment(const std::string& fragment, bool enabled = true);
        Document& enableFragment(int index, bool enable = true);
        Document& toggleFragment(int index);
        Document& removeFragment(const std::string& fragment);
        Document& removeFragment(int index);
        Document& operator+=(const std::string& fragment);
        Document& operator+=(const Document& qss);

        Document inheritable(const std::string& selector) const;

        void parse(const std::string&);
        std::string toString() const;

        const Fragment& operator[](int index) const { return m_fragments[index].first; }
        std::size_t totalFragments() const noexcept { return m_fragments.size(); }
        std::size_t totalActiveFragments() const;

        ConstItr cbegin() const noexcept { return m_fragments.cbegin(); }
        ConstItr cend() const noexcept { return m_fragments.cend(); }
        ConstItr begin() const noexcept { return m_fragments.cbegin(); }
        ConstItr end() const noexcept { return m_fragments.cend(); }
        Itr      begin() noexcept { return m_fragments.begin(); }
        Itr      end() noexcept { return m_fragments.end(); }

        const Fragment& front() const noexcept { return m_fragments.front().first; }
        const Fragment& back() const noexcept { return m_fragments.back().first; }

        Fragment& front() noexcept { return m_fragments.front().first; }
        Fragment& back() noexcept { return m_fragments.back().first; }

        friend Document operator+(const Document& lhs, const Document& rhs);

    private:

        std::deque<QSSFragmentPair> m_fragments;
    };

    Document operator+(const Document& lhs, const Document& rhs);
}

#endif // QSSTEXT_H

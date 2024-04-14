#ifndef QSSFRAGMENT_H
#define QSSFRAGMENT_H

#include "qssselector.h"
#include "qsspropertyblock.h"

namespace qss
{
    class QSS_API Fragment : public IParseable
    {
    public:

        Fragment() {}
        Fragment(const std::string& str);
        Fragment& operator=(const Fragment& fragment);

        Fragment& select(const Selector& selector);
        Fragment& select(const std::string& selector);
        Fragment& addBlock(const PropertyBlock& block);
        Fragment& addBlock(const std::string& block);
        Fragment& addBlock(const QStringPairs& block);
        Fragment& addParam(const QStringPair& param);
        Fragment& addParam(const std::string& key, const std::string& val);
        Fragment& enableParam(const std::string& key, bool enable = false);
        Fragment& remove(const std::string& name);
        Fragment& remove(const std::vector<std::string>& names);

        const Selector& selector() const noexcept { return m_selector; }
        const PropertyBlock& block() const noexcept { return m_block; }

        Selector& selector() noexcept { return m_selector; }
        PropertyBlock& block() noexcept { return m_block; }

        void    parse(const std::string& input);
        std::string toString() const;

        friend bool operator==(const Fragment& lhs, const Fragment& rhs);

    private:

        Selector m_selector;
        PropertyBlock    m_block;
    };

    bool operator==(const Fragment& lhs, const Fragment& rhs);
}

#endif // QSSFRAGMENT_H

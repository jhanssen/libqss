#ifndef QSSPROPERTYBLOCK_H
#define QSSPROPERTYBLOCK_H

#include "qssparseable.h"
#include "qssexception.h"

namespace qss
{
    class QSS_API PropertyBlock : public IParseable
    {
    public:

        typedef typename PropertyMap::const_iterator ConstItr;
        typedef typename PropertyMap::iterator Itr;

        PropertyBlock() {}
        PropertyBlock(const std::string& str);
        PropertyBlock& operator=(const PropertyBlock& block);

        PropertyBlock& addParam(const std::string& key, const std::string& value);
        PropertyBlock& addParam(const QStringPairs& params);
        PropertyBlock& enableParam(const std::string& key, bool enable = true);
        PropertyBlock& toggleParam(const std::string& key);
        PropertyBlock& remove(const std::string& key);
        PropertyBlock& remove(const std::vector<std::string>& keys);

        PropertyBlock& operator+=(const PropertyBlock& block);
        PropertyBlock& operator+=(const std::string& block);

        void    parse(const std::string& input);
        std::string toString() const;
        std::size_t size() const noexcept;

        ConstItr cbegin() const noexcept { return m_params.cbegin(); }
        ConstItr cend() const noexcept { return m_params.cend(); }
        Itr      begin() noexcept { return m_params.begin(); }
        Itr      end() noexcept { return m_params.end(); }

        friend bool operator==(const PropertyBlock& lhs, const PropertyBlock& rhs);
        friend PropertyBlock operator+(const PropertyBlock& lhs, const PropertyBlock& rhs);

    private:

        PropertyMap m_params;
    };

    bool operator==(const PropertyBlock& lhs, const PropertyBlock& rhs);
    
    PropertyBlock operator+(const PropertyBlock& lhs, const PropertyBlock& rhs);
}

#endif // QSSBLOCK_H

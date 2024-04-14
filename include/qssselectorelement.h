#ifndef QSSSELECTORFRAGMENT_H
#define QSSSELECTORFRAGMENT_H

#include "qssparseable.h"
#include "qssexception.h"

namespace qss
{
    class QSS_API SelectorElement : public IParseable
    {
    public:

        enum PositionType
        {
            ADJACENT, PARENT, CHILD, DESCENDANT, SIBLING, GENERAL_SIBLING
        };

        SelectorElement() {}
        SelectorElement(const std::string& str);
        SelectorElement& operator=(const SelectorElement& fragment);

        SelectorElement& select(const std::string& sel);
        SelectorElement& on(const std::string& key, const std::string& value);
        SelectorElement& on(const QStringPairs& params);
        SelectorElement& sub(const std::string& name);
        SelectorElement& when(const std::string& pcl);
        SelectorElement& name(const std::string& str);

        void    parse(const std::string& input);
        std::string toString() const;
        bool    isGeneralizedFrom(const SelectorElement& fragment) const;
        bool    isSpecificThan(const SelectorElement& fragment) const;
        std::string id() const { return m_id; }
        std::string psuedoClass() const { return m_psuedoClass; }
        std::string subControl() const { return m_subControl; }
        std::string name() const { return m_name; }
        std::string value(const std::string& key) const;

        PositionType position() const noexcept { return m_position; }

        const QStringList& classes() const noexcept { return m_classes; }
        const QStringMap& params() const noexcept { return m_params; }

        std::size_t classCount() const noexcept { return m_classes.size(); }
        std::size_t paramCount() const noexcept { return m_params.size(); }

        friend bool operator==(const SelectorElement& lhs, const SelectorElement& rhs);

    private:

        static const std::unordered_map<int, std::string> Combinators;

        friend class Selector;

        std::string extractSubControlAndPsuedoClass(const std::string& str);
        std::string extractParams(const std::string& str);
        void    extractNameAndSelector(const std::string& str);

        std::string      m_name;
        std::string      m_id;
        std::string      m_subControl;
        std::string      m_psuedoClass;
        QStringMap   m_params;
        PositionType m_position = PARENT;
        QStringList  m_classes;
    };

    bool operator==(const SelectorElement& lhs, const SelectorElement& rhs);
}

#endif // QSSSELECTORFRAGMENT_H

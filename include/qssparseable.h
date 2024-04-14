#ifndef QSSPARSEABLE_H
#define QSSPARSEABLE_H

#include "qssutils.h"

namespace qss
{
    class QSS_API IParseable
    {
    public:

        IParseable() {}
        virtual ~IParseable() {}

        virtual std::string toString() const = 0;
        virtual void    parse(const std::string& str) = 0;
    };
}

#endif

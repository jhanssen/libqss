#ifndef QSSUTILS_H
#define QSSUTILS_H

#include <string>
#include <unordered_map>
#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include <type_traits>

#ifdef _WIN32
#ifdef qss_EXPORTS
#define QSS_API __declspec(dllexport)
#else
#ifndef QSS_STATIC
#define QSS_API __declspec(dllimport)
#else
#define QSS_API
#endif
#endif
#else // _WIN32
#define QSS_API
#endif // _WIN32


#define LOG(X)  { std::cout << qss::tab(2) << X << std::endl; }
#define START() { std::cout << qss::tab(1) << "In [" __FUNCTION__ << "]" << std::endl; }
#define END() { qss::tab(0); std::cout << qss::tab(2) << "Out [" __FUNCTION__ << "]" << std::endl; }

namespace qss
{
    inline std::string tab(int in)
    {
        static std::string _tabs;
        auto local = _tabs;
        switch (in)
        {
        case 1: _tabs.append("\t"); break;
        case 2: break;
        case 0: _tabs.erase(0, 1); break;
        }
        return local;
    }

    template <typename T> using InvalidablePair = std::pair<T, bool>;
    using QStringPair = std::pair<std::string, std::string>;
    using QStringPairs = std::vector<QStringPair>;
    using QStringMap = std::unordered_map<std::string, std::string>;
    using QStringList = std::vector<std::string>;
    using PropertyMap = std::unordered_map<std::string, InvalidablePair<std::string>>;

    enum Delimiter
    {
        QSS_ID_DELIMITER,
        QSS_CLASS_DELIMITER,
        QSS_ADJACENT_SIBLING_DELIMITER,
        QSS_GENERAL_SIBLING_DELIMITER,
        QSS_DESCENDANT_DELIMITER,
        QSS_CHILD_DELIMITER,
        QSS_PSEUDO_CLASS_DELIMITER,
        QSS_STATEMENT_END_DELIMITER,
        QSS_PARAM_DELIMITER,
        QSS_BLOCK_START_DELIMITER,
        QSS_BLOCK_END_DELIMITER,
        QSS_SELECT_PARAM_START_DELIMITER,
        QSS_SELECT_PARAM_END_DELIMITER,
        QSS_SUB_CONTROL_DELIMITER
    };

    const std::unordered_map<Delimiter, std::string> Delimiters{
        { QSS_ID_DELIMITER, "#" },
        { QSS_CLASS_DELIMITER, "." },
        { QSS_ADJACENT_SIBLING_DELIMITER, "+" },
        { QSS_GENERAL_SIBLING_DELIMITER, "~" },
        { QSS_DESCENDANT_DELIMITER, " " },
        { QSS_CHILD_DELIMITER, ">" },
        { QSS_PSEUDO_CLASS_DELIMITER, ":" },
        { QSS_STATEMENT_END_DELIMITER, ";" },
        { QSS_PARAM_DELIMITER, "=" },
        { QSS_BLOCK_START_DELIMITER, "{" },
        { QSS_BLOCK_END_DELIMITER, "}" },
        { QSS_SELECT_PARAM_START_DELIMITER, "[" },
        { QSS_SELECT_PARAM_END_DELIMITER, "]" },
        { QSS_SUB_CONTROL_DELIMITER, "::" }
    };

    inline std::string QuotedString(const std::string& input)
    {
        return "\"" + input + "\"";
    }

    inline std::string TrimmedString(std::string str)
    {
        // remove trailing white space
        while( !str.empty() && std::isspace( str.back() ) ) str.pop_back() ;

        // return residue after leading white space
        std::size_t pos = 0 ;
        while( pos < str.size() && std::isspace( str[pos] ) ) ++pos ;
        return str.substr(pos) ;
    }

    inline std::vector<std::string> SplitString(const std::string& str, char delim, bool keepEmpty = true)
    {
        std::vector<std::string> result;
        auto left = str.begin();
        for (auto it = left; it != str.end(); ++it)
        {
            if (*it == delim)
            {
                if (it > left || keepEmpty)
                    result.emplace_back(&*left, it - left);
                left = it + 1;
            }
        }
        if (left != str.end())
            result.emplace_back(&*left, str.end() - left);
        return result;
    }

    inline std::vector<std::string> SplitString(const std::string& str, const std::string& delim, bool keepEmpty = true)
    {
        std::vector<std::string> result;
        const auto dsz = delim.size();
        size_t left = 0;
        for (size_t idx = 0; idx < str.size(); ++idx)
        {
            // ### should be improved
            if (str.substr(idx, dsz) == delim)
            {
                if (idx > left || keepEmpty)
                    result.emplace_back(&str[left], idx - left);
                left = idx + 1;
            }
        }
        if (left != str.size())
            result.emplace_back(&str[left], str.size() - left);
        return result;
    }
}

#endif

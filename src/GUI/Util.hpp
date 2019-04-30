#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include <sstream>
#include <iomanip>

#include <QStringList>

/* Utility functions for GUI code. */

namespace CATE {

/* Given a std::vector, return a QStringList of its values. */
template <class T>
QStringList vector_to_qsl(const std::vector<T> &vec)
{
    QStringList qsl;

    for (const auto &value : vec)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << value;
        std::string str_value = ss.str();
        qsl << QString(QString::fromUtf8(str_value.c_str()));
    }

    return qsl;
}

} // CATE

#endif
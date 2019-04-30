#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include <sstream>
#include <iomanip>

#include <QStringList>
#include <QLabel>

using std::string;
using std::vector;

namespace CATE {

/* Given a std::vector, return a QStringList of its values. */
template <class T>
inline QStringList vector_to_qsl(const vector<T> &vec);

/* Update numeric Qt label with new value. */
template <class T>
inline void update_number_label(QLabel *label, T value);

/* Convert std::string to QString. */
inline QString string_to_qstring(const string &str);

/* Convert QString to std::string. */
inline string qstring_to_string(const QString &qstr);

template <class T>
inline QStringList vector_to_qsl(const vector<T> &vec)
{
    QStringList qsl;

    for (const auto &value : vec)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << value;
        string str_value = ss.str();
        qsl << QString(QString::fromUtf8(str_value.c_str()));
    }

    return qsl;
}

template <class T>
inline void update_number_label(QLabel *label, T value)
{
    label->setText(QString::number(value));
}

inline QString string_to_qstring(const string &str)
{
    QString qstr = QString::fromUtf8(str.c_str());
    return qstr;
}

inline string qstring_to_string(const QString &qstr)
{
    string str = qstr.toUtf8().constData();
    return str;
}

} // CATE

#endif
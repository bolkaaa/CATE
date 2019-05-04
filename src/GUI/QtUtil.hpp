/*
  CATE: Concatenative Audio Processing Application
  Copyright (c) 2019 Liam Wyllie. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef QTUTIL_HPP
#define QTUTIL_HPP

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
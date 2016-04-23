#ifndef GGSERIALIZATION_HPP
#define GGSERIALIZATION_HPP

#include <QVariant>

template <typename T>
void operator << (QVariant &v, const T &t) {
    v = QVariant::fromValue<T>(t);
}

template <typename T>
void operator >> (const QVariant &v, T &t) {
    t = v.value<T>();
}

#endif // GGSERIALIZATION_HPP

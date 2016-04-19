#ifndef GGSERIALIZATION_HPP
#define GGSERIALIZATION_HPP

#include <QVariant>

template <typename T>
void operator << (QVariant &v, const T &t) {
    v = QVariant::fromValue<T>(t);
}

#endif // GGSERIALIZATION_HPP

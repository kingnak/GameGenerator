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

template <typename T>
void operator << (QVariant &v, const QList<T> &l) {
    QVariantList lst;
    lst.reserve(l.size());
    for (int i = 0; i < l.size(); ++i) {
        QVariant d;
        d << l[i];
        lst.append(d);
    }
    v = lst;
}

template <typename T>
void operator >> (const QVariant &v, QList<T> &l) {
    l.clear();
    QVariantList lst = v.toList();
    l.reserve(lst.size());
    for (int i = 0; i < lst.size(); ++i) {
        T d;
        lst[i] >> d;
        l.append(d);
    }
}

#endif // GGSERIALIZATION_HPP

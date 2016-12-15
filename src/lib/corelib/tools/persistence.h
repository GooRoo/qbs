/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qbs.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QBS_PERSISTENCE
#define QBS_PERSISTENCE

#include "persistentobject.h"
#include <logging/logger.h>

#include <QDataStream>
#include <QSharedPointer>
#include <QString>
#include <QVariantMap>
#include <QVector>

#include <type_traits>

namespace qbs {
namespace Internal {

class PersistentPool
{
public:
    PersistentPool(Logger &logger);
    ~PersistentPool();

    class HeadData
    {
    public:
        QVariantMap projectConfig;
    };

    // We need a helper class template, because we require partial specialization for some of
    // the aggregate types, which is not possible with function templates.
    template<typename T, typename Enable = void> struct Helper { };

    template<typename T> void store(const T &value) { Helper<T>().store(value, this); }
    template<typename T> void load(T &value) { Helper<T>().load(value, this); }
    template<typename T> T load() {
        T tmp;
        Helper<T>().load(tmp, this);
        return tmp;
    }

    void load(const QString &filePath);
    void setupWriteStream(const QString &filePath);
    void finalizeWriteStream();
    void closeStream();
    void clear();
    QDataStream &stream();

    const HeadData &headData() const { return m_headData; }
    void setHeadData(const HeadData &hd) { m_headData = hd; }

private:
    typedef int PersistentObjectId;

    template <typename T> T *idLoad();
    template <class T> QSharedPointer<T> idLoadS();
    template <class T> T *loadRaw(PersistentObjectId id);
    template <class T> QSharedPointer<T> load(PersistentObjectId id);

    void storePersistentObject(const PersistentObject *object);

    void storeVariant(const QVariant &variant);
    QVariant loadVariant();

    void storeString(const QString &t);
    QString loadString(int id);
    QString idLoadString();

    QDataStream m_stream;
    HeadData m_headData;
    QVector<PersistentObject *> m_loadedRaw;
    QVector<QSharedPointer<PersistentObject> > m_loaded;
    QHash<const PersistentObject *, int> m_storageIndices;
    PersistentObjectId m_lastStoredObjectId;

    QVector<QString> m_stringStorage;
    QHash<QString, int> m_inverseStringStorage;
    PersistentObjectId m_lastStoredStringId;
    Logger &m_logger;
};

template <typename T> inline T *PersistentPool::idLoad()
{
    PersistentObjectId id;
    stream() >> id;
    return loadRaw<T>(id);
}

template <class T> inline QSharedPointer<T> PersistentPool::idLoadS()
{
    PersistentObjectId id;
    m_stream >> id;
    return load<T>(id);
}

template <class T> inline T *PersistentPool::loadRaw(PersistentObjectId id)
{
    if (id < 0)
        return 0;

    if (id < m_loadedRaw.count()) {
        PersistentObject *obj = m_loadedRaw.value(id);
        return dynamic_cast<T*>(obj);
    }

    int i = m_loadedRaw.count();
    m_loadedRaw.resize(id + 1);
    for (; i < m_loadedRaw.count(); ++i)
        m_loadedRaw[i] = 0;

    T * const t = new T;
    PersistentObject * const po = t;
    m_loadedRaw[id] = po;
    po->load(*this);
    return t;
}

template <class T> inline QSharedPointer<T> PersistentPool::load(PersistentObjectId id)
{
    if (id < 0)
        return QSharedPointer<T>();

    if (id < m_loaded.count()) {
        QSharedPointer<PersistentObject> obj = m_loaded.value(id);
        return obj.dynamicCast<T>();
    }

    m_loaded.resize(id + 1);
    const QSharedPointer<T> t = T::create();
    m_loaded[id] = t;
    PersistentObject * const po = t.data();
    po->load(*this);
    return t;
}

/***** Specializations of Helper class *****/

template<typename T>
struct PersistentPool::Helper<T, typename std::enable_if<std::is_integral<T>::value>::type>
{
    static void store(const T &value, PersistentPool *pool) { pool->stream() << value; }
    static void load(T &value, PersistentPool *pool) { pool->stream() >> value; }
};

// TODO: Use constexpr function once we require MSVC 2015.
template<typename T> struct IsPersistentObject
{
    static const bool value = std::is_base_of<PersistentObject, T>::value;
};

template<typename T>
struct PersistentPool::Helper<QSharedPointer<T>,
                              typename std::enable_if<IsPersistentObject<T>::value>::type>
{
    static void store(const QSharedPointer<T> &value, PersistentPool *pool)
    {
        pool->store(value.data());
    }
    static void load(QSharedPointer<T> &value, PersistentPool *pool)
    {
        value = pool->idLoadS<typename std::remove_const<T>::type>();
    }
};

template<typename T>
struct PersistentPool::Helper<T *, typename std::enable_if<IsPersistentObject<T>::value>::type>
{
    static void store(const T *value, PersistentPool *pool) { pool->storePersistentObject(value); }
    void load(T* &value, PersistentPool *pool) { value = pool->idLoad<T>(); }
};

template<> struct PersistentPool::Helper<QString>
{
    static void store(const QString &s, PersistentPool *pool) { pool->storeString(s); }
    static void load(QString &s, PersistentPool *pool) { s = pool->idLoadString(); }
};

template<> struct PersistentPool::Helper<QVariant>
{
    static void store(const QVariant &v, PersistentPool *pool) { pool->storeVariant(v); }
    static void load(QVariant &v, PersistentPool *pool) { v = pool->loadVariant(); }
};

class ArtifactSet;
template<typename T> struct IsSimpleContainer { static const bool value = false; };
template<> struct IsSimpleContainer<ArtifactSet> { static const bool value = true; };
template<> struct IsSimpleContainer<QStringList> { static const bool value = true; };
template<typename T> struct IsSimpleContainer<QVector<T>> { static const bool value = true; };
template<typename T> struct IsSimpleContainer<QList<T>> { static const bool value = true; };
template<typename T> struct IsSimpleContainer<QSet<T>> { static const bool value = true; };

template<typename T>
struct PersistentPool::Helper<T, typename std::enable_if<IsSimpleContainer<T>::value>::type>
{
    static void store(const T &container, PersistentPool *pool)
    {
        pool->stream() << container.count();
        for (auto it = container.cbegin(); it != container.cend(); ++it)
            pool->store(*it);
    }
    static void load(T &container, PersistentPool *pool)
    {
        const int count = pool->load<int>();
        container.clear();
        container.reserve(count);
        for (int i = count; --i >= 0;)
            container += pool->load<typename T::value_type>();
    }
};

template<typename T> struct IsKeyValueContainer { static const bool value = false; };
template<typename K, typename V> struct IsKeyValueContainer<QMap<K, V>>
{
    static const bool value = true;
};

template<typename T>
struct PersistentPool::Helper<T, typename std::enable_if<IsKeyValueContainer<T>::value>::type>
{
    static void store(const T &container, PersistentPool *pool)
    {
        pool->stream() << container.count();
        for (auto it = container.cbegin(); it != container.cend(); ++it) {
            pool->store(it.key());
            pool->store(it.value());
        }
    }
    static void load(T &container, PersistentPool *pool)
    {
        container.clear();
        const int count = pool->load<int>();
        for (int i = 0; i < count; ++i) {
            const auto &key = pool->load<typename T::key_type>();
            const auto &value = pool->load<typename T::mapped_type>();
            container.insert(key, value);
        }
    }
};

} // namespace Internal
} // namespace qbs

#endif

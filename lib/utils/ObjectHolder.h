#pragma once

#include <stdexcept>
#include <utility>

///@todo rename to ObjectHolder<
template<typename RawType, typename Type>
class ObjectHolder
{
  public:
    ObjectHolder& operator=(const ObjectHolder& other)
    {
        if (this == &other)
            return *this;

        ObjectHolder tmp(other);
        this->swap(tmp);
        return *this;
    }

    ObjectHolder& operator=(ObjectHolder&& other) noexcept
    {
        if (this == &other)
            return *this;

        this->swap(other);
        other.release();
        return *this;
    }

    RawType* raw()
    {
        return m_raw;
    };

    const RawType* raw() const
    {
        return m_raw;
    };

    void attach(RawType* raw)
    {
        release();
        m_raw = raw;
        m_isAcquired = true;
    }

    RawType* detach()
    {
        if (!m_isAcquired)
            throw std::runtime_error("Unable to detach object. Object not acquired");

        m_isAcquired = false;
        return std::exchange(m_raw, nullptr);
    }

    bool isAcquired() noexcept
    {
        return m_isAcquired;
    }

    void release()
    {
        if (m_isAcquired && m_raw)
            Type::destroy(m_raw);

        m_raw = nullptr;
        m_isAcquired = false;
    }

    void cloneIfNotAcquire()
    {
        if (m_isAcquired)
            return;

        m_raw = Type::duplicate(m_raw);
        m_isAcquired = true;
    }

    void swap(ObjectHolder& other) noexcept
    {
        std::swap(other.m_raw, this->m_raw);
        std::swap(other.m_isAcquired, this->m_isAcquired);
    }

  protected:
    ObjectHolder(RawType* raw, bool acquire) noexcept
      : m_raw(raw)
      , m_isAcquired(acquire){};

    ObjectHolder(const ObjectHolder& other)
      : m_raw(Type::duplicate(other.m_raw))
      , m_isAcquired(true)
    {
    }

    ObjectHolder(ObjectHolder&& other) noexcept
      : m_raw(std::exchange(other.m_raw, nullptr))
      , m_isAcquired(std::exchange(other.m_isAcquired, false))
    {
    }

    ~ObjectHolder()
    {
        try
        {
            release();
        }
        catch (...)
        {
        }
    }

  protected:
    RawType* m_raw;
    bool m_isAcquired;
};

namespace std
{
template <class T>
inline void swap(ObjectHolder<typename T::RawType, T>& a, ObjectHolder<typename T::RawType, T>& b) noexcept
{
    a.swap(b);
}
}
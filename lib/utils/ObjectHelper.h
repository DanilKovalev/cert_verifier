#pragma once

#include <stdexcept>
#include <utility>
template<typename RawType, typename Type>
class ObjectHelper
{
  public:
    ObjectHelper& operator=(const ObjectHelper& other)
    {
        if (this == &other)
            return *this;

        ObjectHelper tmp(other);
        this->swap(tmp);
        return *this;
    }

    ObjectHelper& operator=(ObjectHelper&& other) noexcept
    {
        if (this == &other)
            return *this;

        this->swap(other);
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
        if (m_isAcquired)
            Type::destroy(m_raw);

        m_raw = nullptr;
        m_isAcquired = false;
    }

    void acquire()
    {
        if(m_isAcquired)
            return;

        m_raw = Type::duplicate(m_raw);
        m_isAcquired = true;
    }

    void swap(ObjectHelper& other) noexcept
    {
        std::swap(other.m_raw, this->m_raw);
        std::swap(other.m_isAcquired, this->m_isAcquired);
    }

  protected:
    ObjectHelper(RawType* raw, bool acquire) noexcept
      : m_raw(raw)
      , m_isAcquired(acquire){};

    ObjectHelper(const ObjectHelper& other)
      : m_raw(Type::duplicate(other.m_raw))
      , m_isAcquired(true)
    {
    }

    ObjectHelper(ObjectHelper&& other) noexcept
      : m_raw(std::exchange(other.m_raw, nullptr))
      , m_isAcquired(std::exchange(other.m_isAcquired, false))
    {
    }

    ~ObjectHelper()
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

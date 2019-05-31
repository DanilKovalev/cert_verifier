#pragma once

template <typename T>
class ObjectHelper
{
  public:

    static T makeWrapper(typename T::RawType* raw)
    {
        return T(raw, false);
    }

    static T makeAttached(typename T::RawType* raw)
    {
        return T(raw, true);
    }

    static T makeCopied(typename T::RawType* raw)
    {
        return T(T::duplicate(raw), true);
    }
};

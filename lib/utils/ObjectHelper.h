#pragma once

template <typename T>
class ObjectHelper
{
  public:

    static makeWrapper(T::RawType* raw)
    {
        return T(raw, false);
    }

    static makeAttached(T::RawType* raw)
    {
        return T(raw, false);
    }

    static MakeCopied(T::RawType* raw)
    {
        return T(T::duplicate(raw), true);
    }
};

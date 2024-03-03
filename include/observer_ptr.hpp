#ifndef OBSERVER_PTR_HPP
#define OBSERVER_PTR_HPP
#include <memory>

template <typename T>
class observer_ptr
{
public:
    // publish our template parameter and variations thereof
    using element_type = T;
    using __pointer = std::add_pointer_t<T>;            // exposition-only
    using __reference = std::add_lvalue_reference_t<T>; // exposition-only

    // 3.2.2, observer_ptr constructors
    // default c'tor
    constexpr observer_ptr() noexcept
        : __t()
    {
    }

    // pointer-accepting c'tors
    constexpr observer_ptr(std::nullptr_t) noexcept
        : __t()
    {
    }

    constexpr explicit observer_ptr(__pointer __p) noexcept
        : __t(__p)
    {
    }

    // copying c'tors (in addition to compiler-generated copy c'tor)
    template <typename _Up,
              typename = typename std::enable_if<
                  std::is_convertible<
                      typename std::add_pointer<_Up>::type, __pointer>::
                      value>::type>

    constexpr observer_ptr(observer_ptr<_Up> __p) noexcept
        : __t(__p.get())
    {
    }

    // 3.2.3, observer_ptr observers
    constexpr __pointer
    get() const noexcept
    {
        return __t;
    }

    constexpr __reference
    operator*() const
    {
        return *get();
    }

    constexpr __pointer
    operator->() const noexcept
    {
        return get();
    }

    constexpr explicit operator bool() const noexcept
    {
        return get() != nullptr;
    }

    // 3.2.4, observer_ptr conversions
    constexpr explicit operator __pointer() const noexcept
    {
        return get();
    }

    // 3.2.5, observer_ptr modifiers
    constexpr __pointer
    release() noexcept
    {
        __pointer __tmp = get();
        reset();
        return __tmp;
    }

    constexpr void
    reset(__pointer __p = nullptr) noexcept
    {
        __t = __p;
    }

    constexpr void
    swap(observer_ptr &__p) noexcept
    {
        std::swap(__t, __p.__t);
    }

private:
    __pointer __t;
}; // observer_ptr<>

template <typename T>
void swap(observer_ptr<T> &__p1, observer_ptr<T> &__p2) noexcept
{
    __p1.swap(__p2);
}

template <typename T>
observer_ptr<T>
make_observer(T *__p) noexcept
{
    return observer_ptr<T>(__p);
}

template <typename T, typename _Up>
bool operator==(observer_ptr<T> __p1, observer_ptr<_Up> __p2)
{
    return __p1.get() == __p2.get();
}

template <typename T, typename _Up>
bool operator!=(observer_ptr<T> __p1, observer_ptr<_Up> __p2)
{
    return !(__p1 == __p2);
}

template <typename T>
bool operator==(observer_ptr<T> __p, std::nullptr_t) noexcept
{
    return !__p;
}

template <typename T>
bool operator==(std::nullptr_t, observer_ptr<T> __p) noexcept
{
    return !__p;
}

template <typename T>
bool operator!=(observer_ptr<T> __p, std::nullptr_t) noexcept
{
    return bool(__p);
}

template <typename T>
bool operator!=(std::nullptr_t, observer_ptr<T> __p) noexcept
{
    return bool(__p);
}

template <typename T, typename _Up>
bool operator<(observer_ptr<T> __p1, observer_ptr<_Up> __p2)
{
    return std::less<
        typename std::common_type<
            typename std::add_pointer<T>::type,
            typename std::add_pointer<_Up>::type>::type>{}(__p1.get(),
                                                           __p2.get());
}

template <typename T, typename _Up>
bool operator>(observer_ptr<T> __p1, observer_ptr<_Up> __p2)
{
    return __p2 < __p1;
}

template <typename T, typename _Up>
bool operator<=(observer_ptr<T> __p1, observer_ptr<_Up> __p2)
{
    return !(__p2 < __p1);
}

template <typename T, typename _Up>
bool operator>=(observer_ptr<T> __p1, observer_ptr<_Up> __p2)
{
    return !(__p1 < __p2);
}

#endif
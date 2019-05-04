#ifndef NONCOPYABLE_HPP
#define NONCOPYABLE_HPP

/* Base class for adding non-copyable property to classes. */
class NonCopyable
{
public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

#endif

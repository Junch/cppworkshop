#include <gtest/gtest.h>
#include <string>
#include <memory>

#ifdef _WIN32
    #pragma warning(disable : 4100)
#endif

namespace function_signature
{
///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: Specialized template for function signature
// Web: https://stackoverflow.com/questions/13887637/specialized-template-for-function-signature
//      https://stackoverflow.com/questions/34033586/what-does-this-syntax-mean-class-template-class-r-class-args-class-name
//      https://shaharmike.com/cpp/naive-std-function/
///////////////////////////////////////////////////////////////////////////////

template <typename>
class Signal; // This declatation cannot be removed

template <typename R, typename... Args>
class Signal<R(Args...)>
{
  public:
    using F = R (*)(Args...);

    Signal<R(Args...)>(F f) : func_(f) {}

    R operator()(Args... args) { return func_(std::forward<Args>(args)...); }

  private:
    F func_;
};

void print(const std::string &name, int age)
{
    printf("name: %s, age: %d\n", name.c_str(), age);
}

TEST(function_signature, simple)
{
    Signal<void(const std::string &, int)> sig = &print;
    sig("Tom", 42);
}

///////////////////////////////////////////////////////////////////////////////
// No. 2
// Title: How is std::function implemented?
// Web: https://stackoverflow.com/questions/18453145/how-is-stdfunction-implemented
///////////////////////////////////////////////////////////////////////////////

// The mFunc is specified to function pointer. we need to generalize it.
// Type F must be relaced with a more generalized type.

template <typename>
class Signal2; // This declatation cannot be removed

template <typename R, typename... Args>
class Signal2<R(Args...)>
{
    template <typename Functor>
    static R invokeFn(Functor *fn, Args &&... args)
    {
        return (*fn)(std::forward<Args>(args)...);
    }

    template <typename Functor>
    static void destroyFn(Functor *f)
    {
        f->~Functor();
    }

    using invoke_fn_t = R (*)(char *, Args &&...);
    using destroy_fn_t = void (*)(char *);

    invoke_fn_t invoke_f_;
    destroy_fn_t destroy_f_;

  public:
    Signal2() : invoke_f_(nullptr), destroy_f_(nullptr), data_ptr_(nullptr), data_size_(0) {}

    template <typename Functor>
    Signal2<R(Args...)>(Functor f)
        // specialize functions and erase their type info by casting
        : invoke_f_(reinterpret_cast<invoke_fn_t>(invokeFn<Functor>)),
          destroy_f_(reinterpret_cast<destroy_fn_t>(destroyFn<Functor>)), data_ptr_(new char[sizeof(Functor)]),
          data_size_(sizeof(Functor))
    {
        new (data_ptr_.get()) Functor(f); // placement new
    }

    ~Signal2()
    {
        if (data_ptr_ != nullptr)
        {
            destroy_f_(data_ptr_.get());
        }
    }

    R operator()(Args... args)
    {
        if (data_ptr_ != nullptr)
        {
            return invoke_f_(data_ptr_.get(), std::forward<Args>(args)...);
        }
    }

  private:
    // erase the type of any functor and store it into a char*
    // so the storage size should be obtained as well
    std::unique_ptr<char[]> data_ptr_;
    size_t data_size_;
};

TEST(function_signature, construct_from_lambda)
{
    Signal2<void(const std::string &, int)> sig(
        [](const std::string &name, int age) { printf("name: %s, age: %d\n", name.c_str(), age); });
    sig("Daniel", 42);
}

TEST(function_signature, not_assigned)
{
    Signal2<void(const std::string &, int)> sig;
    sig("Daniel", 42);
}

} // namespace function_signature
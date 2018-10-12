#include <gtest/gtest.h>
#include <string>

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

    Signal<R(Args...)>(F f) : mFunc(f) {}

    R operator()(Args... args) { return mFunc(std::forward<Args>(args)...); }

  private:
    F mFunc;
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
    static R invoke_fn(Functor *fn, Args &&... args)
    {
        return (*fn)(std::forward<Args>(args)...);
    }

    template <typename Functor>
    static void destroy_fn(Functor *f)
    {
        f->~Functor();
    }

    using invoke_fn_t = R (*)(char *, Args &&...);
    using destroy_fn_t = void (*)(char *);

    invoke_fn_t invoke_f;
    destroy_fn_t destroy_f;

  public:
    Signal2() : invoke_f(nullptr), destroy_f(nullptr), data_ptr(nullptr), data_size(0) {}

    template <typename Functor>
    Signal2<R(Args...)>(Functor f)
        // specialize functions and erase their type info by casting
        : invoke_f(reinterpret_cast<invoke_fn_t>(invoke_fn<Functor>)),
          destroy_f(reinterpret_cast<destroy_fn_t>(destroy_fn<Functor>)), data_ptr(new char[sizeof(Functor)]),
          data_size(sizeof(Functor))
    {
        new (data_ptr.get()) Functor(f); // placement new
    }

    ~Signal2()
    {
        if (data_ptr != nullptr)
        {
            destroy_f(this->data_ptr.get());
        }
    }

    R operator()(Args... args)
    {
        if (data_ptr != nullptr)
        {
            return invoke_f(this->data_ptr.get(), std::forward<Args>(args)...);
        }
    }

  private:
    // erase the type of any functor and store it into a char*
    // so the storage size should be obtained as well
    std::unique_ptr<char[]> data_ptr;
    size_t data_size;
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
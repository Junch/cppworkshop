#include <chrono>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: GotW #91: Smart Pointer Parameters
// Web: https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/
//      https://stackoverflow.com/questions/3310737/should-we-pass-a-shared-ptr-by-reference-or-by-value
///////////////////////////////////////////////////////////////////////////////

void foo(const std::shared_ptr<int> &p)
{
    *p += 1;
}

void goo(std::shared_ptr<int> p)
{
    *p += 1;
}

TEST(shared_ptr, value_vs_reference)
{
    {
        auto start = std::chrono::high_resolution_clock::now();

        auto p = std::make_shared<int>(0);
        for (int i = 0; i < 10000000; ++i)
        {
            foo(p);
        }

        ASSERT_EQ(10000000, *p);

        auto end = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Passing by reference takes micro-seconds: " << delta.count() << '\n';
    }

    {
        auto start = std::chrono::high_resolution_clock::now();

        auto p = std::make_shared<int>(0);
        for (int i = 0; i < 10000000; ++i)
        {
            goo(p);
        }

        ASSERT_EQ(10000000, *p);

        auto end = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Passing by value takes micro-seconds: " << delta.count() << '\n';
    }
}

TEST(shared_ptr, execute_code_on_block_exit)
{
    // https://www.boost.org/doc/libs/1_57_0/libs/smart_ptr/sp_techniques.html
    // Smart Pointer Programming Techniques
    // Using shared_ptr to execute code on block exit

    std::shared_ptr<void> guard(static_cast<void *>(0), [](void *p) { printf("It will be executed on exit of this block\n"); });
    // std::shared_ptr<int> guard(NULL, [](void *p) { printf("It will be executed on exit of this block\n"); });
   
    printf("It will be executed first\n");
}

TEST(shared_ptr, scope_guard)
{
    // https://www.boost.org/doc/libs/1_57_0/libs/smart_ptr/sp_techniques.html
    // Smart Pointer Programming Techniques
    // Using shared_ptr to execute code on block exit

    // https://stackoverflow.com/questions/23270078/test-a-specific-exception-type-is-thrown-and-the-exception-has-the-right-propert
    // https://stackoverflow.com/questions/10270328/the-simplest-and-neatest-c11-scopeguard

    using namespace std::chrono_literals;

    EXPECT_THROW({
        std::thread trd([](){
            std::this_thread::sleep_for(1000ms);
            printf("run in work thread\n");
        });

        std::shared_ptr<std::thread> guard(&trd, [](std::thread* p) {p->join();});

        throw std::runtime_error("meet an execption!");
    }, std::runtime_error);
}

template <class T>
class pointer
{
  private:
    T *p_;

  public:
    explicit pointer(T *p) : p_(p) {}

    std::shared_ptr<T> operator->() const
    {
        p_->prefix();
        return std::shared_ptr<T>(p_, std::mem_fn(&T::suffix));
    }
};

class X
{
  private:
    void prefix() { printf("prefix\n"); }

    void suffix() { printf("suffix\n"); }

    friend class pointer<X>;

  public:
    void f() { printf("f\n"); }

    void g() { printf("g\n"); }
};

TEST(shared_ptr, wrap_member_function_calls)
{
    X x;
    pointer<X> px(&x);

    px->f();
    px->g();
}

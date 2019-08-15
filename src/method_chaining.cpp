#include <gtest/gtest.h>
#include <memory>

namespace method_chaining
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: C++ Object Method Chaining
// Web: https://helloacm.com/c-object-method-chaining/
///////////////////////////////////////////////////////////////////////////////

class Price
{
  public:
    Price(int v) : v_(v) {}

    Price &raise(int offset)
    {
        v_ += offset;
        return *this;
    }

    Price &drop(int offset)
    {
        v_ -= offset;
        return *this;
    }

    void print() { std::cout << v_ << '\n'; }

  private:
    int v_;
};

TEST(method_chaining, simple)
{
    Price x(10);
    x.raise(200).drop(50).print();
}

///////////////////////////////////////////////////////////////////////////////
// No. 2
// Title: C++ Object Method Chaining
// Web: https://helloacm.com/c-object-method-chaining/
///////////////////////////////////////////////////////////////////////////////

class NewPrice : public std::enable_shared_from_this<NewPrice>
{
  public:
    NewPrice(int v) : v_(v) {}

    std::shared_ptr<NewPrice> raise(int offset)
    {
        v_ += offset;
        return shared_from_this();
    }

    std::shared_ptr<NewPrice> drop(int offset)
    {
        v_ -= offset;
        return shared_from_this();
    }

    void print() { std::cout << v_ << '\n'; }

  private:
    int v_;
};

TEST(method_chaining, shared_ptr)
{
    auto x = std::make_shared<NewPrice>(10);
    x->raise(200)->drop(50)->print();
}

} // namespace method_chaining

#include <gmock/gmock.h>
#include <string>

// https://github.com/google/googletest/blob/master/googlemock/docs/cheat_sheet.md

using testing::_;
using testing::AtLeast;
using testing::DoAll;
using testing::Return;
using testing::SetArgReferee;

class Turtle
{
  public:
    virtual ~Turtle() {}
    virtual void PenUp() = 0;
    virtual void PenDown() = 0;
    virtual void Forward(int distance) = 0;
    virtual void Turn(int degrees) = 0;
    virtual void GoTo(int x, int y) = 0;
    virtual int GetX() const = 0;
    virtual int GetY() const = 0;
    virtual bool GetInfo(std::string &name, int &age) const = 0;
};

class MockTurtle : public Turtle
{
  public:
    MOCK_METHOD(void, PenUp, ());
    MOCK_METHOD(void, PenDown, ());
    MOCK_METHOD(void, Forward, (int distance));
    MOCK_METHOD(void, Turn, (int degrees));
    MOCK_METHOD(void, GoTo, (int x, int y));
    MOCK_METHOD(int, GetX, (), (const));
    MOCK_METHOD(int, GetY, (), (const));
    MOCK_METHOD(bool, GetInfo, (std::string &name, int &age), (const));
};

class Painter
{
    Turtle *turtle;

  public:
    Painter(Turtle *turtle) : turtle(turtle) {}

    bool DrawCircle(int, int, int)
    {
        turtle->PenDown();
        return true;
    }

    std::string GetTurtleName()
    {
        std::string name;
        int age;

        turtle->GetInfo(name, age);
        return name;
    }
};

TEST(PainterTest, canDrawCircle)
{
    MockTurtle turtle;
    EXPECT_CALL(turtle, PenDown()).Times(::testing::AtLeast(1));

    Painter painter(&turtle);
    EXPECT_TRUE(painter.DrawCircle(0, 0, 10));
}

TEST(PainterTest, canGetTurtleName)
{
    MockTurtle turtle;
    std::string name{"tom"};
    EXPECT_CALL(turtle, GetInfo(_, _)).WillOnce(DoAll(SetArgReferee<0>(name), Return(true)));

    Painter painter(&turtle);
    ASSERT_EQ(painter.GetTurtleName(), name);
}

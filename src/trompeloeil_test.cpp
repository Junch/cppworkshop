#include <gtest/gtest.h>
#include "trompeloeil.hpp"

struct IA
{
    virtual int a() = 0;
};

struct IB
{
    virtual int b(int) = 0;
};

struct IC : public IA, public IB
{
};

struct Mock : public IC
{
public:
	MAKE_MOCK0(a, int(), override);
	MAKE_MOCK1(b, int(int), override);
};

// FakeIt mock framework can't mock classes with multiple inheritance.
// But Trompeloeil can mock it well.

void test_trompeloeil(IC* pIC, int value)
{
	printf("a returns %d\n", pIC->a());
	printf("b returns %d\n", pIC->b(value));
}

TEST(trompeloeil, multi_inheritance)
{
	Mock m;

	REQUIRE_CALL(m, a()).RETURN(5);
	REQUIRE_CALL(m, b(ANY(int))).RETURN(10);

	test_trompeloeil(&m, 8);
}

#include "gtest/gtest.h"
#include <cpp3ds/TopScreen.h>

TEST(Test, EqualityCheck){
	cpp3ds::TopScreen ts;
	EXPECT_EQ(true, true);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

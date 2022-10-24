#include <numeric>
#include <chrono>
#include <vector>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "mvector.h"

// 1. Mvector<int> construction        ----------
// 2. Mvector<MPod> construction       ----------
// 3. Mvector<MSome> construction      ----------
// 4. Capacity                         ----------
// 5. Modifiers                        ----------
// 6. Iterator                         ----------
// 7. Chrono                           ----------


// 1. Mvector<int> construction        ----------

//empty object construction Mvector<int>
TEST(MvectorIntConstructTest, Is_mv1_EmptyInitially)
{
	Mvector<int> mv1;
	EXPECT_EQ(mv1.size(), 0);
}

//ten object construction
TEST(MvectorIntConstructTest, Is_mv2_TenInitially)
{
	Mvector<int> mv2(10);
	EXPECT_EQ(mv2.size(), 10);
}

//copy constructor check
TEST(MvectorIntConstructTest, Is_mv3_TenInitially)
{
	Mvector<int> mv2(10);
	Mvector<int> mv3(mv2);
	EXPECT_EQ(mv3.size(), 10);
}

//
TEST(MvectorIntConstructTest, Is_mv4_TenInitially)
{
	Mvector<int> mv4{0,1,2,3,4,5,6,7,8,9};
	EXPECT_EQ(mv4.size(), 10);
}

//
TEST(MvectorIntConstructTest, Is_mv5_TenInitially)
{
	Mvector<int> mv4{0,1,2,3,4,5,6,7,8,9};
	Mvector<int> mv5 = mv4;
	EXPECT_EQ(mv4.size(), 10);
}


// 2. Mvector<MPod> construction       ----------

class MPod
{
public:
    int a;
};

TEST(MvectorMPodConstructTest, Is_mv1_EmptyInitially)
{
	Mvector<MPod> mv1;
	EXPECT_EQ(mv1.size(), 0);
}

TEST(MvectorMPodConstructTest, Is_mv2_TenInitially)
{
	Mvector<MPod> mv2(10);
	EXPECT_EQ(mv2.size(), 10);
}

TEST(MvectorMPodConstructTest, Is_mv3_TenInitially)
{
	Mvector<MPod> mv2(10);
	Mvector<MPod> mv3(mv2);
	EXPECT_EQ(mv3.size(), 10);
}

TEST(MvectorMPodConstructTest, Is_mv4_TenInitially)
{
	Mvector<MPod> mv4{MPod{0},MPod{1},MPod{2},MPod{3},MPod{4},
		              MPod{5},MPod{6},MPod{7},MPod{8},MPod{9}};

	EXPECT_EQ(mv4.size(), 10);
}

TEST(MvectorMPodConstructTest, Is_mv5_TenInitially)
{
	Mvector<MPod> mv4{MPod{0},MPod{1},MPod{2},MPod{3},MPod{4},
		              MPod{5},MPod{6},MPod{7},MPod{8},MPod{9}};
	Mvector<MPod> mv5 = mv4;
	EXPECT_EQ(mv4.size(), 10);
}


// 3. Mvector<MSome> construction      ----------

class MSome: protected MPod
{
public:
    MSome() : a{7} {}
    MSome(int a_) : a{a_} {}
    virtual ~MSome() {}
    virtual int foo() {return a;}
    int a;
};

TEST(MvectorMSomeConstructTest, Is_mv1_EmptyInitially)
{
	Mvector<MSome> mv1;
	EXPECT_EQ(mv1.size(), 0);
}

TEST(MvectorMSomeConstructTest, Is_mv2_TenInitially)
{
	Mvector<MSome> mv2(10);
	EXPECT_EQ(mv2.size(), 10);
}

TEST(MvectorMSomeConstructTest, Is_mv3_TenInitially)
{
	Mvector<MSome> mv2(10);
	Mvector<MSome> mv3(mv2);
	EXPECT_EQ(mv3.size(), 10);
}

TEST(MvectorMSomeConstructTest, Is_mv4_TenInitially)
{
	Mvector<MSome> mv4{MSome{0},MSome{1},MSome{2},MSome{3},MSome{4},
		               MSome{5},MSome{6},MSome{7},MSome{8},MSome{9}};

	EXPECT_EQ(mv4.size(), 10);
}

TEST(MvectorMSomeConstructTest, Is_mv5_TenInitially)
{
	Mvector<MSome> mv4{MSome{0},MSome{1},MSome{2},MSome{3},MSome{4},
		               MSome{5},MSome{6},MSome{7},MSome{8},MSome{9}};
	Mvector<MSome> mv5 = mv4;
	EXPECT_EQ(mv4.size(), 10);
}


// 4. Capacity                         ----------

class MvectorCapacityTest : public ::testing::Test {
 protected:
	MvectorCapacityTest()
	: mv1{0,1,2,3,4,5,6,7,8,9} {}
  // void SetUp() override {}
  // void TearDown() override {}

	Mvector<int> mv1;
};

TEST_F(MvectorCapacityTest, mv1_Size)
{
	EXPECT_EQ(mv1.size(), 10);
}

TEST_F(MvectorCapacityTest, mv1_Capacity)
{
	EXPECT_EQ(mv1.capacity(), 10);
}

TEST_F(MvectorCapacityTest, Is_mv1_Empty)
{
	EXPECT_EQ(mv1.empty(), false);
}

TEST_F(MvectorCapacityTest, Is_mv1_Reserve)
{
	mv1.reserve(32);
	EXPECT_EQ(mv1.size(), 10);
	EXPECT_EQ(mv1.capacity(), 32);
}


// 5. Modifiers                        ----------

class MvectorModifiersTest : public ::testing::Test {
 protected:
	MvectorModifiersTest()
	: mv1(100) {}

	Mvector<int> mv1;
};

TEST_F(MvectorModifiersTest, mv1_Resize)
{
	mv1.resize(50);
	EXPECT_EQ(mv1.size(), 50);
	EXPECT_EQ(mv1.capacity(), 100);
}

TEST_F(MvectorModifiersTest, mv1_Clear)
{
	mv1.clear();
	EXPECT_EQ(mv1.size(), 0);
	EXPECT_EQ(mv1.capacity(), 100);
}

TEST_F(MvectorModifiersTest, mv1_Push_back)
{
	mv1.push_bask(8);
	mv1.push_bask(6);
	mv1.push_bask(7);

	EXPECT_EQ(mv1.size(), 103);
	EXPECT_GE(mv1.capacity(), 103);
}


// 6. Iterator                         ----------

class MvectorIteratorTest : public ::testing::Test {
 protected:
	MvectorIteratorTest()
	: mv1(100) {}

	Mvector<int> mv1;
};

TEST_F(MvectorIteratorTest, mv1_fill_range)
{
	std::iota(mv1.begin(), mv1.end(), 0);
	EXPECT_EQ(mv1[50], 50);
}


// 7. Chrono                           ----------

//class MvectorCronoTest : public ::testing::Test {
// protected:
//	MvectorCronoTest()
//	: mv1(1000000) {}
//
//	Mvector<int> mv1;
//};

#include <iostream>
TEST(MvectorChronoTest, mv1_compare_with_std_vector)
{
	auto start2  = std::chrono::steady_clock::now();
	std::vector<int> sv1(20000000);
	std::iota(sv1.begin(), sv1.end(), 0);
	auto stop2 = std::chrono::steady_clock::now();
	auto elapsed2 = std::chrono::duration_cast<std::chrono::milliseconds>(stop2 - start2);

	auto start1  = std::chrono::steady_clock::now();
	Mvector<int> mv1(20000000);
	std::iota(mv1.begin(), mv1.end(), 0);
	auto stop1 = std::chrono::steady_clock::now();
	auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1);

	std::cout << "\n -------- " << elapsed1.count() << " ---- "  << elapsed2.count() << " -----\n\n";

	EXPECT_LE(elapsed1, elapsed2);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>
#include "rational.cpp"

TEST(RationalTest, DefaultConstructor) {
    Rational<int> a;
    EXPECT_EQ(a.num(), 0);
    EXPECT_EQ(a.den(), 1);
}

TEST(RationalTest, ParameterizedConstructor) {
    Rational<int> b(3, 4);
    EXPECT_EQ(b.num(), 3);
    EXPECT_EQ(b.den(), 4);
}

TEST(RationalTest, Simplification) {
    Rational<int> c(6, 9);
    EXPECT_EQ(c.num(), 2);
    EXPECT_EQ(c.den(), 3);
}

TEST(RationalTest, SignNormalization) {
    Rational<int> d(3, -4);
    EXPECT_EQ(d.num(), -3);
    EXPECT_EQ(d.den(), 4);

    Rational<int> e(-3, -4);
    EXPECT_EQ(e.num(), 3);
    EXPECT_EQ(e.den(), 4);
}

TEST(RationalTest, Normalization) {
    Rational<int> d(2, 0);
    EXPECT_EQ(d.num(), 1);
    EXPECT_EQ(d.den(), 0);

    d = Rational(-2, 0);
    EXPECT_EQ(d.num(), -1);
    EXPECT_EQ(d.den(), 0);
}

TEST(RationalTest, Addition) {
    auto f = Rational<int>(1, 2) + Rational<int>(1, 3);
    EXPECT_EQ(f.num(), 5);
    EXPECT_EQ(f.den(), 6);
}

TEST(RationalTest, Subtraction) {
    auto g = Rational<int>(3, 4) - Rational<int>(1, 4);
    EXPECT_EQ(g.num(), 1);
    EXPECT_EQ(g.den(), 2);
}

TEST(RationalTest, Multiplication) {
    auto h = Rational<int>(2, 3) * Rational<int>(3, 4);
    EXPECT_EQ(h.num(), 1);
    EXPECT_EQ(h.den(), 2);
}

TEST(RationalTest, Division) {
    auto i = Rational<int>(1, 2) / Rational<int>(3, 4);
    EXPECT_EQ(i.num(), 2);
    EXPECT_EQ(i.den(), 3);
}

TEST(RationalTest, CompoundAssignment) {
    Rational<int> j(1, 2);
    j += Rational<int>(1, 3);
    EXPECT_EQ(j.num(), 5);
    EXPECT_EQ(j.den(), 6);
}

TEST(RationalTest, InfSum) {
    auto i = Rational<int>(1, 0) + Rational<int>(3, 4);
    EXPECT_EQ(i.num(), 1);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(-1, 0) + Rational<int>(3, 4);
    EXPECT_EQ(i.num(), -1);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(1, 0) + Rational<int>(+1, 0);
    EXPECT_EQ(i.num(), 1);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(-1, 0) + Rational<int>(-1, 0);
    EXPECT_EQ(i.num(), -1);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(-1, 0) + Rational<int>(1, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);
}

TEST(RationalTest, InfSub) {
    auto i = Rational<int>(1, 0) - Rational<int>(3, 4);
    EXPECT_EQ(i.num(), 1);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(-1, 0) - Rational<int>(3, 4);
    EXPECT_EQ(i.num(), -1);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(1, 2) - Rational<int>(-3, 0);
    EXPECT_EQ(i.num(), 1);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(1, 0) - Rational<int>(-1, 0);
    EXPECT_EQ(i.num(), 1);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(1, 0) - Rational<int>(1, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(-1, 0) - Rational<int>(-1, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);
}

TEST(RationalTest, InfProd) {
    auto i = Rational<int>(1, 0) * Rational<int>(3, 4);
    EXPECT_EQ(i.num(), 1);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(-1, 0) * Rational<int>(3, 4);
    EXPECT_EQ(i.num(), -1);
    EXPECT_EQ(i.den(), 0);
}

TEST(RationalTest, InfDiv) {
    auto i = Rational<int>(1, 0) / Rational<int>(3, 4);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(-1, 0) / Rational<int>(3, 4);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(1, 2) / Rational<int>(-3, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(1, 3) / Rational<int>(-3, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);
}

TEST(RationalTest, NanSum) {
    auto i = Rational<int>(0, 0) + Rational<int>(3, 4);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(5, 2) + Rational<int>(0, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(0, 0) + Rational<int>(0, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);
}

TEST(RationalTest, NanProd) {
    auto i = Rational<int>(0, 0) * Rational<int>(3, 4);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(5, 2) * Rational<int>(0, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(0, 0) * Rational<int>(0, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);
}

TEST(RationalTest, NanDiv) {
    auto i = Rational<int>(0, 0) / Rational<int>(3, 4);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(5, 2) / Rational<int>(0, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);

    i = Rational<int>(0, 0) / Rational<int>(0, 0);
    EXPECT_EQ(i.num(), 0);
    EXPECT_EQ(i.den(), 0);
}

#include <gtest/gtest.h>

#include "../Collections/POSTag.h"

TEST(POSTagTest, Create)
{
    POSTag t1;
    t1.POS = 1;
    t1.features[1] = 1;
    t1.features[2] = 2;

    POSTag t2;
    t2.POS = t1.POS;
    t2.features[1] = 2;
    t2.features[2] = 2;

    POSTag t3;
    t3.POS = 2;
    t3.features = t1.features;

    POSTag t4;
    t4.POS = t3.POS;
    t4.features = t2.features;

    POSTag t5;
    t5.POS = t3.POS;
    t5.features = t2.features;
    t5.features[10] = 10;

    ASSERT_EQ(t1 - t1, 0);
    ASSERT_EQ(t2 - t2, 0);
    ASSERT_EQ(t3 - t3, 0);
    ASSERT_EQ(t4 - t4, 0);
    ASSERT_EQ(t5 - t5, 0);

    ASSERT_EQ(t1 - t2, 1);
    ASSERT_EQ(t2 - t1, 1);

    ASSERT_EQ(t1 - t3, 100);
    ASSERT_EQ(t3 - t1, 100);

    ASSERT_EQ(t1 - t4, 101);
    ASSERT_EQ(t4 - t1, 101);

    ASSERT_EQ(t1 - t5, 102);
    ASSERT_EQ(t5 - t1, 102);

    ASSERT_EQ(t2 - t5, 101);
    ASSERT_EQ(t5 - t2, 101);

    ASSERT_EQ(t3 - t5, 2);
    ASSERT_EQ(t5 - t3, 2);
}

#include "gtest/gtest.h"
#include "database.h"
#include "file.h"

TEST(database_basic, mdb_init) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
}

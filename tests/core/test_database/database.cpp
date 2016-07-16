#include "gtest/gtest.h"
#include <mycroft/mdb.h>
#include "database.h"
#include "file.h"

TEST(db, init) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, load_invalid_db) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_LT(mdb_load_db(&mdb, "xxxx"), 0);
    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, create_db) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_create_db(&mdb, "res/buttdb1"), 0);
    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, load_db) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_load_db(&mdb, "res/buttdb1"), 0);
    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, set_file_size) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_load_db(&mdb, "res/buttdb1"), 0);
    ASSERT_EQ(mdb_set_file_size(&mdb, 9001), 0);
    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, set_file_name) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_load_db(&mdb, "res/buttdb1"), 0);
    ASSERT_EQ(mdb_set_file_name(&mdb, "woop woop"), 0);
    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, set_file_path) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_load_db(&mdb, "res/buttdb1"), 0);
    ASSERT_EQ(mdb_set_file_path(&mdb, "holla holla get dolla"), 0);
    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, set_file_hash) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_load_db(&mdb, "res/buttdb1"), 0);

    mdb_hash_t hash;
    memset(hash.bytes, 'x', sizeof(hash.bytes));

    ASSERT_EQ(mdb_set_file_hash(&mdb, &hash), 0);
    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, get_file_size) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_load_db(&mdb, "res/buttdb1"), 0);

    int size = 0;
    ASSERT_EQ(mdb_get_file_size(&mdb, &size), 0);
    ASSERT_EQ(size, 9001);

    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, get_file_name) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_load_db(&mdb, "res/buttdb1"), 0);

    membuf_t boofur;
    ASSERT_EQ(membuf_init(&boofur, MEMBUF_HEAP), 0);
    ASSERT_EQ(mdb_get_file_name(&mdb, &boofur), 0);
    ASSERT_EQ(strcmp((char*)boofur.data, "woop woop"), 0);
    ASSERT_EQ(membuf_close(&boofur), 0);

    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, get_file_path) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_load_db(&mdb, "res/buttdb1"), 0);

    membuf_t boofur;
    ASSERT_EQ(membuf_init(&boofur, MEMBUF_HEAP), 0);
    ASSERT_EQ(mdb_get_file_path(&mdb, &boofur), 0);
    ASSERT_EQ(strcmp((char*)boofur.data, "holla holla get dolla"), 0);
    ASSERT_EQ(membuf_close(&boofur), 0);    ASSERT_EQ(mdb_close(&mdb), 0);

    ASSERT_EQ(mdb_close(&mdb), 0);
}

TEST(db, get_file_hash) {
    mc_mdb_t mdb;
    ASSERT_EQ(mdb_init(&mdb), 0);
    ASSERT_EQ(mdb_load_db(&mdb, "res/buttdb1"), 0);

    mdb_hash_t expected_hash;
    memset(expected_hash.bytes, 'x', MDB_HASH_SIZE);

    mdb_hash_t hash;
    mdb_get_file_hash(&mdb, &hash);

    ASSERT_EQ(memcmp(hash.bytes, expected_hash.bytes, MDB_HASH_SIZE), 0);

    ASSERT_EQ(mdb_close(&mdb), 0);
}

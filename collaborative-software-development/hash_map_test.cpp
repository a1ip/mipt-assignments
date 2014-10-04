// Copyright (c) 2013 Timur Iskhodzhanov and MIPT students. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "sandbox/hash_map.h"

#include "gtest/gtest.h"

#include "base/common.h"

TEST(HashMapTest, PutGetSimpleTest) {
  HashMap<int, int> hm;
  hm.Put(42, 4242);

  int value = 0;
  ASSERT_TRUE(hm.Get(42, &value));
  EXPECT_EQ(4242, value);

  ASSERT_FALSE(hm.Get(43, &value));
  EXPECT_EQ(4242, value);
}

TEST(HashMapTest, PutGetTest) {
  HashMap<int, int> hm;
  for (int i = 0; i < 10; i++) {
    EXPECT_TRUE(hm.Put(i, i + 1));
  }
  ASSERT_EQ(10u, hm.Size());
  for (int i = 0; i < 10; i++) {
    int value = 0;
    ASSERT_TRUE(hm.Get(i, &value));
    EXPECT_EQ(i + 1, value);
  }
  for (int i = 10; i < 20; i++) {
    int value = 0;
    EXPECT_FALSE(hm.Get(i, &value));
  }
}

TEST(HashMapTest, SizeClearTest) {
  HashMap<int, int> hm;
  EXPECT_EQ(0u, hm.Size());

  EXPECT_TRUE(hm.Put(42, 42));
  EXPECT_EQ(1u, hm.Size());

  for (int i = 0; i < 10; i++) {
    EXPECT_TRUE(hm.Put(i, i + 1));
  }
  EXPECT_EQ(11u, hm.Size());

  hm.Clear();
  EXPECT_EQ(0u, hm.Size());
}

TEST(HashMapTest, ReplaceTest) {
  HashMap<int, int> hm;

  int value = 0;

  EXPECT_TRUE(hm.Put(0, 42));
  ASSERT_TRUE(hm.Get(0, &value));
  EXPECT_EQ(42, value);
  EXPECT_EQ(1u, hm.Size());

  EXPECT_FALSE(hm.Put(0, 4242));
  ASSERT_TRUE(hm.Get(0, &value));
  EXPECT_EQ(4242, value);
  EXPECT_EQ(1u, hm.Size());
}

TEST(HashMapTest, EraseTest) {
  HashMap<int, int> hm;

  for (int i = 0; i < 10; i++) {
    EXPECT_TRUE(hm.Put(i, i + 1));
  }
  EXPECT_EQ(10u, hm.Size());

  for (int i = 0; i < 5; i++) {
    EXPECT_TRUE(hm.Erase(i));
  }
  EXPECT_EQ(5u, hm.Size());

  for (int i = 10; i < 20; i++) {
    EXPECT_FALSE(hm.Erase(i));
  }
  EXPECT_EQ(5u, hm.Size());

  for (int i = 5; i < 10; i++) {
    int value = 0;
    ASSERT_TRUE(hm.Get(i, &value));
    EXPECT_EQ(i + 1, value);
  }
}

struct CollisionHasher {
  static size_t Hash(const int& key) {
    return static_cast<size_t>(key % 2);
  }
};

class HashMapCollisionTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    CHECK_GE(kEntriesNumber, 2);

    for (size_t i = 0; i < kEntriesNumber; i++) {
      keys_[i] = 2 * i + 1;
      hashes_[i] = CollisionHasher::Hash(keys_[i]);
    }

    for (size_t i = 0; i < kEntriesNumber - 1; i++) {
      ASSERT_NE(keys_[i], keys_[i + 1]);
      ASSERT_EQ(hashes_[i], hashes_[i + 1]);
    }

    for (size_t i = 0; i < kEntriesNumber; i++) {
      EXPECT_TRUE(hm_.Put(keys_[i], i));
    }
    EXPECT_EQ(kEntriesNumber, hm_.Size());
  }

  virtual void TearDown() { }

  static const size_t kEntriesNumber = 128;

  HashMap<size_t, size_t, CollisionHasher> hm_;
  size_t keys_[kEntriesNumber];
  size_t hashes_[kEntriesNumber];
};

const size_t HashMapCollisionTest::kEntriesNumber;

TEST_F(HashMapCollisionTest, GeneralCollisionTest) {
  for (size_t i = 0; i < kEntriesNumber; i++) {
    size_t value = 0;
    ASSERT_TRUE(hm_.Get(keys_[i], &value));
    EXPECT_EQ(i, value);
  }
}

TEST_F(HashMapCollisionTest, FirstCollisionEraseTest) {
  for (size_t i = 0; i < kEntriesNumber / 2; i++) {
    EXPECT_TRUE(hm_.Erase(keys_[2 * i]));
  }
  ASSERT_EQ(kEntriesNumber - kEntriesNumber / 2, hm_.Size());

  for (size_t i = 0; i < hm_.Size(); i++) {
    size_t value = 0;
    ASSERT_TRUE(hm_.Get(keys_[2 * i + 1], &value));
    EXPECT_EQ(2 * i + 1, value);
  }
}

TEST_F(HashMapCollisionTest, SecondCollisionEraseTest) {
  for (size_t i = 0; i < kEntriesNumber / 2; i++) {
    EXPECT_TRUE(hm_.Erase(keys_[2 * i + 1]));
  }
  // (x - x / 2) != x / 2 if x is odd.
  ASSERT_EQ(kEntriesNumber - kEntriesNumber / 2, hm_.Size());

  for (size_t i = 0; i < hm_.Size(); i++) {
    size_t value = 0;
    ASSERT_TRUE(hm_.Get(keys_[2 * i], &value));
    EXPECT_EQ(2 * i, value);
  }
}

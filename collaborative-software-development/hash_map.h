// Copyright (c) 2013 Timur Iskhodzhanov and MIPT students. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_HASH_MAP_H_
#define SANDBOX_HASH_MAP_H_

#include <stddef.h>

#include <limits>
#include <list>
#include <vector>

template<class T>
class DefaultHasher {
 public:
  static size_t Hash(const T& value);
};

template<>
class DefaultHasher<int> {
 public:
  static size_t Hash(const int& value) {
    return static_cast<size_t>(value);
  }
};

template<class KeyType, class ValueType,
         class Hasher = DefaultHasher<KeyType> >
class HashMap {
 public:
  HashMap() : num_elements_(0) {
    buckets_.resize(kInitialTableSize);
  }
  ~HashMap() { }

  size_t Size() const {
    return num_elements_;
  }

  void Clear() {
    num_elements_ = 0;
    for (BucketsIterator i = buckets_.begin(); i != buckets_.end(); ++i) {
      i->clear();
    }
  }

  // Returns 'false' if 'key' is already present in the hash map and the value
  // is replaced.
  // Returns 'true' otherwise.
  bool Put(const KeyType& key, const ValueType& value) {
    size_t hash = Hasher::Hash(key);
    size_t index = hash % buckets_.size();

    for (CollisionListIterator i = buckets_[index].begin();
         i != buckets_[index].end(); ++i) {
      if (i->key == key) {
        i->value = value;
        return false;
      }
    }

    buckets_[index].push_back(Item(key, value));
    num_elements_++;
    return true;
  }

  // Returns 'true' if 'key' was found in the hash map.
  // The value is placed in '*output'.
  // Returns 'false' otherwise and '*output' remains unchanged.
  bool Get(const KeyType& key, ValueType* output) const {
    size_t hash = Hasher::Hash(key);
    size_t index = hash % buckets_.size();

    for (CollisionListConstIterator i = buckets_[index].begin();
         i != buckets_[index].end(); ++i) {
      if (i->key == key) {
        *output = i->value;
        return true;
      }
    }

    return false;
  }

  // Returns 'true' if 'key' was found in the hash map and erased.
  // Returns 'false' otherwise.
  bool Erase(const KeyType& key) {
    size_t hash = Hasher::Hash(key);
    size_t index = hash % buckets_.size();

    for (CollisionListIterator i = buckets_[index].begin();
         i != buckets_[index].end(); ++i) {
      if (i->key == key) {
        buckets_[index].erase(i);
        num_elements_--;
        return true;
      }
    }

    return false;
  }

 private:
  struct Item {
    Item(const KeyType& key, const ValueType& value)
      : key(key), value(value) { }

    KeyType key;
    ValueType value;
  };

  // TODO(adech.fo): use vector instead of list.
  typedef std::list<Item> CollisionList;
  typedef typename CollisionList::iterator CollisionListIterator;
  typedef typename CollisionList::const_iterator CollisionListConstIterator;
  typedef typename std::vector<CollisionList>::iterator BucketsIterator;

  static const size_t kInitialTableSize = 32;

  size_t num_elements_;

  std::vector<CollisionList> buckets_;
};

#endif  // SANDBOX_HASH_MAP_H_

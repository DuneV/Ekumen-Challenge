// This koan is about creating a very simple templated class that implements a
// stack with basic push/pop operations.
//
// You **cannot** use std::stack, but you're free to use any other part of the
// C++ standard library.
//
// The expected interface that the class needs to implement can be deduced from
// the tests below. You can also check the std::stack interface this is based
// on.

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

/// BEGIN EDIT ------------------------------------------------------

//

template <typename T>

class Stack {

public:

    // Default builder to define an empty stack

    Stack() = default;
    
    // Push an element at the end of the container elements_
    // Which is a vector

    void push(const T& value) {
        elements_.push_back(value);
    }

    // Push an element directly in case of not belong to a var
    
    void push(T&& value) {
        elements_.push_back(std::move(value));
    }

    // Remove the top element from the stack
    
    void pop() {
        if (elements_.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        // Removes last element 
        elements_.pop_back();
    }

    // Access the top element of the stack

    T& top() {
        if (elements_.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return elements_.back();
    }

    // Access the top element of the stack (const)

    const T& top() const {
        if (elements_.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return elements_.back();
    }

    // Check if the stack is empty
    bool empty() const {
        return elements_.empty();
    }

private:
  // Container to store stack elements
    std::vector<T> elements_; 
};

//

/// END EDIT --------------------------------------------------------

template <typename T> class Values;

template <> struct Values<std::string> {
  static std::string first_value() { return "aaa"; }
  static std::string second_value() { return "bbb"; }
};

template <> struct Values<int> {
  static int first_value() { return 99; }
  static int second_value() { return 77; }
};

template <> struct Values<char> {
  static char first_value() { return 'A'; }
  static char second_value() { return 'Z'; }
};

typedef ::testing::Types<std::string, int, char> Implementations;

template <typename T> class WhatAreYouMadeOf : public testing::Test {}; // Nice name :)

TYPED_TEST_SUITE(WhatAreYouMadeOf, Implementations);

TYPED_TEST(WhatAreYouMadeOf, DefaultConstructible) {
  // test that the class is default constructible, and that the default
  // constructor creates an empty stack
  using T = TypeParam;
  const Stack<T> uut;
  ASSERT_TRUE(uut.empty());
}

TYPED_TEST(WhatAreYouMadeOf, SimplePushPop) {
  // test that the stack supports simple push/pop operations on the stack, that
  // that the top() method allows to inspect the element at the top of the
  // stack, and that empty() returns true when the stack is empty and false
  // otherwise
  using T = TypeParam;
  const auto v1 = Values<T>::first_value();

  Stack<T> uut;
  const auto &const_uut = uut;

  ASSERT_TRUE(uut.empty());

  uut.push(v1);

  ASSERT_FALSE(uut.empty());
  ASSERT_FALSE(const_uut.empty());
  ASSERT_EQ(v1, uut.top());
  ASSERT_EQ(v1, const_uut.top());

  uut.pop();
  ASSERT_TRUE(uut.empty());
  ASSERT_TRUE(const_uut.empty());
}

TYPED_TEST(WhatAreYouMadeOf, StackOrdering) {
  // test that the stack supports multiple push/pop operations on the stack, and
  // that the order in which elements are recovered is the inverse of the order
  // in which they were pushed
  using T = TypeParam;
  const auto v1 = Values<T>::first_value();
  const auto v2 = Values<T>::second_value();

  Stack<T> uut;
  const auto &const_uut = uut;

  ASSERT_TRUE(uut.empty());
  ASSERT_TRUE(const_uut.empty());

  // asymmetric sequence
  const auto test_data =
      std::vector<T>{v1, v2, v2, v1, v2, v2, v2, v1, v2, v2, v2, v2};

  for (auto it = test_data.cbegin(); it != test_data.cend(); ++it) {
    uut.push(*it);
    ASSERT_FALSE(uut.empty());
    ASSERT_FALSE(const_uut.empty());
    ASSERT_EQ(*it, uut.top());
    ASSERT_EQ(*it, const_uut.top());
  }

  for (auto it = test_data.crbegin(); it != test_data.crend(); ++it) {
    ASSERT_FALSE(uut.empty());
    ASSERT_FALSE(const_uut.empty());
    ASSERT_EQ(*it, uut.top());
    ASSERT_EQ(*it, const_uut.top());
    uut.pop();
  }

  ASSERT_TRUE(uut.empty());
  ASSERT_TRUE(const_uut.empty());
}

TEST(WhatAreYouMadeOfFixedType, CanYouAcceptMoveOnlyData) {
  // test that the stack supports move-only data types
  using ContentT = std::string;
  using MoveOnlyT = std::unique_ptr<ContentT>;
  const auto v1 = Values<ContentT>::first_value();
  const auto v2 = Values<ContentT>::second_value();

  Stack<MoveOnlyT> uut;
  const auto &const_uut = uut;

  ASSERT_TRUE(uut.empty());
  ASSERT_TRUE(const_uut.empty());

  // asymmetric sequence
  const auto test_data =
      std::vector<ContentT>{v1, v2, v2, v1, v2, v2, v2, v1, v2, v2, v2, v2};

  for (auto it = test_data.cbegin(); it != test_data.cend(); ++it) {
    uut.push(std::make_unique<ContentT>(*it));
    ASSERT_FALSE(uut.empty());
    ASSERT_FALSE(const_uut.empty());
    ASSERT_EQ(*it, *uut.top());
    ASSERT_EQ(*it, *const_uut.top());
  }

  for (auto it = test_data.crbegin(); it != test_data.crend(); ++it) {
    ASSERT_FALSE(uut.empty());
    ASSERT_FALSE(const_uut.empty());
    ASSERT_EQ(*it, *uut.top());
    ASSERT_EQ(*it, *const_uut.top());
    uut.pop();
  }

  ASSERT_TRUE(uut.empty());
  ASSERT_TRUE(const_uut.empty());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

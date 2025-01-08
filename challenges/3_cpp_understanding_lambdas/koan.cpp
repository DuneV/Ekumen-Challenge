// Lambda functions are a bit of syntactic sugar that allow you to create
// functor classes without the extra boilerplate. In this koan you'll write the
// actual boilerplate to understand what lambdas do under the hood.
//
// The first test is already implemented for you to serve as an example. A
// possible additional reading to understand what this koan is about is the
// following post: https://dassencio.org/95
//
// It is recommended you read about lambda functions, capture lists and operator
// overloading before attempting this koan.
//
// Edit the code within the EDIT marks to create possible implementations that
// would result from the proposed lambdas

#include <memory>
#include <string>

#include "gtest/gtest.h"

TEST(NoLambdasAllowed, Example) {
  // Example:
  // To create a class that implements:
  // auto f = []{ return 5; };

  class F {
  public:
    F() = default;
    int operator()() const { return 5; }
  };
  const auto f = F{};

  ASSERT_EQ(5, f());
}

TEST(NoLambdasAllowed, SimplestLambda) {
  const auto n = 5;
  /// BEGIN EDIT ------------------------------------------------------

  // create a class that implements:
  class F {
  public:
    explicit F(int value) : n_(value) {}
    int operator()() const { return n_; }
  
  private:
    int n_;
  };

  /// END EDIT --------------------------------------------------------

  const auto f = F{n}; // F is your class, this is doing capture by copy of n

  ASSERT_EQ(5, f());
  ASSERT_EQ(5, f());
  ASSERT_EQ(5, n);
}

TEST(NoLambdasAllowed, CountingLambda) {
  auto n = 5;
  const auto s = 2;

  /// BEGIN EDIT ------------------------------------------------------

  // create a class that implements:
  class F {
  public:
    F(int initial, int step) : n_(initial), s_(step) {}

    int operator()() {
      const auto ret = n_;
      n_ += s_;
      return ret;
    }

  private:
    int n_;
    int s_;
  };
  // auto f = [n, s]() mutable {
  //   const auto ret = n;
  //   n += s;
  //   return ret;
  // };

  /// END EDIT --------------------------------------------------------

  auto f = F{n, s}; // F is your class, this is doing capture by copy of both n
                    // and s, keeping them mutable

  ASSERT_EQ(n, f());
  ASSERT_EQ(n + s, f());
  ASSERT_EQ(n + 2 * s, f());

  ASSERT_EQ(5, n);
  ASSERT_EQ(2, s);
}

TEST(NoLambdasAllowed, ParameterizedLambda) {
  const auto m = 5.0;
  const auto b = 2.0;

  /// BEGIN EDIT ------------------------------------------------------

  // create a class that implements:
  class F {
  public:
    F(double slope, double intercept) : m_(slope), b_(intercept) {}

    double operator()(double x) const { return m_ * x + b_; }

  private:
    double m_;
    double b_;
  };
  // auto f = [m, b](double x) { return m * x + b; };

  /// END EDIT --------------------------------------------------------

  const auto f = F{
      m, b}; // F is your class, this is doing capture by value of both m and b

  ASSERT_EQ(m * 1 + b, f(1));
  ASSERT_EQ(m * 3 + b, f(3));
  ASSERT_EQ(m * 10 + b, f(10));

  ASSERT_DOUBLE_EQ(5.0, m);
  ASSERT_DOUBLE_EQ(2.0, b);
}

TEST(NoLambdasAllowed, CaptureByReferenceLambda) {
  auto m = 5.0;
  auto b = 2.0;

  /// BEGIN EDIT ------------------------------------------------------

  // create a class that implements:
  class F {
  public:
    F(double& slope, double& intercept) : m_(slope), b_(intercept) {}

    double operator()(double x) const { return m_ * x + b_; }

  private:
    double& m_;
    double& b_;
  };
  // auto f = [&m, &b](double x) { return m * x + b; };

  /// END EDIT --------------------------------------------------------

  const auto f = F{
      m,
      b}; // F is your class, this is doing capture by reference of both m and b

  ASSERT_EQ(m * 1 + b, f(1));
  ASSERT_EQ(m * 3 + b, f(3));
  ASSERT_EQ(m * 10 + b, f(10));

  ASSERT_DOUBLE_EQ(5.0, m);
  ASSERT_DOUBLE_EQ(2.0, b);

  m = 4.0;
  b = 3.0;

  ASSERT_EQ(m * 1 + b, f(1));
  ASSERT_EQ(m * 3 + b, f(3));
  ASSERT_EQ(m * 10 + b, f(10));

  ASSERT_DOUBLE_EQ(4.0, m);
  ASSERT_DOUBLE_EQ(3.0, b);
}

TEST(NoLambdasAllowed, MoveOnlyCaptures) {
  struct Driver {
    std::string name() const { return "ACME"; }
  };

  auto driver = std::make_unique<Driver>();

  /// BEGIN EDIT ------------------------------------------------------

  // create a class that implements:
  class F {
  public:
    explicit F(std::unique_ptr<Driver> d) : driver_(std::move(d)) {}

    std::string operator()() const { return driver_->name(); }

  private:
    std::unique_ptr<Driver> driver_;
  };
  // auto f = [d = std::move(driver)]() { return d->name(); };

  /// END EDIT --------------------------------------------------------

  const auto f =
      F{std::move(driver)}; // F is your class, this is doing capture by move

  ASSERT_EQ("ACME", f());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

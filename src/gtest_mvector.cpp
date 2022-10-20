#include <string>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MylibInterface {
public:
  virtual ~MylibInterface() = default;
  virtual void setVoltage(int) = 0;
};

class MylibMock : public MylibInterface {
public:
  ~MylibMock() override = default;
  MOCK_METHOD1(setVoltage, void(int));
};

class Mylib : public MylibInterface {
public:
  void setVoltage(int v) {
    // complex logic
  }
};

class Myapp {
  MylibInterface *mylib_;

public:
  explicit Myapp(MylibInterface *mylib) : mylib_(mylib){};
  
  void run(const std::string& cmd) {
    if (cmd == "ON") {
      mylib_->setVoltage(220);
    } else if (cmd == "OFF") {
      mylib_->setVoltage(0);
    }
  }
};

TEST(MylibTestSuite, mock_mylib_setVoltage) {
  MylibMock mylib_mock;
  Myapp myapp_mock(&mylib_mock);

  EXPECT_CALL(mylib_mock, setVoltage(220)).Times(3);

  myapp_mock.run("ON");
}

int main1(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

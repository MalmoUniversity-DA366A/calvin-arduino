/*
 * testJson.cpp
 *
 *      Author: Peter Johansson
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testJson : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

int checkJson(JsonObject &root)
{
  if(!root.success())
  {
      return 0;
  }
  else
  {
      return 1;
  }
}

TEST(testJson, testObjectParsing)
{
  char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[1024,\"inside\"]}";
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(json);

	// Test object parsing
	EXPECT_EQ(1, checkJson(root));

}

TEST(testJson, testObjectContent)
{
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["sensor"] = "gps";
    root["time"] = 1351824120;
    JsonArray &data = root.createNestedArray("data");
    data.add("Nested");
    data.add("Value");

    JsonArray &array = root.get("data").asArray();

    // Test content of object
    EXPECT_TRUE(root.containsKey("sensor"));
    EXPECT_TRUE(root.containsKey("time"));
    EXPECT_TRUE(root.containsKey("data"));
    EXPECT_STREQ("Nested", array.get(0));
    EXPECT_STREQ("Value", array.get(1));
}
TEST(testJson, testSize)
{
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["sensor"] = "gps";
    root["time"] = 1351824120;
    JsonArray &data = root.createNestedArray("data");
    data.add("Nested");
    data.add("Value");

    // Test size of object and size of nested array
    JsonArray &array = root.get("data").asArray();
    EXPECT_EQ(3, root.size());
    EXPECT_EQ(2, array.size());
}
#endif

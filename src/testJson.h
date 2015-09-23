#include "Arduino.h"

class testJson {
public:
  void test();
  void loadJson(String str);
  String jsonUnserialize(char *temp);
  String jsonSerialize(char *str);
private:
};

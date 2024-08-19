#pragma once
#include <time.h>

class timeObj {
public:
  timeObj() {
    this->counter++;
    this->uid = this->counter;
  };
  void setTimeStamp_now() {
    this->timestamp = clock();
  }
  void setTimeStamp(clock_t new_timestamp) {
    this->timestamp = new_timestamp;
  }
  clock_t getTimeStamp() {
    return this->timestamp;
  };
  unsigned int getUid() {
    return this->uid;
  }

protected:
  clock_t timestamp;
  unsigned int uid;
  inline static unsigned int counter = 0;
};

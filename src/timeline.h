#pragma once

class timeline {
public:
  virtual void goToPast() = 0;
  virtual void goToFuture() = 0;
  virtual void createPast(int) = 0;
  virtual void clearFuture() = 0;
};

#pragma once

class timeline {
public:
  virtual void goToPast() = 0;
  virtual void goToFuture() = 0;
  virtual void createPast() = 0;
  virtual void createFuture() = 0;
};

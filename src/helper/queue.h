#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include <stdio.h>

class LimitedQueue {
public:
  LimitedQueue(int size);
  void add(float val);
  float get(int index);
  void clear();
  void setSize(int size);
  int size();
  float last();
  float first();
  
private:
  int _maxSize;
  int _size;
  float *_data;
};

#endif

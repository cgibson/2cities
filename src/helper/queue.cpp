#include "queue.h"

LimitedQueue::LimitedQueue(int size)
{
  _data = NULL;
  setSize(size);
}

void LimitedQueue::add(float val)
{
  //printf("size: %d,  max:  %d\n", _size, _maxSize);
  int tmp = ((_size < _maxSize) ? _size : _maxSize-1);
  //printf("tmp: %d\n", tmp);
  for(int i = 0; i < tmp; i++)
  {
    _data[i+1] = _data[i];
  }
  
  _data[0] = val;
  
  if(_size < _maxSize)
    _size++;
}

float LimitedQueue::get(int index)
{
  if((index > 0) && (index < _maxSize))
    return _data[index];
  else
    return -1;
}
void LimitedQueue::clear()
{
  _size = 0;
}

void LimitedQueue::setSize(int size)
{
  _maxSize = size;
  if(_data == NULL)
  {
    _data = (float*)malloc(sizeof(float) * size);
    clear();
  }else{
    _data = (float*)realloc(_data, sizeof(float) * size);
  }
}

float LimitedQueue::last()
{
  return _data[_size - 1];
}

float LimitedQueue::first()
{
  return _data[0];
}

int LimitedQueue::size()
{
  return _size;
}

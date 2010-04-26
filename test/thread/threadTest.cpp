#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "worker.h"
#include "tclass.h"

void workerFunc(const char* msg, unsigned delaySec)
{
  boost::posix_time::seconds workTime(delaySec);

  std::cout << "Worker: running" << std::endl;

  std::cout << msg << std::endl;

  boost::this_thread::sleep(workTime);

  std::cout << "Worker: finished" << std::endl;
}

void test1()
{

  std::cout << "main: startup" << std::endl;

  boost::thread workerThread(workerFunc, "Hello, boost!", 3);

  std::cout << "main: waiting for thread" << std::endl;

  workerThread.join();

  std::cout << "main: done" << std::endl;
}

void test2()
{
  Worker w(612, 10, 5);
  boost::thread workerThread(w);
  
  std::cout << "main: waiting for thread" << std::endl;
  
  workerThread.join();
  
  std::cout << "main: done" << std::endl;
}

void test3()
{
  threaded_class c1, c2;
  
  std::cout << "main: waiting for thread" << std::endl;
  
  c1.go();
  c2.go();
  
  sleep(3);
  
  int f1 = c1.get_fibonacci_value(3);
  int f2 = c1.get_fibonacci_value(5);
  
  c1.stop();
  c2.stop();
  
  std::cout << "main: done" << std::endl;
  
  std::cout << "got " << f1 << std::endl;
  std::cout << "got " << f2 << std::endl;
}

int main()
{

  test1();
  std::cout << "=====================" << std::endl << std::endl;
  test2();
  std::cout << "=====================" << std::endl << std::endl;
  test3();
  return 0;
}

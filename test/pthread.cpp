#include <iostream>
#include "src/local_store.h"
#include <pthread.h>

using namespace std;

void *write(void *ptr) {
  cout << "To write" << endl;
  char* c = (char*)ptr;
  cout << "convert c done" << endl;
  string key2 = "/tmpfs/chunks/file.txt/";
  key2 += *c;
  cout << "key = " << key2 << endl;
  char buf2[] = "little";
  buf2[0] = *c;
  size_t count2 = 6;
  long offset2 = 0;
  LocalStore::Write(key2, buf2, count2, offset2);
  int len2 = LocalStore::Read(key2, buf2);
  cout << buf2 << endl;
  return nullptr;
}

int main() {
  pthread_t t1, t2;
  char m1 = '1';
  char m2 = '2';
  pthread_create(&t1, NULL, write, (void*)&m1);
  pthread_create(&t2, NULL, write, (void*)&m2);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  cout << "end" << endl;
  return 0;
}

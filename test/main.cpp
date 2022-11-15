#include <iostream>
#include "src/local_store.h"

using namespace std;

void write() {
  string key2 = "/tmpfs/chunks/file.txt/2";
  char buf2[] = "little";
  size_t count2 = 6;
  long offset2 = 0;
  // LocalStore::Write(key2, buf2, count2, offset2);
  int len2 = LocalStore::Read(key2, buf2, count2, 0);
  cout << buf2 << endl;
}



int main() {
  write();
  return 0;
}

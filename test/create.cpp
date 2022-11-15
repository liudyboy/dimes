#include <iostream>
#include "src/local_store.h"

using namespace std;



int main() {

  string key = "/tmpfs/chunks/file.txt/0";
  char buf[] = "abcdefghigklmnopqrstuvwxyz";
  size_t count = 20;
  long offset = 0;
  LocalStore::Write(key, buf, count, offset);
  int len = LocalStore::Read(key, buf);
  cout << "buf = " << buf << endl;

  string key1 = "/tmpfs/chunks/file.txt/1";
  char buf1[] = "one";
  size_t count1 = 3;
  long offset1 = 0;
  LocalStore::Write(key1, buf1, count1, offset1);
  int len1 = LocalStore::Read(key1, buf1);
  cout << buf1 << endl;

  string key2 = "/tmpfs/chunks/file.txt/2";
  char buf2[] = "little";
  size_t count2 = 6;
  long offset2 = 0;
  LocalStore::Write(key2, buf2, count2, offset2);
  int len2 = LocalStore::Read(key2, buf2);
  cout << buf2 << endl;

  return 0;
}

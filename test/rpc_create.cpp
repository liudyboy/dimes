#include <iostream>
#include "src/plasma.h"
#include "src/plasma_client.h"
#include "src/dimes_client.h"
#include <thallium.hpp>
#include <thallium/serialization/stl/string.hpp>
#include "src/share_string.h"

using namespace std;
namespace tl = thallium;

void Create() {
  string key = "/tmpfs/chunks/file.txt/0";
  char buf[] = "abcdefghigklmnopqrstuvwxyz";
  size_t count = 20;
  long offset = 0;
  ObjectId id(key);
  auto conn = plasma_connect("/tmp/plasma", NULL, 0);
  uint8_t* data;
  plasma_create(conn, id, 100, NULL, 0, &data);
  memcpy(data, buf, count);
  plasma_seal(conn, id);
  plasma_release(conn, id);
}

void Get() {
  string key = "/tmpfs/chunks/file.txt/0";
  char buf[40];
  memset(buf, '\0', 40);
  size_t count = 20;
  long offset = 0;
  ObjectId id(key);
  uint8_t* data;
  auto conn = plasma_connect("/tmp/plasma", NULL, 0);
  int64_t size;
  plasma_get(conn, id, &size, &data, NULL, NULL);
  memcpy(buf, data, count);
  cout << buf << endl;
}

void RpcGet() {
  string key = "/tmpfs/chunks/file.txt/0";
  size_t size;
  dimes::ShareString data;
  auto ret = dimes::Get(key, size, data);
  if (ret == dimes::OK)
    cout << "Get object: " << data.data() << endl;
}
int main() {
  Create();
  //Get();
  RpcGet();
  return 0;
}

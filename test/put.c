#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "src/plasma.h"
#include "src/plasma_client.h"

using namespace std;

void create_big_buffer() {
  string key = "123452045451245873345763422";
  plasma_connection *conn = NULL;
  int64_t size;
  uint8_t *data;
  ObjectId id(key);

  conn = plasma_connect("/tmp/plasma", NULL, 0);
  plasma_create(conn, id, 10000, NULL, 0, &data);
  plasma_seal(conn, id);
  plasma_release(conn, id);
  plasma_delete(conn, id);
  plasma_disconnect(conn);
}

void use_small_buffer(plasma_connection *conn, string c) {
  string key = "1234520454512458733457634223";
  key += c;
  /* plasma_connection *conn = NULL; */
  int64_t size;
  uint8_t *data;
  ObjectId id(key);

  /* conn = plasma_connect("/tmp/plasma", NULL, 0); */
  plasma_create(conn, id, 100, NULL, 0, &data);
  plasma_seal(conn, id);
  plasma_release(conn, id);
  plasma_delete(conn, id);
  /* plasma_disconnect(conn); */
}

int main(int argc, char *argv[]) {
  /* create_big_buffer(); */

  for (int i = 0; i < 200; i++) {
    auto conn = plasma_connect("/tmp/plasma", NULL, 0);
    use_small_buffer(conn, std::to_string(i));
    plasma_disconnect(conn);
  }

  /* string key = "123452045451245873345763422"; */
  /* plasma_connection *conn = NULL; */
  /* int64_t size; */
  /* uint8_t *data; */
  /* ObjectId id(key); */

  /* conn = plasma_connect("/tmp/plasma", NULL, 0); */
  /* plasma_create(conn, id, 10000, NULL, 0, &data); */
  /* for (int i = 0; i < 100; i++) { */
  /*   data[i] = '2'; */
  /* } */

  /* plasma_seal(conn, id); */
  /* plasma_delete(conn, id); */
  /* int has_object; */
  /* plasma_contains(conn, id, &has_object); */
  /* uint8_t *get_data; */
  /* plasma_get(conn, id, &size, &get_data, NULL, NULL); */
  /* printf("receive data address: %p\n", get_data); */
  /* for (int i = 0; i < 100; i++) { */
  /*   printf("%c", get_data[i]); */
  /*   get_data[i] = '3'; */
  /* } */
  /* assert(conn != NULL); */
  /* plasma_release(conn, id); */

  /* uint8_t* get_data2; */
  /* plasma_get(conn, id, &size, &get_data2, NULL, NULL); */
  /* printf("receive data address: %p\n", get_data2); */
  /* for (int i = 0; i < 100; i++) { */
  /*   printf("%c", get_data2[i]); */
  /* } */
  /* assert(conn != NULL); */
  /* plasma_release(conn, id); */
  /* plasma_disconnect(conn); */
  return 0;
}

#include "dimes_client.h"
#include "constants.h"
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "common.h"
namespace dimes {

Status Put(const std::string& key, const std::string& data) {
  int has_object;
  ObjectId id(key);
  static auto conn = plasma_connect(dimes::kSocketPath.c_str(), NULL, 0);
  plasma_contains(conn, id, &has_object);
  if (has_object) {
    LOG_ERR("Put failed, the KV store dimes already has a object with key %s",
            key.c_str());
    return FAILED;
  }
  uint8_t* tmp_data;
  int size = data.size();
  plasma_create(conn, id, size, NULL, 0, &tmp_data);
  memcpy(tmp_data, data.c_str(), size);
  plasma_seal(conn, id);
  plasma_release(conn, id);
  return OK;
}


Status Get(const std::string& key, size_t& size,
           ShareString& data, std::string address) {
  static std::string host_ip = GetOwnIp();
  ObjectId id(key);
  if (address == host_ip || address == "127.0.0.1") {
    uint8_t* tmp_data;
    static auto conn = plasma_connect(dimes::kSocketPath.c_str(), NULL, 0);
    int has_object;
    plasma_contains(conn, id, &has_object);
    if (has_object) {
      int64_t size;
      plasma_get(conn, id, &size, &tmp_data, NULL, NULL);
      data.Reset(tmp_data, size);
      return OK;
    } else {
      LOG_ERR("Get failed, the KV store dimes has not a object with key %s",
              key.c_str());
      return FAILED;
    }
  } else {
    static tl::engine my_engine(dimes::kRpcProtocol, THALLIUM_CLIENT_MODE);
    static tl::remote_procedure GetObject = my_engine.define("GetObject");
    static tl::endpoint server =
      my_engine.lookup((dimes::kRpcProtocol + "://" + address + ":" + dimes::kRpcPort).c_str());
    data = GetObject.on(server)(key);
    size = data.size();
    return OK;
  }
}

Status Del(const std::string& key) {
  int has_object;
  ObjectId id(key);
  static auto conn = plasma_connect(kSocketPath.c_str(), NULL, 0);
  plasma_contains(conn, id, &has_object);
  Status status = FAILED;
  if (has_object) {
    plasma_delete(conn, id);
    status = OK;
  }
  plasma_disconnect(conn);
  return status;
}
}  // namespace dimes

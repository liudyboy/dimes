#include "common.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

char c = (char) 255;
std::string s = {c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c};

const unique_id NIL_ID(s);
/* const unique_id NIL_ID = {{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, */
/*                            255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}; */

unique_id globally_unique_id(void) {
  /* Use /dev/urandom for "real" randomness. */
  int fd;
  if ((fd = open("/dev/urandom", O_RDONLY)) == -1) {
    LOG_ERR("Could not generate random number");
  }
  unique_id result;
  read(fd, &result.id[0], UNIQUE_ID_SIZE);
  close(fd);
  return result;
}

char *sha1_to_hex(const unsigned char *sha1, char *buffer) {
  static const char hex[] = "0123456789abcdef";
  char *buf = buffer;

  for (int i = 0; i < UNIQUE_ID_SIZE; i++) {
    unsigned int val = *sha1++;
    *buf++ = hex[val >> 4];
    *buf++ = hex[val & 0xf];
  }
  *buf = '\0';

  return buffer;
}


std::string GetOwnIp() {
  struct ifaddrs* ifAddrStruct = NULL;
  struct ifaddrs* ifa = NULL;
  void* tmpAddrPtr = NULL;

  getifaddrs(&ifAddrStruct);

  char addressBuffer[INET_ADDRSTRLEN];
  std::string ip;
  for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
    if (!ifa->ifa_addr) {
      continue;
    }

    // check it is a valid IP4 address
    if (ifa->ifa_addr->sa_family == AF_INET) {
      tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
      inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      if (!(addressBuffer[0] == '1' && addressBuffer[1] == '2' &&
            addressBuffer[2] == '7')) {
        ip = (std::string)addressBuffer;
        break;
      }
    }
  }
  if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
  if (ip.empty()) {
    LOG_ERR("While getting own ip address error occurred!");
  }
  return ip;
}

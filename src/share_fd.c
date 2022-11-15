/**
 * @file share_fd.cpp
 * @author liudy
 * @email deyin.liu@nscc-gz.cn
 * @version 1.0.0
 * @date 2022-11-08
 */
#include "share_fd.h"
#include "common/constants.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
// #include <glog/logging.h>

namespace dimes {

void init_msg(struct msghdr *msg, struct iovec *iov, char *buf, size_t buf_len) {
  iov->iov_base = buf;
  iov->iov_len = 1;

  msg->msg_flags = 0;
  msg->msg_iov = iov;
  msg->msg_iovlen = 1;
  msg->msg_control = buf;
  msg->msg_controllen = static_cast<socklen_t>(buf_len);
  msg->msg_name = NULL;
  msg->msg_namelen = 0;
}

int send_fd(int conn, int fd) {
  struct msghdr msg;
  struct iovec iov;
  char buf[CMSG_SPACE(sizeof(int))];

  init_msg(&msg, &iov, buf, sizeof(buf));

  struct cmsghdr *header = CMSG_FIRSTHDR(&msg);
  header->cmsg_level = SOL_SOCKET;
  header->cmsg_type = SCM_RIGHTS;
  header->cmsg_len = CMSG_LEN(sizeof(int));
  *(int *)CMSG_DATA(header) = fd;

  return sendmsg(conn, &msg, 0);
}

int recv_fd(int conn) {
  struct msghdr msg;
  struct iovec iov;
  char buf[CMSG_SPACE(sizeof(int))];
  init_msg(&msg, &iov, buf, sizeof(buf));

  if (recvmsg(conn, &msg, 0) == -1)
    return -1;

  int found_fd = -1;
  bool oh_noes = false;
  for (struct cmsghdr *header = CMSG_FIRSTHDR(&msg); header != NULL; header = CMSG_NXTHDR(&msg, header))
    if (header->cmsg_level == SOL_SOCKET && header->cmsg_type == SCM_RIGHTS) {
      int count = (header->cmsg_len - (CMSG_DATA(header) - (unsigned char *)header)) / sizeof(int);
      for (int i = 0; i < count; ++i) {
        int fd = ((int *)CMSG_DATA(header))[i];
        if (found_fd == -1) {
          found_fd = fd;
        } else {
          close(fd);
          oh_noes = true;
        }
      }
    }

  // The sender sent us more than one file descriptor. We've closed
  // them all to prevent fd leaks but notify the caller that we got
  // a bad message.
  if (oh_noes) {
    close(found_fd);
    errno = EBADMSG;
    return -1;
  }

  if (found_fd == -1)
    errno = ENOENT;

  return found_fd;
}

SharedFdServer* SharedFdServer::instance_ = nullptr;

/**
 * @brief Create a pool of files for transmitting fd.
 *
 */
SharedFdServer::SharedFdServer() {
  num_connections_ = kNumSharedUsedFiles;
  for (size_t i = 0; i < num_connections_; i++) {
    int pid = getpid();
    std::string file_name = static_cast<std::string>("/tmp/") + std::to_string(pid) + "-" + std::to_string(i);
    // std::cout << "Create file name = " << file_name << std::endl;
    int fd = socket(PF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, file_name.c_str(), sizeof(addr.sun_path));
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';
    unlink(addr.sun_path);
    int status = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (status == -1) {
      std::cout << "Bind error" << std::endl;
    }
    connections_[file_name] = fd;
    free_socket_files_.push(file_name);
    std::cout << "fd = " << fd << std::endl;
  }
}

/**
 * @brief Remove files in the pool.
 *
 */
SharedFdServer::~SharedFdServer() {
  for (size_t i = 0; i < num_connections_; i++) {
    int pid = getpid();
    std::string file_name = static_cast<std::string>("/tmp/") + std::to_string(pid) + "-" + std::to_string(i);
    close(connections_[file_name]);
    int status = remove(file_name.c_str());
    if (status != 0) {
      // LOG(WARNING) << "Delete file " << file_name << " failed!".
      std::cout << "Delete file " << file_name << " failed!" << std::endl;
    }
  }
}

std::string SharedFdServer::GetSocketFilePath() {
  std::string file_name;
  {
    std::unique_lock<std::mutex> lck(lock_);
    while (free_socket_files_.empty()) {
      cv_.wait(lck);
    }
    file_name = free_socket_files_.front();
    free_socket_files_.pop();
  }
  return file_name;
}

SharedFdServer* SharedFdServer::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new SharedFdServer();
  }
  return instance_;
}

int SharedFdServer::Recv(std::string local_file_path) {
  std::cout << "connection fd = " << connections_[local_file_path] << std::endl;
  return recv_fd(connections_[local_file_path]);
}




SharedFdClient* SharedFdClient::instance_ = nullptr;

SharedFdClient::SharedFdClient() {}

SharedFdClient::~SharedFdClient() {}

SharedFdClient* SharedFdClient::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new SharedFdClient();
  }
  return instance_;
}

int SharedFdClient::Send(int fd, std::string socket_file_path) {
  auto it = connections_.find(socket_file_path);
  int local_fd;
  if (it == connections_.end()) {
    local_fd = socket(PF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, socket_file_path.c_str(), sizeof(addr.sun_path));
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';
    if (connect(local_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
      return -1;
    connections_[socket_file_path] = local_fd;
  } else {
    local_fd = connections_[socket_file_path];
  }
  return send_fd(local_fd, fd);
}

}  // namespace dimes

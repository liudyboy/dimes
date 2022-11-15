/**
 * @file share_fd.h
 * @author liudy
 * @email deyin.liu@nscc-gz.cn
 * @version 1.0.0
 * @date 2022-11-07
 */
#pragma once
#include <condition_variable>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <string>


namespace dimes {

/**
 * @brief Send a file descriptor over a unix domain socket.
 *
 * @param[in] conn Unix domain socket to send the file descriptor over.
 * @param[in] fd File descriptor to send over.
 * @return Status code which is < 0 on failure.
 */
int send_fd(int conn, int fd);

/**
 * @brief Receive a file descriptor over a unix domain socket.
 *
 * @param[in] conn Unix domain socket to receive the file descriptor from.
 * @return File descriptor or a value < 0 on failure.
 */
int recv_fd(int conn);

/**
 * @brief Sharing fd between processes on the same host.
 * According to the flingfd library(https://github.com/sharvil/flingfd),
 transmits fd between process need rely on a socket connection. We create a pool
 of socket connections  to transmit fd between processes in concern of
 efficiency.

 */
class SharedFdServer {
 public:
  SharedFdServer();
  ~SharedFdServer();

  /**
   * @brief Get the path of a socket file used to transmit fd.
   */
  std::string GetSocketFilePath();

  static SharedFdServer* GetInstance();

  /**
   * @brief Receive a file descriptor over a unix domain socket.
   *
   * @param[in] socket_file_path The path of a socket file to send file descriptor.
   * @return File descriptor or a value < 0 on failure.
   */
  int Recv(std::string socket_file_path);
 private:
  std::mutex lock_;
  std::condition_variable cv_;
  std::map<std::string, int> connections_;  // {socket_file_path, socket_fd}
  std::queue<std::string> free_socket_files_;
  size_t num_connections_;  // The total files used for sharing fd between processes.
  static SharedFdServer* instance_;
};

class SharedFdClient {
 public:
  SharedFdClient();
  ~SharedFdClient();

  static SharedFdClient* GetInstance();

  /**
   * @brief Send a file descriptor over a unix domain socket.
   *
   * @param[in] fd File descriptor to send over.
   * @param[in] socket_file_path The path of a socket file to send file descriptor.
   * @return Status code which is < 0 on failure.
   */
  int Send(int fd, std::string socket_file_path);

 private:
  std::map<std::string, int> connections_;    // {socket_file_path, socket_fd}
  size_t num_connections_;  // The total files used for sharing fd between
                            // processes.
  static SharedFdClient* instance_;
};
}  // namespace dimes

#pragma once
#include <string>
namespace dimes {
const size_t kNumSharedUsedFiles = 10;
const int kRpcWorkerPoolSize = 10;
const std::string kRpcProtocol = "ofi+sockets";
cosnt std::string kRpcPort = "2111";
const std::string kSocketPath = "/tmp/dimes";
}  // namespace dimes

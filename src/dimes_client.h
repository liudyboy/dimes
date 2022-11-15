#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <thallium.hpp>
#include <thallium/serialization/stl/string.hpp>

#include "plasma.h"
#include "plasma_client.h"
#include "share_string.h"

namespace tl = thallium;

namespace dimes {

enum Status { OK, FAILED };

Status Put(const std::string& key, const std::string& data);

/**
 * @brief Get data from KV store dimes.
 *
 * @param[in] key The key of data object.
 * @param[out] size The size of data object in Byte.
 * @param[out] data The data object we got.
 * @param[in] address The address of the node where the data object stored.
 * @return OK means success.
 */
Status Get(const std::string& key, size_t& size, ShareString& data,
           std::string address = "127.0.0.1");

Status Del(const std::string& key);

}  // namespace dimes

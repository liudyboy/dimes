#include "share_string.h"

namespace dimes {

ShareString::ShareString() {
  size_ = 0;
  data_ = nullptr;
  own_data_ = true;
}

ShareString::ShareString(uint8_t* data, size_t size) {
  size_ = size;
  data_ = data;
  own_data_ = false;
}

ShareString::ShareString(const ShareString& s) {
  size_ = s.size_;
  data_ = s.data_;
  own_data_ = s.own_data_;
}

ShareString::~ShareString() {}

 void ShareString::Reset(uint8_t* data, size_t size) {
   if (own_data_ && data_ != nullptr)
     free(data_);
   data_ = data;
   size_ = size;
   own_data_ = false;
 }

void ShareString::Resize(size_t size) {
  if (size_ == size) {
    return;
  }
  size_ = size;
  if (own_data_)
    free(data_);
  data_ = (uint8_t*)malloc(size * sizeof(uint8_t));
  own_data_ = true;
}

uint8_t* ShareString::data() const {
  return data_;
}

size_t ShareString::size() const {
  return size_;
}

bool ShareString::IsOwnData() const {
  return own_data_;
}

void ShareString::Clear() {
  if (own_data_ && data_ != nullptr) {
    free(data_);
    data_ = nullptr;
  }
  size_ = 0;
}

ShareString& ShareString::operator=(const ShareString& s) {
  size_ = s.size_;
  data_ = s.data_;
  own_data_ = s.own_data_;
  return *this;
}


}  // namespace dimes

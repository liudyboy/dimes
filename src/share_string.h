#pragma once
#include <iostream>
#include <memory>

namespace dimes {

class ShareString {
public:
  ShareString();
  ShareString(uint8_t* data, size_t size);
  ShareString(const ShareString& s);
  ~ShareString();
  void Resize(size_t size);
  void Reset(uint8_t* data, size_t size);
  uint8_t* data() const;
  bool IsOwnData() const;
  size_t size() const;
  void Clear();
  ShareString& operator=(const ShareString& s);

 private:
  size_t size_;
  uint8_t* data_;
  bool own_data_;
};

template<class A>
inline void save(A& ar, const dimes::ShareString& s) {
    size_t size = s.size();
    ar & size;
    ar.write((const uint8_t*)(s.data()), size);
}

template<class A>
inline void load(A& ar, dimes::ShareString& s) {
    size_t size;
    ar & size;
    s.Resize(size);
    ar.read(s.data(), size);
}

}  // namespace dimes

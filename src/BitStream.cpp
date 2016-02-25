//
//  BitStream.cpp
//
//  Created by otita on 2016/02/24.
//
/*
The MIT License (MIT)

Copyright (c) 2016 otita.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <memory>
#include <algorithm>

#include "BitStream.h"

using namespace std;

namespace otita {

namespace io {

BitReader::BitReader(const char filename[]) {
  _bits = nullptr;
  _len = 0;
  _pos = 0;
  _fp = fopen(filename, "rb");
  _buf = 0;
  _n = 64;
  if (!_fp) {
    _fp = nullptr;
  }
}

BitReader::BitReader(uint64_t *bits, uint64_t len) {
  _bits = bits;
  _len = len;
  _pos = 0;
  _fp = nullptr;
  _buf = 0;
  _n = 64;
}

BitReader::~BitReader() {
  close();
}

bool BitReader::read(uint64_t *bits_ptr, uint64_t len) {
  if (len > 64 || len == 0) {
    return false;
  }
  if (!(_bits || _fp)) {
    return false;
  }
  *bits_ptr = 0;
  if (_bits) {
    for (uint64_t i = 0; i <  len; i++) {
      if (_pos >= _len) {
        return false;
      }
      uint64_t idx = _pos / 64;
      uint64_t shift = _pos % 64;
      if (_bits[idx] & (uint64_t(1) << shift)) {
        *bits_ptr |= uint64_t(1) << i;
      }
      _pos++;
    }
  }
  else if (_fp) {
    if (_n == 64) {
      fread(&_buf, 8, 1, _fp);
      _n = 0;
    }
    if (_n + len <= 64) {
      *bits_ptr = (_buf >> _n) & ((~uint64_t(0)) >> (64 - len));
      _n += len;
    }
    else {
      uint64_t l = 64 - _n;
      if (!read(bits_ptr, l)) {
        return false;
      }
      uint64_t b = 0;
      if (!read(&b, len - l)) {
        return false;
      }
      *bits_ptr |= b << l;
    }
  }
  return true;
}

bool BitReader::read(uint64_t **bits_ptr, uint64_t len) {
  uint64_t size = (len + 64 - 1) / 64;
  *bits_ptr = new uint64_t[size];
  for (uint64_t i = 0; i < size; i++) {
    uint64_t bits = 0;
    if (!read(&bits, min(uint64_t(64), len))) {
      return false;
    }
    (*bits_ptr)[i] = bits;
    len -= 64;
  }
  return true;
}

void BitReader::close() {
  if (_fp) {
    fclose(_fp);
    _fp = nullptr;
  }
}

BitWriter::BitWriter(const char filename[]) {
  _bits = nullptr;
  _len = 0;
  _pos = 0;
  _fp = fopen(filename, "wb");
  _buf = 0;
  _n = 0;
}

BitWriter::BitWriter(uint64_t *bits, uint64_t len) {
  _bits = bits;
  _len = len;
  _pos = 0;
  _fp = nullptr;
  _buf = 0;
  _n = 0;
}

BitWriter::~BitWriter() {
  close();
}

bool BitWriter::write(uint64_t bits, uint64_t len) {
  if (len > 64 || len == 0) {
    return false;
  }
  if (!(_bits || _fp)) {
    return false;
  }
  if (_bits) {
    for (uint64_t i = 0; i < len; i++) {
      if (_pos >= _len) {
        return false;
      }
      uint64_t idx = _pos / 64;
      uint64_t shift = _pos % 64;
      if (bits & (uint64_t(1) << i)) {
        _bits[idx] |= (uint64_t(1) << shift);
      }
      else {
        _bits[idx] &= ~(uint64_t(1) << shift);
      }
      _pos++;
    }
  }
  else if (_fp) {
    if (_n == 64) {
      fwrite(&_buf, 8, 1, _fp);
      _buf = 0;
      _n = 0;
    }
    if (_n + len <= 64) {
      _buf |= (bits & ((~uint64_t(0) >> (64 - len)))) << _n;
      _n += len;
    }
    else {
      uint64_t l = 64 - _n;
      if (!write(bits, l)) {
        return false;
      }
      bits >>= l;
      if (!write(bits, len - l)) {
        return false;
      }
    }
  }
  return true;
}

bool BitWriter::write(uint64_t *bits, uint64_t len) {
  uint64_t size = (len + 64 - 1) / 64;
  for (uint64_t i = 0; i < size; i++) {
    if (!write(bits[i], min(uint64_t(64), len))) {
      return false;
    }
    len -= 64;
  }
  return true;
}

void BitWriter::close() {
  if (_fp) {
    if (_n) {
      fwrite(&_buf, 8, 1, _fp);
    }
    fclose(_fp);
    _fp = nullptr;
  }
}
  
} // io

} // otita
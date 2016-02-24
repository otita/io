//
//  BitStream.h
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

#ifndef _BIT_STREAM_H_
#define _BIT_STREAM_H_

#include <cstdio>
#include <cstdint>

namespace otita {

namespace io {

class BitReader {
public:
  BitReader(const char filename[]);
  BitReader(uint64_t *bits, uint64_t len);
  virtual ~BitReader();

  // 1 <= len <= 64
  bool read(uint64_t *bits_ptr, uint64_t len);
  // 1 <= len
  bool read(uint64_t **bits_ptr, uint64_t len);
  void close();
private:
  uint64_t *_bits;
  uint64_t _len;
  uint64_t _pos;

  FILE *_fp;
  uint64_t _buf;
  uint8_t _n;
};

class BitWriter {
public:
  BitWriter(const char filename[]);
  BitWriter(uint64_t *bits, uint64_t len);
  virtual ~BitWriter();

  // 1 <= len <= 64
  bool write(uint64_t bits, uint64_t len);
  // 1 <= len
  bool write(uint64_t *bits, uint64_t len);
  void close();
private:
  uint64_t *_bits;
  uint64_t _len;
  uint64_t _pos;

  FILE *_fp;
  uint64_t _buf;
  uint8_t _n;
};

} // io

} // otita

#endif  // _BIT_STREAM_H_

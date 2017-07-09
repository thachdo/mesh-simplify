//============================================================================
//
// DGP: Digital Geometry Processing toolkit
// Copyright (C) 2016, Siddhartha Chaudhuri
//
// This software is covered by a BSD license. Portions derived from other
// works are covered by their respective licenses. For full licensing
// information see the LICENSE.txt file.
//
//============================================================================

/*
 ORIGINAL HEADER

 @file BinaryOutput.cpp

 @author Morgan McGuire, http://graphics.cs.williams.edu
 Copyright 2002-2010, Morgan McGuire, All rights reserved.

 @created 2002-02-20
 @edited  2010-03-17
*/

#include "BinaryOutputStream.hpp"
#include "FilePath.hpp"
#include "FileSystem.hpp"
#include <stdio.h>

// Largest memory buffer that the system will use for writing to disk.  After this (or if the system runs out of memory) chunks
// of the file will be dumped to disk. Currently 400 MB.
#define DGP_MAX_WRITE_BUFFER_SIZE 400000000

namespace DGP {

void
BinaryOutputStream::writeBool8(int64 n, std::vector<bool> const & out)
{
  // std::vector optimizes bool in a way that prevents fast writing
  for (int64 i = 0; i < n; ++i)
    writeBool8(out[(size_t)i]);
}

#define DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(fname, tname) \
  void BinaryOutputStream::write##fname(int64 n, std::vector<tname> const & out) \
  { \
    write##fname(n, &out[0]); \
  }

DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(UInt8,               uint8)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Int8,                int8)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(UInt16,              uint16)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Int16,               int16)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(UInt32,              uint32)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Int32,               int32)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(UInt64,              uint64)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Int64,               int64)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Float32,             float32)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Float64,             float64)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Vector2,             Vector2)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Vector3,             Vector3)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Vector4,             Vector4)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorL8,             ColorL8)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorL,              ColorL)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorRGB8,           ColorRGB8)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorRGB,            ColorRGB)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorRGBA8,          ColorRGBA8)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorRGBA,           ColorRGBA)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Matrix2,             Matrix2)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Matrix3,             Matrix3)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Matrix4,             Matrix4)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(CoordinateFrame3,    CoordinateFrame3)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Plane3,              Plane3)

#undef DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER

// Data structures that are one byte per element can be directly copied, regardles of endianness.
#define DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(fname, tname) \
  void BinaryOutputStream::write##fname(int64 n, tname const * out) \
  { \
    if (sizeof(tname) == 1) \
      writeBytes(n, (void *)out); \
    else \
    { \
      for (int64 i = 0; i < n ; ++i) \
        write##fname(out[i]); \
    } \
  }

DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Bool8,               bool)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(UInt8,               uint8)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Int8,                int8)

#undef DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER

#define DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(fname, tname) \
  void BinaryOutputStream::write##fname(int64 n, tname const * out) \
  { \
    if (m_swapBytes) \
    { \
      for (int64 i = 0; i < n; ++i) \
        write##fname(out[i]); \
    } \
    else \
      writeBytes(sizeof(tname) * n, (void const *)out); \
  }

DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(UInt16,              uint16)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Int16,               int16)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(UInt32,              uint32)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Int32,               int32)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(UInt64,              uint64)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Int64,               int64)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Float32,             float32)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Float64,             float64)

#undef DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER

#define DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(fname, tname) \
  void BinaryOutputStream::write##fname(int64 n, tname const * out) \
  { \
    for (int64 i = 0; i < n; ++i) \
      write##fname(out[i]); \
  }

DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Vector2,             Vector2)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Vector3,             Vector3)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Vector4,             Vector4)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorL8,             ColorL8)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorL,              ColorL)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorRGB8,           ColorRGB8)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorRGB,            ColorRGB)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorRGBA8,          ColorRGBA8)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(ColorRGBA,           ColorRGBA)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Matrix2,             Matrix2)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Matrix3,             Matrix3)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Matrix4,             Matrix4)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(CoordinateFrame3,    CoordinateFrame3)
DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER(Plane3,              Plane3)

#undef DGP_BINARY_OUTPUT_STREAM_DEFINE_WRITER

void
BinaryOutputStream::reallocBuffer(size_t bytes, size_t oldBufferLen)
{
  size_t newBufferLen = (size_t)(m_bufferLen * 1.5) + 100;
  uint8 * newBuffer = NULL;

  if ((m_path == "<memory>") || (newBufferLen < DGP_MAX_WRITE_BUFFER_SIZE))
  {
    // We're either writing to memory (in which case we *have* to try and allocate) or we've been asked to allocate a reasonable
    // size buffer.
    newBuffer = (uint8 *)std::realloc(m_buffer, newBufferLen);

    if (newBuffer != NULL)
      m_bufferCapacity = newBufferLen;
  }

  if ((newBuffer == NULL) && (bytes > 0))
  {
    // Realloc failed; we're probably out of memory. Back out the entire call and try to dump some data to disk.
    m_bufferLen = oldBufferLen;
    reserveBytesWhenOutOfMemory(bytes);
  }
  else
    m_buffer = newBuffer;
}

void
BinaryOutputStream::reserveBytesWhenOutOfMemory(size_t bytes)
{
  if (m_path == "<memory>")
  {
    throw Error(getNameStr() + ": Out of memory while writing to memory (no RAM left)");
  }
  else if ((int64)bytes > m_bufferCapacity)
  {
    throw Error("Out of memory while writing to disk (could not create a large enough buffer)");
  }
  else
  {
    // Dump the contents to disk. In order to enable seeking backwards, we keep the last 10 MB in memory.
    int64 bytesToWrite = m_bufferLen - 10 * 1024 * 1024;

    if (bytesToWrite < m_bufferLen / 3)
    {
      // We're going to write less than 1/3 of the file; give up and just write the whole thing.
      bytesToWrite = m_bufferLen;
    }

    debugAssertM(bytesToWrite > 0, getNameStr() + ": No bytes to write");

    // Write to the file
    char const * mode = (m_alreadyWritten > 0) ? "ab" : "wb";
    FILE * file = fopen(m_path.c_str(), mode);
    debugAssertM(file, getNameStr() + ": Could not open file for writing");

    size_t count = fwrite(m_buffer, 1, (size_t)bytesToWrite, file);
    debugAssertM((int64)count == bytesToWrite, getNameStr() + ": All bytes were not written");
    (void)count;  // avoid unused variable warning

    fclose(file);
    file = NULL;

    // Record that we saved this data.
    m_alreadyWritten += bytesToWrite;
    m_bufferLen -= bytesToWrite;
    m_pos -= bytesToWrite;

    debugAssertM(m_bufferLen < m_bufferCapacity, getNameStr() + ": Buffer exceeds maximum size");
    debugAssertM(m_bufferLen >= 0, getNameStr() + ": Buffer has negative size");
    debugAssertM(m_pos >= 0, getNameStr() + ": Write position is negative");
    debugAssertM(m_pos <= m_bufferLen, getNameStr() + ": Write position is beyond end of buffer");

    // Shift the unwritten data back appropriately in the buffer.
    std::memmove(m_buffer, m_buffer + bytesToWrite, (size_t)m_bufferLen);

    // *Now* we allocate bytes (there should presumably be enough space in the buffer; if not, we'll come back through this code
    // and dump the last 10MB to disk as well.  Note that the bytes > maxBufferLen case above would already have triggered if
    // this call couldn't succeed.
    reserveBytes(bytes);
  }
}

BinaryOutputStream::BinaryOutputStream(Endianness endian)
: NamedObject("<memory>"),
  m_path("<memory>"),
  m_beginEndBits(0),
  m_bitString(0),
  m_bitPos(0),
  m_buffer(NULL),
  m_bufferLen(0),
  m_bufferCapacity(0),
  m_pos(0),
  m_alreadyWritten(0),
  m_ok(true)
{
  setEndianness(endian);
}

BinaryOutputStream::BinaryOutputStream(std::string const & path, Endianness file_endian)
: NamedObject(FilePath::objectName(path)),
  m_path(path),
  m_beginEndBits(0),
  m_bitString(0),
  m_bitPos(0),
  m_buffer(NULL),
  m_bufferLen(0),
  m_bufferCapacity(0),
  m_pos(0),
  m_alreadyWritten(0),
  m_ok(true)
{
  setEndianness(file_endian);

  // Verify ability to write to disk
  _commit(false, true);
}

void
BinaryOutputStream::reset()
{
  debugAssertM(m_beginEndBits == 0, getNameStr() + ": Can't reset in beginBits/endBits block");
  alwaysAssertM(m_path == "<memory>", getNameStr() + ": Can only reset a BinaryOutputStream that writes to memory");

  // Do not reallocate, just clear the size of the buffer.
  m_pos = 0;
  m_alreadyWritten = 0;
  m_bufferLen = 0;
  m_beginEndBits = 0;
  m_bitString = 0;
  m_bitPos = 0;
}

BinaryOutputStream::~BinaryOutputStream()
{
  if (m_path != "<memory>")
    commit(true);

  std::free(m_buffer);
}

void
BinaryOutputStream::setEndianness(Endianness endian)
{
  m_fileEndian = endian;
  m_swapBytes = (endian != Endianness::machine());
}

bool
BinaryOutputStream::ok() const
{
  return m_ok;
}

bool
BinaryOutputStream::commit(bool flush)
{
  return _commit(flush, false);
}

bool
BinaryOutputStream::_commit(bool flush, bool force)
{
  // If there is already an error, the commit fails
  if (!m_ok)
    return false;

  // Nothing to commit for memory streams
  if (m_path == "<memory>")
    return true;

  // Is there anything new to write?
  if (!force && m_bufferLen <= 0)
    return true;

  debugAssertM(m_beginEndBits == 0, getNameStr() + ": Missing endBits before commit");

//   // Make sure the directory exists
//   std::string dir = FilePath::parent(m_path);
//   if (!FileSystem::exists(dir))
//     if (!FileSystem::createDirectory(dir))
//     {
//       DGP_ERROR << "BinaryOutputStream: Could not create parent directory of '" << m_path << "'";
//       m_ok = false;
//     }

  FILE * file = NULL;
  if (m_ok)
  {
    char const * mode = (m_alreadyWritten > 0 ? "ab" : "wb");
    file = fopen(m_path.c_str(), mode);
    if (!file)
    {
      DGP_ERROR << "BinaryOutputStream: Could not open file '" << m_path << "' for writing";
      m_ok = false;
    }
  }

  if (m_ok)
  {
    if (m_buffer != NULL && m_bufferLen > 0)
    {
      size_t success = fwrite(m_buffer, (size_t)m_bufferLen, 1, file);
      debugAssertM(success == 1, getNameStr() + ": Could not write buffer contents to disk");
      (void)success;

      m_alreadyWritten += m_bufferLen;
      m_bufferLen = 0;
      m_pos = 0;
    }

    if (flush)
      fflush(file);

    fclose(file);
    file = NULL;
  }

  return m_ok;
}

void
BinaryOutputStream::commit(uint8 * dst) const
{
  alwaysAssertM(m_path == "<memory>", getNameStr() + ": Can only commit buffer contents of memory streams to memory");
  std::memcpy(dst, m_buffer, (size_t)m_bufferLen);
}

void
BinaryOutputStream::writeUInt16(uint16 u)
{
  reserveBytes(2);
  uint8 * convert = (uint8 *)&u;

  if (m_swapBytes)
  {
    m_buffer[m_pos + 0] = convert[1];
    m_buffer[m_pos + 1] = convert[0];
  }
  else
  {
#ifdef DGP_ALLOW_UNALIGNED_WRITES
    *(uint16 *)(m_buffer + m_pos) = u;
#else
    m_buffer[m_pos + 0] = convert[0];
    m_buffer[m_pos + 1] = convert[1];
#endif
  }

  m_pos += 2;
}

void
BinaryOutputStream::writeUInt32(uint32 u)
{
  reserveBytes(4);
  uint8 * convert = (uint8 *)&u;
  debugAssertM(m_beginEndBits == 0, getNameStr() + ": Can't write non-bit data within beginBits/endBits block");

  if (m_swapBytes)
  {
    m_buffer[m_pos + 0] = convert[3];
    m_buffer[m_pos + 1] = convert[2];
    m_buffer[m_pos + 2] = convert[1];
    m_buffer[m_pos + 3] = convert[0];
  }
  else
  {
#ifdef DGP_ALLOW_UNALIGNED_WRITES
    *(uint32 *)(m_buffer + m_pos) = u;
#else
    m_buffer[m_pos + 0] = convert[0];
    m_buffer[m_pos + 1] = convert[1];
    m_buffer[m_pos + 2] = convert[2];
    m_buffer[m_pos + 3] = convert[3];
#endif
  }

  m_pos += 4;
}

void
BinaryOutputStream::writeUInt64(uint64 u)
{
  reserveBytes(8);
  uint8 * convert = (uint8 *)&u;

  if (m_swapBytes)
  {
    m_buffer[m_pos + 0] = convert[7];
    m_buffer[m_pos + 1] = convert[6];
    m_buffer[m_pos + 2] = convert[5];
    m_buffer[m_pos + 3] = convert[4];
    m_buffer[m_pos + 4] = convert[3];
    m_buffer[m_pos + 5] = convert[2];
    m_buffer[m_pos + 6] = convert[1];
    m_buffer[m_pos + 7] = convert[0];
  }
  else
  {
#ifdef DGP_ALLOW_UNALIGNED_WRITES
    *(uint64 *)(m_buffer + m_pos) = u;
#else
    m_buffer[m_pos + 0] = convert[0];
    m_buffer[m_pos + 1] = convert[1];
    m_buffer[m_pos + 2] = convert[2];
    m_buffer[m_pos + 3] = convert[3];
    m_buffer[m_pos + 4] = convert[4];
    m_buffer[m_pos + 5] = convert[5];
    m_buffer[m_pos + 6] = convert[6];
    m_buffer[m_pos + 7] = convert[7];
#endif
  }

  m_pos += 8;
}

void
BinaryOutputStream::writeAlignedString(std::string const & s, int alignment)
{
  int length = (int)s.length();
  writeInt32(length);
  writeBytes(length, s.c_str());

  int padding = (alignment - (length % alignment)) % alignment;
  if (padding > 0)
  {
    for (int i = 0; i < padding; ++i)
      writeUInt8(0);  // zero not necessary but let's use it to keep things clean instead of calling skip(padding)
  }
}

void
BinaryOutputStream::beginBits()
{
  debugAssertM(m_beginEndBits == 0, getNameStr() + ": beginBits/endBits mismatch");

  m_bitString = 0x00;
  m_bitPos = 0;
  m_beginEndBits = 1;
}

void
BinaryOutputStream::writeBits(int num_bits, uint32 value)
{
  debugAssertM(m_beginEndBits == 1, getNameStr() + ": Can't call writeBits outside beginBits/endBits block");

  while (num_bits > 0)
  {
    // Extract the current bit of value and insert it into the current byte
    m_bitString |= (value & 1) << m_bitPos;
    ++m_bitPos;
    value = value >> 1;
    --num_bits;

    if (m_bitPos > 7)
    {
      // We've reached the end of this byte
      writeUInt8(m_bitString);
      m_bitString = 0x00;
      m_bitPos = 0;
    }
  }
}

void
BinaryOutputStream::endBits()
{
  debugAssertM(m_beginEndBits == 1, getNameStr() + ": beginBits/endBits mismatch");

  if (m_bitPos > 0)
  {
    writeUInt8(m_bitString);
  }

  m_bitString = 0;
  m_bitPos = 0;
  m_beginEndBits = 0;
}

} // namespace DGP

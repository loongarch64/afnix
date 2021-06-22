// ---------------------------------------------------------------------------
// - cmem.hpp                                                                -
// - standard platform library - c memory function definitions               -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#ifndef  AFNIX_CMEM_HPP
#define  AFNIX_CMEM_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the procedures that manage the memory structure of
  /// the running process. The basic functions allocate and free memory. In
  /// some cases, the memory can be allocated for tracing.
  /// @author amaury darsch

  /// allocate some memory
  /// @param size the size to allocate
  void* c_malloc (const long size);

  /// free some memory
  /// @param ptr the memory handle
  void c_free (void* ptr);

  /// initialize a memory block with a byte value
  /// @param pdst  the memory pointer
  /// @param size the size of the memory block  
  /// @param bval the byte value to set
  void c_memset (void* ptr, const long size, const t_byte bval);

  /// copy a memory block with a byte block
  /// @param pdst  the memory pointer
  /// @param size the size of the memory block  
  /// @param psrc the source pointer
  void c_memcpy (void* ptr, const long size, const void* psrc);

  /// allocate some memory for tracing
  /// @param size the memory to allocate
  void* c_galloc (const long size);

  /// free some previously traced memory
  /// @param ptr the pointer to free
  void c_gfree (void* ptr);

  /// register a memory cleanup function
  /// @param func the cleanup function
  void c_gcleanup (void (*func) (void));

  /// set the memory debug flag
  /// @param flag the flag to set
  void c_setmdbg (const bool flag);

  /// set the memory exit flag
  /// @param flag the flag to set
  void c_setmext (const bool flag);

  /// set a memory label memory 
  /// @param label the label to set
  void c_setmlbl (const char* label);

  /// @return the system page size
  long c_pagesize (void);

  /// allocate some memory.
  /// @param size the size to allocate
  /// @return the base pointer of the allocated memory
  void* c_mmap (const long size);

  /// map a file to to some memory
  /// @param sid the file descriptor
  /// @param size the size to map
  /// @param foff the file offset to use
  void* c_mmap (const int sid, const long size, const long foff);

  /// reallocate a memory previously allocated with c_mmap
  /// @param optr  the old base memory pointer
  /// @param osize the old memory size
  /// @param nsize the new memory size
  /// @return the base pointer of the new allocated memory
  void* c_mremap (void* optr, const long osize, const long nsize);

  /// deallocate a memory block previously allocated with c_mmap
  /// @param ptr the base memory pointer
  /// @param size the size of the memory block
  void c_munmap (void* ptr, const long size);

  /// check if an address is within a stack barrier range
  /// @param a the address to check
  bool c_isbstk (void* a);
}

#endif

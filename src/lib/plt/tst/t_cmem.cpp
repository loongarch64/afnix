// ---------------------------------------------------------------------------
// - t_cmem.cpp                                                              -
// - standard platform library - memory map function tester                  -
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

#include "cmem.hpp"

int main (int, char**) {
  using namespace afnix;

  // simple things first
  c_free  (c_malloc (4096));
  c_gfree (c_galloc (4096));

  // allocate 8 pages and see
  int size = 8 * c_pagesize ();
  char* blk = (char*) c_mmap (size);
  if (blk == 0) return 1;

  // mark these pages
  for (int i = 0; i < size; i++) blk[i] = '\a';

  // remap this memory
  blk = (char*) c_mremap ((void*) blk, size, 2 * size);
  for (int i = 0; i < size; i++) 
    if (blk[i] != '\a') return 1;

  // unmape everything
  c_munmap ((void*) blk, 16 * c_pagesize ());
  // here it is
  return 0;
}

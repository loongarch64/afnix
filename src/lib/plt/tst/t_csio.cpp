// ---------------------------------------------------------------------------
// - t_csio.cpp                                                              -
// - standard platform library - simple io tester module                     -
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

#include "csio.hpp"
#include "cstr.hpp"

int main (int, char**) {
  using namespace afnix;

  // check for standard io stream
  if (c_stdin  () < 0) return 1;
  if (c_stdout () < 0) return 1;
  if (c_stderr () < 0) return 1;

  // check if our file exists
  if (c_isfile ("t_csio.cpp") == false) return 1;
  
  // try to open the file
  int sid = c_openr ("t_csio.cpp");
  if (sid < 0) return 1;
  c_close (sid);

  // get the file information
  s_finfo* info = c_finfo ("t_csio.cpp");
  if (info == nullptr) return 1;
  if (info->d_size  != 2200) return 1;
  if (info->d_mtime == 0)    return 1;
  delete info;

  // try to create a selector
  void* sh = c_shnew (true);
  // check the marking
  if (c_shmtst (sh) == true) return 1;
  // mark and test again
  c_shmark (sh);
  if (c_shmtst (sh) == true) return 1;
  // wait and test
  c_shwait (sh, -1);
  if (c_shmtst (sh) == false) return 1;
  if (c_shmtst (sh) == true)  return 1;
  // close the selector
  c_shfree (sh);

  // ok this is enough
  return 0;
}

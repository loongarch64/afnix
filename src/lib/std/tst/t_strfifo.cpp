// ---------------------------------------------------------------------------
// - t_strfifo.cpp                                                           -
// - standard object library - string fifo class tester                      -
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

#include "Strfifo.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new multi fifo
  Strfifo mfifo;
  
  // check the size and flag
  if (mfifo.empty  () == false)          return 1;
  if (mfifo.length () != 0)              return 1;

  // add some elements in the fifo
  if (mfifo.add ("hello") == false)      return 1;
  if (mfifo.add ("world") == false)      return 1;
  if (mfifo.add ("hello") == false)      return 1;
  // check the size and flag 
  if (mfifo.empty  () == true)           return 1;
  if (mfifo.length () != 3)              return 1;
  // check existence
  if (mfifo.exists ("hello") == false)   return 1;
  if (mfifo.exists ("world") == false)   return 1;

  // pop and check
  if (mfifo.pop () != "hello")           return 1;
  if (mfifo.length () != 2)              return 1;
  // check existence
  if (mfifo.exists ("hello") == false)   return 1;
  if (mfifo.exists ("world") == false)   return 1;

  // pop and check
  if (mfifo.pop () != "world")           return 1;
  if (mfifo.length () != 1)              return 1;
  // check existence
  if (mfifo.exists ("hello") == false)   return 1;
  if (mfifo.exists ("world") == true)    return 1;

  // pop and check
  if (mfifo.pop () != "hello")           return 1;
  if (mfifo.length () != 0)              return 1;
  // check existence
  if (mfifo.exists ("hello") == true)    return 1;
  if (mfifo.exists ("world") == true)    return 1;

  // create a new uniq fifo
  Strfifo ufifo (true);
  
  // check the size and flag
  if (ufifo.empty  () == false)          return 1;
  if (ufifo.length () != 0)              return 1;

  // add some elements in the fifo
  if (ufifo.add ("hello") == false)      return 1;
  if (ufifo.add ("world") == false)      return 1;
  //if (ufifo.add ("hello") == true)       return 1;
  // check the size and flag
  if (ufifo.empty  () == true)           return 1;
  if (ufifo.length () != 2)              return 1;
  // check existence
  if (ufifo.exists ("hello") == false)   return 1;
  if (ufifo.exists ("world") == false)   return 1;

  // pop and check
  if (ufifo.pop () != "world")           return 1;
  if (ufifo.length () != 1)              return 1;
  // check existence
  if (ufifo.exists ("hello") == false)   return 1;
  if (ufifo.exists ("world") == true)    return 1;

  // pop and check
  if (ufifo.pop () != "hello")           return 1;
  if (ufifo.length () != 0)              return 1;
  // check existence
  if (ufifo.exists ("hello") == true)    return 1;
  if (ufifo.exists ("world") == true)    return 1;

  // ok - everything is fine
  return 0;
}

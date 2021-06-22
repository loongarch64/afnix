// ---------------------------------------------------------------------------
// - t_cnet.cpp                                                              -
// - standard platform library - network function tester                     -
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

#include "cnet.hpp"
#include "cstr.hpp"

// IPV4 loopback address
const char* AFNIX_IPV4_LOOPBACK = "127.0.0.1";
// IPV6 loopback address
const char* AFNIX_IPV6_LOOPBACK = "::1";

int main (int, char**) {
  using namespace afnix;

  // get the loopback name
  const char* lname = c_loopname ();
  if (lname == nullptr) return 1;

  // check numerical address
  if (c_isipn ("127.0.0.1", IAPF_XANY) == false) return 1;
  if (c_isipn ("127.0.0.1", IAPF_IPV4) == false) return 1;
  if (c_isipn ("127.0.0.1", IAPF_IPV6) == true)  return 1;
  if (c_isipn ("::1",       IAPF_XANY) == false) return 1;
  if (c_isipn ("::1",       IAPF_IPV4) == true)  return 1;
  if (c_isipn ("::1",       IAPF_IPV6) == false) return 1;
  if (c_isipn ("localhost", IAPF_XANY) == true)  return 1;
  if (c_isipn ("localhost", IAPF_IPV4) == true)  return 1;
  if (c_isipn ("localhost", IAPF_IPV6) == true)  return 1;
  
  // try to resolve the loopback name
  s_ipaddr* ipaddr = c_getipa (lname, IAPF_XANY);
  if (ipaddr == nullptr) return 1;
  // get the byte address
  if (ipaddr->d_size == 0) return 1;
  t_byte* addr = c_cpaddr (ipaddr->p_addr[0]);

  // convert back the ip address to a string
  char* ipname = c_getipn (addr, true);
  if (ipname == nullptr) return 1;
  if ((long) (addr[0]) == 4) {
    if (c_strcmp (ipname, AFNIX_IPV4_LOOPBACK) == false) return 1;
  } else if ((long) (addr[0]) == 16) {
    if (c_strcmp (ipname, AFNIX_IPV6_LOOPBACK) == false) {
      return 1;
    }
  } else {
      return 1;
  }
  // clean the address
  delete [] addr;
  delete ipaddr;
  // compute next address after nil
  const t_byte ADDR1[] = {0x04, 0x00, 0x00, 0xFF, 0xFF};
  const t_byte ADDR2[] = {0x04, 0x00, 0x01, 0x00, 0x00};
  addr = c_nxaddr (ADDR1);
  if (c_eqaddr (addr, ADDR2) == false) return 1;
  if (c_ltaddr (ADDR1, addr) == false) return 1;
  if (c_leaddr (ADDR1, addr) == false) return 1;
  if (c_leaddr (ADDR2, addr) == false) return 1;
  delete [] addr;
  // check for max
  const t_byte ZADDR[] = {0x04, 0x00, 0x00, 0x00, 0x00};
  const t_byte MADDR[] = {0x04, 0xFF, 0xFF, 0xFF, 0xFF};
  addr = c_nxaddr (MADDR);
  if (c_eqaddr (addr, ZADDR) == false) return 1;
  // clean and return
  delete [] addr;
  return 0;
}

// ---------------------------------------------------------------------------
// - ccnf.cpp                                                                -
// - standard platform library - config file generator                       -
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
// - author (c) 1999-2021 amaury   darsch                                    -
// - author (c) 2010-2013 nobuhiro iwamatsu                 superh processor -
// - author (c) 2011-2013 pino     toscano                     hurd platform -
// ---------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

// supported platforms
#define AFNIX_PLATFORM_UNKNOWN   0
#define AFNIX_PLATFORM_LINUX     1
#define AFNIX_PLATFORM_SOLARIS   2
#define AFNIX_PLATFORM_FREEBSD   3
#define AFNIX_PLATFORM_DARWIN    4
#define AFNIX_PLATFORM_GNUKBSD   5
#define AFNIX_PLATFORM_GNU       6
#define AFNIX_PLATFORM_ANDROID   7

// recognized platform name
#define AFNIX_PLATNAME_LINUX     "linux"
#define AFNIX_PLATNAME_SOLARIS   "solaris"
#define AFNIX_PLATNAME_FREEBSD   "freebsd"
#define AFNIX_PLATNAME_DARWIN    "darwin"
#define AFNIX_PLATNAME_GNUKBSD   "gnukbsd"
#define AFNIX_PLATNAME_GNU       "gnu"
#define AFNIX_PLATNAME_ANDROID   "android"

// supported processors
#define AFNIX_PROCTYPE_UNKNOWN    0
#define AFNIX_PROCTYPE_IA32       1
#define AFNIX_PROCTYPE_SPARC      2
#define AFNIX_PROCTYPE_ALPHA      3
#define AFNIX_PROCTYPE_ARM        4
#define AFNIX_PROCTYPE_PPC        5
#define AFNIX_PROCTYPE_M68K       6
#define AFNIX_PROCTYPE_MIPS       7
#define AFNIX_PROCTYPE_MIPSEL     8
#define AFNIX_PROCTYPE_PA64       9
#define AFNIX_PROCTYPE_IA64      10
#define AFNIX_PROCTYPE_S390      11
#define AFNIX_PROCTYPE_S390X     12
#define AFNIX_PROCTYPE_X64       13
#define AFNIX_PROCTYPE_SH        14
#define AFNIX_PROCTYPE_MIPS64    15
#define AFNIX_PROCTYPE_MIPS64EL  16
#define AFNIX_PROCTYPE_PPC64EL   17
#define AFNIX_PROCTYPE_AARCH64   18
#define AFNIX_PROCTYPE_RISCV32   19
#define AFNIX_PROCTYPE_RISCV64   20
#define AFNIX_PROCTYPE_LOONGARCH64     21

// recognized processor name
#define AFNIX_PROCNAME_UNKNOWN   "unknown"
#define AFNIX_PROCNAME_IA32      "ia32"
#define AFNIX_PROCNAME_SPARC     "sparc"
#define AFNIX_PROCNAME_ALPHA     "alpha"
#define AFNIX_PROCNAME_ARM       "arm"
#define AFNIX_PROCNAME_PPC       "ppc"
#define AFNIX_PROCNAME_PPC64EL   "ppc64el"
#define AFNIX_PROCNAME_M68K      "m68k"
#define AFNIX_PROCNAME_MIPS      "mips"
#define AFNIX_PROCNAME_MIPSEL    "mipsel"
#define AFNIX_PROCNAME_PA64      "pa64"
#define AFNIX_PROCNAME_IA64      "ia64"
#define AFNIX_PROCNAME_S390      "s390"
#define AFNIX_PROCNAME_S390X     "s390x"
#define AFNIX_PROCNAME_X64       "x64"
#define AFNIX_PROCNAME_SH        "sh"
#define AFNIX_PROCNAME_MIPS64    "mips64"
#define AFNIX_PROCNAME_MIPS64EL  "mips64el"
#define AFNIX_PROCNAME_AARCH64   "aarch64"
#define AFNIX_PROCNAME_RISCV32   "riscv32"
#define AFNIX_PROCNAME_RISCV64   "riscv64"
#define AFNIX_PROCNAME_LOONGARCH64   "loongarch64"

// force size type with S390/clang
#if defined(__s390__) || defined(__clang__)
const bool AFNIX_FORCE_LONG      = true;
#else
const bool AFNIX_FORCE_LONG      = false;
#endif

// special darwin flag
#if defined(__APPLE__)
const bool AFNIX_FORCE_DARWIN    = true;
#else
const bool AFNIX_FORCE_DARWIN    = false;
#endif

// compute the platform id
static int get_platid (const char* plat) {
  if (!plat || (strlen (plat) == 0)) return AFNIX_PLATFORM_UNKNOWN;
  // linux
  if (strcmp (plat, AFNIX_PLATNAME_LINUX) == 0) 
    return AFNIX_PLATFORM_LINUX;
  // solaris
  if (strcmp (plat, AFNIX_PLATNAME_SOLARIS) == 0) 
    return AFNIX_PLATFORM_SOLARIS;
  // freebsd
  if (strcmp (plat, AFNIX_PLATNAME_FREEBSD) == 0) 
    return AFNIX_PLATFORM_FREEBSD;
  // darwin
  if (strcmp (plat, AFNIX_PLATNAME_DARWIN) == 0) 
    return AFNIX_PLATFORM_DARWIN;
  // gnu/freebsd
  if (strcmp (plat, AFNIX_PLATNAME_GNUKBSD) == 0) 
    return AFNIX_PLATFORM_GNUKBSD;
  // gnu/hurd
  if (strcmp (plat, AFNIX_PLATNAME_GNU) == 0) 
    return AFNIX_PLATFORM_GNU;
  // android
  if (strcmp (plat, AFNIX_PLATNAME_ANDROID) == 0) 
    return AFNIX_PLATFORM_ANDROID;
  // unknown
  return AFNIX_PLATFORM_UNKNOWN; 
}

// compute the processor id
static int get_procid (const char* proc) {
  if (!proc || (strlen (proc) == 0)) return AFNIX_PROCTYPE_UNKNOWN;
  // ia
  if (strcmp (proc, AFNIX_PROCNAME_IA32) == 0) 
    return AFNIX_PROCTYPE_IA32;
  // sparc
  if (strcmp (proc, AFNIX_PROCNAME_SPARC) == 0) 
    return AFNIX_PROCTYPE_SPARC;
  // alpha
  if (strcmp (proc, AFNIX_PROCNAME_ALPHA) == 0) 
    return AFNIX_PROCTYPE_ALPHA;
  // arm
  if (strcmp (proc, AFNIX_PROCNAME_ARM) == 0) 
    return AFNIX_PROCTYPE_ARM;
  // aarch64
  if (strcmp (proc, AFNIX_PROCNAME_AARCH64) == 0)
    return AFNIX_PROCTYPE_AARCH64;
  // ppc
  if (strcmp (proc, AFNIX_PROCNAME_PPC) == 0) 
    return AFNIX_PROCTYPE_PPC;
  // ppc64el
  if (strcmp (proc, AFNIX_PROCNAME_PPC64EL) == 0)
    return AFNIX_PROCTYPE_PPC64EL;
  // m68k
  if (strcmp (proc, AFNIX_PROCNAME_M68K) == 0) 
    return AFNIX_PROCTYPE_M68K;
  // mips
  if (strcmp (proc, AFNIX_PROCNAME_MIPS) == 0)
    return AFNIX_PROCTYPE_MIPS;
  // mipsel
  if (strcmp (proc, AFNIX_PROCNAME_MIPSEL) == 0)
    return AFNIX_PROCTYPE_MIPSEL;
  // mips64
  if (strcmp (proc, AFNIX_PROCNAME_MIPS64) == 0)
    return AFNIX_PROCTYPE_MIPS64;
  // mips64el
  if (strcmp (proc, AFNIX_PROCNAME_MIPS64EL) == 0)
    return AFNIX_PROCTYPE_MIPS64EL;
  // pa64
  if (strcmp (proc, AFNIX_PROCNAME_PA64) == 0)
    return AFNIX_PROCTYPE_PA64;
  // ia64
  if (strcmp (proc, AFNIX_PROCNAME_IA64) == 0)
    return AFNIX_PROCTYPE_IA64;
  // riscv32
  if (strcmp (proc, AFNIX_PROCNAME_RISCV32) == 0)
    return AFNIX_PROCTYPE_RISCV32;
  // riscv64
  if (strcmp (proc, AFNIX_PROCNAME_RISCV64) == 0)
    return AFNIX_PROCTYPE_RISCV64;
  // s390
  if (strcmp (proc, AFNIX_PROCNAME_S390) == 0)
    return AFNIX_PROCTYPE_S390;
  // x86-64
  if (strcmp (proc, AFNIX_PROCNAME_X64) == 0) {
    switch (sizeof(void*)) {
    case 4: 
      return AFNIX_PROCTYPE_IA32;
    case 8: 
      return AFNIX_PROCTYPE_X64;
    default:
      return AFNIX_PROCTYPE_UNKNOWN;
    }
  }
  // SuperH
  if (strcmp (proc, AFNIX_PROCNAME_SH) == 0)
    return AFNIX_PROCTYPE_SH;
  // loongarch64
  if (strcmp (proc, AFNIX_PROCNAME_LOONGARCH64) == 0)
    return AFNIX_PROCTYPE_LOONGARCH64;
  // unknown
  return AFNIX_PROCTYPE_UNKNOWN;
}

// get the processor name by id - this is needed as the procid
// can remap the processor
static const char* get_procnm (const int id) {
  if (id ==  AFNIX_PROCTYPE_IA32)     return AFNIX_PROCNAME_IA32;
  if (id ==  AFNIX_PROCTYPE_SPARC)    return AFNIX_PROCNAME_SPARC;
  if (id ==  AFNIX_PROCTYPE_ALPHA)    return AFNIX_PROCNAME_ALPHA;
  if (id ==  AFNIX_PROCTYPE_ARM)      return AFNIX_PROCNAME_ARM;
  if (id ==  AFNIX_PROCTYPE_AARCH64)  return AFNIX_PROCNAME_AARCH64;
  if (id ==  AFNIX_PROCTYPE_PPC)      return AFNIX_PROCNAME_PPC;
  if (id ==  AFNIX_PROCTYPE_PPC64EL)  return AFNIX_PROCNAME_PPC64EL;
  if (id ==  AFNIX_PROCTYPE_M68K)     return AFNIX_PROCNAME_M68K;
  if (id ==  AFNIX_PROCTYPE_MIPS)     return AFNIX_PROCNAME_MIPS;
  if (id ==  AFNIX_PROCTYPE_MIPSEL)   return AFNIX_PROCNAME_MIPSEL;
  if (id ==  AFNIX_PROCTYPE_MIPS64)   return AFNIX_PROCNAME_MIPS64;
  if (id ==  AFNIX_PROCTYPE_MIPS64EL) return AFNIX_PROCNAME_MIPS64EL;
  if (id ==  AFNIX_PROCTYPE_PA64)     return AFNIX_PROCNAME_PA64;
  if (id ==  AFNIX_PROCTYPE_IA64)     return AFNIX_PROCNAME_IA64;
  if (id ==  AFNIX_PROCTYPE_RISCV32)  return AFNIX_PROCNAME_RISCV32;
  if (id ==  AFNIX_PROCTYPE_RISCV64)  return AFNIX_PROCNAME_RISCV64;
  if (id ==  AFNIX_PROCTYPE_S390)     return AFNIX_PROCNAME_S390;
  if (id ==  AFNIX_PROCTYPE_S390X)    return AFNIX_PROCNAME_S390X;
  if (id ==  AFNIX_PROCTYPE_X64)      return AFNIX_PROCNAME_X64;
  if (id ==  AFNIX_PROCTYPE_SH)       return AFNIX_PROCNAME_SH;
  if (id ==  AFNIX_PROCTYPE_LOONGARCH64)       return AFNIX_PROCNAME_LOONGARCH64;
  return AFNIX_PROCNAME_UNKNOWN;
}

// the ccnf header
static const char* header = 
"// ------------------------------------------------------------------------\n"
"// - ccnf.hpp                                                             -\n"
"// - standard platform library - base type configuration                  -\n"
"// ------------------------------------------------------------------------\n"
"// - This program is free software; you can redistribute it and/or modify -\n"
"// - it provided that this copyright notice is kept intact.               -\n"
"// -                                                                      -\n"
"// - This program is distributed in the hope  that it will be useful, but -\n"
"// - without  any  warranty;  without  even  the  implied   warranty   of -\n"
"// - merchantability  or  fitness  for  a particular purpose. In no event -\n"
"// - shall the  copyright  holder  be  liable  for any  direct, indirect, -\n"
"// - incidental or special  damages  arising in any way out of the use of -\n"
"// - this software.                                                       -\n"
"// -                                                                      -\n"
"// - This file was automatically generated by ccnf                        -\n"
"// ------------------------------------------------------------------------\n"
"// - copyright (c) 1999-2021 amaury darsch                                -\n"
"// ------------------------------------------------------------------------";

static const char* darwindefs =
  "// special darwin definitions\n"
  "#if defined (__APPLE__)\n"
  "#if defined (__ppc__)\n"
  "#define AFNIX_DARWIN_PROCID       AFNIX_PROCTYPE_PPC\n"
  "#define AFNIX_DARWIN_PROC         AFNIX_PROCNAME_PPC\n"
  "#elif defined (__ppc64__)\n"
  "#define AFNIX_DARWIN_PROCID       AFNIX_PROCTYPE_PPC\n"
  "#define AFNIX_DARWIN_PROC         AFNIX_PROCNAME_PPC\n"
  "#elif defined (__i386__)\n"
  "#define AFNIX_DARWIN_PROCID       AFNIX_PROCTYPE_IA32\n"
  "#define AFNIX_DARWIN_PROC         AFNIX_PROCNAME_IA32\n"
  "#else\n"
  "#error \"unsupported darwin architecture\"\n"
  "#endif\n"
  "#endif\n";

static const char* types = 
  "  // fundamental types of the afnix system\n"
  "  using t_byte = unsigned char;\n"
  "  using t_word = unsigned short;\n"
  "  using t_quad = unsigned int;\n"
  "  using t_octa = unsigned long long int;\n"
  "  using t_long = long long int;\n"
  "  using t_real = double;\n";

static const char* constants =
  "  // fundamental characters of the afnix system\n"
  "  const   char     nilc      = 0x00;                // nil character\n"
  "  const   char     sohc      = 0x01;                // start of heading\n"
  "  const   char     stxc      = 0x02;                // start of text\n"
  "  const   char     etxc      = 0x03;                // end of text\n"
  "  const   char     eosc      = 0x04;                // end of stream\n"
  "  const   char     bspc      = 0x08;                // backspace\n"
  "  const   char     delc      = 0x7f;                // delete\n"
  "  const   char     eolc      = 0x0a;                // end of line\n"
  "  const   char     crlc      = 0x0d;                // carriage return\n"
  "  const   char     blkc      = 0x20;                // blank\n"
  "  const   char     tabc      = 0x09;                // tab\n"
  "\n"
  "  // fundamental bytes of the afnix system\n"
  "  const   t_byte   nilb      = 0x00U;               // nil byte\n"
  "  const   t_byte   eosb      = 0x04U;               // eos byte\n"
  "\n"
  "  // fundamental words of the afnix system\n"
  "  const   t_word   nilw      = 0x0000U;             // nil word\n"
  "  const   t_word   eosw      = 0x0004U;             // eos word\n"
  "\n"
  "  // fundamental quads of the afnix system\n"
  "  const   t_quad   nilq      = 0x00000000U;         // nil quad\n"
  "  const   t_quad   sohq      = 0x00000001U;         // start of heading\n"
  "  const   t_quad   stxq      = 0x00000002U;         // start of text\n"
  "  const   t_quad   etxq      = 0x00000003U;         // end of text\n"
  "  const   t_quad   eosq      = 0x00000004U;         // end of stream\n"
  "  const   t_quad   bspq      = 0x00000008U;         // backspace\n"
  "  const   t_quad   delq      = 0x0000007fU;         // delete\n"
  "  const   t_quad   eolq      = 0x0000000aU;         // end of line\n"
  "  const   t_quad   crlq      = 0x0000000dU;         // carriage return\n"
  "  const   t_quad   blkq      = 0x00000020U;         // blank\n"
  "  const   t_quad   tabq      = 0x00000009U;         // tab\n"
  "\n"
  "  // fundamental octas of the afnix system\n"
  "  const   t_octa   nilo      = 0x0000000000000000U; // nil octa\n"
  "\n"
  "  // fundamental constants of the afnix system\n"
  "  const   t_long   minl      = 0x8000000000000000U; // min long long\n"
  "  const   t_long   maxl      = 0x7FFFFFFFFFFFFFFFU; // max long long\n";

int main (int, char**) {
  // print the header
  fprintf (stdout, "%s\n\n", header);

  // install the control header
  fprintf (stdout, "#ifndef  AFNIX_CCNF_HPP\n");
  fprintf (stdout, "#define  AFNIX_CCNF_HPP\n\n");

  // define current revision
  fprintf (stdout, "// afnix revision\n");
  fprintf (stdout, "#define  AFNIX_VERSION_MAJOR      %s\n", PLTMAJOR);
  fprintf (stdout, "#define  AFNIX_VERSION_MINOR      %s\n", PLTMINOR);
  fprintf (stdout, "#define  AFNIX_VERSION_PATCH      %s\n", PLTPATCH);
  fprintf (stdout, "#define  AFNIX_VERSION_TOTAL      \"%s.%s.%s\"\n", 
	   PLTMAJOR, PLTMINOR, PLTPATCH);  
  fprintf (stdout, "\n");

  // define the platform list
  fprintf (stdout, "// supported platforms\n");
  fprintf (stdout, "#define  AFNIX_PLATFORM_UNKNOWN   %d\n",
	   AFNIX_PLATFORM_UNKNOWN);
  fprintf (stdout, "#define  AFNIX_PLATFORM_LINUX     %d\n",
	   AFNIX_PLATFORM_LINUX);
  fprintf (stdout, "#define  AFNIX_PLATFORM_SOLARIS   %d\n",
	   AFNIX_PLATFORM_SOLARIS);
  fprintf (stdout, "#define  AFNIX_PLATFORM_FREEBSD   %d\n",
	   AFNIX_PLATFORM_FREEBSD);
  fprintf (stdout, "#define  AFNIX_PLATFORM_DARWIN    %d\n",
	   AFNIX_PLATFORM_DARWIN);
  fprintf (stdout, "#define  AFNIX_PLATFORM_GNUKBSD   %d\n",
	   AFNIX_PLATFORM_GNUKBSD);
  fprintf (stdout, "#define  AFNIX_PLATFORM_GNU       %d\n",
	   AFNIX_PLATFORM_GNU);
  fprintf (stdout, "#define  AFNIX_PLATFORM_ANDROID   %d\n",
	   AFNIX_PLATFORM_ANDROID);
  fprintf (stdout, "\n");

  // define the processor list
  fprintf (stdout, "// supported processors\n");
  fprintf (stdout, "#define  AFNIX_PROCTYPE_UNKNOWN   %d\n",
	   AFNIX_PROCTYPE_UNKNOWN);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_IA32      %d\n",
	   AFNIX_PROCTYPE_IA32);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_SPARC     %d\n",
	   AFNIX_PROCTYPE_SPARC);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_ALPHA     %d\n",
	   AFNIX_PROCTYPE_ALPHA);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_ARM       %d\n",
	   AFNIX_PROCTYPE_ARM);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_AARCH64   %d\n",
           AFNIX_PROCTYPE_AARCH64);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_PPC       %d\n",
	   AFNIX_PROCTYPE_PPC);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_PPC64EL   %d\n",
	   AFNIX_PROCTYPE_PPC64EL);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_M68K      %d\n",
	   AFNIX_PROCTYPE_M68K);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_MIPS      %d\n",
	   AFNIX_PROCTYPE_MIPS);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_MIPSEL    %d\n",
	   AFNIX_PROCTYPE_MIPSEL);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_MIPS64    %d\n",
	   AFNIX_PROCTYPE_MIPS64);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_MIPS64EL  %d\n",
	   AFNIX_PROCTYPE_MIPS64EL);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_PA64      %d\n",
	   AFNIX_PROCTYPE_PA64);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_IA64      %d\n",
	   AFNIX_PROCTYPE_IA64);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_RISCV32   %d\n",
           AFNIX_PROCTYPE_RISCV32);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_RISCV64   %d\n",
           AFNIX_PROCTYPE_RISCV64);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_S390      %d\n",
	   AFNIX_PROCTYPE_S390);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_S390X     %d\n",
	   AFNIX_PROCTYPE_S390X);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_X64       %d\n",
	   AFNIX_PROCTYPE_X64);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_SH        %d\n",
	   AFNIX_PROCTYPE_SH);
  fprintf (stdout, "#define  AFNIX_PROCTYPE_LOONGARCH64        %d\n",
           AFNIX_PROCTYPE_LOONGARCH64);
  fprintf (stdout, "\n");

  // recognized processor names
  fprintf (stdout, "// recognized processor names\n");
  fprintf (stdout, "#define  AFNIX_PROCNAME_UNKNOWN   \"%s\"\n",
	   AFNIX_PROCNAME_UNKNOWN);
  fprintf (stdout, "#define  AFNIX_PROCNAME_IA32      \"%s\"\n",
	   AFNIX_PROCNAME_IA32);
  fprintf (stdout, "#define  AFNIX_PROCNAME_SPARC     \"%s\"\n",
	   AFNIX_PROCNAME_SPARC);
  fprintf (stdout, "#define  AFNIX_PROCNAME_ALPHA     \"%s\"\n",
	   AFNIX_PROCNAME_ALPHA);
  fprintf (stdout, "#define  AFNIX_PROCNAME_ARM       \"%s\"\n",
	   AFNIX_PROCNAME_ARM);
  fprintf (stdout, "#define  AFNIX_PROCNAME_AARCH64   \"%s\"\n",
           AFNIX_PROCNAME_AARCH64);
  fprintf (stdout, "#define  AFNIX_PROCNAME_PPC       \"%s\"\n",
	   AFNIX_PROCNAME_PPC);
  fprintf (stdout, "#define  AFNIX_PROCNAME_PPC64EL   \"%s\"\n",
	   AFNIX_PROCNAME_PPC64EL);
  fprintf (stdout, "#define  AFNIX_PROCNAME_M68K      \"%s\"\n",
	   AFNIX_PROCNAME_M68K);
  fprintf (stdout, "#define  AFNIX_PROCNAME_MIPS      \"%s\"\n",
	   AFNIX_PROCNAME_MIPS);
  fprintf (stdout, "#define  AFNIX_PROCNAME_MIPSEL    \"%s\"\n",
	   AFNIX_PROCNAME_MIPSEL);
  fprintf (stdout, "#define  AFNIX_PROCNAME_MIPS64    \"%s\"\n",
	   AFNIX_PROCNAME_MIPS64);
  fprintf (stdout, "#define  AFNIX_PROCNAME_MIPS64EL  \"%s\"\n",
	   AFNIX_PROCNAME_MIPS64EL);
  fprintf (stdout, "#define  AFNIX_PROCNAME_PA64      \"%s\"\n",
	   AFNIX_PROCNAME_PA64);
  fprintf (stdout, "#define  AFNIX_PROCNAME_IA64      \"%s\"\n",
	   AFNIX_PROCNAME_IA64);
  fprintf (stdout, "#define  AFNIX_PROCNAME_RISCV32   \"%s\"\n",
           AFNIX_PROCNAME_RISCV32);
  fprintf (stdout, "#define  AFNIX_PROCNAME_RISCV64   \"%s\"\n",
           AFNIX_PROCNAME_RISCV64);
  fprintf (stdout, "#define  AFNIX_PROCNAME_S390      \"%s\"\n",
	   AFNIX_PROCNAME_S390);
  fprintf (stdout, "#define  AFNIX_PROCNAME_S390X     \"%s\"\n",
	   AFNIX_PROCNAME_S390X);
  fprintf (stdout, "#define  AFNIX_PROCNAME_X64       \"%s\"\n",
	   AFNIX_PROCNAME_X64);
  fprintf (stdout, "#define  AFNIX_PROCNAME_SH        \"%s\"\n",
	   AFNIX_PROCNAME_SH);
  fprintf (stdout, "#define  AFNIX_PROCNAME_LOONGARCH64     \"%s\"\n",
           AFNIX_PROCNAME_LOONGARCH64);
  fprintf (stdout, "\n");

  // install the darwin definition
  if (AFNIX_FORCE_DARWIN == true) {
    fprintf (stdout, "%s", darwindefs);
    fprintf (stdout, "\n");
  }

  // compute id and name
  const int   platid = get_platid (PLATNAME);
  const int   procid = get_procid (PROCNAME);
  const char* procnm = get_procnm (procid);
  // compute bits size
  long bitssz = sizeof(void*) * 8;
  if (platid == AFNIX_PLATFORM_ANDROID) {
    switch (procid) {
    case AFNIX_PROCTYPE_IA32:
      bitssz = 32;
      break;
    case AFNIX_PROCTYPE_X64:
      bitssz = 64;
      break;
    case AFNIX_PROCTYPE_ARM:
      bitssz = 32;
      break;
    case AFNIX_PROCTYPE_AARCH64:
      bitssz = 64;
      break;
    default:
      fprintf (stderr, "ccnf: invalid processor type %d\n", procid);
      return 1;
    }
  }
  // install the config names
  fprintf (stdout, "// platform definitions\n");
  fprintf (stdout, "#define  AFNIX_PLATFORM_PLATID    %d\n",       platid);
  fprintf (stdout, "#define  AFNIX_PLATFORM_VERSION   %s\n",       PLATVERS);
  fprintf (stdout, "#define  AFNIX_PLATFORM_MAJOR     %s\n",       PLATVMAJ);
  fprintf (stdout, "#define  AFNIX_PLATFORM_MINOR     %s\n",       PLATVMIN);
  fprintf (stdout, "#define  AFNIX_PLATFORM_NAME      \"%s\"\n",   PLATNAME);
  fprintf (stdout, "#define  AFNIX_PLATFORM_VINFO     \"%s\"\n",   PLATVERS);
  // special case for darwin
  if (AFNIX_FORCE_DARWIN == true) {
    fprintf (stdout, "#define  AFNIX_PLATFORM_PROCID    AFNIX_DARWIN_PROCID\n");
    fprintf (stdout, "#define  AFNIX_PLATFORM_PROC      AFNIX_DARWIN_PROC\n");
  } else {
    fprintf (stdout, "#define  AFNIX_PLATFORM_PROCID    %d\n",     procid);
    fprintf (stdout, "#define  AFNIX_PLATFORM_PROC      \"%s\"\n", procnm);
  }
  fprintf (stdout, "#define  AFNIX_PLATFORM_BITS      %ld\n",      bitssz);
  fprintf (stdout, "\n");

  // install installation onfiguration
  fprintf (stdout, "// configuration definitions\n");
  if (strcmp (LINKTYPE, "static") == 0) {
    fprintf (stdout, "#define  AFNIX_REGISTER_STATIC\n");
    fprintf (stdout, "#define  AFNIX_LINKTYPE_STATIC    true\n");
  } else {
    fprintf (stdout, "#define  AFNIX_LINKTYPE_STATIC    false\n");
  }
  fprintf (stdout, "\n");

  // start the namespace
  fprintf (stdout, "namespace afnix {\n");

  // install the base types
  fprintf (stdout, "%s", types);
  fprintf (stdout, "\n");

  // install the size type
  fprintf (stdout, "  // pointer size type\n");
  if (platid == AFNIX_PLATFORM_DARWIN)
    fprintf (stdout, "  using t_size = unsigned long int;\n");
  else if (AFNIX_FORCE_LONG == true)
    fprintf (stdout, "  using t_size = unsigned long int;\n");
  else if (bitssz == 64)
    fprintf (stdout, "  using t_size = unsigned long int;\n");
  else if (bitssz == 32)
    fprintf (stdout, "  using t_size = unsigned int;\n");
  else
    fprintf (stdout, "  using t_size = size_t;\n");    
  fprintf (stdout, "\n");
  
  // install the constants
  fprintf (stdout, "%s", constants);

  // install the footer
  fprintf (stdout, "}\n\n");
  fprintf (stdout, "#endif\n");

  return 0;
}

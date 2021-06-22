// ---------------------------------------------------------------------------
// - cmem.cxx                                                                -
// - standard platform library - c memory function implementation            -
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

#include "cstr.hpp"
#include "csys.hpp"
#include "csio.hpp"
#include "cthr.hpp"
#include "cmem.hpp"
#include "cmem.hxx"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the structure for traced memory
  struct s_gptr {
    s_gptr* p_prev;
    s_gptr* p_next;
    void*   p_bptr;
    char*   p_label;
    long    d_size;
    t_octa  d_magic;
  };

  // this function computes the number of pages for a given size
  static long cmem_getpsize (const long size) {
    long psize = c_pagesize ();
    long result = size / psize;
    if ((size % psize) != 0) result++;
    return (result * psize);
  }

  // this function computes the number of pages for a given size
  static long cmem_getpsize (const long size, const long foff) {
    long psize = c_pagesize ();
    long result = size / psize + ((foff == 0) ? 0 : 1);
    if ((size % psize) != 0) result++;
    return (result * psize);
  }

  // this function adjust an offset on a page boundary
  static long cmem_getosize (const long size) {
    long psize = c_pagesize ();
    long result = size / psize;
    return (result * psize);
  }

  // simple function to align on a 8 bytes basis
  static t_size cmem_align (const t_size size) {
    t_size pad = size % 8;
    if (pad == 0) return size;
    return (((size / 8) + 1) * 8);
  }

  // the memory access control lock
  static void* cmem_mtx = nullptr;

  // the memory stack barrier width
  static const t_long CMEM_BSTKW = 1024LL;
  // the memory magic number
  static const t_octa CMEM_MAGIC = 0x1234567890ABCDEFULL;
  // the size with the padding
  static const t_size CMEM_GMOFF = cmem_align (sizeof (s_gptr));
  
  // the root pointer for traced memory
  static s_gptr* cmem_groot = nullptr;

  // the memory tracing counter and the magic number
  static long cmem_gacnt = 0;
  static long cmem_gfcnt = 0;

  // the cleanup counter and function array
  using  t_cfunc = void (*) (void);
  static long     cmem_gccnt = 0;
  static t_cfunc* cmem_gcfcn = nullptr;

  // the initialize flag for memory tracing
  static bool  cmem_gexit = false;
  static bool  cmem_gflag = false;
  static bool  cmem_gmchk = (c_getenv ("AFNIX_GALLOC_CHECK") != nullptr);
  static bool  cmem_gpstk = (c_getenv ("AFNIX_GALLOC_DEBUG") != nullptr);
  static bool  cmem_gmcnt = (c_getenv ("AFNIX_GALLOC_COUNT") != nullptr);
  static bool  cmem_gctrc = (c_getenv ("AFNIX_GALLOC_TRACE") != nullptr);
  static bool  cmem_gdymd = (c_getenv ("AFNIX_GALLOC_DYNMD") != nullptr);
  static bool  cmem_gctrl = cmem_gmchk||cmem_gpstk||cmem_gmcnt||cmem_gctrc;
  static bool  cmem_gcall = cmem_gctrl||cmem_gdymd;
  static bool  cmem_gdctr = false;
  static char* cmem_label = nullptr;

  // this function report the garbage memory at exit
  static void cmem_galloc_report (void) {
    // security check
    if ((cmem_groot != nullptr) && (cmem_gacnt == cmem_gfcnt)) {
      fprintf (stderr, "galloc: inconsistent count and trace pointer");
      abort ();
    }
    // loop for report
    while (cmem_groot != nullptr) {
      if (cmem_groot->d_magic != CMEM_MAGIC) {
        fprintf (stderr, "galloc: invalid pointer at %p\n", cmem_groot);
        abort ();
      }
      void* handle = ((char*) cmem_groot) + CMEM_GMOFF;
      fprintf (stderr, "garbage allocation of %ld bytes\n",cmem_groot->d_size);
      if (cmem_groot->p_label != nullptr) {
    	fprintf (stderr, "\tlabel: %s\n", cmem_groot->p_label);
      }
      fprintf (stderr, "\tobject: %p\n",handle); 
      c_printtrace (cmem_groot->p_bptr);
      cmem_groot = cmem_groot->p_next;
    }
    // report nothing in check mode
    if (cmem_gmchk == true) return;
    // report nothing in count mode if equal
    if ((cmem_gmcnt == true) && (cmem_gacnt == cmem_gfcnt)) return;
    // normal allocation report
    fprintf (stderr, "total allocated memory: %ld\n", cmem_gacnt);
    fprintf (stderr, "total freed     memory: %ld\n", cmem_gfcnt);
  }

  // this function calls the memory cleanup functions and the report
  // function for garbage memory
  static void cmem_galloc_cleanup (void) {
    if (cmem_gexit == false) {
      // call the cleanup functions
      for (long i = 0; i < cmem_gccnt; i++) {
	t_cfunc func = (t_cfunc) cmem_gcfcn[i];
	func ();
      }
      // do the report
      cmem_galloc_report ();
    }
    // clean the mutex
    c_mtxdestroy (cmem_mtx);
  }

  // this function initalize the memory tracing
  static void cmem_galloc_init (void) {
    // do nothing if not requested
    if ((cmem_gctrl == false) || (cmem_gflag == true)) return;
    // install cleanup handlers
    c_atexit (cmem_galloc_cleanup);
    // marked initialized
    cmem_gflag = true;
    // create the memory mutex
    cmem_mtx = c_mtxcreate ();
  }

  // this function clean the memory debug stack
  static void cmem_galloc_clean (s_gptr* handle) {
    // get the lock before cleaning up
    c_mtxlock (cmem_mtx);
    // unlink the structure and free it
    s_gptr* prev = handle->p_prev;
    s_gptr* next = handle->p_next;
    if ((handle == cmem_groot) || (prev == nullptr)) {
      cmem_groot = next;
      if (cmem_groot != nullptr) cmem_groot->p_prev = nullptr;
    } else {
      prev->p_next = next;
      if (next != nullptr) next->p_prev = prev;
    }
    cmem_gfcnt += handle->d_size;

    // check if we print the stack trace
    if (cmem_gpstk == true) {
      fprintf (stderr, "destruction of %ld bytes\n", handle->d_size);
      if (handle->p_label != nullptr) 
	fprintf (stderr, "\tlabel: %s\n", handle->p_label);
      fprintf (stderr, "\tobject: %p\n", handle);
      c_printtrace (handle->p_bptr);
    }
    c_destroytrace (handle->p_bptr);
    if (handle->p_label != nullptr) free (handle->p_label);
    free (handle);
    // release the lock
    c_mtxunlock (cmem_mtx);
  }

  // this function allocates some memory for tracing
  static void* cmem_galloc (const long size) {
    // get the lock before allocation
    c_mtxlock (cmem_mtx);
    // allocate the memory block
    s_gptr* handle  = (s_gptr*) malloc (size + CMEM_GMOFF);
    if (cmem_groot != nullptr) cmem_groot->p_prev = handle;
    handle->p_next  = cmem_groot;
    handle->p_prev  = nullptr;
    handle->p_label = cmem_label;
    handle->d_size  = size;
    handle->d_magic = CMEM_MAGIC;
    handle->p_bptr  = cmem_gctrc ? c_backtrace () : nullptr;
    // set global root and counter
    cmem_groot  = handle;
    cmem_gacnt += size;
    // clean label
    cmem_label = nullptr;
    // re-align result
    void* result = ((char*) handle) + CMEM_GMOFF;

    // check if we print the stack trace for the allocation
    if (cmem_gpstk == true) {
      fprintf (stderr, "allocation of %ld bytes\n", size);
      if (handle->p_label != nullptr) 
	fprintf (stderr, "\tlabel: %s\n", handle->p_label);
      fprintf (stderr, "\tobject: %p\n", handle);
      c_printtrace (handle->p_bptr);
    }
    c_mtxunlock (cmem_mtx);
    // here it is
    return result;
  }

  // this function free the memory in tracing mode
  static void cmem_gfree (void* ptr) {
    // handle the simple free vs complex one
    if (cmem_gctrl == false) {
      if (cmem_gdctr == false) {
	// here the memory debugger was never turned on dynamically
	free (ptr);
      } else {
	// here the memory debugger was turned off dynamically
	s_gptr* handle = (s_gptr*) ((char*) (ptr) - CMEM_GMOFF);
	if (handle->d_magic == CMEM_MAGIC) {
	  cmem_galloc_clean (handle);
	} else {
	  free (ptr);
	}
      }
      return;
    }
    // get the structure and check the magic number
    s_gptr* handle = (s_gptr*) ((char*) (ptr) - CMEM_GMOFF);
    if (handle->d_magic != CMEM_MAGIC) {
      if (cmem_gdctr == false) {
	// here the memory debugger was never turned on dynamically
	fprintf (stderr, "galloc: invalid pointer to free at %p\n", ptr);
	abort ();
      } else {
	// assume here the pointer was allocated before turning on
	// the memory debugger
	free (ptr);
      }
      return;
    }
    // clean the handle
    cmem_galloc_clean (handle);
  }


#ifdef AFNIX_HAVE_MPROTECT
  static void cmem_mprotect_none (void* addr, t_size size) {
    // get the page size
    long psize = c_pagesize ();
    // just check for alignement
    if (((t_size) addr % psize) != 0) abort ();
    if (((t_size) size % psize) != 0) abort ();
    // protect the page
    if (mprotect (addr, size, PROT_NONE) != 0) abort ();
  }
  static void cmem_mprotect_read (void* addr, t_size size) {
    // get the page size
    long psize = c_pagesize ();
    // just check for alignement
    if (((t_size) addr % psize) != 0) abort ();
    if (((t_size) size % psize) != 0) abort ();
    // protect the page
    if (mprotect (addr, size, PROT_READ) != 0) abort ();
  }
#else
  static void cmem_mprotect_none (void* addr, t_size size) {
    // get the page size
    long psize = c_pagesize ();
    // just check for alignement
    if (((t_size) addr % psize) != 0) abort ();
    if (((t_size) size % psize) != 0) abort ();
  }
  static void cmem_mprotect_read (void* addr, t_size size) {
    // get the page size
    long psize = c_pagesize ();
    // just check for alignement
    if (((t_size) addr % psize) != 0) abort ();
    if (((t_size) size % psize) != 0) abort ();
  }
#endif

  // this function allocates a protected memory zone - the memeory
  // is allocated by mmap if possible between two protected pages
  static void* cmem_palloc (const long size) {
    // check for null first
    if (size == 0) abort ();
    // get the page size
    long psize = c_pagesize ();
    // compute the aligned size
    long asize = cmem_getpsize (size);
    // compute the requested size with fences
    long rsize = psize + asize + psize;
    // allocate the memory
    void* bptr = c_mmap (rsize);
    void* mptr = (t_byte*) bptr + psize;
    void* eptr = (t_byte*) bptr + psize + asize;
    // check the memory
    if (bptr == nullptr) abort ();
    // mark the memory size
    *((t_octa*)bptr) = (t_octa) rsize;
    // protect the first and last page
    cmem_mprotect_none (bptr, psize);
    cmem_mprotect_none (eptr, psize);
    // here it is
    return mptr;
  }

  // free a protected memory zone
  static void cmem_pfree (void* ptr) {
    // check for nil first
    if (ptr == nullptr) return;
    // get the lock
    c_mtxlock (cmem_mtx);
    // get the page size
    long psize = c_pagesize ();
    // adjust memory pointer
    void* bptr = (t_byte*) ptr - psize;
    // unprotect the first page
    cmem_mprotect_read (bptr, psize);
    // read the block size
    long rsize = *((t_octa*) bptr);
    // unprotect the last page
    void* eptr = (t_byte*) bptr + rsize - psize;
    cmem_mprotect_read (eptr, psize);
    // free the memory
    c_munmap (bptr, rsize);
    // release the lock
    c_mtxunlock (cmem_mtx);
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // enable/disable dynamic memory debug

  void c_setmdbg (const bool flag) {
    // do nothing if the dynamic debugging
    // was not previsouly authorized
    if (cmem_gdymd == false) return;
    // get the memory mutex
    c_mtxlock (cmem_mtx);
    // set the control flag
    cmem_gctrl = flag;
    // set the dynamic control
    cmem_gdctr = true;
    // release memeory mutex
    c_mtxunlock (cmem_mtx);
  }

  // set the memory exit flag

  void c_setmext (const bool flag) {
    // get the memory mutex
    c_mtxlock (cmem_mtx);
    // set thr flag
    cmem_gexit = flag;
    // release memeory mutex
    c_mtxunlock (cmem_mtx);
  }

  // set a memory label

  void c_setmlbl (const char* label) {
    // do nothing if the memory analysis is not enabled
    if (cmem_gcall == false) return;
    // get the memory mutex
    c_mtxlock (cmem_mtx);
    // clean the old label
    if (cmem_label != nullptr) free (cmem_label);
    cmem_label = nullptr;
    // create a new label
    long llen = c_strlen (label);
    if (llen > 0) {
      cmem_label = (char*) malloc (llen + 1);
      c_strcpy (cmem_label, label);
    }
    // release the memory mutex
    c_mtxunlock (cmem_mtx);
  }

  // allocate some memory for tracing
  
  void* c_galloc (const long size) {
    // do nothing in non tracing mode
    if (cmem_gctrl == false) return malloc (size);

    // initialize the memory subsystem
    if (cmem_gflag == false) cmem_galloc_init ();

    // handle the memory check mode
    if (cmem_gmchk == true) return cmem_palloc (size);

    // handle the allocation for tracing
    return cmem_galloc (size);
  }

  // free some memory in tracing mode

  void c_gfree (void* ptr) {
    // handle the memory check
    if (cmem_gmchk == true) {
      cmem_pfree (ptr);
      return;
    }
    // do normal free or memory tracing 
    cmem_gfree (ptr);
  }

  // register a memory cleanup function

  void c_gcleanup (void (*func) (void)) {
    // just use atexit if no tracing
    if (cmem_gctrl == false) {
      c_atexit (func);
      return;
    }
    // allocate a new array of cleanup functions
    t_cfunc* array = (t_cfunc*) malloc ((cmem_gccnt+1) * sizeof (t_cfunc));
    // copy the old array to the new one
    for (long i = 0; i < cmem_gccnt; i++) array[i] = cmem_gcfcn[i];
    array[cmem_gccnt++] = func;
    free (cmem_gcfcn);
    cmem_gcfcn = array;
  }

  // allocate some memory with malloc

  void* c_malloc (const long size) {
    if (size <= 0) return 0;
    return malloc (size);
  }

  // free some memory with normal free

  void c_free (void* handle) {
    if (handle == 0) return;
    free (handle);
  }

  // set a memory block with a value

  void c_memset (void* pdst, const long size, const t_byte bval) {
    // check for nil first
    if ((pdst == nullptr) || (size <= 0)) return;
    // map the pointer and loop
    auto dst = reinterpret_cast<t_byte*>(pdst);
    for (long k = 0; k < size; k++) dst[k] = bval;
  }

  // copy a memory block with a byte block

  void c_memcpy (void* pdst, const long size, const void* psrc) {
    // check for nil first
    if ((pdst == nullptr) || (size <= 0) || (psrc == nullptr)) return;
    // map the pointer and loop
    auto src = reinterpret_cast<const t_byte*>(psrc);
    auto dst = reinterpret_cast<t_byte*>(pdst);
    for (long k = 0L; k < size; k++) dst[k] = src[k];
  }

  // map some memory with a file descriptor
  
  void* c_mmap (const int sid, const long size, const long foff) {
    if ((sid == -1) || (size == 0)) return nullptr;
    // get the memory to allocate in page
    long psize = cmem_getpsize (size, foff);
    // get the offset aligned to pages
    long osize = cmem_getosize (foff);
    long opage = foff - osize;
    char* ptr  = (char*) mmap (0, psize, PROT_READ|PROT_WRITE, MAP_PRIVATE,
			       sid, osize);
    if (ptr == MAP_FAILED) return nullptr;
    return (ptr + opage);
  }

  // unmap a memory block

  void c_munmap (void* ptr, const long size) {
    long psize = cmem_getpsize (size);
    munmap ((caddr_t) ptr, psize);
  }

  // check if memory address is on the stack - this function assumes than
  // the adress is eventually within a local stack barrier and therefore
  // is effective if called with the address when just allocated.

  bool c_isbstk (void* a) {
    // local address within the address barrier
    void* p = nullptr;
    // delta value [stack grows up or down]
    t_long d = (t_long) (a) - (t_long) (&p);
    if (d < 0LL) d = -d;
    // delta check
    return (d < CMEM_BSTKW) ? true : false;
  }
}

#ifdef AFNIX_HAVE_SYSCONF
namespace afnix {
  // return the system memory page
  long c_pagesize (void) {
    return sysconf (_SC_PAGESIZE);
  }
}
#endif

#ifdef AFNIX_HAVE_PAGESIZE
namespace afnix {
  // return the system memory page
  long c_pagesize (void) {
    return getpagesize ();
  }
}
#endif

#if defined(AFNIX_HAVE_MAPANONYMOUS)
namespace afnix {
  // allocate a block of memory
  void* c_mmap (const long size) {
    long  psize = cmem_getpsize (size);
    void* ptr   = mmap (0, psize, PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    return (ptr == MAP_FAILED) ? nullptr : ptr;
  }
}
#elif defined(AFNIX_HAVE_MAPANON)
namespace afnix {
  // allocate a block of memory
  void* c_mmap (const long size) {
    long  psize = cmem_getpsize (size);
    void* ptr   = mmap (0, psize, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON,
			-1, 0);
    return (ptr == MAP_FAILED) ? nullptr : ptr;
  }
}
#else
namespace afnix {
  // allocate a block of memory
  void* c_mmap (const long size) {
    int fd = open ("/dev/zero", O_RDWR);
    if (fd == -1) return nullptr;
    long psize = get_psize (size);
    void* ptr  = mmap (0,psize,PROT_READ|PROT_WRITE,MAP_PRIVATE, fd ,0);
    close (fd);
    return (ptr == MAP_FAILED) ? nullptr : ptr;
  }
}
#endif

#ifdef AFNIX_HAVE_MREMAP
namespace afnix { 
  // reallocate a block of memory
  void* c_mremap (void* optr, const long osize, const long nsize) {
    long  posize = cmem_getpsize (osize);
    long  pnsize = cmem_getpsize (nsize);
    void* ptr    = mremap ((caddr_t) optr,posize,pnsize,MREMAP_MAYMOVE);
    return (ptr == MAP_FAILED) ? nullptr : ptr;
  }
}
#else
namespace afnix {
  // reallocate a block of memory - on sun we have to allocate and copy
  void* c_mremap (void* optr, const long osize, const long nsize) {
    if (nsize <= osize) return optr;
    void* nptr = c_mmap (nsize);
    unsigned char* cnptr = (unsigned char*) nptr;
    unsigned char* coptr = (unsigned char*) optr;
    for (long i = 0; i < osize; i++) *cnptr++ = *coptr++;
    c_munmap (optr, osize);;
    return nptr;
  }
}
#endif

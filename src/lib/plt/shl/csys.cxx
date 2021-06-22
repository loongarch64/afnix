// ---------------------------------------------------------------------------
// - csys.cxx                                                                -
// - standard platform library - c system function implementation            -
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

#include "csys.hpp"
#include "cstr.hpp"
#include "cthr.hpp"
#include "cmem.hpp"
#include "csys.hxx"

namespace afnix {

  // static mutex creation function
  static void* csys_smtx_create (void);
  // mutex for various calls
  static void* csys_smtx = csys_smtx_create ();
  // this function destroy the mutex at exit
  static void csys_smtx_destroy (void) {
    c_mtxdestroy (csys_smtx);
  }
  // this function initialize a mutex statically
  static void* csys_smtx_create (void) {
    void* result = c_mtxcreate ();
    c_atexit (csys_smtx_destroy);
    return result;
  }

  // the program name
  static char* pgm = nullptr;

  // this function destroy the program name
  static void sys_pgm_destroy (void) {
    delete [] pgm;
  }

  // return true if the system is 32 bit

  bool c_is32 (void) {
    return (sizeof (long) == 4);
  }

  // return true if the system is 64 bit

  bool c_is64 (void) {
    return (sizeof (long) == 8);
  }

  // return a unique id

  t_long c_uniqid (void) {
    // the global unique id
    static t_long uniqid = 0;
    // lock the mutex before access
    c_mtxlock (csys_smtx);
    t_long result = uniqid++;
    c_mtxunlock (csys_smtx);
    return result;
  }

  // set the program name

  void c_setpgm (const char* name) {
    if (pgm == nullptr) c_atexit (sys_pgm_destroy);
    if (pgm != nullptr) delete [] pgm;
    pgm = c_strdup (name);
  }

  // return the program name
  
  char* c_getpgm (void) {
    return c_strdup (pgm);
  }

  // return a temporary name

  char* c_tempnam (const char* prefix) {
    static long tcnt = 0;
    // lock the mutex
    c_mtxlock (csys_smtx);
    // control the final prefix
    char* base = c_strtrc (prefix, 64);
    char buffer[512];    
    sprintf (buffer, "%s-%ld-%ld", base, c_getpid (), tcnt++);
    delete [] base;
    // unlock the mutex
    c_mtxunlock (csys_smtx);
    return c_strdup (buffer);
  }

  // return the os name
  const char* c_osname (void) {
    return AFNIX_PLATFORM_NAME;
  }

  // return the os type

  const char* c_ostype (void) {
    return AFNIX_PLATFORM_TYPE;
  }

  // return the native option separator

  const char c_optsep (void) {
    return AFNIX_PLATFORM_OSEP;
  }

  // exit without executing the register code

  void c_abort (void) {
    _exit (1);
  }

  // exit unconditionnaly with a status code

  void c_exit (int status) {
    // exit abruply when called from a thread
    if (c_thrmaster() == false) _exit(status);
    // set the memory exit flag
    c_setmext (true);
    // exit immediately
    exit (status);
  }

#ifdef AFNIX_PROFILED
  // register a function to execute at exit
  void c_atexit (void (*func) (void)) {
  }
#else
  // register a function to execute at exit
  void c_atexit (void (*func) (void)) {
    atexit (func);
  }
#endif

  // return the process id

  long c_getpid (void) {
    return getpid ();
  }

  // check if a process exists

  bool c_ispid (const long pid) {
    return (kill (pid, 0) == 0) ? true : false; 
  }

  // return an environment variable value

  const char* c_getenv (const char* name) {
    if (c_strlen (name) == 0) return nullptr;
    return getenv (name);
  }
}

#ifdef AFNIX_HAVE_HOSTNAME
namespace afnix {
  // return the host name
  char* c_hostname (void) {
    char name[1024];
    if (gethostname (name,1024) != 0) return nullptr;
    return c_strdup (name);
  }
}
#endif

#ifdef AFNIX_HAVE_SYSINFO
namespace afnix {
  // return the host name
  char* c_hostname (void) {
    char name[1024];
    if (sysinfo (SI_HOSTNAME, name,1024) == -1) return nullptr;
    return c_strdup (name);
  }
}
#endif

#ifdef AFNIX_HAVE_UID
namespace afnix {
  // return the user name
  char* c_username (void) {
    // get the user id
    uid_t uid = getuid ();
    // get the password line entry
    struct passwd  pwbuf;
    struct passwd* result = NULL;
    char buf[1024];
    if (getpwuid_r (uid, &pwbuf, buf, 1024, &result) != 0) {
      return c_strdup (c_getenv ("USER"));
    }
    return c_strdup (pwbuf.pw_name);
  }

    // return the user home
  char* c_userhome (void) {
    // get the user id
    uid_t uid = getuid ();
    // get the password line entry
    struct passwd  pwbuf;
    struct passwd* result = NULL;
    char buf[1024];
    if (getpwuid_r (uid, &pwbuf, buf, 1024, &result) != 0) {
      return c_strdup (c_getenv ("HOME"));
    }
    return c_strdup (pwbuf.pw_dir);
  }
}
#endif

#ifdef AFNIX_HAVE_BACKTRACE
#if defined(__GNUC__) && defined(AFNIX_DEBUG)
#define GET_CALLER_FRAME __builtin_frame_address(0);
#else
#define GET_CALLER_FRAME nullptr
#endif

namespace afnix {
  // the begining of the frame
  struct s_frame {
    s_frame*    p_prev;
    void*       p_pc;
  };

  struct s_finfo {
    void*    p_pc;
    char*    p_name;
    long     d_fidx;
    s_finfo* p_prev;
  };
  
  // create a new stack info structure
  static s_finfo* sys_finfo_new (void) {
    s_finfo* finfo = (s_finfo*) c_malloc (sizeof (s_finfo));
    finfo->p_pc   = nullptr;
    finfo->p_name = nullptr;
    finfo->d_fidx = 0;
    finfo->p_prev = nullptr;
    return finfo;
  }

  // destroy a stack info structure
  static void sys_finfo_del (s_finfo* finfo) {
    s_finfo* elem = finfo;
    while (elem != nullptr) {
      s_finfo* prev = elem->p_prev;
      c_free (elem->p_name);
      c_free (elem);
      elem = prev;
    }
  }

  // this function resolve the pc name from its address
  static char* sys_get_pc_name (void* pc) {
    if (pc == nullptr) return nullptr;
    Dl_info dlinfo;
    if (dladdr (pc, &dlinfo) == 0) return nullptr;
    // extract the string length
    long slen = c_strlen (dlinfo.dli_sname);
    if (slen == 0) return nullptr;
    // allocate the copy string with the base malloc
    char* result = (char*) c_malloc (slen+1);
    c_strcpy (result, dlinfo.dli_sname);
    return result;
  }

  // return the backtrace of the calling function
  void* c_backtrace (void) {
    c_mtxlock (csys_smtx);
    // get the caller frame
    s_frame* frame = (s_frame*) GET_CALLER_FRAME;
    s_finfo* finfo = nullptr;
    long     index = 0;
    // loop in the frame
    while (frame != nullptr) {
      // allocate a new frame
      s_finfo* data = sys_finfo_new ();
      // fill the frame
      data->p_pc   = frame->p_pc;
      data->p_name = sys_get_pc_name (data->p_pc);
      data->p_prev = finfo;
      data->d_fidx = index++;
      finfo = data;
      frame = frame->p_prev;
      if (data->p_name == nullptr) break;
    }
    c_mtxunlock (csys_smtx);
    return finfo;
  }

  // print the stack trace
  void c_printtrace (void* bptr) {
    if (bptr == nullptr) return;
    // get to the first frame
    s_finfo* finfo = (s_finfo*) bptr;
    if (finfo->p_prev != nullptr) c_printtrace (finfo->p_prev);
    // print frame info
    if (finfo->d_fidx == 0)
      fprintf (stderr, "\n\t%ld: %s\n", finfo->d_fidx, finfo->p_name);
    else
      fprintf (stderr, "\t%ld: %s\n", finfo->d_fidx, finfo->p_name);
  }

  // destroy a stack trace
  void c_destroytrace (void* bptr) {
    if (bptr == nullptr) return;
    s_finfo* finfo = (s_finfo*) bptr;
    sys_finfo_del (finfo);
  }
}
#else
namespace afnix {
  // return the stack trace
  void* c_backtrace (void) {
    return nullptr;
  }

  // print the stack trace
  void c_printtrace (void*) {
    return;
  }

  // destroy a stack trace
  void c_destroytrace (void*) {
    return;
  }
}
#endif

namespace afnix {

  // preset the system signal
  static void sys_set_signal (void) {
    // the sigaction stucture
    struct sigaction sa;
    c_memset (&sa, sizeof (sa), nilc);
    // SIGPIPE: ignore
    sa.sa_handler  = SIG_IGN;
    sigemptyset (&sa.sa_mask);
    sigaction (SIGPIPE, &sa, NULL);
  }

  // preset the system by name

  void c_preset (const char* name) {
    // preset the system name
    c_setpgm (name);
    // set the system signal
    sys_set_signal ();
  }

  // daemonize a running process

  bool c_daemon (void) {    
    // check if runnig as a dameon
    if (getppid () == 1) return true;

    // for the process and exit parent
    pid_t pid = fork ();
    if (pid < 0) return false;
    if (pid > 0) c_exit (0);
    
    // check the mask
    umask (S_IRUSR | S_IWUSR);
    // reset the session
    pid_t sid = setsid ();
    if (sid < 0) return false;
    // reset the root directory
    if (chdir ("/") < 0) return false;
    // close standard descriptors
    close (STDIN_FILENO);
    close (STDOUT_FILENO);
    close (STDERR_FILENO);
    // reopen with dev null
    int fd = open ("/dev/null", O_RDWR);
    if (fd != STDIN_FILENO) return false;
    if (dup (fd) != STDOUT_FILENO) return false;
    if (dup (fd) != STDERR_FILENO) return false;
    return true;
  }
}

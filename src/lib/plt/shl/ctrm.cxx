// ---------------------------------------------------------------------------
// - ctrm.cxx                                                                -
// - standard platform library : c terminal function implementation          -
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
#include "ccnv.hpp"
#include "csys.hpp"
#include "cthr.hpp"
#include "ctrm.hpp"
#include "ctrm.hxx" 

#ifdef AFNIX_HAVE_TERM
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the terminal structure
  struct s_ctrm {
    // the terminal
    TERMINAL* p_term;
    // the reference count
    long d_rcnt;
    // the next terminal
    s_ctrm* p_next;
    // create a default terminal
    s_ctrm (void) {
      p_term = nullptr;
      d_rcnt = 0L;
      p_next = nullptr;
    }
    ~s_ctrm (void) {
      if (p_term != nullptr) {
        #ifdef AFNIX_HAVE_DELTERM
        del_curterm(p_term);
        #endif
      }
      if (p_next != nullptr) delete p_next;
    }
  };

  // static mutex creation function
  static void* ctrm_rmtx_create (void);
  // mutex of terminal root list
  static void* ctrm_rmtx = ctrm_rmtx_create ();
  // terminal root list
  static s_ctrm* ctrm_root = nullptr;
  // this function destroy the mutex at exit
  static void ctrm_rmtx_destroy (void) {
    delete ctrm_root;
    c_mtxdestroy (ctrm_rmtx);
  }
  // this function initialize a mutex statically
  static void* ctrm_rmtx_create (void) {
    void* result = c_mtxcreate ();
    c_atexit (ctrm_rmtx_destroy);
    return result;
  }

  // set the terminal current
  bool ctrm_set_term (s_ctrm* ctrm) {
    if (ctrm == nullptr) return false;
    c_mtxlock (ctrm_rmtx);
    if (ctrm->p_term == nullptr) return false;
    set_curterm (ctrm->p_term);
    c_mtxunlock (ctrm_rmtx);
    return true;
  }

  // get a terminal structure
  s_ctrm* ctrm_get_term (TERMINAL* term) {
    // check for nil
    if ((term == nullptr) || (ctrm_root == nullptr)) return nullptr;
    // loop in list
    s_ctrm* ctrm = ctrm_root;
    while (ctrm != nullptr) {
      if (ctrm->p_term == term) return ctrm;
      ctrm = ctrm->p_next;
    }
    return nullptr;
  }
  
  // create a new terminal
  s_ctrm* ctrm_new_term (void) {
    // take the lock
    c_mtxlock (ctrm_rmtx);
    // setup terminal terminal
    int status = 0;
    if (setupterm (getenv ("TERM"),STDOUT_FILENO,&status) != OK) {
      c_mtxunlock (ctrm_rmtx);
      return nullptr;
    }
    // check for existing terminat
    s_ctrm* ctrm = ctrm_get_term (cur_term);
    if (ctrm == nullptr) {
      ctrm = new s_ctrm;
      ctrm->p_term = cur_term;
      ctrm->d_rcnt = 1;
      ctrm->p_next = ctrm_root;
      ctrm_root = ctrm;
    } else {
      ctrm->d_rcnt++;
    }
    c_mtxunlock (ctrm_rmtx);
    return ctrm;
  }

  // delete a terminal
  void ctrm_del_term (s_ctrm* ctrm) {
    if (ctrm == nullptr) return;
    c_mtxlock (ctrm_rmtx);
    if (--ctrm->d_rcnt <= 0) {
      #ifdef AFNIX_HAVE_DELTERM
      del_curterm(ctrm->p_term);
      #endif
      ctrm->p_term = nullptr;
      ctrm->d_rcnt = 0;
    }
    c_mtxunlock (ctrm_rmtx);
  }
}

#ifdef AFNIX_HAVE_NONCONST_TIGET
namespace afnix {
  // get capability flag
  static bool c_tigetflag (const char* capname) {
    char* buf = const_cast <char*> (capname);
    int   val = tigetflag (buf);
    return (val <= 0) ? false : true;
  }
  // get capability string
  static char* c_tigetstr (const char* capname) {
    char* buf = const_cast <char*> (capname);
    char* ptr = tigetstr (buf);
    return (ptr == (char*)-1) ? nullptr : ptr;
  }
}
#else
namespace afnix {
  // get capability flag
  static bool c_tigetflag (const char* capname) {
    int val = tigetflag (capname);
    return (val <= 0) ? false : true;
  }
  // get capability string
  static char* c_tigetstr (const char* capname) {
    char* ptr = tigetstr (capname);
    return (ptr == (char*)-1) ? nullptr : ptr;
  }
}
#endif

#ifdef AFNIX_HAVE_COMPLEX_TPARM
namespace afnix {
  static char* c_tparm_0 (char* data) {
    return tparm (data,0,0,0,0,0,0,0,0,0);
  }
  static char* c_tparm_1 (char* data, long p0) {
    return tparm (data,p0,0,0,0,0,0,0,0,0);
  }
}
#else
namespace afnix {
  static char* c_tparm_0 (char* data) {
    return tparm (data);
  }
  static char* c_tparm_1 (char* data, long p0) {
    return tparm (data, p0);
  }
}
#endif

#ifdef AFNIX_DELETE_ISBS
namespace afnix {
  // change the backspace setting
  static void set_delete_isbs (char** tinfo) {
    tinfo[ITERM_BACKSPACE] = c_strmak (delc);
  }
}
#else
namespace afnix {
  // change the backspace setting
  static void set_delete_isbs (char** tinfo) {
    if (tinfo[ITERM_BACKSPACE] == nullptr) 
      tinfo[ITERM_BACKSPACE] = c_strmak (bspc);
  }
}
#endif
#endif

namespace afnix {
  
  // check if the stream id is a terminal
 
  #ifdef AFNIX_HAVE_TERM
  bool c_istty (const int sid) {
    return (isatty (sid) == 1) ? true : false;
  }
  #else
  bool c_istty (const int sid) {
    return false;
  }
  #endif

  // get the terminal attributes

  #ifdef AFNIX_HAVE_TERM
  void* c_gtattr (const int sid) {
    struct termios* tattr = new struct termios;

    // check for terminal first
    if (c_istty (sid) == false) return nullptr;
    // get the terminal attributes
    if (tcgetattr (sid, tattr) != 0) return nullptr;
    return tattr;
  }
  #else
  void* c_gtattr (const int) {
    return nullptr;
  }
  #endif
  
  // set the terminal attributes

  #ifdef AFNIX_HAVE_TERM
  void c_stattr (const int sid, void* tattr) {
    // check for structure and tty
    if (tattr == nullptr) return;
    if (c_istty (sid) == false) return;
 
    // set terminal attributes
    tcsetattr (sid, TCSANOW, (struct termios*) tattr);
  }
  #else
  void c_stattr (const int, void*) {
    return;
  }
  #endif
  
  // reset terminal mode - put in non canonical mode

  #ifdef AFNIX_HAVE_TERM
  bool c_stcanon (const int sid) {
    // check for terminal first
    if (c_istty (sid) == false) return false;

    // reset canonical mode and echo
    struct termios tattr;

    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag    &= ~(ICANON|ECHO);
    tattr.c_cc[VMIN]  = 1;
    tattr.c_cc[VTIME] = 0;
    if (tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr) != 0) return false;
    return true;
  }
  #else
  bool c_stcanon (const int) {
    return false;
  }
  #endif

  #ifdef AFNIX_HAVE_TERM
  // this function checks if a string is bounded in the tinfo array
  static bool check_tinfo (char** tinfo, long size, const char* name) {
    for (long i = 0; i < size; i++) 
      if (c_strcmp (tinfo[i], name) == true) return true;
    return false;
  }

  // these functions fix some inconsistencies in the terminfo/termcap world
  // this is only coming from various experiments
  static char** fix_tinfo_input (char** tinfo) {
    // fix delete character
    set_delete_isbs (tinfo);

    // fix arrows and delete
    if (check_tinfo (tinfo, ITERM_PARMS_MAX, XTERM_ARROW_UP) == false) 
      tinfo[ITERM_STD_UP] = c_strdup (XTERM_ARROW_UP);
    if (check_tinfo (tinfo, ITERM_PARMS_MAX, XTERM_ARROW_DOWN) == false) 
      tinfo[ITERM_STD_DOWN] = c_strdup (XTERM_ARROW_DOWN);
    if (check_tinfo (tinfo, ITERM_PARMS_MAX, XTERM_ARROW_RIGHT) == false) 
      tinfo[ITERM_STD_RIGHT] = c_strdup (XTERM_ARROW_RIGHT);
    if (check_tinfo (tinfo, ITERM_PARMS_MAX, XTERM_ARROW_LEFT) == false) 
      tinfo[ITERM_STD_LEFT ] = c_strdup (XTERM_ARROW_LEFT);
    if (check_tinfo (tinfo, ITERM_PARMS_MAX, XTERM_DELETE_KEY) == false) 
      tinfo[ITERM_STD_DELETE] = c_strdup (XTERM_DELETE_KEY);
    if (check_tinfo (tinfo, ITERM_PARMS_MAX, XTERM_INSERT_KEY) == false) 
      tinfo[ITERM_STD_INSERT] = c_strdup (XTERM_INSERT_KEY);

    // and hopefully, it is working
    return tinfo;
  }

  static char** fix_tinfo_output (char** tinfo) {
    // check for cub1 - if not set we use arrow left
    if (c_strlen (tinfo[OTERM_MOVE_LEFT]) == 0)
      tinfo[OTERM_MOVE_LEFT] = c_strdup (XTERM_ARROW_LEFT);
    // check for cuf1 - if not set we use the standard right arrow
    if (c_strlen (tinfo[OTERM_MOVE_RIGHT]) == 0)
      tinfo[OTERM_MOVE_RIGHT] = c_strdup (XTERM_ARROW_RIGHT);
    // check for cuu1 - if not set we use the standard up arrow
    if (c_strlen (tinfo[OTERM_MOVE_UP]) == 0)
      tinfo[OTERM_MOVE_UP] = c_strdup (XTERM_ARROW_UP);
    // check for cud1 - if not set we use the standard up arrow
    if (c_strlen (tinfo[OTERM_MOVE_DOWN]) == 0)
      tinfo[OTERM_MOVE_DOWN] = c_strdup (XTERM_ARROW_DOWN);
    // check that we can fix background colors
    if (c_strlen (tinfo[OTERM_RESET_COLOR]) == 0) {
      delete [] tinfo[OTERM_SETFG_MODE];
      delete [] tinfo[OTERM_RESET_COLOR];
      tinfo[OTERM_SETFG_MODE]  = nullptr;
      tinfo[OTERM_RESET_COLOR] = nullptr;
    }
    // hopefully, we are set
    return tinfo;
  }
  #endif
  
  // allocate a terminal struture

  #ifdef AFNIX_HAVE_TERM
  void* c_talloc (void) {
    return ctrm_new_term ();
  }
  #else
  void* c_talloc (void) {
    return nullptr;
  }
  #endif
  
  // free a terminal structure

  #ifdef AFNIX_HAVE_TERM
  void c_tfree (void* term) {
    // map the terminal
    auto ctrm = static_cast<s_ctrm*> (term);
    if (ctrm == nullptr) return;
    // free the terminal
    ctrm_del_term (ctrm);
  }
  #else
  void c_tfree (void*) {
  }
  #endif
  
  // return an array of terminal capabilities

  #ifdef AFNIX_HAVE_TERM
  char** c_tinfo (void* term, const bool imode) {
    // create an array of capabilities and initialize it
    long len = imode ? ITERM_PARMS_MAX : OTERM_PARMS_MAX;
    char** result = new char*[len];
    for (long i = 0; i < len; i++) result[i] = nullptr;
    // map the terminal
    auto ctrm = static_cast<s_ctrm*> (term);
    if (ctrm == nullptr) return result;

    // query input capabilities
    if (imode == true) {
      result[ITERM_BACKSPACE]    = c_strdup (c_tigetstr ("kbs"));
      result[ITERM_DELETE]       = c_strdup (c_tigetstr ("kdch1"));
      result[ITERM_ARROW_UP]     = c_strdup (c_tigetstr ("kcuu1"));
      result[ITERM_ARROW_DOWN]   = c_strdup (c_tigetstr ("kcud1"));
      result[ITERM_ARROW_LEFT]   = c_strdup (c_tigetstr ("kcub1"));
      result[ITERM_ARROW_RIGHT]  = c_strdup (c_tigetstr ("kcuf1"));
      result[ITERM_INSERT_KEY]   = c_strdup (c_tigetstr ("kich1"));
      result[ITERM_STD_UP]       = nullptr;
      result[ITERM_STD_DOWN]     = nullptr;
      result[ITERM_STD_RIGHT]    = nullptr;
      result[ITERM_STD_LEFT]     = nullptr;
      result[ITERM_STD_DELETE]   = nullptr;
      result[ITERM_STD_INSERT]   = nullptr;
    } else {
      result[OTERM_DELETE_CHAR]  = c_strdup (c_tigetstr ("dch1"));
      result[OTERM_MOVE_LEFT]    = c_strdup (c_tigetstr ("cub1"));
      result[OTERM_MOVE_RIGHT]   = c_strdup (c_tigetstr ("cuf1"));
      result[OTERM_MOVE_UP]      = c_strdup (c_tigetstr ("cuu1"));
      result[OTERM_MOVE_DOWN]    = c_strdup (c_tigetstr ("cud1"));
      result[OTERM_MOVE_BOL]     = c_strdup (c_tigetstr ("cr"));
      result[OTERM_INSERT_CHAR]  = c_strdup (c_tigetstr ("ich1"));
      result[OTERM_IMODE_START]  = c_strdup (c_tigetstr ("smir"));
      result[OTERM_IMODE_END]    = c_strdup (c_tigetstr ("rmir"));
      result[OTERM_SETFG_MODE]   = c_strdup (c_tigetstr ("setaf"));
      result[OTERM_RESET_COLOR]  = c_strdup (c_tigetstr ("op"));
      result[OTERM_CLEAR_SCREEN] = c_strdup (c_tigetstr ("clear"));
      // local fix for color
      if (c_strlen (result[OTERM_RESET_COLOR]) == 0) {
	if (result[OTERM_RESET_COLOR] != nullptr) 
	  delete [] result[OTERM_RESET_COLOR];
	result[OTERM_RESET_COLOR] = c_strdup (c_tigetstr ("oc"));
      }
    }
    // here is our array
    return imode ? fix_tinfo_input (result) : fix_tinfo_output (result);
  }
  #else
  char** c_tinfo (void*, const bool) {
    return nullptr;
  }
  #endif
  
  // return an array of terminal capabilities flag

  #ifdef AFNIX_HAVE_TERM
  bool* c_tbool (void* term) {
    // map the terminal
    auto ctrm = static_cast<s_ctrm*> (term);
    if (term == nullptr) return nullptr;
    // set the terminal current
    if (ctrm_set_term (ctrm) == false) return nullptr;

    // create an array of capabilities and initialize it
    bool* result = new bool [BTERM_PARMS_MAX];
    // query capabilities
    result[BTERM_AUTO_WRAP] = c_tigetflag ("am") && c_tigetflag ("xn");
    // the array of capabilities
    return result;
  }
  #else
  bool* c_tbool (void*) {
    return nullptr;
  }
  #endif
  
  // return true if a parameter is valid

  #ifdef AFNIX_HAVE_TERM
  bool c_tpvld (char** tinfo, const long index, const bool mode) {
    // check terminal info and index
    if ((tinfo == nullptr) || (index < 0)) return false;
    // check bound based on mode
    if ((mode == true)  && (index >= OTERM_PARMS_MAX)) return false;
    if ((mode == false) && (index >= ITERM_PARMS_MAX)) return false;
    // check parameter
    if (c_strlen (tinfo[index]) == 0) return false;
    return true;
  }
  #else
  bool c_tpvld (char**, const long, const bool) {
    return false;
  }
  #endif
  
  // send a character to the standard output

  #ifdef AFNIX_HAVE_TERM
  void c_tparm (const int sid, char** tinfo, const long index) {
    if ((tinfo == nullptr) || (index < 0) || (index >= OTERM_PARMS_MAX)) return;
    char* data = tinfo[index];
    if (data == nullptr) return;
    char* buffer = c_tparm_0 (data);
    long  length = c_strlen (buffer);
    c_write (sid, buffer, length);
  }
  #else
  void c_tparm (const int, char**, const long) {
    return;
  }
  #endif
  
  // turn on/off the terminal error mode

  #ifdef AFNIX_HAVE_TERM
  void c_temode (const int sid, char** tinfo, const bool mode) {
    if (tinfo == nullptr) return;
    char* setaf = tinfo[OTERM_SETFG_MODE];
    char* rstaf = tinfo[OTERM_RESET_COLOR];
    if ((setaf == nullptr) || (rstaf == nullptr)) return;
    char* buffer = mode ? c_tparm_1 (setaf,1) : c_tparm_0 (rstaf);
    long  length = c_strlen (buffer);
    c_write (sid, buffer, length);
  }
  #else
  void c_temode (const int, char**, const bool) {
    return;
  }
  #endif
  
  // free the terminal attributes structure

  #ifdef AFNIX_HAVE_TERM
  void c_ftattr (void* ptr) {
    struct termios* tattr = (struct termios*) ptr;
    delete tattr;
  }
  #else
  void c_ftattr (void*) {
    return;
  }
  #endif
}

namespace afnix {
  #ifdef AFNIX_HAVE_WINSIZE
  // get the number of columns for a tty 
  long c_getcols (const int sid) {
    // try ioctl first
    if (c_istty (sid) == true) {
      struct winsize size;
      if (ioctl (STDOUT_FILENO, TIOCGWINSZ, &size) == 0) {
        long cols = (long) size.ws_col;
        if (cols != 0) return cols;
      }
    }
    // may be the environment will help us !!!
    bool status = true;
    long  cols  = c_atoll (getenv ("COLUMNS"), status);
    if ((status == true) && (cols != 0)) return cols;
    // nothing match so we return 0
    return 0;
  }
  #else
  long c_getcols (const int ) {
    return 0L;
  }
  #endif
}


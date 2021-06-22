// ---------------------------------------------------------------------------
// - csio.cxx                                                                -
// - standard platform library - c i/o function implementation               -
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
#include "ccnv.hpp"
#include "cerr.hpp"
#include "cclk.hpp"
#include "cmem.hpp"
#include "csio.hpp"
#include "csio.hxx"

namespace afnix {

  // the selector handle structure
  struct s_select {
    // input reference set
    fd_set d_irfd;
    // output reference set
    fd_set d_orfd;
    // input select set
    fd_set d_isfd;
    // output select set
    fd_set d_osfd;
    // max sid
    int    d_smax;
    // the mark flag
    bool   d_mflg;
    // the pipe sid
    int    d_psid[2];
    // create a new handle by mode
    s_select (const bool mflg) {
      FD_ZERO (&d_irfd);
      FD_ZERO (&d_orfd);
      FD_ZERO (&d_isfd);
      FD_ZERO (&d_osfd);
      d_smax = 0;
      d_mflg = false;
      d_psid[0] = -1;
      d_psid[1] = -1;
      if ((mflg == true) && (pipe (d_psid) == -1)) {
	d_psid[0] = -1;
	d_psid[1] = -1;
      }	
    }
    // destroy this handle
    ~s_select (void) {
      if (d_psid[0] != -1) close (d_psid[0]);
      if (d_psid[1] != -1) close (d_psid[1]);
    }
    // check if the marking mode is active
    bool ismmod (void) const {
      return (d_psid[0] != -1) && (d_psid[1] != -1);
    }
    // copy the reference sets
    void fdcopy (void) {
      // clear set
      FD_ZERO (&d_isfd);
      FD_ZERO (&d_osfd);
      // loop with all descriptor
      for (int i = 0; i <= d_smax; i++) {
	if (FD_ISSET (i, &d_irfd) != 0) FD_SET (i, &d_isfd);
	if (FD_ISSET (i, &d_orfd) != 0) FD_SET (i, &d_osfd);
      }
    }
  };

  // return the default input stream associated with this process

  int c_stdin (void) {
    return STDIN_FILENO;
  }

  // return the default output stream associated with this process

  int c_stdout (void) {
    return STDOUT_FILENO;
  }

  // return the default error stream associated with this process

  int c_stderr (void) {
    return STDERR_FILENO;
  }

  // check if the name corresponds to a file name and can be accessed.
  // it does not necessarily means that it can be opened.

  bool c_isfile (const char* name) {
    int         status;
    struct stat buffer;
  
    // check the file name
    if (c_strlen (name) == 0) return false;
    // check the file status
    status = stat (name,&buffer);
    if ((status == 0) && S_ISREG (buffer.st_mode)) return true;
    return false;
  }

  // check if the name corresponds to a directory name and can be accessed.
  // it does not necessarily means that it can be opened.

  bool c_isdir (const char* name) {
    int         status;
    struct stat buffer;
  
    // check the directory name
    if (c_strlen (name) == 0) return false;
    // check the directory status
    status = stat (name,&buffer);
    if ((status == 0) && S_ISDIR (buffer.st_mode)) return true;
    return false;
  }

  // open a file for reading. a file descriptor is returned on success or
  // -1 in case of error.

  int c_openr (const char* name) {
    int fd = open (name, O_RDONLY);
    return (fd == -1) ? c_errmap (errno) : fd;
  }

  // open a file for writing. a file descriptor is returned on success or
  // -1 in case of error.

  int c_openw (const char* name, const bool tflag, const bool aflag) {
    // compute flag and mode
    int    flag = O_WRONLY | O_CREAT;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if (tflag == true) flag |= O_TRUNC;
    if (aflag == true) flag |= O_APPEND;
    // try to open the file
    int fd = open (name, flag, mode);
    return (fd == -1) ? c_errmap (errno) : fd;
  }

  // open a file for read write. A file descriptor is returned on success or
  // -1 in case of error.

  int c_openrw (const char* name, const bool tflag, const bool aflag) {
    // compute flag and mode
    int    flag = O_RDWR  | O_CREAT;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if (tflag == true) flag |= O_TRUNC;
    if (aflag == true) flag |= O_APPEND;
    // try to open the file
    int fd = open (name, flag, mode);
    return (fd == -1) ? c_errmap (errno) : fd;
  }

  // open a session file for read write by user and read by other.


  int c_opensd (const char* name) {
    // compute flag and mode
    int    flag = O_RDWR  | O_CREAT | O_EXCL;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    // try to open the file
    int fd = open (name, flag, mode);
    return (fd == -1) ? c_errmap (errno) : fd;
  }

  // open a private file

  int c_openpd (const char* name) {
    // compute flag and mode
    int    flag = O_RDWR  | O_CREAT;
    mode_t mode = S_IRUSR | S_IWUSR;
    // try to open the file
    int fd = open (name, flag, mode);
    return (fd == -1) ? c_errmap (errno) : fd;
  }
  
  // wait for a character or timeout

  bool c_rdwait (const int sid, const long tout) {
    // chck for good descriptor
    if (sid < 0) return false;
    // initialize the set
    fd_set  set;
    FD_ZERO (&set);
    FD_SET  (sid, &set);
     
    // initialize the timeout
    struct timeval timeout;
    timeout.tv_sec  = tout / 1000;
    timeout.tv_usec = (tout % 1000) * 1000;

    // wait on select
    int status = 0;
    if (tout == -1) {
      status = select (sid+1, &set, NULL, NULL, NULL);
    } else {
      status = select (sid+1, &set, NULL, NULL, &timeout);
    }
    return (status == 1) ? true : false;
  }

  // read n bytes in a buffer. The buffer must be pre-allocated. The number
  // of bytes read is returned

  t_long c_read (const int sid, char* buffer, const t_long count) {  
    // standard checks
    if ((sid < 0) || (buffer == nullptr)) return AFNIX_ERR_IARG;
    // read n bytes and return error code
    t_long result = 0LL;
    while (result != count) {
      t_long size = read (sid, &buffer[result], count-result);
      // check for eos
      if (size == 0LL) break;
      // check for error
      if (size == -1LL) {
	if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
	result = -1LL;
	break;
      }
      result += size;
    }
    return (result == -1) ? c_errmap (errno) : result;
  }

  // write n bytes from a buffer. The buffer must be pre-allocated. The number
  // of bytes written is returned

  t_long c_write (const int sid, const char* buffer, const t_long count) {
    // standard checks
    if ((sid < 0) || (buffer == nullptr)) return AFNIX_ERR_IARG;
    if (count == 0LL) return 0LL;
    // write n bytes and return
    t_long result = 0LL;
    while (result != count) {
      t_long size = write (sid, &buffer[result], count-result);
      // check for eos
      if (size == 0LL) break;
      // check for error
      if (size == -1LL) {
	if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
	result = -1LL;
	break;
      }
      result += size;
    }
    return (result == -1) ? c_errmap (errno) : result;
  }

  // duplicate a stream id.

  int c_dup (const int sid) {
    if (sid < 0) return -1;
    return dup (sid);
  }

  // close this stream.

  bool c_close (const int sid) {
    if (sid < 0) return AFNIX_ERR_IARG;
    return (close (sid) == -1) ? false : true;
  }

  // return the size of a file

  t_long c_fsize (const int sid) {
    struct stat buffer;
    // check that we have a regular file
    int status = fstat (sid, &buffer);
    if ((status != 0) || (!S_ISREG (buffer.st_mode))) return AFNIX_ERR_IARG;
    // return the file size
    return buffer.st_size;
  }

  // return the file modification time

  t_long c_mtime (const int sid) {
    struct stat buffer;
    // check that we have a regular file
    int status = fstat (sid, &buffer);
    if ((status != 0) || (!S_ISREG (buffer.st_mode))) return AFNIX_ERR_IARG;
    // return the file size
    return buffer.st_mtime + c_epoch ();
  }

  // return some file information

  s_finfo* c_finfo (const char* name) {
    // check for a file name
    if (name == nullptr) return nullptr;
    // try to fill the stat info
    struct stat buffer;
    int status = stat (name, &buffer);
    // check for a good status
    if ((status != 0) || (!S_ISREG (buffer.st_mode))) return nullptr;
    // allocate the result structure
    s_finfo* result = new s_finfo;
    result->d_size  = buffer.st_size;
    result->d_mtime = buffer.st_mtime + c_epoch ();
    // here is the result
    return result;    
  }

  // set a file at a certain position

  bool c_lseek (const int sid, const t_long pos) {
    off_t status = lseek (sid, (off_t) pos, SEEK_SET);
    if (status == (off_t) -1) return false;
    return true;
  }

  // lock completly a file

  bool c_flock (const int sid, const bool wlk) {
    // standard check
    if (sid == -1) return false;
    // make a full range locking
    struct flock fl;
    c_memset (&fl, sizeof(fl), nilc);
    fl.l_type   = wlk ? F_WRLCK : F_RDLCK;
    fl.l_start  = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len    = 0;
    fl.l_pid    = getpid ();
    // lock or wait
    return (fcntl (sid, F_SETLKW, &fl) == -1) ? false : true;
  }

  // unlock a file

  bool c_funlock (const int sid) {
    // standard check
    if (sid == -1) return false;
    // unlock the file
    struct flock fl;
    fl.l_type   = F_UNLCK;
    fl.l_start  = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len    = 0;
    fl.l_pid    = getpid ();
    // lock or wait
    return (fcntl (sid, F_SETLK, &fl) == -1) ? false : true;
  }

  // move a file int the file system

  bool c_mv (const char* name, const char* path) {
    if ((name == nullptr) || (path == nullptr)) return false;
    return (rename (name, path) == 0) ? true : false;
  }

  // remove the file given its name.

  bool c_rm (const char* name) {
    if (name == nullptr) return false;
    return (unlink (name) == 0) ? true : false;
  }

  // create a new selector handle

  void* c_shnew (const bool mflg) {
    // create a selector by mode
    s_select* sh = new s_select (mflg);
    // check for marking mode
    if (mflg == true) {
      c_shiadd (sh, sh->d_psid[0]);
    }
    return sh;
  }

  // free a selector handle

  void c_shfree (void* handle) {
    s_select* sh = (s_select*) handle;
    delete sh;
  }

  // add an input descriptor to the select handle

  void c_shiadd (void* handle, const int sid) {
    if ((handle == nullptr) || (sid < 0)) return;
    s_select* sh = (s_select*) handle;
    FD_SET (sid, &(sh->d_irfd));
    if (sid > sh->d_smax) sh->d_smax = sid;
  } 

  // add an output descriptor to the select handle

  void c_shoadd (void* handle, const int sid) {
    if ((handle == nullptr) || (sid < 0)) return;
    s_select* sh = (s_select*) handle;
    FD_SET (sid, &(sh->d_orfd));
    if (sid > sh->d_smax) sh->d_smax = sid;
  }

  // wait for a descriptor to be ready

  long c_shwait (void* handle, const long tout) {
    // check for valid call
    if (handle == nullptr) return 0;
    s_select* sh = (s_select*) handle;
    // copy the reference set
    sh->fdcopy ();
    // initialize the timeout
    struct timeval timeout;
    timeout.tv_sec  = tout / 1000;
    timeout.tv_usec = (tout % 1000) * 1000;
    // now call select
    int result = 0;
    int nsh     = sh->d_smax + 1;
    if (tout < 0) {
      result = select (nsh, &(sh->d_isfd), &(sh->d_osfd), NULL, NULL);
    } else {
      result = select (nsh, &(sh->d_isfd), &(sh->d_osfd), NULL, &timeout);
    }
    // check for error and remap
    if (result == -1) result = c_errmap (errno);
    return result;
  }

  // mark a selector if possible

  void c_shmark (void* handle) {
    // check valid handle
    if (handle == nullptr) return;
    s_select* sh = (s_select*) handle;
    // check for marking mode
    if (sh->ismmod () == false) return;
    // check for marking set
    if (sh->d_mflg == true) return;
    // write a control byte
    if (write (sh->d_psid[1], "\0", 1) == 1) {
      sh->d_mflg = true;
    }
  }

  // return true if a marking descriptor is set

  bool c_shmtst (void* handle) {
    // check valid handle
    if (handle == nullptr) return false;
    s_select* sh = (s_select*) handle;
    // check for marking mode
    if (sh->ismmod () == false) return false;
    // check for marking set
    if (sh->d_mflg == false) return false;
    // check the selector
    if (FD_ISSET (sh->d_psid[0], &(sh->d_isfd)) == 0) return false;
    // try to read the control byte
    char c = nilc;
    bool result = false;
    if (read (sh->d_psid[0], &c, 1) == 1) {
      result = (c == nilc) ? true : false;
    }
    sh->d_mflg = false;
    return result;      
  }

  // return true if an input descriptor is set

  bool c_shitst (void* handle, const int sid) {
    // check for valid call
    if ((handle == nullptr) || (sid < 0)) return false;
    s_select* sh = (s_select*) handle;
    return (FD_ISSET (sid, &(sh->d_isfd)) == 0) ? false : true;
  }

  // return true if an output descriptor is set

  bool c_shotst (void* handle, const int sid) {
    // check for valid call
    if ((handle == nullptr) || (sid < 0)) return false;
    s_select* sh = (s_select*) handle;
    return (FD_ISSET (sid, &(sh->d_osfd)) == 0) ? false : true;
  }
}

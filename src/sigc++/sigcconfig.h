/* sigc++/config/sigcconfig.h.in. */
/*
  This file controls all the configurablity of sigc++ with regards
  to different compilers.  If you are begining a new port of sigc++
  to a compiler this is where to start.  

  Unix compilers are handled automatically by configure.  Other
  platforms require proper identification here.  To add a new
  port, first identify your compilers unique predefine and 
  create a LIBSIGC_{compiler} in the detection stage.  Then
  place a section which defines for the behavior of your compiler
  in the platform section.
*/
#ifndef _SIGC_CONFIG_H_
#define _SIGC_CONFIG_H_

// autoconf likes to place a lot of stuff we don't want.
#if 0

/* Define if you have the <dlfcn.h> header file.  */
#undef HAVE_DLFCN_H

/* Name of package */
#undef PACKAGE

/* Version number of package */
#undef VERSION

#endif /* 0 */

#define SIGC_MAJOR_VERSION 1
#define SIGC_MINOR_VERSION 2
#define SIGC_MICRO_VERSION 5

// Detect common platforms

#define LIBSIGC_MSVC
#define LIBSIGC_DLL

// Compiler specific definitions


// I guess the configure checks can't be run on these plaforms,
// so we specify these compiler features based on our experience.
// These platforms are detected above based on compiler-defined macros.
// murrayc:


#ifdef LIBSIGC_MSVC
#define SIGC_CXX_INT_CTOR 1
#define SIGC_CXX_NAMESPACES 1
//#define SIGC_CXX_MEMBER_FUNC_TEMPLATES 1
//#define SIGC_CXX_MEMBER_CLASS_TEMPLATES 1
//#define SIGC_CXX_TEMPLATE_CCTOR 1
//#define SIGC_CXX_MUTABLE 1
#define SIGC_CXX_EXPLICIT explicit
#define SIGC_CXX_EXPLICIT_COPY explicit
#endif /* LIBSIGC_MSVC */


// Window DLL declarations 

#define LIBSIGC_API
#define LIBSIGC_TMPL 

#endif /* _SIGC_CONFIG_H_ */


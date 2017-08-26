/*
 * Name  : DeclSpec
 * Path  : 
 * Use   : Export symbols into DLL
 * Author: Sven Schmidt
 * Date  : 08/25/2017
 */
#pragma once

// KDTREE_EXPORTS is defined by the compiler in this project only
#ifdef KDTREE_EXPORTS
  #define KDTREE_EXPORTS_DECL_SYMBOLS __declspec(dllexport)
#else
  #define KDTREE_EXPORTS_DECL_SYMBOLS __declspec(dllimport)
#endif

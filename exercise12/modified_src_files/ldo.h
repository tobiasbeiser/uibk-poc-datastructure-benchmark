/*
** $Id: ldo.h $
** Stack and Call structure of Lua
** See Copyright Notice in lua.h
*/

#ifndef ldo_h
#define ldo_h

#include "lapi.h"
#include "lgc.h"
#include "llimits.h"
#include "lobject.h"
#include "lstate.h"
#include "lzio.h"



/*
** Macro to check stack size and grow stack if needed.  Parameters
** 'pre'/'pos' allow the macro to preserve a pointer into the
** stack across reallocations, doing the work only when needed.
** It also allows the running of one GC step when the stack is
** reallocated.
** 'condmovestack' is used in heavy tests to force a stack reallocation
** at every check.
*/
#define luaD_checkstackaux(L,n,pre,pos)  \
	if (l_unlikely(L->stack_last.p - L->top.p <= (n))) \
	  { pre; luaD_growstack(L, n, 1); pos; } \
        else { condmovestack(L,pre,pos); }

/* In general, 'pre'/'pos' are empty (nothing to save) */
#define luaD_checkstack(L,n)	luaD_checkstackaux(L,n,(void)0,(void)0)



#define savestack(L,pt)		(cast_charp(pt) - cast_charp(L->stack.p))
#define restorestack(L,n)	cast(StkId, cast_charp(L->stack.p) + (n))


/* macro to check stack size, preserving 'p' */
#define checkstackp(L,n,p)  \
  luaD_checkstackaux(L, n, \
    ptrdiff_t t__ = savestack(L, p),  /* save 'p' */ \
    p = restorestack(L, t__))  /* 'pos' part: restore 'p' */


/* macro to check stack size and GC, preserving 'p' */
#define checkstackGCp(L,n,p)  \
  luaD_checkstackaux(L, n, \
    ptrdiff_t t__ = savestack(L, p);  /* save 'p' */ \
    luaC_checkGC(L),  /* stack grow uses memory */ \
    p = restorestack(L, t__))  /* 'pos' part: restore 'p' */


/* macro to check stack size and GC */
#define checkstackGC(L,fsize)  \
	luaD_checkstackaux(L, (fsize), luaC_checkGC(L), (void)0)


/* type of protected functions, to be ran by 'runprotected' */
typedef void (*Pfunc) (lua_State *L, void *ud);

LUAI_FUNC void luaD_seterrorobj (lua_State *L, int errcode, StkId oldtop);
LUAI_FUNC int luaD_protectedparser (lua_State *L, ZIO *z, const char *name,
                                                  const char *mode);
LUAI_FUNC void luaD_hook (lua_State *L, int event, int line,
                                        int fTransfer, int nTransfer);
LUAI_FUNC void luaD_hookcall (lua_State *L, CallInfo *ci);
// inline int luaD_pretailcall (lua_State *L, CallInfo *ci, StkId func,
//                                               int narg1, int delta) __attribute__((allways_inline));
// inline CallInfo *luaD_precall (lua_State *L, StkId func, int nResults) __attribute__((allways_inline));
LUAI_FUNC void luaD_call (lua_State *L, StkId func, int nResults);
LUAI_FUNC void luaD_callnoyield (lua_State *L, StkId func, int nResults);
LUAI_FUNC StkId luaD_tryfuncTM (lua_State *L, StkId func);
LUAI_FUNC int luaD_closeprotected (lua_State *L, ptrdiff_t level, int status);
LUAI_FUNC int luaD_pcall (lua_State *L, Pfunc func, void *u,
                                        ptrdiff_t oldtop, ptrdiff_t ef);
LUAI_FUNC void luaD_poscall (lua_State *L, CallInfo *ci, int nres);
LUAI_FUNC int luaD_reallocstack (lua_State *L, int newsize, int raiseerror);
LUAI_FUNC int luaD_growstack (lua_State *L, int n, int raiseerror);
LUAI_FUNC void luaD_shrinkstack (lua_State *L);
LUAI_FUNC void luaD_inctop (lua_State *L);

LUAI_FUNC l_noret luaD_throw (lua_State *L, int errcode);
LUAI_FUNC int luaD_rawrunprotected (lua_State *L, Pfunc f, void *ud);




#define next_ci(L)  (L->ci->next ? L->ci->next : luaE_extendCI(L))


l_sinline CallInfo *prepCallInfo (lua_State *L, StkId func, int nret,
                                                int mask, StkId top) {
  CallInfo *ci = L->ci = next_ci(L);  /* new frame */
  ci->func.p = func;
  ci->nresults = nret;
  ci->callstatus = mask;
  ci->top.p = top;
  return ci;
}


/*
** precall for C functions
*/
l_sinline int precallC (lua_State *L, StkId func, int nresults,
                                            lua_CFunction f) {
  int n;  /* number of returns */
  CallInfo *ci;
  checkstackGCp(L, LUA_MINSTACK, func);  /* ensure minimum stack size */
  L->ci = ci = prepCallInfo(L, func, nresults, CIST_C,
                               L->top.p + LUA_MINSTACK);
  lua_assert(ci->top.p <= L->stack_last.p);
  if (l_unlikely(L->hookmask & LUA_MASKCALL)) {
    int narg = cast_int(L->top.p - func) - 1;
    luaD_hook(L, LUA_HOOKCALL, -1, 1, narg);
  }
  lua_unlock(L);
  n = (*f)(L);  /* do the actual call */
  lua_lock(L);
  api_checknelems(L, n);
  luaD_poscall(L, ci, n);
  return n;
}


/*
** Prepare a function for a tail call, building its call info on top
** of the current call info. 'narg1' is the number of arguments plus 1
** (so that it includes the function itself). Return the number of
** results, if it was a C function, or -1 for a Lua function.
*/
static inline int luaD_pretailcall (lua_State *L, CallInfo *ci, StkId func,
                                    int narg1, int delta) {
 retry:
  switch (ttypetag(s2v(func))) {
    case LUA_VCCL:  /* C closure */
      return precallC(L, func, LUA_MULTRET, clCvalue(s2v(func))->f);
    case LUA_VLCF:  /* light C function */
      return precallC(L, func, LUA_MULTRET, fvalue(s2v(func)));
    case LUA_VLCL: {  /* Lua function */
      Proto *p = clLvalue(s2v(func))->p;
      int fsize = p->maxstacksize;  /* frame size */
      int nfixparams = p->numparams;
      int i;
      checkstackGCp(L, fsize - delta, func);
      ci->func.p -= delta;  /* restore 'func' (if vararg) */
      for (i = 0; i < narg1; i++)  /* move down function and arguments */
        setobjs2s(L, ci->func.p + i, func + i);
      func = ci->func.p;  /* moved-down function */
      for (; narg1 <= nfixparams; narg1++)
        setnilvalue(s2v(func + narg1));  /* complete missing arguments */
      ci->top.p = func + 1 + fsize;  /* top for new function */
      lua_assert(ci->top.p <= L->stack_last.p);
      ci->u.l.savedpc = p->code;  /* starting point */
      ci->callstatus |= CIST_TAIL;
      L->top.p = func + narg1;  /* set top */
      return -1;
    }
    default: {  /* not a function */
      func = luaD_tryfuncTM(L, func);  /* try to get '__call' metamethod */
      /* return luaD_pretailcall(L, ci, func, narg1 + 1, delta); */
      narg1++;
      goto retry;  /* try again */
    }
  }
}


/*
** Prepares the call to a function (C or Lua). For C functions, also do
** the call. The function to be called is at '*func'.  The arguments
** are on the stack, right after the function.  Returns the CallInfo
** to be executed, if it was a Lua function. Otherwise (a C function)
** returns NULL, with all the results on the stack, starting at the
** original function position.
*/
static inline CallInfo *luaD_precall (lua_State *L, StkId func, int nresults) {
 retry:
  switch (ttypetag(s2v(func))) {
    case LUA_VCCL:  /* C closure */
      precallC(L, func, nresults, clCvalue(s2v(func))->f);
      return NULL;
    case LUA_VLCF:  /* light C function */
      precallC(L, func, nresults, fvalue(s2v(func)));
      return NULL;
    case LUA_VLCL: {  /* Lua function */
      CallInfo *ci;
      Proto *p = clLvalue(s2v(func))->p;
      int narg = cast_int(L->top.p - func) - 1;  /* number of real arguments */
      int nfixparams = p->numparams;
      int fsize = p->maxstacksize;  /* frame size */
      checkstackGCp(L, fsize, func);
      L->ci = ci = prepCallInfo(L, func, nresults, 0, func + 1 + fsize);
      ci->u.l.savedpc = p->code;  /* starting point */
      for (; narg < nfixparams; narg++)
        setnilvalue(s2v(L->top.p++));  /* complete missing arguments */
      lua_assert(ci->top.p <= L->stack_last.p);
      return ci;
    }
    default: {  /* not a function */
      func = luaD_tryfuncTM(L, func);  /* try to get '__call' metamethod */
      /* return luaD_precall(L, func, nresults); */
      goto retry;  /* try again with metamethod */
    }
  }
}


#endif


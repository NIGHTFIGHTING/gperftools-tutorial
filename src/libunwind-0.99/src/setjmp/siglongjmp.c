/* libunwind - a platform-independent unwind library
   Copyright (C) 2003-2005 Hewlett-Packard Co
	Contributed by David Mosberger-Tang <davidm@hpl.hp.com>

This file is part of libunwind.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#define UNW_LOCAL_ONLY

#include <setjmp.h>

#include "libunwind_i.h"
#include "jmpbuf.h"
#include "setjmp_i.h"

void
siglongjmp (sigjmp_buf env, int val)
{
  unw_word_t *wp = (unw_word_t *) env;
  extern int _UI_siglongjmp_cont;
  extern int _UI_longjmp_cont;
  unw_context_t uc;
  unw_cursor_t c;
  unw_word_t sp;
  int *cont;

  if (unw_getcontext (&uc) < 0 || unw_init_local (&c, &uc) < 0)
    abort ();

  do
    {
      if (unw_get_reg (&c, UNW_REG_SP, &sp) < 0)
	abort ();
      if (sp != wp[JB_SP])
	continue;

      if (!bsp_match (&c, wp))
	continue;

      /* found the right frame: */

      /* default to resuming without restoring signal-mask */
      cont = &_UI_longjmp_cont;

      /* Order of evaluation is important here: if unw_resume()
	 restores signal mask, we must set it up appropriately, even
	 if wp[JB_MASK_SAVED] is FALSE.  */
      if (!resume_restores_sigmask (&c, wp) && wp[JB_MASK_SAVED])
	{
	  /* sigmask was saved */
	  if (UNW_NUM_EH_REGS < 4 || _NSIG >= 16 * sizeof (unw_word_t))
	    /* signal mask doesn't fit into EH arguments and we can't
	       put it on the stack without overwriting something
	       else... */
	    abort ();
	  else
	    if (unw_set_reg (&c, UNW_REG_EH + 2, wp[JB_MASK]) < 0
		|| (_NSIG > 8 * sizeof (unw_word_t)
		    && unw_set_reg (&c, UNW_REG_EH + 3, wp[JB_MASK + 1]) < 0))
	      abort ();
	  cont = &_UI_siglongjmp_cont;
	}

      if (unw_set_reg (&c, UNW_REG_EH + 0, wp[JB_RP]) < 0
	  || unw_set_reg (&c, UNW_REG_EH + 1, val) < 0
	  || unw_set_reg (&c, UNW_REG_IP, (unw_word_t) (uintptr_t) cont))
	abort ();

      unw_resume (&c);

      abort ();
    }
  while (unw_step (&c) >= 0);

  abort ();
}

/* ############# NEW */

/* The kernel call implemented in this file:
 *   m_type:	SYS_UNLOCKPRIORITY
 *
 * The parameters for this kernel call are:
 *    m1_i1:	child's endpoint number
 */

#include "../system.h"
#include <minix/endpoint.h>

/*===========================================================================*
 *				do_unlockpriority                              *
 *===========================================================================*/
PUBLIC int do_unlockpriority(m_ptr)
register message *m_ptr;	/* pointer to request message */
{
  register struct proc *childproc; proc_nr_t childproc_nr;
  int child_endnr = m_ptr->m1_i1;

  if (child_endnr == SELF)
	child_endnr = m_ptr->m_source;
  if (!isokendpt(child_endnr, &childproc_nr))
          return(-1);

  childproc = proc_addr(childproc_nr);
  if (childproc->p_lock == 0)
          return(-1);
  if (childproc->p_rts_flags == 0)
          lock_dequeue(childproc);
  childproc->p_priority = childproc->p_lock - 1;
  childproc->p_lock = 0;
  if (childproc->p_rts_flags == 0)
          lock_enqueue(childproc);
  kprintf("do_unlockpriority kernel: unlock priority of process '%s' (p_nr %d) to %d.\n",
                  childproc->p_name, childproc->p_nr, childproc->p_priority);
  return(0);
}

/* ############# */

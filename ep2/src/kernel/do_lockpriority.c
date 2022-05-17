/* ############# NEW */

/* The kernel call implemented in this file:
 *   m_type:	SYS_LOCKPRIORITY
 *
 * The parameters for this kernel call are:
 *    m1_i1:	child's endpoint number
 *    m1_i2:	new priority
 */

#include "../system.h"
#include <minix/endpoint.h>

/*===========================================================================*
 *				do_lockpriority                              *
 *===========================================================================*/
PUBLIC int do_lockpriority(m_ptr)
register message *m_ptr;	/* pointer to request message */
{
  register struct proc *childproc; proc_nr_t childproc_nr;
  int child_endnr = m_ptr->m1_i1;
  int priority = m_ptr->m1_i2;

  if (child_endnr == SELF)
	child_endnr = m_ptr->m_source;
  if (!isokendpt(child_endnr, &childproc_nr))
          return(-2);
  if (priority < MAX_USER_Q || priority > MIN_USER_Q)
          return(-1);

  childproc = proc_addr(childproc_nr);
  if (childproc->p_lock == 0)
          childproc->p_lock = childproc->p_priority + 1;
  if (childproc->p_rts_flags == 0)
          lock_dequeue(childproc);
  childproc->p_priority = priority;
  if (childproc->p_rts_flags == 0)
          lock_enqueue(childproc);
  kprintf("do_lockpriority kernel: lock priority of process '%s' (p_nr %d, old priority %d) to %d.\n",
                  childproc->p_name, childproc->p_nr, childproc->p_lock - 1, childproc->p_priority);
  return(priority);
}

/* ############# */

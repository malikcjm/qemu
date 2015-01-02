#ifndef QEMU_H
#define QEMU_H

#include "cpu.h"
#include "exec/cpu_ldst.h"

void cpu_list_lock(void);
void cpu_list_unlock(void);

#include <pthread.h>

#define VERIFY_READ 0
#define VERIFY_WRITE 1 /* implies read access */

static inline void *lock_user(int type, abi_ulong guest_addr, long len, int copy)
{
	return NULL;
}


static inline void unlock_user(void *host_ptr, abi_ulong guest_addr,
                               long len)
{
}

void mmap_lock(void);
void mmap_unlock(void);

#endif /* QEMU_H */

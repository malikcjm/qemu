#include "qemu.h"
#include "qemu-common.h"
#include "cpu.h"
#include "tcg.h"

#include "qemu/timer.h"
#include "qemu/envlist.h"

static const char *cpu_model;
int singlestep;
envlist_t *envlist;

static const char *interp_prefix = CONFIG_QEMU_INTERP_PREFIX;

#if defined(CONFIG_USE_GUEST_BASE)
unsigned long guest_base;
#if (TARGET_LONG_BITS == 32) && (HOST_LONG_BITS == 64)
/*
 * When running 32-on-64 we should make sure we can fit all of the possible
 * guest address space into a contiguous chunk of virtual host memory.
 *
 * This way we will never overlap with our own libraries or binaries or stack
 * or anything else that QEMU maps.
 */
# ifdef TARGET_MIPS
/* MIPS only supports 31 bits of virtual address space for user space */
unsigned long reserved_va = 0x77000000;
# else
unsigned long reserved_va = 0xf7000000;
# endif
#else
unsigned long reserved_va;
#endif
#endif

#if defined(TARGET_I386)
int cpu_get_pic_interrupt(CPUX86State *env)
{
    return -1;
}
#endif


uint64_t cpu_get_tsc(CPUX86State *env)
{
    return cpu_get_real_ticks();
}

static pthread_mutex_t cpu_list_mutex = PTHREAD_MUTEX_INITIALIZER;

void cpu_list_lock(void)
{
    pthread_mutex_lock(&cpu_list_mutex);
}

void cpu_list_unlock(void)
{
    pthread_mutex_unlock(&cpu_list_mutex);
}

int main(int argc, char **argv, char **envp)
{
    CPUArchState *env;
    CPUState *cpu;
    char **wrk;

    module_call_init(MODULE_INIT_QOM);

    if ((envlist = envlist_create()) == NULL) {
        (void) fprintf(stderr, "Unable to allocate envlist\n");
        exit(1);
    }

    /* add current environment into the list */
    for (wrk = environ; *wrk != NULL; wrk++) {
        (void) envlist_setenv(envlist, *wrk);
    }

#if defined(cpudef_setup)
    cpudef_setup(); /* parse cpu definitions in target config file (TBD) */
#endif

    srand(time(NULL));

    /* Scan interp_prefix dir for replacement files. */
    init_paths(interp_prefix);

	cpu_model = "qemu32";

    tcg_exec_init(0);
    cpu_exec_init_all();
    /* NOTE: we need to init the CPU at this stage to get
       qemu_host_page_size */
    env = cpu_init(cpu_model);
    if (!env) {
        fprintf(stderr, "Unable to find CPU definition\n");
        exit(1);
    }
    cpu = ENV_GET_CPU(env);
    cpu_reset(cpu);

	return 0;
}

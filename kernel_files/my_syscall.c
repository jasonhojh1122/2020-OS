#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

SYSCALL_DEFINE5(my_printk_time, long, pid, int, a1, long, b1, int, a2, long, b2){
        printk("[Project1] %ld %d.%ld %d.%ld \n", pid, a1, b1, a2, b2);
        return 0;
}

// functions below are for testing purpose
SYSCALL_DEFINE2(my_get_time, int __user *, a, int __user *, b){
        *a = 1;
        *b = 2;
        return 0;
}

SYSCALL_DEFINE2(my_add, int, a, int, b)
{
        printk("[TEST] my_add is invoked! Result: %d", a + b);
        return a + b;
}


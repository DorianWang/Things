
#include <linux/module.h>
#include <linux/kernel.h>



int initialization(void){
   printk(KERN_INFO "Hello World\n");
   return 0;
}

void cleanup(void){
   printk(KERN_INFO "Bye-bye World!\n");
}


module_init(initialization);
module_exit(cleanup);

// In makefile:
// obj-m += hello.o
// all:
//    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
// clean:
//    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

// To run:
// sudo insmod mymod.ko
// lsmod
// sudo rmmod mymod.ko
// modinfo mymod.ko







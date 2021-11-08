
#include <linux/module.h>
#include <linux/kernel.h>

static struct nf_hook_ops hook1, hook2;


unsigned int blockUDP(void* priv, struct sk_buff* skb, const struct nf_hook_state* state){
   struct iphdr* iph;
   struct udphdr* udph;
   u32 ip_addr;
   char ip[16] = "8.8.8.8";

   // Convert IPv4 address to 32-bit integer
   in4pton(ip, -1, (u8*)&ip_addr, '\0', NULL);

   iph = ip_hdr(skb);
   if (iph->protocol == IPPROTO_UDP){
      udph = udp_hdr(skb);
      if (up->daddr == ipaddr && ntohs(udph->dest) == 53){
         printk(KERN_DEBUG "****Dropping %pI4 (UDP), port %d\n", &(iph->daddr), port);
         return NF_DROP;
      }
   }
   return NF_ACCEPT;
}




int registerFilter(void){
   printk(KERN_INFO "Registering filters.\n");

   // Hook 1
   hook1.hook = printInfo;
   hook1.hooknum = NF_INET_LOCAL_IN;
   hook1.pf = PF_INET;
   hook1.priority = NF_IP_PRI_FIRST;
   nf_register_net_hook(&init_net, &hook1);

   // Hook 2
   hook1.hook = blockUDP;
   hook1.hooknum = NF_INET_POST_ROUTING;
   hook1.pf = PF_INET;
   hook1.priority = NF_IP_PRI_FIRST;
   nf_register_net_hook(&init_net, &hook2);

   return 0;
}

void removeFilter(void){
   printk(KERN_INFO "The filters are being removed.\n");
   nf_unregister_net_hook(&init_net, &hook1);
   nf_unregister_net_hook(&init_net, &hook2);
}


module_init(registerFilter);
module_exit(removeFilter);

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







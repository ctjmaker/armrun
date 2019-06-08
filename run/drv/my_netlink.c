#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <net/sock.h>
#include <linux/netlink.h>

#include "ker.h"

#define NETLINK_TEST	30
#define MSG_LEN			125
#define USER_PORT		100

struct sock *nlsk = NULL;
extern struct net init_net;

int send_usrmsg(char *pbuf,uint16_t len){
	struct sk_buff *nl_skb;
	struct nlmsghdr *nlh;
	int ret;
	
	//创建sk_buff空间
	nl_skb = nlmsg_new(len,GFP_ATOMIC);
	if(!nl_skb){
		printk(KERN_ERR"netlink alloc failed\n");
		return -1;
	}

	//设置netlink消息头部
	nlh = nlmsg_put(nl_skb,0,0,NETLINK_TEST,len,0);
	if(NULL == nlh){
		printk(KERN_ERR"nlmsg_put failed\n");
		nlmsg_free(nl_skb);
		return -1;
	}

	//拷贝数据发送
	memcpy(nlmsg_data(nlh),pbuf,len);
	ret = netlink_unicast(nlsk,nl_skb,USER_PORT,MSG_DONTWAIT);
	return ret;
}

static void netlink_rcv_msg(struct sk_buff *skb){
	struct nlmsghdr *nlh = NULL;
	char *umsg = NULL;
	char *kmsg = "--------------";

	if(skb->len >= nlmsg_total_size(0)){
		nlh = nlmsg_hdr(skb);
		printk(KERN_ERR"nlmsg_len : %d\n",nlh->nlmsg_len);
		printk(KERN_ERR"nlmsg_type : %d\n",nlh->nlmsg_type);
		printk(KERN_ERR"nlmsg_flags : %d\n",nlh->nlmsg_flags);
		printk(KERN_ERR"nlmsg_seq : %d\n",nlh->nlmsg_seq);
		printk(KERN_ERR"nlmsg_pid : %d\n",nlh->nlmsg_pid);
		umsg = NLMSG_DATA(nlh);
		if(umsg){
			printk(KERN_ERR"ker recv from user: %s\n",umsg);
			send_usrmsg(kmsg,strlen(kmsg));
		}
	}
}

/*
ker version > 3.6.0
struct netlink_kernel_cfg cfg = {
	.input = netlink_rcv_msg,
};
*/

int my_netlink_init(void){
	//ker > 3.6.0
	//nlsk = (struct sock *)netlink_kernel_create(&init_net,NETLINK_TEST,&cfg);
	nlsk = netlink_kernel_create(&init_net,NETLINK_TEST,0, netlink_rcv_msg, NULL, THIS_MODULE);
	if(NULL == nlsk){
		printk(KERN_ERR"netlink_kernel_create failed\n");
		return -1;
	}

	printk(KERN_ERR"netlink_kernel_create\n");
	return 0;
}

void my_netlink_exit(void){
	if(nlsk){
		netlink_kernel_release(nlsk);
		nlsk = NULL;
	}
	printk(KERN_ERR"my_netlink_exit\n");
	return;
}


/*
===============================================================================
Driver Name		:		testmodv2
Author			:		GREG C
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include "testmodv2.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GREG C");

#define MACRO(x) x


static int __init testmodv2_init(void) {
	printk(KERN_INFO "Hello, World! My name gregs\n");
	return 0;
}

static void __exit testmodv2_exit(void) {
	/* TODO Auto-generated Function Stub */

	printk(KERN_INFO "Goodbye, World! -- Greg\n");

}

module_init(testmodv2_init);
module_exit(testmodv2_exit);


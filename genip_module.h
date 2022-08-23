#ifndef GENIP_MODULE_H
#define GENIP_MODULE_H

// make the IOW macros work *properly* without including linux/x.h in userspace
// this file can be included in either kernel or userspace code
#ifdef __KERNEL__
#include <linux/ioctl.h>
#include <linux/types.h>
#else
#include <stdint.h>
#include <sys/ioctl.h>
#endif //__KERNEL__

/*
 * IOCTL configuration & commands
 */

// ioctl type; this doesnt really matter. Apperantly 't' is used rarely
#define GENIP_IOCTL_TYPE 't'
//#define GENIP_IOCTL_REG_PREFIX	  (0x80)
// Copy Settings from device tree to userspace
#define GENIP_IOCTL_NR_SETTINGS (0x01)
// write a register
#define GENIP_IOCTL_NR_REG_WRITE (0x02)
// read a register
#define GENIP_IOCTL_NR_REG_READ (0x03)

// argument = pointer to genip_reg_access
#define GENIP_IOCTL_W (_IOW(GENIP_IOCTL_TYPE, GENIP_IOCTL_NR_REG_WRITE, struct genip_reg_access))
// argument = pointer to genip_reg_access
#define GENIP_IOCTL_R (_IOWR(GENIP_IOCTL_TYPE, GENIP_IOCTL_NR_REG_READ, struct genip_reg_access))
// argument = pointer to a genip_settings to write the information to
#define GENIP_IOCTL_GET_SETTINGS (_IOR(GENIP_IOCTL_TYPE, GENIP_IOCTL_NR_SETTINGS, struct genip_settings))

/*
 * char device config & device tree matching
 */

#define GENIP_DRIVER_NAME "tes-ipcore" //basic driver name
#define GENIP_MAX_DEVICES 15 //number of maximum supported IPs (total)
#define GENIP_DEVCLASS_NAME "tes-ipcore-class" //device class name
#define GENIP_CHRDEV_NAME "tes-ipcores" //chrdev region name

// struct to hold data specific to each IP that is compatible with this driver
struct genip_platform_data {
	// The name of the device created in /dev/
	const char *fs_dev_name;
	// the id of the version register in hardware
	const uint32_t version_reg;
	// the expected value of that register
	const uint32_t version_reg_expected;
	// mask for the relevant bits of the version register
	const uint32_t version_reg_mask;
	// the id of the IRQ status register
	const uint32_t irq_status_reg;
	// the name of the irq for that device
	const char *irq_name;
};

#define GENIP_COMPAT_STR_CDC "tes,cdc-2.1"
const static struct genip_platform_data genip_cdc_pdata = {
	.fs_dev_name = "cdc",
	.irq_name = "cdc_irq",
	.version_reg = 0,
	.version_reg_mask = 0xffffff00,
	.version_reg_expected = 0x00040000,
	.irq_status_reg = 0};//TODO IRQ status reg

#define GENIP_COMPAT_STR_DHD "tes,dhd-1.0"
const static struct genip_platform_data genip_dhd_pdata = {
	.fs_dev_name = "dhd",
	.irq_name = "dhd_irq",
	.version_reg = 0,
	/* todo: reg mask and expected value */
	.irq_status_reg = 0}; //TODO IRQ status reg

#define GENIP_COMPAT_STR_WARP "tes,warp-1.0"
const static struct genip_platform_data genip_warp_pdata = {
	.fs_dev_name = "warp",
	.irq_name = "warp_irq",
	.version_reg = 0x0,
	.irq_status_reg = 0x11};

#define GENIP_COMPAT_STR_D2D "tes,d2d-1.0"
const static struct genip_platform_data genip_d2d_pdata = {
	.fs_dev_name = "d2d",
	.irq_name = "d2d_irq",
	.version_reg = 0,
	/* todo: reg mask and expected value */
	.irq_status_reg = 0}; //TODO IRQ status reg

// physical resource information
// this information is supplied by the device tree
// this struct is used to send the data to userspace
struct genip_settings {
	size_t base_phys; // start address of the registers
	size_t span;	  // size of the register area
};

/* This struct is used for reading/writing registers.
   Use u64 instead of pointer to stay compatible with 32/64 bit systems. */
struct genip_reg_access {
	__u64 offset;         /* in, register ID to read from / write to */
	__u32 value;          /* in/out, register value read or written */
};

#endif // GENIP_MODULE_H
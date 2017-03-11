/**
 * @file    main.c
 * @author  Robert Rouquette
 * @date    10 March 2017
 * @version 0.1
 * @brief   Driver for custom SDR ADC with parallel interface (Charecter Device)
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "sdradc0"
#define CLASS_NAME "sdradc"
#define SAMPLE_BUFFER_LENGTH (51200000) // 1 second at 51.2 MS/s (integral number of 4 KiB pages as well)
#define SAMPLE_BUFFER_SIZE (51200000) // buffer size in bytes

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert Rouquette");
MODULE_DESCRIPTION("RPi CM3 SDR ADC 51.2 MS/s");
MODULE_VERSION("0.1");

// circular sample buffer
static char buffSamples[SAMPLE_BUFFER_LENGTH];
static unsigned long buffer_start;
static unsigned long buffer_end;

static unsigned int epoch_offset;

// device registration
static int majorNumber;
static struct class *adcClass  = NULL;
static struct device *adcDevice = NULL;

// character driver prototypes
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
static int     dev_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
//static int     dev_mmap(struct file *, struct vm_area_struct *);

// characer driver definition
static struct file_operations fops =
{
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .ioctl = dev_ioctl,
    .release = dev_release//,
    //.mmap = dev_mmap
};

// LKM init function
static int __init rpiSdrAdc_init(void) {
   printk(KERN_INFO "SDR_ADC: Configuring Parallel Interface for 51.2 MS/s ADC\n");
   
   // initialze buffer start/end addresses
   buffer_start = (unsigned long)buffSamples;
   buffer_end = buffer_start + SAMPLE_BUFFER_SIZE;
   
   return 0;
}

// @brief LKM cleanup function
static void __exit rpiSdrAdc_exit(void) {
   printk(KERN_INFO "SDR_ADC: Unloading Parallel Interface for 51.2 MS/s ADC\n");
}

// ioctl interface is used to report buffer alignment with PPS signal
int dev_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {
    int i;
    char *temp;
    char ch;

    /*
     * Switch according to the ioctl called
     */
    switch (ioctl_num) {
        case IOCTL_SET_MSG:
            /*
             * Receive a pointer to a message (in user space) and set that
             * to be the device's message.  Get the parameter given to
             * ioctl by the process.
             */
            temp = (char *)ioctl_param;

            /*
             * Find the length of the message
             */
            get_user(ch, temp);
            for (i = 0; ch && i < BUF_LEN; i++, temp++)
                get_user(ch, temp);

            device_write(file, (char *)ioctl_param, i, 0);
            break;

        case IOCTL_GET_MSG:
            /*
             * Give the current message to the calling process -
             * the parameter we got is a pointer, fill it.
             */
            i = device_read(file, (char *)ioctl_param, 99, 0);

            /*
             * Put a zero at the end of the buffer, so it will be
             * properly terminated
             */
            put_user('\0', (char *)ioctl_param + i);
            break;

        case IOCTL_GET_NTH_BYTE:
            /*
             * This ioctl is both input (ioctl_param) and
             * output (the return value of this function)
             */
            return Message[ioctl_param];
            break;
    }

    return SUCCESS;
}

/*
void simple_vma_open(struct vm_area_struct *vma) {
    //MOD_INC_USE_COUNT;
}

void simple_vma_close(struct vm_area_struct *vma) {
    //MOD_DEC_USE_COUNT;
}

struct page *simple_vma_nopage(struct vm_area_struct *vma, unsigned long address, int write_access) {
    struct page *pageptr;
    unsigned long physaddr = address - vma->vm_start + VMA_OFFSET(vma);
    
    // verify physical address
    if(physaddr < buffer_start || physaddr >= buffer_end )
        return NOPAGE_SIGBUS; // send a SIGBUS
    
    pageptr = virt_to_page(__va(physaddr));
    get_page(pageptr);
    return pageptr;
}

static struct vm_operations_struct simple_remap_vm_ops = {
        .open = simple_vma_open,
        .close = simple_vma_close,
        .nopage = simple_vma_nopage
};

int dev_mmap(struct file *filp, struct vm_area_struct *vma) {
    unsigned long off = vma->vm_pgoff << PAGE_SHIFT;
    unsigned long physical = buffer_start + off;
    unsigned long vsize = vma->vm_end - vma->vm_start;
    unsigned long psize = SAMPLE_BUFFER_SIZE - off;

    if (vsize > psize) return -EINVAL; // spans too high

    vma->vm_flags |= VM_RESERVED;
    if(remap_page_range(vma->vm_start, physical, vsize, vma->vm_page_prot))
        return -EAGAIN;

    vma->vm_ops = &simple_remap_vm_ops;
    simple_vma_open(vma);
    return 0;
}
*/

// register module init/exit functions
module_init(rpiSdrAdc_init);
module_exit(rpiSdrAdc_exit);

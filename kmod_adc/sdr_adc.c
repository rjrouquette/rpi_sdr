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
#include <linux/gpio.h>
//#include <linux/mm.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "sdradc0"
#define CLASS_NAME "sdradc"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert Rouquette");
MODULE_DESCRIPTION("RPi CM3 SDR ADC 51.2 MS/s");
MODULE_VERSION("0.1");

sdr_adc_t adc_data;

// device registration
static int majorNumber;
static struct class *adcClass  = NULL;
static struct device *adcDevice = NULL;

// character driver prototypes
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
//static int     dev_mmap(struct file *, struct vm_area_struct *);

// character driver definition
static struct file_operations fops =
{
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
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

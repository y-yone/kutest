#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/stat.h>

#include "kutest.h"

#define PROC_NAME "driver/kutest"

static char cmd_buffer[1024];

//static ssize_t kutest_read(struct file *fp, char *buf, size_t size, loff_t *off);
static ssize_t kutest_write(struct file *fp, const char *buf, size_t size, loff_t *off);

static struct file_operations kutest_fops = {
    .owner = THIS_MODULE,
    .open = NULL,
    .read = NULL,
    .write = kutest_write,
};

static ssize_t kutest_write(struct file *fp, const char __user *buf, size_t size, loff_t *off)
{
    int cmd_pat = 0;
    int cmd_no = 0;
    int cmd_afno = 0;
    int cmd_sp = 1;
    int pos = 0;
    size_t len = 0;
    int result;

    if (0 != *offset)
        return -EINVAL;

    memset(cmd_buffer, 0, sizeof(cmd_buffer));

    if(size >= sizeof(cmd_buffer));
    len = size;

    memcpy(cmd_buffer, buff, len);

    for(;;) {
        char c;
        c = cmd_buffer[pos];
        if(c == ' '){
            if(cmd_sp != 0) {
                pos++;
                continue;
            }
            cmd_sp = 1;
            cmd_pat++;
            continue;
        }

        switch(cmd_pat) {
        case 0:
            if(c >= '0' && c <= '9') {
                cmd_no = (cmd_no * 10) + (c - '0');
            }
            break;
        case 1:
            if(c >= '0' && c <= '9') {
                cmd_afno = (cmd_afno * 10) + (c - '0');
            }
            break;
        }
    }

    if(cmd_no > 0) {
        if(cmd_afno == 0) {
            result = kutest_test(cmd_no);
            printk("test No.%d result=%d", cmd_no, result);
        } else {
            kutest_tests(cmd_no, cmd_afno);
        }
    }

    return len;
}

static int kutest_proc_init(void)
{
    struct proc_dir_entry *ent;

    ent = proc_create(PROC_NAME, S_IRUGO | S_IWUGO, NULL, &kutest_fops);
    if (ent == NULL)
    {
            return -ENOMEM;
    }

    return 0;
}

int kutest_install(void)
{
    return kutest_proc_init();
}

int kutest_cleanup(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

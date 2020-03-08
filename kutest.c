#include "kutest.h"

#define KUTEST_METHOD_MAX 1024

typedef struct _kutest_mng {
    KUTEST_METHOD methods[KUTEST_METHOD_MAX];
} kutest_mng;

static kutest_mng kutest;

int kutest_init(void)
{
    memset(&kutest, 0, sizeof(kutest_mng));
    
    return 0;
}

int kuteset_register(int no, KUTEST_METHOD method)
{
    if(no >= KUTEST_METHOD_MAX) {
        return -EINVAL;
    }

    if(kutest.methods[no] != NULL) {
        return -EINVAL;
    }

    kutest.methods[no] = method;
    return 0;
}

int kutest_test(int no)
{
    if(kutest.methods[no] == NULL) {
        return -EINVAL;
    }

    kutest.methods[no]();
    return 0;
}

int kutest_tests(int low, int high)
{
    int result;
    int no;

    if(high >= KUTEST_METHOD_MAX) {
        return -EINVAL;
    }

    for( no = low; no <= high; no++) {
        result = kutest.methods[no]();
        printk("test No.%d result=%d", no, result);
    }

    return 0;
}

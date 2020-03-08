
typedef int (*KUTEST_METHOD)(void);


kuteset_register(int no, KUTEST_METHOD method);
int kutest_test(int no);
int kutest_tests(int low, int high);

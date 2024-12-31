#include "unity/fixture/unity_fixture.h"

static void run_all_tests(void);

int main(int argc, const char **argv)
{
   return UnityMain(argc, argv, run_all_tests);
}

static void run_all_tests(void)
{
   RUN_TEST_GROUP(sys_comm_master_req);
   RUN_TEST_GROUP(sys_comm_slave_resp);
   RUN_TEST_GROUP(sys_comm_send_recv);
}
set(CMAKE_C_COMPILER gcc)
set(CMAKE_AR ar)
set(CMAKE_RANLIB ranlib)

set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SRC_PATH ../../../src)

set(INCLUDE_DIRS
	${SRC_PATH}
)

set(TEST_INCLUDE_DIRS
	..
	.
)

set(SRCS
	${SRC_PATH}/sys_comm/sys_comm.c
	${SRC_PATH}/sys_comm/sys_comm_send_recv.c
	${SRC_PATH}/events/events.c
)

set(TEST_NAME sys_comm)

set(TEST_SRCS
	${TEST_NAME}_test_main.c
	${TEST_NAME}_test_runner.c
	${TEST_NAME}_master_req_test.c
	${TEST_NAME}_slave_resp_test.c
	${TEST_NAME}_send_recv_test.c
	mock_get_data.c
)

set(GLOBAL_DEFINES
	-DDEVICE_ID=0xAA
)

add_executable(unit_test_${TEST_NAME} ${SRCS} ${TEST_SRCS})
add_test(${TEST_NAME} unit_test_${TEST_NAME})

target_include_directories(unit_test_${TEST_NAME} PRIVATE ${INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
target_compile_definitions(unit_test_${TEST_NAME} PRIVATE ${GLOBAL_DEFINES})
target_link_libraries(unit_test_${TEST_NAME} PRIVATE unity)
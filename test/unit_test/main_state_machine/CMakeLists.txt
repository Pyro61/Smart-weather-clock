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
	${SRC_PATH}/main_state_machine/state_mode_selection/state_mode_selection.c
	${SRC_PATH}/main_state_machine/state_set_time/state_set_time.c
	${SRC_PATH}/main_state_machine/state_set_alarm/state_set_alarm.c
)

set(TEST_NAME states)

set(TEST_SRCS
	${TEST_NAME}_test_main.c
	${TEST_NAME}_test_runner.c
	mode_selection_test.c
	set_time_test.c
	set_alarm_test.c
	display_mock.c
	time_mock.c
	alarm_mock.c
)

set(GLOBAL_DEFINES

)

add_executable(unit_test_${TEST_NAME} ${SRCS} ${TEST_SRCS})
add_test(${TEST_NAME} unit_test_${TEST_NAME})

target_include_directories(unit_test_${TEST_NAME} PRIVATE ${INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
target_compile_definitions(unit_test_${TEST_NAME} PRIVATE ${GLOBAL_DEFINES})
target_link_libraries(unit_test_${TEST_NAME} PRIVATE unity)
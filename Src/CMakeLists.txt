#set(CMAKE_TOOLCHAIN_FILE ../cmake/toolchain-arm-gcc.cmake)
include(../cmake/compiler_flags.cmake)


set(INCLUDE_DIRS
    #Drivers
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32G4xx/Include
)

set(C_SRCS
    #Cube generated files
    Core/Src/main.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c

    #Application logic files
    app_control/control.c
    events/events.c
    safe_state/safe_state.c
    sys_comm/sys_comm.c
    sys_comm/sys_comm_send_recv.c
    sys_comm/adapter_sys_comm.c
    weather_control/weather_sensors_control.c
    alarm/alarm.c
    main_state_machine/state_machine.c
    main_state_machine/state_mode_selection/state_mode_selection.c
    main_state_machine/state_weather_out/state_weather_out.c
    main_state_machine/state_weather_in/state_weather_in.c
    main_state_machine/state_set_time/state_set_time.c
    main_state_machine/state_set_alarm/state_set_alarm.c
    logic_init.c

    #External hw files
    hw/external_hw/external_hw_init.c
    hw/external_hw/LPS25HB/LPS25HB.c
    hw/external_hw/aht15/aht15.c
    hw/external_hw/HC05/HC05.c

    #MCU hw files
    hw/mcu_hw/mcu_hw_init.c
    hw/mcu_hw/fpu/fpu.c
    hw/mcu_hw/core_clock/core_clock.c
    hw/mcu_hw/gpio/gpio.c
    hw/mcu_hw/spi/spi.c
    hw/mcu_hw/tim/tim.c
    hw/mcu_hw/i2c/i2c.c
    hw/mcu_hw/uart/uart.c
)

set(ASM_SRCS
    #Startup file
    Core/Startup/startup_stm32g474retx.s
)

set(GLOBAL_DEFINES
    #Board name
    -DSTM32G474xx

    #Device id used by application
    -DDEVICE_ID=0xCA
    
    #Measurements delay in seconds
    -DMEASUREMENT_S_DELAY=30
)

set(LINKED_LIBS

)

link_directories(

)

include_directories(${INCLUDE_DIRS})
add_definitions(${GLOBAL_DEFINES})

# generate files
if(DEFINED DEBUG)
    add_arm_executable(${CMAKE_PROJECT_NAME}_debug ${C_SRCS} ${ASM_SRCS})
else()
    add_arm_executable(${CMAKE_PROJECT_NAME}_release ${C_SRCS} ${ASM_SRCS})
endif()
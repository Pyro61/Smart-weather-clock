#pragma once


/* XL - MSL, L - MIDDLE, H - MSB */

/* Register addresses */
/* Reference pressure resisters */
#define REG_ADDR_REF_P_XL       0x08
#define REG_ADDR_REF_P_L        0x09
#define REG_ADDR_REF_P_H        0x0A

/* Device ID register */
#define REG_ADDR_WHO_AM_I       0x0F

/* Resolution register */
#define REG_ADDR_RES_CONF       0x10

/* Control registers */
#define REG_ADDR_CTRL_1         0x20
#define REG_ADDR_CTRL_2         0x21
#define REG_ADDR_CTRL_3         0x22
#define REG_ADDR_CTRL_4         0x23

/* Interrupt registers */
#define REG_ADDR_IRQ_CFG        0x24
#define REG_ADDR_IRQ_SRC        0x25

/* Status register */
#define REG_ADDR_STATUS         0x27

/* Output value registers */
#define REG_ADDR_OUT_P_XL       0x28
#define REG_ADDR_OUT_P_L        0x29
#define REG_ADDR_OUT_P_H        0x2A
#define REG_ADDR_OUT_T_XL       0x2B
#define REG_ADDR_OUT_T_H        0x2C

/* FIFO configure registers */
#define REG_ADDR_FIFO_CTRL      0x2E
#define REG_ADDR_FIFO_STATUS    0x2F

/* Pressure threshold registers */
#define REG_ADDR_THS_P_XL       0x30
#define REG_ADDR_THS_P_H        0x31

/* Pressure offset registers */
#define REG_ADDR_RPDS_XL        0x39
#define REG_ADDR_RPDS_H         0x3A

/* Device ID */
#define LPS25HB_DEVICE_ID       0xBD

/* Temperature resolution */
#define RESOLUTION_T_8          (0 << 2)
#define RESOLUTION_T_16         (1 << 2)
#define RESOLUTION_T_32         (2 << 2)
#define RESOLUTION_T_64         (3 << 2)

/* Pressure resolution */
#define LPS_RESOLUTION_T_8      (0 << 0)
#define LPS_RESOLUTION_T_32     (1 << 0)
#define LPS_RESOLUTION_T_128    (2 << 0)
#define LPS_RESOLUTION_T_512    (3 << 0)

/* Control register 1 */
#define POWER_UP_DEVICE         (1 << 7)
#define IRQ_EN                  (1 << 3)
#define BLOCK_DATA_UPDATE       (1 << 2)
#define RESET_AUTOZERO          (1 << 1)
#define SPI_3WIRE               (1 << 0) /* 4 wire if negated */

/* Output data rate */
#define OUTPUT_RATE_ONE_SHOT    (0 << 4)
#define OUTPUT_1HZ              (1 << 4)
#define OUTPUT_RATE_7HZ         (2 << 4)
#define OUTPUT_RATE_12_5HZ      (3 << 4)
#define OUTPUT_RATE_25HZ        (4 << 4)

/* Control register 2 */
#define REBOOT                  (1 << 7)
#define FIFO_EN                 (1 << 6)
#define STOP_ON_FIFO_THRESHOLD  (1 << 5)
#define FIFO_MEAN_DECIMATE      (1 << 4)
#define I2C_DISABLE             (1 << 3) /* Enabled when negated (default value) */
#define SOFTWARE_RESET          (1 << 2)
#define AUTOZERO_EN             (1 << 1)
#define ONE_SHOT_EN             (1 << 0)

/* Control register 3 */
#define IRQ_ACTIVE_NH_L         (1 << 7) /* Active high if set to 0, otherwise active low */
#define PP_NOD_IRQ_PINS         (1 << 6) /* IRQ pins state: 1 - push-pull, 0 - open drain */
#define IRQ_SIG_DATA_READY      (0 << 0) /* IRQ signal: data ready to read */
#define IRQ_SIG_P_H             (1 << 0) /* IRQ signal: pressure high */
#define IRQ_SIG_P_L             (2 << 0) /* IRQ signal: pressure low */
#define IRQ_SIG_P_H_OR_L        (3 << 0) /* IRQ signal: pressure high or low */

/* Control register 4 */
#define FIFO_EMPTY_EN           (1 << 3)
#define FIFO_THRESHOLD_IRQ_EN   (1 << 2)
#define FIFO_OVERRUN_IRQ_EN     (1 << 1)
#define DATA_READY_EN           (1 << 0)

/* Interrupt configuration register */
#define LATCH_IRQ_EN            (1 << 2)
#define DIFF_P_L_IRQ_EN         (1 << 1) /* IRQ generation when measured differential pressure is lower than preset threshold */
#define DIFF_P_H_IRQ_EN         (1 << 0) /* IRQ generation when measured differential pressure is higher than preset threshold */

/* IRQ source register */
#define IRQ_ACTIVE              (1 << 2)
#define DIFF_P_L                (1 << 1)
#define DIFF_P_H                (1 << 0)

/* Status register */
#define P_DATA_OVERRUN          (1 << 5)
#define T_DATA_OVERRUN          (1 << 4)
#define P_DATA_READY            (1 << 1)
#define T_DATA_READY            (1 << 0)

/* FIFO control register */
#define FIFO_MODE_BYPASS        (0 << 5)
#define FIFO_MODE_FIFO          (1 << 5)
#define FIFO_MODE_STREAM        (2 << 5)
#define FIFO_MODE_STR_TO_FIFO   (3 << 5)
#define FIFO_MODE_BYP_TO_STR    (4 << 5)
#define FIFO_MODE_MEAN          (6 << 5)
#define FIFO_MODE_BYP_TO_FIFO   (7 << 5)

#define MEAN_SAMPLING_X2        (1 << 0)
#define MEAN_SAMPLING_X4        (3 << 0)
#define MEAN_SAMPLING_X8        (7 << 0)
#define MEAN_SAMPLING_X16       (15 << 0)
#define MEAN_SAMPLING_X32       (31 << 0)

/* FIFO status register */
#define FIFO_THRESHOLD_EQ_HR    (1 << 7)
#define FIFO_OVERRUN_YES        (1 << 6)
#define FIFO_EMPTY_YES          (1 << 5)
#define FIFO_STORED_FULL        (31 << 0)
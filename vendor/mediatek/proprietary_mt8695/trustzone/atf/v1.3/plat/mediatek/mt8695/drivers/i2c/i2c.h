/*
 * Copyright (c) 2018 MediaTek Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#define MTK_I2C_SOURCE_CLK 273000
#define MTK_I2C_CLK_DIV 4

#define MTK_I2C0_BASE (0x11007000)
#define MTK_I2C1_BASE (0x11008000)
#define MTK_I2C2_BASE (0x11009000)
#define MTK_I2C3_BASE (0x11010000)


#define MTK_I2C0_DMA (0x11000080)
#define MTK_I2C1_DMA (0x11000100)
#define MTK_I2C2_DMA (0x11000180)
#define MTK_I2C3_DMA (0x11000200)

#define AP_DMA_GLOBAL_SEC_E     (0x11000010)
#define MTK_AP_DMA_GL_GSEC_EN   (0x11000014)
#define MTK_I2C0_DMA_SEC_EN     (0x11000020)
#define MTK_I2C1_DMA_SEC_EN     (0x11000024)
#define MTK_I2C2_DMA_SEC_EN     (0x11000028)
#define MTK_I2C3_DMA_SEC_EN     (0x1100002C)

#define MTK_I2C0_GIC_IRQ 116
#define MTK_I2C1_GIC_IRQ 117
#define MTK_I2C2_GIC_IRQ 118
#define MTK_I2C3_GIC_IRQ 119

#define MTK_GPIO_I2C_BASE0 (0x10005670)
#define MTK_GPIO_I2C_BASE1 (0x100055b0)
#define MTK_GPIO_I2C_BASE2 (0x100055f0)
#define MTK_GPIO_I2C_BASE3 (0x10005640)

#define MTK_GPIO_SDA0 6
#define MTK_GPIO_SCL0 9
#define MTK_GPIO_SDA1 3
#define MTK_GPIO_SCL1 6
#define MTK_GPIO_SDA2 3
#define MTK_GPIO_SCL2 6
#define MTK_GPIO_SDA3 9
#define MTK_GPIO_SCL3 12

#define MTK_I2C_CLK_SET0 (0x10003008)
#define MTK_I2C_CLK_CLR0 (0x10003010)
#define MTK_I2C_CLK_STA0 (0x10003018)
#define MTK_I2C0_CLK_OFFSET (0x1 << 24)
#define MTK_I2C1_CLK_OFFSET (0x1 << 25)
#define MTK_I2C2_CLK_OFFSET (0x1 << 26)
#define MTK_I2C3_CLK_OFFSET (0x1 << 27)
#define MTK_APDMA_CLK_OFFSET (0x1 << 13)

#define I2C_CONTROL_RS                    (0x1 << 1)
#define I2C_CONTROL_DMA_EN                (0x1 << 2)
#define I2C_CONTROL_CLK_EXT_EN            (0x1 << 3)
#define I2C_CONTROL_DIR_CHANGE            (0x1 << 4)
#define I2C_CONTROL_ACKERR_DET_EN         (0x1 << 5)
#define I2C_CONTROL_TRANSFER_LEN_CHANGE   (0x1 << 6)
#define I2C_CONTROL_WRAPPER               (0x1 << 0)

#define I2C_RS_TRANSFER             (1 << 4)
#define I2C_HS_NACKERR              (1 << 2)
#define I2C_ACKERR                  (1 << 1)
#define I2C_TRANSAC_COMP            (1 << 0)
#define I2C_TRANSAC_START           (1 << 0)
#define I2C_RS_MUL_CNFG             (1 << 15)
#define I2C_RS_MUL_TRIG             (1 << 14)

#define I2C_DCM_DISABLE             0x0000
#define I2C_IO_CONFIG_OPEN_DRAIN    0x0003
#define I2C_IO_CONFIG_PUSH_PULL     0x0000
#define I2C_SOFT_RST                0x0001
#define I2C_FIFO_ADDR_CLR           0x0001
#define I2C_DELAY_LEN               0x0002
#define I2C_ST_START_CON            0x8001
#define I2C_FS_START_CON            0x1800
#define I2C_TIME_CLR_VALUE          0x0000
#define I2C_TIME_DEFAULT_VALUE      0x0003
#define I2C_WRRD_TRANAC_VALUE       0x0002
#define I2C_RD_TRANAC_VALUE         0x0001
#define I2C_DMA_CON_TX              0x0000
#define I2C_DMA_CON_RX              0x0001
#define I2C_DMA_START_EN            0x0001
#define I2C_DMA_INT_FLAG_NONE       0x0000
#define I2C_DMA_CLR_FLAG            0x0000
#define I2C_DMA_HARD_RST            0x0002

#define DMA_GSEC_ENABLE             0x0001
#define DMA_GSEC_DISABLE            0x0000
#define DMA_SEC_ENABLE              0x0001
#define DMA_SEC_DISABLE             0x0000

#define I2C_FIFO_SIZE             8
#define I2C_DEFAULT_CLK_DIV       2
#define MAX_ST_MODE_SPEED         100   /* khz */
#define MAX_FS_MODE_SPEED         400   /* khz */
#define MAX_HS_MODE_SPEED         3400  /* khz */
#define MAX_DMA_TRANS_SIZE        65532 /* max(65535) aligned to 4 bytes = 65532 */
#define MAX_DMA_TRANS_NUM         256
#define MAX_SAMPLE_CNT_DIV        8
#define MAX_STEP_CNT_DIV          64
#define MAX_HS_STEP_CNT_DIV       8
#define DMA_ADDRESS_HIGH          (0xC0000000)

#define I2C_HS_NACKERR            (1 << 2)
#define I2C_ACKERR                (1 << 1)
#define I2C_TRANSAC_COMP          (1 << 0)

#define I2C_OK         0
#define EAGAIN_I2C     11  /* try again */
#define EINVAL_I2C     22  /* invalid argument */
#define EOPNOTSUPP_I2C 95  /* operation not supported on transport endpoint */
#define ETIMEDOUT_I2C  110 /* connection timed out */
#define EACK_I2C  6        /* remote I/O error */

#define I2CTAG         "[I2C] "

#define DIV_ROUND_UP(x, y) (((x) + ((y) - 1)) / (y))

enum I2C_REGS_OFFSET {
	OFFSET_DATA_PORT            = 0x0,
	OFFSET_SLAVE_ADDR           = 0x04,
	OFFSET_INTR_MASK            = 0x08,
	OFFSET_INTR_STAT            = 0x0C,
	OFFSET_CONTROL              = 0x10,
	OFFSET_TRANSFER_LEN         = 0x14,
	OFFSET_TRANSAC_LEN          = 0x18,
	OFFSET_DELAY_LEN            = 0x1C,
	OFFSET_TIMING               = 0x20,
	OFFSET_START                = 0x24,
	OFFSET_EXT_CONF             = 0x28,
	OFFSET_FIFO_STAT            = 0x30,
	OFFSET_FIFO_THRESH          = 0x34,
	OFFSET_FIFO_ADDR_CLR        = 0x38,
	OFFSET_IO_CONFIG            = 0x40,
	OFFSET_RSV_DEBUG            = 0x44,
	OFFSET_HS                   = 0x48,
	OFFSET_SOFTRESET            = 0x50,
	OFFSET_DCM_EN               = 0x54,
	OFFSET_DEBUGSTAT            = 0x64,
	OFFSET_DEBUGCTRL            = 0x68,
	OFFSET_TRANSFER_LEN_AUX     = 0x6C,
	OFFSET_CLOCK_DIV            = 0x70,
	OFFSET_SCL_HL_RATIO         = 0x74,
	OFFSET_SCL_HS_HL_RATIO      = 0x78,
	OFFSET_SCL_MIS_COMP_POINT   = 0x7C,
	OFFSET_STA_STOP_AC_TIME     = 0x80,
	OFFSET_HS_STA_STOP_AC_TIME  = 0x84,
	OFFSET_DATA_TIME            = 0x88,
	OFFSET_TIME_OUT             = 0x8C,
};

enum DMA_REGS_OFFSET {
	OFFSET_INT_FLAG       = 0x0,
	OFFSET_INT_EN         = 0x04,
	OFFSET_EN             = 0x08,
	OFFSET_RST            = 0x0C,
	OFFSET_CON            = 0x18,
	OFFSET_TX_MEM_ADDR    = 0x1C,
	OFFSET_RX_MEM_ADDR    = 0x20,
	OFFSET_TX_LEN         = 0x24,
	OFFSET_RX_LEN         = 0x28,
};

enum I2C_SPEED_MODE {
	ST_MODE,
	FS_MODE,
	HS_MODE,
};

enum mt_trans_op {
	I2C_MASTER_NONE = 0,
	I2C_MASTER_WR = 1,
	I2C_MASTER_RD,
	I2C_MASTER_WRRD,
};

struct i2c_msg {
	uint16_t addr;                       /* slave address */
	uint16_t flags;
#define I2C_M_TEN           0x0010  /* this is a ten bit chip address */
#define I2C_M_RD            0x0001  /* read data, from slave to master */
#define I2C_M_STOP          0x8000  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NOSTART       0x4000  /* if I2C_FUNC_NOSTART */
#define I2C_M_REV_DIR_ADDR  0x2000  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK    0x1000  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NO_RD_ACK     0x0800  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_RECV_LEN      0x0400  /* length will be first received byte */
	uint16_t len;                        /* msg length */
	uint8_t *buf;                        /* pointer to msg data */
};

struct mt_i2c {
	unsigned int   dma_en;
	unsigned int   poll_en;
	unsigned int   pushpull;                /* open drain */
	unsigned int   filter_msg;              /* filter msg error log */
	unsigned int   dcm;
	unsigned int   auto_restart;
	unsigned int   aux_len_reg;
	unsigned int   hscode;
	unsigned int   msg_complete;
	uint8_t     addr;                    /* slave device 7bit addr */
	uint8_t     mode;                    /* ST/FS/HS mode */
	uint16_t    id;
	uint16_t    irqnr;                   /* i2c interrupt number */
	uint16_t    irq_stat;                /* i2c interrupt status */
	uint16_t    delay_len;               /* num of half pulse between transfers */
	uint16_t    timing_reg;
	uint16_t    high_speed_reg;
	uint16_t    control_reg;
	uint16_t    ext_time;
	uint16_t    scl_ratio;
	uint16_t    scl_hs_ratio;
	uint16_t    scl_mis_comp;
	uint16_t    sta_stop_time;
	uint16_t    hs_sta_stop_time;
	uint16_t    data_time;
	uint32_t    clk;                     /* source clock khz */
	uint32_t    clk_src_div;
	void   *base;                    /* i2c base addr */
	void   *pdmabase;
	uint32_t    speed;                   /* khz */
	uint8_t     *tx_buff;
	uint8_t     *rx_buff;
	enum   mt_trans_op op;
};

/* external API */
int i2c_cust_read(uint16_t bus_num, uint8_t device_addr, uint32_t speed,
					uint8_t *buffer, uint32_t len);
int i2c_cust_write(uint16_t bus_num, uint8_t device_addr, uint32_t speed,
					uint8_t *buffer, uint32_t len);
int i2c_cust_write_read(uint16_t bus_num, uint8_t device_addr, uint32_t speed,
			uint8_t *write_buffer, uint8_t *read_buffer,
			uint32_t write_len, uint32_t read_len);


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
#include <arch_helpers.h>
#include <debug.h>
#include <mmio.h>
#include "i2c.h"

struct mt_i2c *i2c_g;

static inline void i2c_writel(struct mt_i2c *i2c, uint8_t offset,
			      uint32_t value)
{
	mmio_write_32((uintptr_t)(i2c->base) + offset, value);
}

static inline uint32_t i2c_readl(struct mt_i2c *i2c, uint8_t offset)
{
	return mmio_read_32((uintptr_t)(i2c->base) + offset);
}

static int mtk_i2c_clock_enable(struct mt_i2c *i2c)
{
	switch (i2c->id) {
	case 0:
		mmio_write_32((uintptr_t)MTK_I2C_CLK_CLR0,
				      MTK_I2C0_CLK_OFFSET);
		break;
	case 1:
		mmio_write_32((uintptr_t)MTK_I2C_CLK_CLR0,
				      MTK_I2C1_CLK_OFFSET);
		break;
	case 2:
		mmio_write_32((uintptr_t)MTK_I2C_CLK_CLR0,
				      MTK_I2C2_CLK_OFFSET);
		break;
	case 3:
		mmio_write_32((uintptr_t)MTK_I2C_CLK_CLR0,
				      MTK_I2C3_CLK_OFFSET);
		break;
	default:
#ifdef I2C_DEBUG
		ERROR(I2CTAG "clk enable, invalid i2c->id = %d\n",
			      i2c->id);
#endif
		return -EINVAL_I2C;
	}

	mmio_write_32((uintptr_t)MTK_I2C_CLK_CLR0, MTK_APDMA_CLK_OFFSET);

	return 0;
}

static int mtk_i2c_clock_disable(struct mt_i2c *i2c)
{
	switch (i2c->id) {
	case 0:
		mmio_write_32((uintptr_t)MTK_I2C_CLK_SET0,
				      MTK_I2C0_CLK_OFFSET);
		break;
	case 1:
		mmio_write_32((uintptr_t)MTK_I2C_CLK_SET0,
				      MTK_I2C1_CLK_OFFSET);
		break;
	case 2:
		mmio_write_32((uintptr_t)MTK_I2C_CLK_SET0,
				      MTK_I2C2_CLK_OFFSET);
		break;
	case 3:
		mmio_write_32((uintptr_t)MTK_I2C_CLK_SET0,
				      MTK_I2C3_CLK_OFFSET);
		break;
	default:
#ifdef I2C_DEBUG
		ERROR(I2CTAG "clk disable, invalid i2c->id=%d\n",
			      i2c->id);
#endif
		return -EINVAL_I2C;
	}

	mmio_write_32((uintptr_t)MTK_I2C_CLK_SET0, MTK_APDMA_CLK_OFFSET);

	return 0;
}

static void mtk_i2c_gpio_set(struct mt_i2c *i2c)
{
	uint32_t gpio_reg;

	switch (i2c->id) {
	case 0:
		gpio_reg = (mmio_read_32((uintptr_t)MTK_GPIO_I2C_BASE0)
				   & (~(0x7 << MTK_GPIO_SDA0)))
				   | (0x1 << MTK_GPIO_SDA0);
		mmio_write_32((uintptr_t)MTK_GPIO_I2C_BASE0, gpio_reg);
		gpio_reg = (mmio_read_32((uintptr_t)MTK_GPIO_I2C_BASE0)
				   & (~(0x7 << MTK_GPIO_SCL0)))
				   | (0x1 << MTK_GPIO_SCL0);
		mmio_write_32((uintptr_t)MTK_GPIO_I2C_BASE0, gpio_reg);
		break;
	case 1:
		gpio_reg = (mmio_read_32((uintptr_t)MTK_GPIO_I2C_BASE1)
				   & (~(0x7 << MTK_GPIO_SDA1)))
				   | (0x3 << MTK_GPIO_SDA1);
		mmio_write_32((uintptr_t)MTK_GPIO_I2C_BASE1, gpio_reg);
		gpio_reg = (mmio_read_32((uintptr_t)MTK_GPIO_I2C_BASE1)
				   & (~(0x7 << MTK_GPIO_SCL1)))
				   | (0x3 << MTK_GPIO_SCL1);
		mmio_write_32((uintptr_t)MTK_GPIO_I2C_BASE1, gpio_reg);
		break;
	case 2:
		gpio_reg = (mmio_read_32((uintptr_t)MTK_GPIO_I2C_BASE2)
				   & (~(0x7 << MTK_GPIO_SDA2)))
				   | (0x2 << MTK_GPIO_SDA2);
		mmio_write_32((uintptr_t)MTK_GPIO_I2C_BASE2, gpio_reg);
		gpio_reg = (mmio_read_32((uintptr_t)MTK_GPIO_I2C_BASE2)
				   & (~(0x7 << MTK_GPIO_SCL2)))
				   | (0x2 << MTK_GPIO_SCL2);
		mmio_write_32((uintptr_t)MTK_GPIO_I2C_BASE2, gpio_reg);
		break;
	case 3:
		gpio_reg = (mmio_read_32((uintptr_t)MTK_GPIO_I2C_BASE3)
				   & (~(0x7 << MTK_GPIO_SDA3)))
				   | (0x5 << MTK_GPIO_SDA3);
		mmio_write_32((uintptr_t)MTK_GPIO_I2C_BASE3, gpio_reg);
		gpio_reg = (mmio_read_32((uintptr_t)MTK_GPIO_I2C_BASE3)
				   & (~(0x7 << MTK_GPIO_SCL3)))
				   | (0x5 << MTK_GPIO_SCL3);
		mmio_write_32((uintptr_t)MTK_GPIO_I2C_BASE3, gpio_reg);
		break;
	default:
		break;
	}
}

void mtk_i2c_irq(void)
{
	uint16_t restart_flag = 0;
	uint16_t intr_stat;

	if (i2c_g->auto_restart)
		restart_flag = I2C_RS_TRANSFER;

	intr_stat = i2c_readl(i2c_g, OFFSET_INTR_STAT);
	i2c_writel(i2c_g, OFFSET_INTR_STAT, intr_stat);

    /*
     * when occurs ack error, i2c controller generate two interrupts
     * first is the ack error interrupt, then the complete interrupt
     * i2c->irq_stat need keep the two interrupt value.
     */
	i2c_g->irq_stat |= intr_stat;
	if (i2c_g->irq_stat & (I2C_TRANSAC_COMP | restart_flag))
		i2c_g->msg_complete = 1;
}

static int mtk_i2c_init_base(struct mt_i2c *i2c)
{
	switch (i2c->id) {
	case 0:
		i2c->base = (void *)MTK_I2C0_BASE;
		i2c->pdmabase = (void *)MTK_I2C0_DMA;
		i2c->irqnr = MTK_I2C0_GIC_IRQ;
			break;
	case 1:
		i2c->base = (void *)MTK_I2C1_BASE;
		i2c->pdmabase = (void *)MTK_I2C1_DMA;
		i2c->irqnr = MTK_I2C1_GIC_IRQ;
		break;
	case 2:
		i2c->base = (void *)MTK_I2C2_BASE;
		i2c->pdmabase = (void *)MTK_I2C2_DMA;
		i2c->irqnr = MTK_I2C2_GIC_IRQ;
		break;
	case 3:
		i2c->base = (void *)MTK_I2C3_BASE;
		i2c->pdmabase = (void *)MTK_I2C3_DMA;
		i2c->irqnr = MTK_I2C3_GIC_IRQ;
		break;
	default:
#ifdef I2C_DEBUG
		ERROR(I2CTAG "invalid para: i2c->id=%d\n", i2c->id);
#endif
		return -EINVAL_I2C;
	}

	mtk_i2c_gpio_set(i2c);

	i2c->clk = MTK_I2C_SOURCE_CLK;
	i2c->clk_src_div = MTK_I2C_CLK_DIV;
	i2c->aux_len_reg = 1;

	return 0;
}

static void mtk_i2c_init_hw(struct mt_i2c *i2c)
{
	uint16_t control_reg;

	i2c_writel(i2c, OFFSET_SOFTRESET, I2C_SOFT_RST);

	/* set ioconfig */
	if (i2c->pushpull)
		i2c_writel(i2c, OFFSET_IO_CONFIG, I2C_IO_CONFIG_PUSH_PULL);
	else
		i2c_writel(i2c, OFFSET_IO_CONFIG, I2C_IO_CONFIG_OPEN_DRAIN);

	i2c_writel(i2c, OFFSET_DCM_EN, I2C_DCM_DISABLE);

	control_reg = I2C_CONTROL_ACKERR_DET_EN | I2C_CONTROL_CLK_EXT_EN;

	i2c_writel(i2c, OFFSET_CONTROL, control_reg);
	i2c_writel(i2c, OFFSET_DELAY_LEN, I2C_DELAY_LEN);

	i2c_writel(i2c, OFFSET_CLOCK_DIV, (I2C_DEFAULT_CLK_DIV - 1));

	mmio_write_32((uintptr_t)(i2c->pdmabase) + OFFSET_RST,
			I2C_DMA_HARD_RST);
	mmio_write_32((uintptr_t)(i2c->pdmabase) + OFFSET_RST,
			I2C_DMA_CLR_FLAG);
}

/*
 * Calculate i2c port speed
 *
 * Hardware design:
 * i2c_bus_freq = parent_clk / (clock_div * 2 * sample_cnt * step_cnt)
 * clock_div: fixed in hardware, but may be various in different SoCs
 *
 * The calculation want to pick the highest bus frequency that is still
 * less than or equal to i2c->speed_hz. The calculation try to get
 * sample_cnt and step_cn
 */
static int mtk_i2c_calculate_speed(unsigned int clk_src,
				   unsigned int target_speed,
				   unsigned int *timing_step_cnt,
				   unsigned int *timing_sample_cnt)
{
	unsigned int step_cnt;
	unsigned int sample_cnt;
	unsigned int max_step_cnt;
	unsigned int base_sample_cnt = MAX_SAMPLE_CNT_DIV;
	unsigned int base_step_cnt;
	unsigned int opt_div;
	unsigned int best_mul;
	unsigned int cnt_mul;

	if (target_speed > MAX_HS_MODE_SPEED)
		target_speed = MAX_HS_MODE_SPEED;

	if (target_speed > MAX_FS_MODE_SPEED)
		max_step_cnt = MAX_HS_STEP_CNT_DIV;
	else
		max_step_cnt = MAX_STEP_CNT_DIV;

	base_step_cnt = max_step_cnt;

	/* find the best combination */
	opt_div = DIV_ROUND_UP(clk_src >> 1, target_speed);
	best_mul = MAX_SAMPLE_CNT_DIV * max_step_cnt;

	/* Search for the best pair (sample_cnt, step_cnt) with
	 * 0 < sample_cnt < MAX_SAMPLE_CNT_DIV
	 * 0 < step_cnt < max_step_cnt
	 * sample_cnt * step_cnt >= opt_div
	 * optimizing for sample_cnt * step_cnt being minimal
	 */
	for (sample_cnt = 1; sample_cnt <= MAX_SAMPLE_CNT_DIV; sample_cnt++) {
		step_cnt = DIV_ROUND_UP(opt_div, sample_cnt);
		cnt_mul = step_cnt * sample_cnt;
		if (step_cnt > max_step_cnt)
			continue;

		if (cnt_mul < best_mul) {
			best_mul = cnt_mul;
			base_sample_cnt = sample_cnt;
			base_step_cnt = step_cnt;
			if (best_mul == opt_div)
				break;
		}
	}

	sample_cnt = base_sample_cnt;
	step_cnt = base_step_cnt;

	if ((clk_src / (2 * sample_cnt * step_cnt)) > target_speed) {
	/* In this case, hardware can't support such low i2c_bus_freq*/

#ifdef I2C_DEBUG
		ERROR(I2CTAG "Unsupported speed (%u khz)\n", target_speed);
#endif

		return -EINVAL_I2C;
	}

	*timing_step_cnt = step_cnt - 1;
	*timing_sample_cnt = sample_cnt - 1;

	return 0;
}

static int mtk_i2c_set_speed(struct mt_i2c *i2c)
{
	unsigned int clk_src;
	unsigned int step_cnt;
	unsigned int sample_cnt;
	unsigned int target_speed;
	int ret;

	if (i2c->speed == 0)
		i2c->speed = MAX_ST_MODE_SPEED;

	if (i2c->clk_src_div == 0)
		i2c->clk_src_div = MTK_I2C_CLK_DIV;

	i2c->clk_src_div *= I2C_DEFAULT_CLK_DIV;

	clk_src = (i2c->clk) / (i2c->clk_src_div);
	target_speed = i2c->speed;

	if (target_speed > MAX_FS_MODE_SPEED) {
		/* set master code speed register */
		ret = mtk_i2c_calculate_speed(clk_src, MAX_FS_MODE_SPEED,
					      &step_cnt, &sample_cnt);
		if (ret < 0)
			return ret;

		i2c->timing_reg = (sample_cnt << 8) | step_cnt;
		/* set the high speed mode register */
		ret = mtk_i2c_calculate_speed(clk_src, target_speed,
					      &step_cnt, &sample_cnt);
		if (ret < 0)
			return ret;

		i2c->high_speed_reg = I2C_TIME_DEFAULT_VALUE |
				      (sample_cnt << 12) |
				      (step_cnt << 8);
	} else {

		ret = mtk_i2c_calculate_speed(clk_src, target_speed,
					      &step_cnt, &sample_cnt);
		if (ret < 0)
			return ret;

		i2c->timing_reg = (sample_cnt << 8) | step_cnt;

		/* disable the high speed transaction */
		i2c->high_speed_reg = I2C_TIME_CLR_VALUE;
	}

	i2c_writel(i2c, OFFSET_TIMING, i2c->timing_reg);
	i2c_writel(i2c, OFFSET_HS, i2c->high_speed_reg);

	return 0;
}

static int mtk_i2c_set_security(struct mt_i2c *i2c, unsigned int status)
{
	switch (i2c->id) {
	case 0:
		if (status)
			mmio_write_32((uintptr_t)MTK_I2C0_DMA_SEC_EN,
					      DMA_SEC_ENABLE);
		else
			mmio_write_32((uintptr_t)MTK_I2C0_DMA_SEC_EN,
					      DMA_SEC_DISABLE);
		break;
	case 1:
		if (status)
			mmio_write_32((uintptr_t)MTK_I2C1_DMA_SEC_EN,
					      DMA_SEC_ENABLE);
		else
			mmio_write_32((uintptr_t)MTK_I2C1_DMA_SEC_EN,
					      DMA_SEC_DISABLE);
		break;
	case 2:
		if (status)
			mmio_write_32((uintptr_t)MTK_I2C2_DMA_SEC_EN,
					      DMA_SEC_ENABLE);
		else
			mmio_write_32((uintptr_t)MTK_I2C2_DMA_SEC_EN,
					      DMA_SEC_DISABLE);
		break;
	case 3:
		if (status)
			mmio_write_32((uintptr_t)MTK_I2C3_DMA_SEC_EN,
					      DMA_SEC_ENABLE);
		else
			mmio_write_32((uintptr_t)MTK_I2C3_DMA_SEC_EN,
					      DMA_SEC_DISABLE);
		break;
	default:
		goto err_exit;
	}

	if (i2c->id >= 0 && i2c->id <= 3) {
		if (status)
			mmio_write_32((uintptr_t)MTK_AP_DMA_GL_GSEC_EN,
				      DMA_GSEC_ENABLE);
		else
			mmio_write_32((uintptr_t)MTK_AP_DMA_GL_GSEC_EN,
				      DMA_GSEC_DISABLE);
	} else
		goto err_exit;

	return 0;

err_exit:

#ifdef I2C_DEBUG
	ERROR(I2CTAG "invalid para: i2c->id=%d\n", i2c->id);
#endif

	return -EINVAL_I2C;
}

#ifdef I2C_DEBUG
static void i2c_dump_info(struct mt_i2c *i2c)
{
	ERROR(I2CTAG "I2C structure:\n"
	      I2CTAG"Id=%d,Dma_en=%x,Auto_restart=%x,Poll_en=%x,Op=%x\n"
	      I2CTAG"Irq_stat=%x,source_clk=%d,clk_div=%d,speed=%d\n",
	      i2c->id, i2c->dma_en, i2c->auto_restart, i2c->poll_en, i2c->op,
	      i2c->irq_stat, i2c->clk, i2c->clk_src_div, i2c->speed);

	ERROR(I2CTAG "base address 0x%p\n", i2c->base);
	ERROR(I2CTAG "I2C register:\n"
	      I2CTAG"SLAVE_ADDR=%x,INTR_MASK=%x,INTR_STAT=%x,CONTROL=%x\n"
	      I2CTAG"TRANSFER_LEN=%x,TRANSAC_LEN=%x,DELAY_LEN=%x\n"
	      I2CTAG"TIMING=%x,START=%x,FIFO_STAT=%x,IO_CONFIG=%x,HS=%x\n"
	      I2CTAG"DCM_EN=%x,DEBUGSTAT=%x,EXT_CONF=%x\n"
	      I2CTAG"TRANSFER_LEN_AUX=%x,FIFO_THRESH=%x,RSV_DEBUG=%x\n"
	      I2CTAG"DEBUGCTRL=%x,CLOCK_DIV=%x,SCL_HL_RATIO=%x\n"
	      I2CTAG"SCL_HS_HL_RATIO=%x,SCL_MIS_COMP_POINT=%x\n"
	      I2CTAG"STA_STOP_AC_TIME=%x,HS_STA_STOP_AC_TIME=%x\n"
	      I2CTAG"DATA_TIME=%x,TIME_OUT=%x\n",
	      (i2c_readl(i2c, OFFSET_SLAVE_ADDR)),
	      (i2c_readl(i2c, OFFSET_INTR_MASK)),
	      (i2c_readl(i2c, OFFSET_INTR_STAT)),
	      (i2c_readl(i2c, OFFSET_CONTROL)),
	      (i2c_readl(i2c, OFFSET_TRANSFER_LEN)),
	      (i2c_readl(i2c, OFFSET_TRANSAC_LEN)),
	      (i2c_readl(i2c, OFFSET_DELAY_LEN)),
	      (i2c_readl(i2c, OFFSET_TIMING)),
	      (i2c_readl(i2c, OFFSET_START)),
	      (i2c_readl(i2c, OFFSET_FIFO_STAT)),
	      (i2c_readl(i2c, OFFSET_IO_CONFIG)),
	      (i2c_readl(i2c, OFFSET_HS)),
	      (i2c_readl(i2c, OFFSET_DCM_EN)),
	      (i2c_readl(i2c, OFFSET_DEBUGSTAT)),
	      (i2c_readl(i2c, OFFSET_EXT_CONF)),
	      (i2c_readl(i2c, OFFSET_TRANSFER_LEN_AUX)),
	      (i2c_readl(i2c, OFFSET_FIFO_THRESH)),
	      (i2c_readl(i2c, OFFSET_RSV_DEBUG)),
	      (i2c_readl(i2c, OFFSET_DEBUGCTRL)),
	      (i2c_readl(i2c, OFFSET_CLOCK_DIV)),
	      (i2c_readl(i2c, OFFSET_SCL_HL_RATIO)),
	      (i2c_readl(i2c, OFFSET_SCL_HS_HL_RATIO)),
	      (i2c_readl(i2c, OFFSET_SCL_MIS_COMP_POINT)),
	      (i2c_readl(i2c, OFFSET_STA_STOP_AC_TIME)),
	      (i2c_readl(i2c, OFFSET_HS_STA_STOP_AC_TIME)),
	      (i2c_readl(i2c, OFFSET_DATA_TIME)),
	      (i2c_readl(i2c, OFFSET_TIME_OUT)));
	      ERROR("APDMA base address 0x%lx\n",
		    (unsigned long)i2c->pdmabase);
	      ERROR("APDMA register:\n");
	      ERROR(I2CTAG "DMA_INT_FLAG=%x,DMA_INT_EN=%x,DMA_EN=%x\n"
		I2CTAG "DMA_RST=%x,DMA_CON=%x,DMA_TX_MEM_ADDR=%x\n"
		I2CTAG "DMA_RX_MEM_ADDR=%x,OFFSET_TX_LEN=%x\n"
		I2CTAG "OFFSET_RX_LEN=%x,AP_DMA_GLOBAL_SEC_E=%x\n",
		(mmio_read_32((uintptr_t)i2c->pdmabase + OFFSET_INT_FLAG)),
		(mmio_read_32((uintptr_t)i2c->pdmabase + OFFSET_INT_EN)),
		(mmio_read_32((uintptr_t)i2c->pdmabase + OFFSET_EN)),
		(mmio_read_32((uintptr_t)i2c->pdmabase + OFFSET_RST)),
		(mmio_read_32((uintptr_t)i2c->pdmabase + OFFSET_CON)),
		(mmio_read_32((uintptr_t)i2c->pdmabase + OFFSET_TX_MEM_ADDR)),
		(mmio_read_32((uintptr_t)i2c->pdmabase + OFFSET_RX_MEM_ADDR)),
		(mmio_read_32((uintptr_t)i2c->pdmabase + OFFSET_TX_LEN)),
		(mmio_read_32((uintptr_t)i2c->pdmabase + OFFSET_RX_LEN)),
		(mmio_read_32((uintptr_t)AP_DMA_GLOBAL_SEC_E)));
}

#endif

static int mtk_i2c_do_transfer(struct mt_i2c *i2c, struct i2c_msg *msgs,
			       int num, int left_num)
{
	int ret = 0;
	uint16_t addr_reg;
	uint16_t start_reg;
	uint16_t control_reg;
	uint16_t restart_flag = 0;
	uint8_t trans_error = 0;
	uint8_t tmo = 1;
	uint16_t data_size = msgs->len;
	uint32_t tmo_poll = 0xfffff;
	uint8_t *ptr = msgs->buf;

	i2c->irq_stat = 0;
	i2c->msg_complete = 0;

	if (i2c->auto_restart)
		restart_flag = I2C_RS_TRANSFER;

	control_reg = i2c_readl(i2c, OFFSET_CONTROL)
		      & (~(I2C_CONTROL_DIR_CHANGE | I2C_CONTROL_RS));

	if ((i2c->speed > MAX_FS_MODE_SPEED) || (num > 1))
		control_reg |= I2C_CONTROL_RS;

	if (i2c->op == I2C_MASTER_WRRD)
		control_reg |= I2C_CONTROL_DIR_CHANGE | I2C_CONTROL_RS;

	if (i2c->dma_en)
		control_reg |= I2C_CONTROL_DMA_EN;

	i2c_writel(i2c, OFFSET_CONTROL, control_reg);

	/* set start condition */
	if (i2c->speed <= MAX_ST_MODE_SPEED)
		i2c_writel(i2c, OFFSET_EXT_CONF, I2C_ST_START_CON);
	else
		i2c_writel(i2c, OFFSET_EXT_CONF, I2C_FS_START_CON);

	addr_reg = msgs->addr << 1;
	if (i2c->op == I2C_MASTER_RD)
		addr_reg |= 0x1;

	i2c_writel(i2c, OFFSET_SLAVE_ADDR, addr_reg);

	/* clear interrupt status */
	i2c_writel(i2c, OFFSET_INTR_STAT, restart_flag | I2C_HS_NACKERR |
					  I2C_ACKERR | I2C_TRANSAC_COMP);
	i2c_writel(i2c, OFFSET_FIFO_ADDR_CLR, I2C_FIFO_ADDR_CLR);

	/* enable interrupt */
	i2c_writel(i2c, OFFSET_INTR_MASK, restart_flag | I2C_HS_NACKERR |
					  I2C_ACKERR | I2C_TRANSAC_COMP);

	/* set transfer and transaction len */
	if (i2c->op == I2C_MASTER_WRRD) {
		if (i2c->aux_len_reg) {
			i2c_writel(i2c, OFFSET_TRANSFER_LEN, msgs->len);
			i2c_writel(i2c, OFFSET_TRANSFER_LEN_AUX,
				   (msgs + 1)->len);
		} else {
			i2c_writel(i2c, OFFSET_TRANSFER_LEN,
				   (msgs->len | (((msgs + 1)->len) << 8)));
		}
		i2c_writel(i2c, OFFSET_TRANSAC_LEN, I2C_WRRD_TRANAC_VALUE);
	} else {
		i2c_writel(i2c, OFFSET_TRANSFER_LEN, msgs->len);
		i2c_writel(i2c, OFFSET_TRANSAC_LEN, num);
	}

	if (i2c->dma_en) {
		/* prepare buffer data to start transfer */
		if (i2c->op == I2C_MASTER_RD) {
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_INT_FLAG),
				      I2C_DMA_INT_FLAG_NONE);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_CON), I2C_DMA_CON_RX);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_RX_LEN), msgs->len);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_RX_MEM_ADDR),
				      (uint32_t)((unsigned long)i2c->rx_buff));
		} else if (i2c->op == I2C_MASTER_WR) {
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_INT_FLAG),
				      I2C_DMA_INT_FLAG_NONE);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_CON), I2C_DMA_CON_TX);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_TX_LEN), msgs->len);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_TX_MEM_ADDR),
				      (uint32_t)((unsigned long)i2c->tx_buff));
		} else {
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_INT_FLAG), I2C_DMA_CLR_FLAG);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_CON), I2C_DMA_CLR_FLAG);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_TX_MEM_ADDR),
				      (uint32_t)((unsigned long)i2c->tx_buff));
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_RX_MEM_ADDR),
				      (uint32_t)((unsigned long)i2c->rx_buff));
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_TX_LEN), msgs->len);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_RX_LEN), (msgs + 1)->len);
			mmio_write_32((uintptr_t)((unsigned long)(i2c->pdmabase)
				      + OFFSET_EN), I2C_DMA_START_EN);
		}

		mmio_write_32((uintptr_t)(i2c->pdmabase) + OFFSET_EN,
			       I2C_DMA_START_EN);
	} else {
		if (i2c->op != I2C_MASTER_RD) {
			while (data_size--) {
				i2c_writel(i2c, OFFSET_DATA_PORT, *ptr);
				ptr++;
			}
		}
	}

	if (!i2c->auto_restart) {
		start_reg = I2C_TRANSAC_START;
	} else {
		start_reg = I2C_TRANSAC_START | I2C_RS_MUL_TRIG;
		if (left_num >= 1)
			start_reg |= I2C_RS_MUL_CNFG;
	}

	i2c_writel(i2c, OFFSET_START, start_reg);

	if (i2c->poll_en) {
		for (;;) {
			i2c->irq_stat = i2c_readl(i2c, OFFSET_INTR_STAT);

			if (i2c->irq_stat & (I2C_TRANSAC_COMP | restart_flag)) {
				tmo = 1;
				if (i2c->irq_stat &
				    (I2C_HS_NACKERR | I2C_ACKERR))
					trans_error = 1;
				break;
			}

			tmo_poll--;
			if (tmo_poll == 0) {
				tmo = 0;
				break;
			}
		}
	} else {
		for (;;) {
			if (i2c->msg_complete && (i2c->irq_stat &
			    (I2C_TRANSAC_COMP | restart_flag))) {
				tmo = 1;

				if (i2c->irq_stat & (I2C_HS_NACKERR |
						     I2C_ACKERR))
					trans_error = 1;
				break;
			}
			tmo_poll--;
			if (tmo_poll == 0) {
				tmo = 0;
				break;
			}
		}
	}

	/* clear interrupt mask */
	i2c_writel(i2c, OFFSET_INTR_MASK, ~(restart_flag | I2C_HS_NACKERR |
					  I2C_ACKERR | I2C_TRANSAC_COMP));

	if ((tmo != 0) && (trans_error == 0)) {
		/* transfer success ,we need to get data from fifo */
		if ((i2c->op == I2C_MASTER_RD) ||
		    (i2c->op == I2C_MASTER_WRRD)) {
			ptr = (i2c->op == I2C_MASTER_RD) ?
			       msgs->buf : (msgs + 1)->buf;
			if (!i2c->dma_en) {
				data_size = (i2c_readl(i2c, OFFSET_FIFO_STAT) >>
					     4) & 0x000F;
				while (data_size--)
					*ptr++ = i2c_readl(i2c,
							   OFFSET_DATA_PORT);
			}
		}
	} else {
		/* timeout or ACKERR */
		if (tmo == 0)
			ret = -ETIMEDOUT_I2C;
		else
			ret = -EACK_I2C;

#ifdef I2C_DEBUG
		if (tmo == 0) {
			ERROR(I2CTAG "id=%d,addr: %x, transfer timeout\n",
			      i2c->id, msgs->addr);
		} else {
			ERROR(I2CTAG "id=%d,addr: %x, transfer error\n",
			      i2c->id, msgs->addr);
		}
		if (i2c->irq_stat & I2C_HS_NACKERR)
			ERROR(I2CTAG "I2C_HS_NACKERR\n");
		if (i2c->irq_stat & I2C_ACKERR)
			ERROR(I2CTAG "I2C_ACKERR\n");
		i2c_dump_info(i2c);
#endif
		mtk_i2c_init_hw(i2c);
	}

	return ret;
}

int mtk_i2c_transfer(struct mt_i2c *i2c,
		     struct i2c_msg msgs[], int num)
{
	int ret;
	int left_num = num;
	uint8_t num_cnt;

	ret = mtk_i2c_init_base(i2c);
	if (ret) {
#ifdef I2C_DEBUG
		ERROR(I2CTAG "Failed to init i2c base.\n");
#endif
		return ret;
	}

	mtk_i2c_clock_enable(i2c);

	mtk_i2c_init_hw(i2c);

	ret = mtk_i2c_set_speed(i2c);
	if (ret) {
#ifdef I2C_DEBUG
		ERROR(I2CTAG "Failed to set the speed.\n");
#endif
		return ret;
	}

	i2c_g = i2c;

	for (num_cnt = 0; num_cnt < num; num_cnt++) {
		if (((msgs+num_cnt)->addr) > 0x7f) {
#ifdef I2C_DEBUG
			ERROR(I2CTAG "i2c msgs[%d]->addr(%x) > 0x7f, error!\n",
					num_cnt, ((msgs+num_cnt)->addr));
#endif
			return -EINVAL_I2C;
		}
		if (!(msgs+num_cnt)->buf) {
#ifdef I2C_DEBUG
			ERROR(I2CTAG "messages buffer is NULL.\n");
#endif
			return -EINVAL_I2C;
		}
		if (((msgs+num_cnt)->len) > 8) {

			i2c->dma_en = 1;

			mtk_i2c_set_security(i2c, 1);

			if (i2c->op == I2C_MASTER_RD) {
				i2c->rx_buff = msgs->buf;
			} else if (i2c->op == I2C_MASTER_WR) {
				i2c->tx_buff = msgs->buf;
			} else {
				i2c->tx_buff = msgs->buf;
				i2c->rx_buff = (msgs+1)->buf;
			}
			if (((msgs+num_cnt)->len) > 255) {
#ifdef I2C_DEBUG
				ERROR(I2CTAG "msgs[%d]->len(%d) > 255,error!\n",
				      num_cnt, ((msgs+num_cnt)->len));
#endif
				return -EINVAL_I2C;
			}
		}
	}
	while (left_num--) {
		if (!msgs->buf) {
#ifdef I2C_DEBUG
			ERROR(I2CTAG "data buffer is NULL.\n");
#endif
			ret = -EINVAL_I2C;
			goto err_exit;
		}
		if (msgs->flags & I2C_M_RD)
			i2c_g->op = I2C_MASTER_RD;
		else
			i2c_g->op = I2C_MASTER_WR;
		if (!i2c_g->auto_restart) {
			if (num > 1) {
				/* combined two messages into one transaction */
				i2c_g->op = I2C_MASTER_WRRD;
				left_num--;
			}
		}
		ret = mtk_i2c_do_transfer(i2c_g, msgs, num, left_num);
		if (ret < 0)
			goto err_exit;
		msgs++;
	}
	ret = I2C_OK;
err_exit:
	mtk_i2c_set_security(i2c, 0);

	mtk_i2c_clock_disable(i2c);
	return ret;
}

/*
 *   Read interface: Read bytes.
 *   bus_num:    I2C bus number.
 *   device_addr: address of device.
 *   speed: Transmission speed(kHz).
 *   buffer:  Where to read/write the data(no cache).
 *   len:     How many bytes to read/write
 *   Returns: ERROR_CODE
 */

int i2c_cust_read(uint16_t bus_num, uint8_t device_addr, uint32_t speed,
		  uint8_t *buffer, uint32_t len)
{
	int ret = I2C_OK;
	struct i2c_msg msgs;
	struct mt_i2c i2c_mtk;
	struct mt_i2c *i2c = &i2c_mtk;

	i2c->poll_en = 1;
	i2c->dma_en = 0;
	i2c->auto_restart = 0;
	i2c->pushpull = 0;
	i2c->filter_msg = 0;
	i2c->id = bus_num;
	i2c->addr = device_addr;
	i2c->speed = speed;
	i2c->op = I2C_MASTER_RD;

	msgs.addr = i2c->addr;
	msgs.flags = 1;
	msgs.buf = buffer;
	msgs.len = len;

	ret = mtk_i2c_transfer(i2c, &msgs, 1);
	return ret;
}

/*
 *   write interface: Write bytes
 *   bus_num:    I2C bus number.
 *   device_addr: address of device.
 *   speed: Transmission speed(kHz).
 *   buffer:  Where to read/write the data(no cache).
 *   len:     How many bytes to read/write
 *   Returns: ERROR_CODE
 */
int i2c_cust_write(uint16_t bus_num, uint8_t device_addr, uint32_t speed,
		   uint8_t *buffer, uint32_t len)
{
	int ret = I2C_OK;
	struct i2c_msg msgs;
	struct mt_i2c i2c_mtk;
	struct mt_i2c *i2c = &i2c_mtk;

	i2c->poll_en = 1;
	i2c->dma_en = 0;
	i2c->auto_restart = 0;
	i2c->pushpull = 0;
	i2c->filter_msg = 0;
	i2c->id = bus_num;
	i2c->addr = device_addr;
	i2c->speed = speed;
	i2c->op = I2C_MASTER_WR;

	msgs.addr = i2c->addr;
	msgs.flags = 0;
	msgs.buf = buffer;
	msgs.len = len;
	ret = mtk_i2c_transfer(i2c, &msgs, 1);
	return ret;
}

/*
 *   Write then read back interface: Write bytes then read bytes
 *   bus_num:    I2C bus number.
 *   device_addr: address of device.
 *   speed: Transmission speed(kHz).
 *   write_buffer: Where to write the data(no cache).
 *   read_buffer:  Where to read the data(no cache).
 *   write_len:    How many bytes to write.
 *   read_len:     How many bytes to read.
 *   Returns: ERROR_CODE.
 */
int i2c_cust_write_read(uint16_t bus_num, uint8_t device_addr, uint32_t speed,
			uint8_t *write_buffer, uint8_t *read_buffer,
			uint32_t write_len, uint32_t read_len)
{
	int ret = I2C_OK;
	struct i2c_msg msgs[2];
	struct mt_i2c i2c_mtk;
	struct mt_i2c *i2c = &i2c_mtk;

	i2c->poll_en = 1;
	i2c->dma_en = 0;
	i2c->auto_restart = 0;
	i2c->pushpull = 0;
	i2c->filter_msg = 0;
	i2c->id = bus_num;
	i2c->addr = device_addr;
	i2c->speed = speed;
	i2c->op = I2C_MASTER_WRRD;

	msgs[0].addr = i2c->addr;
	msgs[0].flags = 0;
	msgs[0].buf = write_buffer;
	msgs[0].len = write_len;

	msgs[1].addr = i2c->addr;
	msgs[1].flags = 1;
	msgs[1].buf = read_buffer;
	msgs[1].len = read_len;
	ret = mtk_i2c_transfer(i2c, msgs, 2);
	return ret;
}


<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2017-2020, The Linux Foundation. All rights reserved.
 */

#include <linux/clk.h>
#include <linux/delay.h>
=======
// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.

#include <linux/clk.h>
#include <linux/dma-mapping.h>
>>>>>>> rebase
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
#include <linux/dma-mapping.h>
#include <linux/qcom-geni-se.h>
#include <linux/ipc_logging.h>
#include <linux/dmaengine.h>
#include <linux/msm_gpi.h>
#include <linux/ioctl.h>
#include <linux/pinctrl/consumer.h>
#include <linux/slab.h>

#define SE_I2C_TX_TRANS_LEN		(0x26C)
#define SE_I2C_RX_TRANS_LEN		(0x270)
#define SE_I2C_SCL_COUNTERS		(0x278)
#define SE_GENI_IOS			(0x908)

#define SE_I2C_ERR  (M_CMD_OVERRUN_EN | M_ILLEGAL_CMD_EN | M_CMD_FAILURE_EN |\
			M_GP_IRQ_1_EN | M_GP_IRQ_3_EN | M_GP_IRQ_4_EN)
#define SE_I2C_ABORT (1U << 1)
/* M_CMD OP codes for I2C */
#define I2C_WRITE		(0x1)
#define I2C_READ		(0x2)
#define I2C_WRITE_READ		(0x3)
#define I2C_ADDR_ONLY		(0x4)
#define I2C_BUS_CLEAR		(0x6)
#define I2C_STOP_ON_BUS		(0x7)
/* M_CMD params for I2C */
#define PRE_CMD_DELAY		(BIT(0))
#define TIMESTAMP_BEFORE	(BIT(1))
#define STOP_STRETCH		(BIT(2))
#define TIMESTAMP_AFTER		(BIT(3))
#define POST_COMMAND_DELAY	(BIT(4))
#define IGNORE_ADD_NACK		(BIT(6))
#define READ_FINISHED_WITH_ACK	(BIT(7))
#define BYPASS_ADDR_PHASE	(BIT(8))
#define SLV_ADDR_MSK		(GENMASK(15, 9))
#define SLV_ADDR_SHFT		(9)

#define I2C_PACK_EN		(BIT(0) | BIT(1))
#define I2C_CORE2X_VOTE		(960)
#define GP_IRQ0			0
#define GP_IRQ1			1
#define GP_IRQ2			2
#define GP_IRQ3			3
#define GP_IRQ4			4
#define GP_IRQ5			5
#define GENI_OVERRUN		6
#define GENI_ILLEGAL_CMD	7
#define GENI_ABORT_DONE		8
#define GENI_TIMEOUT		9

#define I2C_NACK		GP_IRQ1
#define I2C_BUS_PROTO		GP_IRQ3
#define I2C_ARB_LOST		GP_IRQ4
#define DM_I2C_CB_ERR		((BIT(GP_IRQ1) | BIT(GP_IRQ3) | BIT(GP_IRQ4)) \
									<< 5)

#define I2C_AUTO_SUSPEND_DELAY	250

#define I2C_TIMEOUT_SAFETY_COEFFICIENT	10

#define I2C_TIMEOUT_MIN_USEC	500000

#define MAX_SE	20

enum i2c_se_mode {
	UNINITIALIZED,
	FIFO_SE_DMA,
	GSI_ONLY,
};

struct dbg_buf_ctxt {
	void *virt_buf;
	void *map_buf;
};

struct geni_i2c_dev {
	struct device *dev;
	void __iomem *base;
	unsigned int tx_wm;
	int irq;
	int err;
	u32 xfer_timeout;
	struct i2c_adapter adap;
	struct completion xfer;
	struct i2c_msg *cur;
	struct se_geni_rsc i2c_rsc;
	int cur_wr;
	int cur_rd;
	struct device *wrapper_dev;
	void *ipcl;
	int clk_fld_idx;
	struct dma_chan *tx_c;
	struct dma_chan *rx_c;
	struct msm_gpi_tre lock_t;
	struct msm_gpi_tre unlock_t;
	struct msm_gpi_tre cfg0_t;
	struct msm_gpi_tre go_t;
	struct msm_gpi_tre tx_t;
	struct msm_gpi_tre rx_t;
	dma_addr_t tx_ph;
	dma_addr_t rx_ph;
	struct msm_gpi_ctrl tx_ev;
	struct msm_gpi_ctrl rx_ev;
	struct scatterlist tx_sg[5]; /* lock, cfg0, go, TX, unlock */
	struct scatterlist rx_sg;
	int cfg_sent;
	struct dma_async_tx_descriptor *tx_desc;
	struct dma_async_tx_descriptor *rx_desc;
	struct msm_gpi_dma_async_tx_cb_param tx_cb;
	struct msm_gpi_dma_async_tx_cb_param rx_cb;
	enum i2c_se_mode se_mode;
	bool cmd_done;
	bool is_shared;
	u32 dbg_num;
	struct dbg_buf_ctxt *dbg_buf_ptr;
};

static struct geni_i2c_dev *gi2c_dev_dbg[MAX_SE];
static int arr_idx;

=======
#include <linux/qcom-geni-se.h>
#include <linux/spinlock.h>

#define SE_I2C_TX_TRANS_LEN		0x26c
#define SE_I2C_RX_TRANS_LEN		0x270
#define SE_I2C_SCL_COUNTERS		0x278

#define SE_I2C_ERR  (M_CMD_OVERRUN_EN | M_ILLEGAL_CMD_EN | M_CMD_FAILURE_EN |\
			M_GP_IRQ_1_EN | M_GP_IRQ_3_EN | M_GP_IRQ_4_EN)
#define SE_I2C_ABORT		BIT(1)

/* M_CMD OP codes for I2C */
#define I2C_WRITE		0x1
#define I2C_READ		0x2
#define I2C_WRITE_READ		0x3
#define I2C_ADDR_ONLY		0x4
#define I2C_BUS_CLEAR		0x6
#define I2C_STOP_ON_BUS		0x7
/* M_CMD params for I2C */
#define PRE_CMD_DELAY		BIT(0)
#define TIMESTAMP_BEFORE	BIT(1)
#define STOP_STRETCH		BIT(2)
#define TIMESTAMP_AFTER		BIT(3)
#define POST_COMMAND_DELAY	BIT(4)
#define IGNORE_ADD_NACK		BIT(6)
#define READ_FINISHED_WITH_ACK	BIT(7)
#define BYPASS_ADDR_PHASE	BIT(8)
#define SLV_ADDR_MSK		GENMASK(15, 9)
#define SLV_ADDR_SHFT		9
/* I2C SCL COUNTER fields */
#define HIGH_COUNTER_MSK	GENMASK(29, 20)
#define HIGH_COUNTER_SHFT	20
#define LOW_COUNTER_MSK		GENMASK(19, 10)
#define LOW_COUNTER_SHFT	10
#define CYCLE_COUNTER_MSK	GENMASK(9, 0)

enum geni_i2c_err_code {
	GP_IRQ0,
	NACK,
	GP_IRQ2,
	BUS_PROTO,
	ARB_LOST,
	GP_IRQ5,
	GENI_OVERRUN,
	GENI_ILLEGAL_CMD,
	GENI_ABORT_DONE,
	GENI_TIMEOUT,
};

#define DM_I2C_CB_ERR		((BIT(NACK) | BIT(BUS_PROTO) | BIT(ARB_LOST)) \
									<< 5)

#define I2C_AUTO_SUSPEND_DELAY	250
#define KHZ(freq)		(1000 * freq)
#define PACKING_BYTES_PW	4

#define ABORT_TIMEOUT		HZ
#define XFER_TIMEOUT		HZ
#define RST_TIMEOUT		HZ

struct geni_i2c_dev {
	struct geni_se se;
	u32 tx_wm;
	int irq;
	int err;
	struct i2c_adapter adap;
	struct completion done;
	struct i2c_msg *cur;
	int cur_wr;
	int cur_rd;
	spinlock_t lock;
	u32 clk_freq_out;
	const struct geni_i2c_clk_fld *clk_fld;
	int suspended;
};

>>>>>>> rebase
struct geni_i2c_err_log {
	int err;
	const char *msg;
};

<<<<<<< HEAD
static struct geni_i2c_err_log gi2c_log[] = {
	[GP_IRQ0] = {-EINVAL, "Unknown I2C err GP_IRQ0"},
	[I2C_NACK] = {-ENOTCONN,
			"NACK: slv unresponsive, check its power/reset-ln"},
	[GP_IRQ2] = {-EINVAL, "Unknown I2C err GP IRQ2"},
	[I2C_BUS_PROTO] = {-EPROTO,
				"Bus proto err, noisy/unepxected start/stop"},
	[I2C_ARB_LOST] = {-EBUSY,
				"Bus arbitration lost, clock line undriveable"},
	[GP_IRQ5] = {-EINVAL, "Unknown I2C err GP IRQ5"},
	[GENI_OVERRUN] = {-EIO, "Cmd overrun, check GENI cmd-state machine"},
	[GENI_ILLEGAL_CMD] = {-EILSEQ,
				"Illegal cmd, check GENI cmd-state machine"},
=======
static const struct geni_i2c_err_log gi2c_log[] = {
	[GP_IRQ0] = {-EIO, "Unknown I2C err GP_IRQ0"},
	[NACK] = {-ENXIO, "NACK: slv unresponsive, check its power/reset-ln"},
	[GP_IRQ2] = {-EIO, "Unknown I2C err GP IRQ2"},
	[BUS_PROTO] = {-EPROTO, "Bus proto err, noisy/unepxected start/stop"},
	[ARB_LOST] = {-EAGAIN, "Bus arbitration lost, clock line undriveable"},
	[GP_IRQ5] = {-EIO, "Unknown I2C err GP IRQ5"},
	[GENI_OVERRUN] = {-EIO, "Cmd overrun, check GENI cmd-state machine"},
	[GENI_ILLEGAL_CMD] = {-EIO, "Illegal cmd, check GENI cmd-state machine"},
>>>>>>> rebase
	[GENI_ABORT_DONE] = {-ETIMEDOUT, "Abort after timeout successful"},
	[GENI_TIMEOUT] = {-ETIMEDOUT, "I2C TXN timed out"},
};

struct geni_i2c_clk_fld {
	u32	clk_freq_out;
	u8	clk_div;
<<<<<<< HEAD
	u8	t_high;
	u8	t_low;
	u8	t_cycle;
};

static struct geni_i2c_clk_fld geni_i2c_clk_map[] = {
	{KHz(100), 7, 10, 11, 26},
	{KHz(400), 2,  5, 12, 24},
	{KHz(1000), 1, 3,  9, 18},
=======
	u8	t_high_cnt;
	u8	t_low_cnt;
	u8	t_cycle_cnt;
};

/*
 * Hardware uses the underlying formula to calculate time periods of
 * SCL clock cycle. Firmware uses some additional cycles excluded from the
 * below formula and it is confirmed that the time periods are within
 * specification limits.
 *
 * time of high period of SCL: t_high = (t_high_cnt * clk_div) / source_clock
 * time of low period of SCL: t_low = (t_low_cnt * clk_div) / source_clock
 * time of full period of SCL: t_cycle = (t_cycle_cnt * clk_div) / source_clock
 * clk_freq_out = t / t_cycle
 * source_clock = 19.2 MHz
 */
static const struct geni_i2c_clk_fld geni_i2c_clk_map[] = {
	{KHZ(100), 7, 10, 11, 26},
	{KHZ(400), 2,  5, 12, 24},
	{KHZ(1000), 1, 3,  9, 18},
>>>>>>> rebase
};

static int geni_i2c_clk_map_idx(struct geni_i2c_dev *gi2c)
{
	int i;
<<<<<<< HEAD
	int ret = 0;
	bool clk_map_present = false;
	struct geni_i2c_clk_fld *itr = geni_i2c_clk_map;

	for (i = 0; i < ARRAY_SIZE(geni_i2c_clk_map); i++, itr++) {
		if (itr->clk_freq_out == gi2c->i2c_rsc.clk_freq_out) {
			clk_map_present = true;
			break;
		}
	}

	if (clk_map_present)
		gi2c->clk_fld_idx = i;
	else
		ret = -EINVAL;

	return ret;
}

static inline void qcom_geni_i2c_conf(struct geni_i2c_dev *gi2c, int dfs)
{
	struct geni_i2c_clk_fld *itr = geni_i2c_clk_map + gi2c->clk_fld_idx;

	geni_write_reg(dfs, gi2c->base, SE_GENI_CLK_SEL);

	geni_write_reg((itr->clk_div << 4) | 1, gi2c->base, GENI_SER_M_CLK_CFG);
	geni_write_reg(((itr->t_high << 20) | (itr->t_low << 10) |
			itr->t_cycle), gi2c->base, SE_I2C_SCL_COUNTERS);

	/*
	 * Ensure Clk config completes before return.
	 */
	mb();
}

static inline void qcom_geni_i2c_calc_timeout(struct geni_i2c_dev *gi2c)
{

	struct geni_i2c_clk_fld *clk_itr = geni_i2c_clk_map + gi2c->clk_fld_idx;
	size_t bit_cnt = gi2c->cur->len*9;
	size_t bit_usec = (bit_cnt*USEC_PER_SEC)/clk_itr->clk_freq_out;
	size_t xfer_max_usec = (bit_usec*I2C_TIMEOUT_SAFETY_COEFFICIENT) +
							I2C_TIMEOUT_MIN_USEC;

	gi2c->xfer_timeout = usecs_to_jiffies(xfer_max_usec);

=======
	const struct geni_i2c_clk_fld *itr = geni_i2c_clk_map;

	for (i = 0; i < ARRAY_SIZE(geni_i2c_clk_map); i++, itr++) {
		if (itr->clk_freq_out == gi2c->clk_freq_out) {
			gi2c->clk_fld = itr;
			return 0;
		}
	}
	return -EINVAL;
}

static void qcom_geni_i2c_conf(struct geni_i2c_dev *gi2c)
{
	const struct geni_i2c_clk_fld *itr = gi2c->clk_fld;
	u32 val;

	writel_relaxed(0, gi2c->se.base + SE_GENI_CLK_SEL);

	val = (itr->clk_div << CLK_DIV_SHFT) | SER_CLK_EN;
	writel_relaxed(val, gi2c->se.base + GENI_SER_M_CLK_CFG);

	val = itr->t_high_cnt << HIGH_COUNTER_SHFT;
	val |= itr->t_low_cnt << LOW_COUNTER_SHFT;
	val |= itr->t_cycle_cnt;
	writel_relaxed(val, gi2c->se.base + SE_I2C_SCL_COUNTERS);
}

static void geni_i2c_err_misc(struct geni_i2c_dev *gi2c)
{
	u32 m_cmd = readl_relaxed(gi2c->se.base + SE_GENI_M_CMD0);
	u32 m_stat = readl_relaxed(gi2c->se.base + SE_GENI_M_IRQ_STATUS);
	u32 geni_s = readl_relaxed(gi2c->se.base + SE_GENI_STATUS);
	u32 geni_ios = readl_relaxed(gi2c->se.base + SE_GENI_IOS);
	u32 dma = readl_relaxed(gi2c->se.base + SE_GENI_DMA_MODE_EN);
	u32 rx_st, tx_st;

	if (dma) {
		rx_st = readl_relaxed(gi2c->se.base + SE_DMA_RX_IRQ_STAT);
		tx_st = readl_relaxed(gi2c->se.base + SE_DMA_TX_IRQ_STAT);
	} else {
		rx_st = readl_relaxed(gi2c->se.base + SE_GENI_RX_FIFO_STATUS);
		tx_st = readl_relaxed(gi2c->se.base + SE_GENI_TX_FIFO_STATUS);
	}
	dev_dbg(gi2c->se.dev, "DMA:%d tx_stat:0x%x, rx_stat:0x%x, irq-stat:0x%x\n",
		dma, tx_st, rx_st, m_stat);
	dev_dbg(gi2c->se.dev, "m_cmd:0x%x, geni_status:0x%x, geni_ios:0x%x\n",
		m_cmd, geni_s, geni_ios);
>>>>>>> rebase
}

static void geni_i2c_err(struct geni_i2c_dev *gi2c, int err)
{
<<<<<<< HEAD
	if (err == I2C_NACK || err == GENI_ABORT_DONE) {
		GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev, "%s\n",
			     gi2c_log[err].msg);
		goto err_ret;
	} else {
		GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev, "%s\n",
			     gi2c_log[err].msg);
	}
	geni_se_dump_dbg_regs(&gi2c->i2c_rsc, gi2c->base, gi2c->ipcl);
err_ret:
	gi2c->err = gi2c_log[err].err;
=======
	if (!gi2c->err)
		gi2c->err = gi2c_log[err].err;
	if (gi2c->cur)
		dev_dbg(gi2c->se.dev, "len:%d, slv-addr:0x%x, RD/WR:%d\n",
			gi2c->cur->len, gi2c->cur->addr, gi2c->cur->flags);

	if (err != NACK && err != GENI_ABORT_DONE) {
		dev_err(gi2c->se.dev, "%s\n", gi2c_log[err].msg);
		geni_i2c_err_misc(gi2c);
	}
>>>>>>> rebase
}

static irqreturn_t geni_i2c_irq(int irq, void *dev)
{
	struct geni_i2c_dev *gi2c = dev;
<<<<<<< HEAD
	int i, j;
	u32 m_stat = readl_relaxed(gi2c->base + SE_GENI_M_IRQ_STATUS);
	u32 rx_st = readl_relaxed(gi2c->base + SE_GENI_RX_FIFO_STATUS);
	u32 dm_tx_st = readl_relaxed(gi2c->base + SE_DMA_TX_IRQ_STAT);
	u32 dm_rx_st = readl_relaxed(gi2c->base + SE_DMA_RX_IRQ_STAT);
	u32 dma = readl_relaxed(gi2c->base + SE_GENI_DMA_MODE_EN);
	struct i2c_msg *cur = gi2c->cur;

	if (!cur) {
		geni_se_dump_dbg_regs(&gi2c->i2c_rsc, gi2c->base, gi2c->ipcl);
		GENI_SE_ERR(gi2c->ipcl, false, gi2c->dev, "Spurious irq\n");
		goto irqret;
	}

	if ((m_stat & M_CMD_FAILURE_EN) ||
		    (dm_rx_st & (DM_I2C_CB_ERR)) ||
		    (m_stat & M_CMD_CANCEL_EN) ||
		    (m_stat & M_CMD_ABORT_EN)) {

		if (m_stat & M_GP_IRQ_1_EN)
			geni_i2c_err(gi2c, I2C_NACK);
		if (m_stat & M_GP_IRQ_3_EN)
			geni_i2c_err(gi2c, I2C_BUS_PROTO);
		if (m_stat & M_GP_IRQ_4_EN)
			geni_i2c_err(gi2c, I2C_ARB_LOST);
=======
	int j;
	u32 m_stat;
	u32 rx_st;
	u32 dm_tx_st;
	u32 dm_rx_st;
	u32 dma;
	struct i2c_msg *cur;
	unsigned long flags;

	spin_lock_irqsave(&gi2c->lock, flags);
	m_stat = readl_relaxed(gi2c->se.base + SE_GENI_M_IRQ_STATUS);
	rx_st = readl_relaxed(gi2c->se.base + SE_GENI_RX_FIFO_STATUS);
	dm_tx_st = readl_relaxed(gi2c->se.base + SE_DMA_TX_IRQ_STAT);
	dm_rx_st = readl_relaxed(gi2c->se.base + SE_DMA_RX_IRQ_STAT);
	dma = readl_relaxed(gi2c->se.base + SE_GENI_DMA_MODE_EN);
	cur = gi2c->cur;

	if (!cur ||
	    m_stat & (M_CMD_FAILURE_EN | M_CMD_ABORT_EN) ||
	    dm_rx_st & (DM_I2C_CB_ERR)) {
		if (m_stat & M_GP_IRQ_1_EN)
			geni_i2c_err(gi2c, NACK);
		if (m_stat & M_GP_IRQ_3_EN)
			geni_i2c_err(gi2c, BUS_PROTO);
		if (m_stat & M_GP_IRQ_4_EN)
			geni_i2c_err(gi2c, ARB_LOST);
>>>>>>> rebase
		if (m_stat & M_CMD_OVERRUN_EN)
			geni_i2c_err(gi2c, GENI_OVERRUN);
		if (m_stat & M_ILLEGAL_CMD_EN)
			geni_i2c_err(gi2c, GENI_ILLEGAL_CMD);
		if (m_stat & M_CMD_ABORT_EN)
			geni_i2c_err(gi2c, GENI_ABORT_DONE);
		if (m_stat & M_GP_IRQ_0_EN)
			geni_i2c_err(gi2c, GP_IRQ0);

<<<<<<< HEAD
		if (!dma)
			writel_relaxed(0, (gi2c->base +
					   SE_GENI_TX_WATERMARK_REG));
		gi2c->cmd_done = true;
		goto irqret;
	}

	if (((m_stat & M_RX_FIFO_WATERMARK_EN) ||
		(m_stat & M_RX_FIFO_LAST_EN)) && (cur->flags & I2C_M_RD)) {
		u32 rxcnt = rx_st & RX_FIFO_WC_MSK;

		for (j = 0; j < rxcnt; j++) {
			u32 temp;
			int p;

			temp = readl_relaxed(gi2c->base + SE_GENI_RX_FIFOn);
			for (i = gi2c->cur_rd, p = 0; (i < cur->len && p < 4);
				i++, p++)
				cur->buf[i] = (u8) ((temp >> (p * 8)) & 0xff);
			gi2c->cur_rd = i;
			if (gi2c->cur_rd == cur->len) {
				dev_dbg(gi2c->dev, "FIFO i:%d,read 0x%x\n",
					i, temp);
				break;
			}
		}
	} else if ((m_stat & M_TX_FIFO_WATERMARK_EN) &&
					!(cur->flags & I2C_M_RD)) {
		for (j = 0; j < gi2c->tx_wm; j++) {
			u32 temp = 0;
			int p;

			for (i = gi2c->cur_wr, p = 0; (i < cur->len && p < 4);
				i++, p++)
				temp |= (((u32)(cur->buf[i]) << (p * 8)));
			writel_relaxed(temp, gi2c->base + SE_GENI_TX_FIFOn);
			gi2c->cur_wr = i;
			dev_dbg(gi2c->dev, "FIFO i:%d,wrote 0x%x\n", i, temp);
			if (gi2c->cur_wr == cur->len) {
				dev_dbg(gi2c->dev, "FIFO i2c bytes done writing\n");
				writel_relaxed(0,
				(gi2c->base + SE_GENI_TX_WATERMARK_REG));
=======
		/* Disable the TX Watermark interrupt to stop TX */
		if (!dma)
			writel_relaxed(0, gi2c->se.base +
					   SE_GENI_TX_WATERMARK_REG);
		goto irqret;
	}

	if (dma) {
		dev_dbg(gi2c->se.dev, "i2c dma tx:0x%x, dma rx:0x%x\n",
			dm_tx_st, dm_rx_st);
		goto irqret;
	}

	if (cur->flags & I2C_M_RD &&
	    m_stat & (M_RX_FIFO_WATERMARK_EN | M_RX_FIFO_LAST_EN)) {
		u32 rxcnt = rx_st & RX_FIFO_WC_MSK;

		for (j = 0; j < rxcnt; j++) {
			u32 val;
			int p = 0;

			val = readl_relaxed(gi2c->se.base + SE_GENI_RX_FIFOn);
			while (gi2c->cur_rd < cur->len && p < sizeof(val)) {
				cur->buf[gi2c->cur_rd++] = val & 0xff;
				val >>= 8;
				p++;
			}
			if (gi2c->cur_rd == cur->len)
				break;
		}
	} else if (!(cur->flags & I2C_M_RD) &&
		   m_stat & M_TX_FIFO_WATERMARK_EN) {
		for (j = 0; j < gi2c->tx_wm; j++) {
			u32 temp;
			u32 val = 0;
			int p = 0;

			while (gi2c->cur_wr < cur->len && p < sizeof(val)) {
				temp = cur->buf[gi2c->cur_wr++];
				val |= temp << (p * 8);
				p++;
			}
			writel_relaxed(val, gi2c->se.base + SE_GENI_TX_FIFOn);
			/* TX Complete, Disable the TX Watermark interrupt */
			if (gi2c->cur_wr == cur->len) {
				writel_relaxed(0, gi2c->se.base +
						SE_GENI_TX_WATERMARK_REG);
>>>>>>> rebase
				break;
			}
		}
	}
irqret:
	if (m_stat)
<<<<<<< HEAD
		writel_relaxed(m_stat, gi2c->base + SE_GENI_M_IRQ_CLEAR);

	if (dma) {
		if (dm_tx_st)
			writel_relaxed(dm_tx_st, gi2c->base +
				       SE_DMA_TX_IRQ_CLR);

		if (dm_rx_st)
			writel_relaxed(dm_rx_st, gi2c->base +
				       SE_DMA_RX_IRQ_CLR);
		/* Ensure all writes are done before returning from ISR. */
		wmb();

		if ((dm_tx_st & TX_DMA_DONE) || (dm_rx_st & RX_DMA_DONE))
			gi2c->cmd_done = true;
	}

	else if (m_stat & M_CMD_DONE_EN)
		gi2c->cmd_done = true;

	if (gi2c->cmd_done) {
		gi2c->cmd_done = false;
		complete(&gi2c->xfer);
	}

	return IRQ_HANDLED;
}

static void gi2c_ev_cb(struct dma_chan *ch, struct msm_gpi_cb const *cb_str,
		       void *ptr)
{
	struct geni_i2c_dev *gi2c = ptr;
	u32 m_stat = cb_str->status;

	switch (cb_str->cb_event) {
	case MSM_GPI_QUP_ERROR:
	case MSM_GPI_QUP_SW_ERROR:
	case MSM_GPI_QUP_MAX_EVENT:
		/* fall through to stall impacted channel */
	case MSM_GPI_QUP_CH_ERROR:
	case MSM_GPI_QUP_FW_ERROR:
	case MSM_GPI_QUP_PENDING_EVENT:
	case MSM_GPI_QUP_EOT_DESC_MISMATCH:
		break;
	case MSM_GPI_QUP_NOTIFY:
		if (m_stat & M_GP_IRQ_1_EN)
			geni_i2c_err(gi2c, I2C_NACK);
		if (m_stat & M_GP_IRQ_3_EN)
			geni_i2c_err(gi2c, I2C_BUS_PROTO);
		if (m_stat & M_GP_IRQ_4_EN)
			geni_i2c_err(gi2c, I2C_ARB_LOST);
		complete(&gi2c->xfer);
		break;
	default:
		break;
	}
	if (cb_str->cb_event != MSM_GPI_QUP_NOTIFY)
		GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
			"GSI QN err:0x%x, status:0x%x, err:%d\n",
			cb_str->error_log.error_code, m_stat,
			cb_str->cb_event);
}

static void gi2c_gsi_cb_err(struct msm_gpi_dma_async_tx_cb_param *cb,
								char *xfer)
{
	struct geni_i2c_dev *gi2c = cb->userdata;

	if (cb->status & DM_I2C_CB_ERR) {
		GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
			    "%s TCE Unexpected Err, stat:0x%x\n",
				xfer, cb->status);
		if (cb->status & (BIT(GP_IRQ1) << 5))
			geni_i2c_err(gi2c, I2C_NACK);
		if (cb->status & (BIT(GP_IRQ3) << 5))
			geni_i2c_err(gi2c, I2C_BUS_PROTO);
		if (cb->status & (BIT(GP_IRQ4) << 5))
			geni_i2c_err(gi2c, I2C_ARB_LOST);
	}
}

static void gi2c_gsi_tx_cb(void *ptr)
{
	struct msm_gpi_dma_async_tx_cb_param *tx_cb = ptr;
	struct geni_i2c_dev *gi2c = tx_cb->userdata;

	gi2c_gsi_cb_err(tx_cb, "TX");
	complete(&gi2c->xfer);
}

static void gi2c_gsi_rx_cb(void *ptr)
{
	struct msm_gpi_dma_async_tx_cb_param *rx_cb = ptr;
	struct geni_i2c_dev *gi2c = rx_cb->userdata;

	if (gi2c->cur->flags & I2C_M_RD) {
		gi2c_gsi_cb_err(rx_cb, "RX");
		complete(&gi2c->xfer);
	}
}

static int geni_i2c_gsi_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[],
			     int num)
{
	struct geni_i2c_dev *gi2c = i2c_get_adapdata(adap);
	int i, ret = 0, timeout = 0;

	if (!gi2c->tx_c) {
		gi2c->tx_c = dma_request_slave_channel(gi2c->dev, "tx");
		if (!gi2c->tx_c) {
			GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
				    "tx dma req slv chan ret :%d\n", ret);
			ret = -EIO;
			goto geni_i2c_gsi_xfer_out;
		}
		gi2c->tx_ev.init.callback = gi2c_ev_cb;
		gi2c->tx_ev.init.cb_param = gi2c;
		gi2c->tx_ev.cmd = MSM_GPI_INIT;
		gi2c->tx_c->private = &gi2c->tx_ev;
		ret = dmaengine_slave_config(gi2c->tx_c, NULL);
		if (ret) {
			GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
				    "tx dma slave config ret :%d\n", ret);
			goto geni_i2c_gsi_xfer_out;
		}
	}

	if (!gi2c->rx_c) {
		gi2c->rx_c = dma_request_slave_channel(gi2c->dev, "rx");
		if (!gi2c->rx_c) {
			GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
				    "rx dma req slv chan ret :%d\n", ret);
			ret = -EIO;
			goto geni_i2c_gsi_xfer_out;
		}
		gi2c->rx_ev.init.cb_param = gi2c;
		gi2c->rx_ev.init.callback = gi2c_ev_cb;
		gi2c->rx_ev.cmd = MSM_GPI_INIT;
		gi2c->rx_c->private = &gi2c->rx_ev;
		ret = dmaengine_slave_config(gi2c->rx_c, NULL);
		if (ret) {
			GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
				    "rx dma slave config ret :%d\n", ret);
			goto geni_i2c_gsi_xfer_out;
		}
	}

	if (gi2c->is_shared) {
		struct msm_gpi_tre *lock_t = &gi2c->lock_t;
		struct msm_gpi_tre *unlock_t = &gi2c->unlock_t;

		/* lock */
		lock_t->dword[0] = MSM_GPI_LOCK_TRE_DWORD0;
		lock_t->dword[1] = MSM_GPI_LOCK_TRE_DWORD1;
		lock_t->dword[2] = MSM_GPI_LOCK_TRE_DWORD2;
		lock_t->dword[3] = MSM_GPI_LOCK_TRE_DWORD3(0, 0, 0, 0, 1);

		/* unlock tre: ieob set */
		unlock_t->dword[0] = MSM_GPI_UNLOCK_TRE_DWORD0;
		unlock_t->dword[1] = MSM_GPI_UNLOCK_TRE_DWORD1;
		unlock_t->dword[2] = MSM_GPI_UNLOCK_TRE_DWORD2;
		unlock_t->dword[3] = MSM_GPI_UNLOCK_TRE_DWORD3(0, 0, 0, 1, 0);
	}

	if (!gi2c->cfg_sent) {
		struct geni_i2c_clk_fld *itr = geni_i2c_clk_map +
							gi2c->clk_fld_idx;
		struct msm_gpi_tre *cfg0 = &gi2c->cfg0_t;

		/* config0 */
		cfg0->dword[0] = MSM_GPI_I2C_CONFIG0_TRE_DWORD0(I2C_PACK_EN,
								itr->t_cycle,
								itr->t_high,
								itr->t_low);
		cfg0->dword[1] = MSM_GPI_I2C_CONFIG0_TRE_DWORD1(0, 0);
		cfg0->dword[2] = MSM_GPI_I2C_CONFIG0_TRE_DWORD2(0,
								itr->clk_div);
		cfg0->dword[3] = MSM_GPI_I2C_CONFIG0_TRE_DWORD3(0, 0, 0, 0, 1);

		gi2c->tx_cb.userdata = gi2c;
		gi2c->rx_cb.userdata = gi2c;
	}

	for (i = 0; i < num; i++) {
		u8 op = (msgs[i].flags & I2C_M_RD) ? 2 : 1;
		int segs = 3 - op;
		int index = 0;
		u8 *dma_buf = NULL;
		int stretch = (i < (num - 1));
		dma_cookie_t tx_cookie, rx_cookie;
		struct msm_gpi_tre *go_t = &gi2c->go_t;
		struct device *rx_dev = gi2c->wrapper_dev;
		struct device *tx_dev = gi2c->wrapper_dev;

		reinit_completion(&gi2c->xfer);

		gi2c->cur = &msgs[i];

		dma_buf = i2c_get_dma_safe_msg_buf(&msgs[i], 1);
		if (!dma_buf) {
			ret = -ENOMEM;
			goto geni_i2c_gsi_xfer_out;
		}

		qcom_geni_i2c_calc_timeout(gi2c);

		if (!gi2c->cfg_sent)
			segs++;
		if (gi2c->is_shared && (i == 0 || i == num-1)) {
			segs++;
			if (num == 1)
				segs++;
			sg_init_table(gi2c->tx_sg, segs);
			if (i == 0)
				/* Send lock tre for first transfer in a msg */
				sg_set_buf(&gi2c->tx_sg[index++], &gi2c->lock_t,
					sizeof(gi2c->lock_t));
		} else {
			sg_init_table(gi2c->tx_sg, segs);
		}

		/* Send cfg tre when cfg not sent already */
		if (!gi2c->cfg_sent) {
			sg_set_buf(&gi2c->tx_sg[index++], &gi2c->cfg0_t,
						sizeof(gi2c->cfg0_t));
			gi2c->cfg_sent = 1;
		}

		go_t->dword[0] = MSM_GPI_I2C_GO_TRE_DWORD0((stretch << 2),
							   msgs[i].addr, op);
		go_t->dword[1] = MSM_GPI_I2C_GO_TRE_DWORD1;

		if (msgs[i].flags & I2C_M_RD) {
			go_t->dword[2] = MSM_GPI_I2C_GO_TRE_DWORD2(msgs[i].len);
			/*
			 * For Rx Go tre: Set ieob for non-shared se and for all
			 * but last transfer in shared se
			 */
			if (!gi2c->is_shared || (gi2c->is_shared && i != num-1))
				go_t->dword[3] = MSM_GPI_I2C_GO_TRE_DWORD3(1, 0,
								0, 1, 0);
			else
				go_t->dword[3] = MSM_GPI_I2C_GO_TRE_DWORD3(1, 0,
								0, 0, 0);
		} else {
			/* For Tx Go tre: ieob is not set, chain bit is set */
			go_t->dword[2] = MSM_GPI_I2C_GO_TRE_DWORD2(0);
			go_t->dword[3] = MSM_GPI_I2C_GO_TRE_DWORD3(0, 0, 0, 0,
								1);
		}

		sg_set_buf(&gi2c->tx_sg[index++], &gi2c->go_t,
						  sizeof(gi2c->go_t));

		if (msgs[i].flags & I2C_M_RD) {
			GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
				"msg[%d].len:%d R\n", i, gi2c->cur->len);
			sg_init_table(&gi2c->rx_sg, 1);
			ret = geni_se_iommu_map_buf(rx_dev, &gi2c->rx_ph,
						dma_buf, msgs[i].len,
						DMA_FROM_DEVICE);
			if (ret) {
				GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
					    "geni_se_iommu_map_buf for rx failed :%d\n",
					    ret);
				i2c_put_dma_safe_msg_buf(dma_buf, &msgs[i],
								false);
				goto geni_i2c_gsi_xfer_out;

			} else if (gi2c->dbg_buf_ptr) {
				gi2c->dbg_buf_ptr[i].virt_buf =
							(void *)dma_buf;
				gi2c->dbg_buf_ptr[i].map_buf =
							(void *)&gi2c->rx_ph;
			}
			gi2c->rx_t.dword[0] =
				MSM_GPI_DMA_W_BUFFER_TRE_DWORD0(gi2c->rx_ph);
			gi2c->rx_t.dword[1] =
				MSM_GPI_DMA_W_BUFFER_TRE_DWORD1(gi2c->rx_ph);
			gi2c->rx_t.dword[2] =
				MSM_GPI_DMA_W_BUFFER_TRE_DWORD2(msgs[i].len);
			/* Set ieot for all Rx/Tx DMA tres */
			gi2c->rx_t.dword[3] =
				MSM_GPI_DMA_W_BUFFER_TRE_DWORD3(0, 0, 1, 0, 0);

			sg_set_buf(&gi2c->rx_sg, &gi2c->rx_t,
						 sizeof(gi2c->rx_t));
			gi2c->rx_desc = dmaengine_prep_slave_sg(gi2c->rx_c,
							&gi2c->rx_sg, 1,
							DMA_DEV_TO_MEM,
							(DMA_PREP_INTERRUPT |
							 DMA_CTRL_ACK));
			if (!gi2c->rx_desc) {
				GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
					    "prep_slave_sg for rx failed\n");
				gi2c->err = -ENOMEM;
				goto geni_i2c_err_prep_sg;
			}
			gi2c->rx_desc->callback = gi2c_gsi_rx_cb;
			gi2c->rx_desc->callback_param = &gi2c->rx_cb;

			/* Issue RX */
			rx_cookie = dmaengine_submit(gi2c->rx_desc);
			dma_async_issue_pending(gi2c->rx_c);
		} else {
			GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
				"msg[%d].len:%d W\n", i, gi2c->cur->len);
			ret = geni_se_iommu_map_buf(tx_dev, &gi2c->tx_ph,
						dma_buf, msgs[i].len,
						DMA_TO_DEVICE);
			if (ret) {
				GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
					    "geni_se_iommu_map_buf for tx failed :%d\n",
					    ret);
				i2c_put_dma_safe_msg_buf(dma_buf, &msgs[i],
								false);
				goto geni_i2c_gsi_xfer_out;

			} else if (gi2c->dbg_buf_ptr) {
				gi2c->dbg_buf_ptr[i].virt_buf =
							(void *)dma_buf;
				gi2c->dbg_buf_ptr[i].map_buf =
							(void *)&gi2c->tx_ph;
			}

			gi2c->tx_t.dword[0] =
				MSM_GPI_DMA_W_BUFFER_TRE_DWORD0(gi2c->tx_ph);
			gi2c->tx_t.dword[1] =
				MSM_GPI_DMA_W_BUFFER_TRE_DWORD1(gi2c->tx_ph);
			gi2c->tx_t.dword[2] =
				MSM_GPI_DMA_W_BUFFER_TRE_DWORD2(msgs[i].len);
			if (gi2c->is_shared && i == num-1)
				/*
				 * For Tx: unlock tre is send for last transfer
				 * so set chain bit for last transfer DMA tre.
				 */
				gi2c->tx_t.dword[3] =
				MSM_GPI_DMA_W_BUFFER_TRE_DWORD3(0, 0, 1, 0, 1);
			else
				gi2c->tx_t.dword[3] =
				MSM_GPI_DMA_W_BUFFER_TRE_DWORD3(0, 0, 1, 0, 0);

			sg_set_buf(&gi2c->tx_sg[index++], &gi2c->tx_t,
							  sizeof(gi2c->tx_t));
		}

		if (gi2c->is_shared && i == num-1) {
			/* Send unlock tre at the end of last transfer */
			sg_set_buf(&gi2c->tx_sg[index++],
				&gi2c->unlock_t, sizeof(gi2c->unlock_t));
		}

		gi2c->tx_desc = dmaengine_prep_slave_sg(gi2c->tx_c, gi2c->tx_sg,
						segs, DMA_MEM_TO_DEV,
						(DMA_PREP_INTERRUPT |
						 DMA_CTRL_ACK));
		if (!gi2c->tx_desc) {
			GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
				    "prep_slave_sg for tx failed\n");
			gi2c->err = -ENOMEM;
			goto geni_i2c_err_prep_sg;
		}
		gi2c->tx_desc->callback = gi2c_gsi_tx_cb;
		gi2c->tx_desc->callback_param = &gi2c->tx_cb;

		/* Issue TX */
		tx_cookie = dmaengine_submit(gi2c->tx_desc);
		dma_async_issue_pending(gi2c->tx_c);

		timeout = wait_for_completion_timeout(&gi2c->xfer,
						gi2c->xfer_timeout);
		if (!timeout) {
			GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
				"I2C gsi xfer timeout:%u flags:%d addr:0x%x\n",
				gi2c->xfer_timeout, gi2c->cur->flags,
				gi2c->cur->addr);
			geni_se_dump_dbg_regs(&gi2c->i2c_rsc, gi2c->base,
						gi2c->ipcl);
			gi2c->err = -ETIMEDOUT;
		}
geni_i2c_err_prep_sg:
		if (gi2c->err) {
			dmaengine_terminate_all(gi2c->tx_c);
			gi2c->cfg_sent = 0;
		}
		if (gi2c->is_shared)
			/* Resend cfg tre for every new message on shared se */
			gi2c->cfg_sent = 0;

		if (msgs[i].flags & I2C_M_RD)
			geni_se_iommu_unmap_buf(rx_dev, &gi2c->rx_ph,
				msgs[i].len, DMA_FROM_DEVICE);
		else
			geni_se_iommu_unmap_buf(tx_dev, &gi2c->tx_ph,
				msgs[i].len, DMA_TO_DEVICE);
		i2c_put_dma_safe_msg_buf(dma_buf, &msgs[i], !gi2c->err);
		if (gi2c->err)
			goto geni_i2c_gsi_xfer_out;
	}

geni_i2c_gsi_xfer_out:
	if (!ret && gi2c->err)
		ret = gi2c->err;
	return ret;
=======
		writel_relaxed(m_stat, gi2c->se.base + SE_GENI_M_IRQ_CLEAR);

	if (dma) {
		if (dm_tx_st)
			writel_relaxed(dm_tx_st, gi2c->se.base +
						SE_DMA_TX_IRQ_CLR);
		if (dm_rx_st)
			writel_relaxed(dm_rx_st, gi2c->se.base +
						SE_DMA_RX_IRQ_CLR);
	}
	/* if this is err with done-bit not set, handle that through timeout. */
	if (m_stat & M_CMD_DONE_EN || m_stat & M_CMD_ABORT_EN)
		complete(&gi2c->done);
	else if (dm_tx_st & TX_DMA_DONE || dm_tx_st & TX_RESET_DONE)
		complete(&gi2c->done);
	else if (dm_rx_st & RX_DMA_DONE || dm_rx_st & RX_RESET_DONE)
		complete(&gi2c->done);

	spin_unlock_irqrestore(&gi2c->lock, flags);
	return IRQ_HANDLED;
}

static void geni_i2c_abort_xfer(struct geni_i2c_dev *gi2c)
{
	u32 val;
	unsigned long time_left = ABORT_TIMEOUT;
	unsigned long flags;

	spin_lock_irqsave(&gi2c->lock, flags);
	geni_i2c_err(gi2c, GENI_TIMEOUT);
	gi2c->cur = NULL;
	geni_se_abort_m_cmd(&gi2c->se);
	spin_unlock_irqrestore(&gi2c->lock, flags);
	do {
		time_left = wait_for_completion_timeout(&gi2c->done, time_left);
		val = readl_relaxed(gi2c->se.base + SE_GENI_M_IRQ_STATUS);
	} while (!(val & M_CMD_ABORT_EN) && time_left);

	if (!(val & M_CMD_ABORT_EN))
		dev_err(gi2c->se.dev, "Timeout abort_m_cmd\n");
}

static void geni_i2c_rx_fsm_rst(struct geni_i2c_dev *gi2c)
{
	u32 val;
	unsigned long time_left = RST_TIMEOUT;

	writel_relaxed(1, gi2c->se.base + SE_DMA_RX_FSM_RST);
	do {
		time_left = wait_for_completion_timeout(&gi2c->done, time_left);
		val = readl_relaxed(gi2c->se.base + SE_DMA_RX_IRQ_STAT);
	} while (!(val & RX_RESET_DONE) && time_left);

	if (!(val & RX_RESET_DONE))
		dev_err(gi2c->se.dev, "Timeout resetting RX_FSM\n");
}

static void geni_i2c_tx_fsm_rst(struct geni_i2c_dev *gi2c)
{
	u32 val;
	unsigned long time_left = RST_TIMEOUT;

	writel_relaxed(1, gi2c->se.base + SE_DMA_TX_FSM_RST);
	do {
		time_left = wait_for_completion_timeout(&gi2c->done, time_left);
		val = readl_relaxed(gi2c->se.base + SE_DMA_TX_IRQ_STAT);
	} while (!(val & TX_RESET_DONE) && time_left);

	if (!(val & TX_RESET_DONE))
		dev_err(gi2c->se.dev, "Timeout resetting TX_FSM\n");
}

static int geni_i2c_rx_one_msg(struct geni_i2c_dev *gi2c, struct i2c_msg *msg,
				u32 m_param)
{
	dma_addr_t rx_dma;
	enum geni_se_xfer_mode mode;
	unsigned long time_left = XFER_TIMEOUT;
	void *dma_buf;

	gi2c->cur = msg;
	mode = GENI_SE_FIFO;
	dma_buf = i2c_get_dma_safe_msg_buf(msg, 32);
	if (dma_buf)
		mode = GENI_SE_DMA;

	geni_se_select_mode(&gi2c->se, mode);
	writel_relaxed(msg->len, gi2c->se.base + SE_I2C_RX_TRANS_LEN);
	geni_se_setup_m_cmd(&gi2c->se, I2C_READ, m_param);
	if (mode == GENI_SE_DMA) {
		int ret;

		ret = geni_se_rx_dma_prep(&gi2c->se, dma_buf, msg->len,
								&rx_dma);
		if (ret) {
			mode = GENI_SE_FIFO;
			geni_se_select_mode(&gi2c->se, mode);
			i2c_put_dma_safe_msg_buf(dma_buf, msg, false);
		}
	}

	time_left = wait_for_completion_timeout(&gi2c->done, XFER_TIMEOUT);
	if (!time_left)
		geni_i2c_abort_xfer(gi2c);

	gi2c->cur_rd = 0;
	if (mode == GENI_SE_DMA) {
		if (gi2c->err)
			geni_i2c_rx_fsm_rst(gi2c);
		geni_se_rx_dma_unprep(&gi2c->se, rx_dma, msg->len);
		i2c_put_dma_safe_msg_buf(dma_buf, msg, !gi2c->err);
	}
	return gi2c->err;
}

static int geni_i2c_tx_one_msg(struct geni_i2c_dev *gi2c, struct i2c_msg *msg,
				u32 m_param)
{
	dma_addr_t tx_dma;
	enum geni_se_xfer_mode mode;
	unsigned long time_left;
	void *dma_buf;

	gi2c->cur = msg;
	mode = GENI_SE_FIFO;
	dma_buf = i2c_get_dma_safe_msg_buf(msg, 32);
	if (dma_buf)
		mode = GENI_SE_DMA;

	geni_se_select_mode(&gi2c->se, mode);
	writel_relaxed(msg->len, gi2c->se.base + SE_I2C_TX_TRANS_LEN);
	geni_se_setup_m_cmd(&gi2c->se, I2C_WRITE, m_param);
	if (mode == GENI_SE_DMA) {
		int ret;

		ret = geni_se_tx_dma_prep(&gi2c->se, dma_buf, msg->len,
								&tx_dma);
		if (ret) {
			mode = GENI_SE_FIFO;
			geni_se_select_mode(&gi2c->se, mode);
			i2c_put_dma_safe_msg_buf(dma_buf, msg, false);
		}
	}

	if (mode == GENI_SE_FIFO) /* Get FIFO IRQ */
		writel_relaxed(1, gi2c->se.base + SE_GENI_TX_WATERMARK_REG);

	time_left = wait_for_completion_timeout(&gi2c->done, XFER_TIMEOUT);
	if (!time_left)
		geni_i2c_abort_xfer(gi2c);

	gi2c->cur_wr = 0;
	if (mode == GENI_SE_DMA) {
		if (gi2c->err)
			geni_i2c_tx_fsm_rst(gi2c);
		geni_se_tx_dma_unprep(&gi2c->se, tx_dma, msg->len);
		i2c_put_dma_safe_msg_buf(dma_buf, msg, !gi2c->err);
	}
	return gi2c->err;
>>>>>>> rebase
}

static int geni_i2c_xfer(struct i2c_adapter *adap,
			 struct i2c_msg msgs[],
			 int num)
{
	struct geni_i2c_dev *gi2c = i2c_get_adapdata(adap);
<<<<<<< HEAD
	int i, ret = 0, timeout = 0;

	gi2c->err = 0;

	/* Client to respect system suspend */
	if (!pm_runtime_enabled(gi2c->dev)) {
		GENI_SE_ERR(gi2c->ipcl, false, gi2c->dev,
			"%s: System suspended\n", __func__);
		return -EACCES;
	}

	ret = pm_runtime_get_sync(gi2c->dev);
	if (ret < 0) {
		GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
			    "error turning SE resources:%d\n", ret);
		pm_runtime_put_noidle(gi2c->dev);
		/* Set device in suspended since resume failed */
		pm_runtime_set_suspended(gi2c->dev);
		return ret;
	}

	GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
		"n:%d addr:0x%x\n", num, msgs[0].addr);

	gi2c->dbg_num = num;
	kfree(gi2c->dbg_buf_ptr);
	gi2c->dbg_buf_ptr =
		kcalloc(num, sizeof(struct dbg_buf_ctxt), GFP_KERNEL);
	if (!gi2c->dbg_buf_ptr)
		GENI_SE_ERR(gi2c->ipcl, false, gi2c->dev,
			"Buf logging pointer not available\n");

	if (gi2c->se_mode == GSI_ONLY) {
		ret = geni_i2c_gsi_xfer(adap, msgs, num);
		goto geni_i2c_txn_ret;
	} else {
		/* Don't set shared flag in non-GSI mode */
		gi2c->is_shared = false;
	}

	for (i = 0; i < num; i++) {
		int stretch = (i < (num - 1));
		u32 m_param = 0;
		u32 m_cmd = 0;
		u8 *dma_buf = NULL;
		dma_addr_t tx_dma = 0;
		dma_addr_t rx_dma = 0;
		enum se_xfer_mode mode = FIFO_MODE;
		reinit_completion(&gi2c->xfer);

		m_param |= (stretch ? STOP_STRETCH : 0);
		m_param |= ((msgs[i].addr & 0x7F) << SLV_ADDR_SHFT);

		gi2c->cur = &msgs[i];
		qcom_geni_i2c_calc_timeout(gi2c);
		mode = msgs[i].len > 32 ? SE_DMA : FIFO_MODE;

		ret = geni_se_select_mode(gi2c->base, mode);
		if (ret) {
			dev_err(gi2c->dev, "%s: Error mode init %d:%d:%d\n",
				__func__, mode, i, msgs[i].len);
			break;
		}

		if (mode == SE_DMA) {
			dma_buf = i2c_get_dma_safe_msg_buf(&msgs[i], 1);
			if (!dma_buf) {
				ret = -ENOMEM;
				goto geni_i2c_txn_ret;
			}
		}

		if (msgs[i].flags & I2C_M_RD) {
			GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
				"msgs[%d].len:%d R\n", i, msgs[i].len);
			geni_write_reg(msgs[i].len,
				       gi2c->base, SE_I2C_RX_TRANS_LEN);
			m_cmd = I2C_READ;
			geni_setup_m_cmd(gi2c->base, m_cmd, m_param);
			if (mode == SE_DMA) {
				ret = geni_se_rx_dma_prep(gi2c->wrapper_dev,
							gi2c->base, dma_buf,
							msgs[i].len, &rx_dma);
				if (ret) {
					i2c_put_dma_safe_msg_buf(dma_buf,
							&msgs[i], false);
					mode = FIFO_MODE;
					ret = geni_se_select_mode(gi2c->base,
								  mode);
				} else if (gi2c->dbg_buf_ptr) {
					gi2c->dbg_buf_ptr[i].virt_buf =
								(void *)dma_buf;
					gi2c->dbg_buf_ptr[i].map_buf =
								(void *)&rx_dma;
				}
			}
		} else {
			GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
				"msgs[%d].len:%d W\n", i, msgs[i].len);
			geni_write_reg(msgs[i].len, gi2c->base,
						SE_I2C_TX_TRANS_LEN);
			m_cmd = I2C_WRITE;
			geni_setup_m_cmd(gi2c->base, m_cmd, m_param);
			if (mode == SE_DMA) {
				ret = geni_se_tx_dma_prep(gi2c->wrapper_dev,
							gi2c->base, dma_buf,
							msgs[i].len, &tx_dma);
				if (ret) {
					i2c_put_dma_safe_msg_buf(dma_buf,
							&msgs[i], false);
					mode = FIFO_MODE;
					ret = geni_se_select_mode(gi2c->base,
								  mode);
				} else if (gi2c->dbg_buf_ptr) {
					gi2c->dbg_buf_ptr[i].virt_buf =
								(void *)dma_buf;
					gi2c->dbg_buf_ptr[i].map_buf =
								(void *)&tx_dma;
				}
			}
			if (mode == FIFO_MODE) /* Get FIFO IRQ */
				geni_write_reg(1, gi2c->base,
						SE_GENI_TX_WATERMARK_REG);
		}
		/* Ensure FIFO write go through before waiting for Done evet */
		mb();
		timeout = wait_for_completion_timeout(&gi2c->xfer,
						gi2c->xfer_timeout);
		if (!timeout) {
			GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
				"I2C xfer timeout: %d\n", gi2c->xfer_timeout);
			geni_i2c_err(gi2c, GENI_TIMEOUT);
		}

		if (gi2c->err) {
			reinit_completion(&gi2c->xfer);
			geni_cancel_m_cmd(gi2c->base);
			timeout = wait_for_completion_timeout(&gi2c->xfer, HZ);
			if (!timeout) {
				GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
					"Cancel failed\n");
				reinit_completion(&gi2c->xfer);
				geni_abort_m_cmd(gi2c->base);
				timeout =
				wait_for_completion_timeout(&gi2c->xfer, HZ);
				if (!timeout)
					GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
						"Abort failed\n");
			}
		}
		gi2c->cur_wr = 0;
		gi2c->cur_rd = 0;

		if (mode == SE_DMA) {
			if (gi2c->err) {
				reinit_completion(&gi2c->xfer);
				if (msgs[i].flags != I2C_M_RD)
					writel_relaxed(1, gi2c->base +
							SE_DMA_TX_FSM_RST);
				else
					writel_relaxed(1, gi2c->base +
							SE_DMA_RX_FSM_RST);
				wait_for_completion_timeout(&gi2c->xfer, HZ);
			}
			geni_se_rx_dma_unprep(gi2c->wrapper_dev, rx_dma,
					      msgs[i].len);
			geni_se_tx_dma_unprep(gi2c->wrapper_dev, tx_dma,
					      msgs[i].len);
			i2c_put_dma_safe_msg_buf(dma_buf, &msgs[i], !gi2c->err);
		}

		ret = gi2c->err;
		if (gi2c->err) {
			GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
				"i2c error :%d\n", gi2c->err);
			break;
		}
	}
geni_i2c_txn_ret:
	if (ret == 0)
		ret = num;

	pm_runtime_mark_last_busy(gi2c->dev);
	pm_runtime_put_autosuspend(gi2c->dev);
	gi2c->cur = NULL;
	gi2c->err = 0;
	GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
			"i2c txn ret:%d\n", ret);
=======
	int i, ret;

	gi2c->err = 0;
	reinit_completion(&gi2c->done);
	ret = pm_runtime_get_sync(gi2c->se.dev);
	if (ret < 0) {
		dev_err(gi2c->se.dev, "error turning SE resources:%d\n", ret);
		pm_runtime_put_noidle(gi2c->se.dev);
		/* Set device in suspended since resume failed */
		pm_runtime_set_suspended(gi2c->se.dev);
		return ret;
	}

	qcom_geni_i2c_conf(gi2c);
	for (i = 0; i < num; i++) {
		u32 m_param = i < (num - 1) ? STOP_STRETCH : 0;

		m_param |= ((msgs[i].addr << SLV_ADDR_SHFT) & SLV_ADDR_MSK);

		if (msgs[i].flags & I2C_M_RD)
			ret = geni_i2c_rx_one_msg(gi2c, &msgs[i], m_param);
		else
			ret = geni_i2c_tx_one_msg(gi2c, &msgs[i], m_param);

		if (ret)
			break;
	}
	if (ret == 0)
		ret = num;

	pm_runtime_mark_last_busy(gi2c->se.dev);
	pm_runtime_put_autosuspend(gi2c->se.dev);
	gi2c->cur = NULL;
	gi2c->err = 0;
>>>>>>> rebase
	return ret;
}

static u32 geni_i2c_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | (I2C_FUNC_SMBUS_EMUL & ~I2C_FUNC_SMBUS_QUICK);
}

static const struct i2c_algorithm geni_i2c_algo = {
	.master_xfer	= geni_i2c_xfer,
	.functionality	= geni_i2c_func,
};

static int geni_i2c_probe(struct platform_device *pdev)
{
	struct geni_i2c_dev *gi2c;
	struct resource *res;
<<<<<<< HEAD
	struct platform_device *wrapper_pdev;
	struct device_node *wrapper_ph_node;
=======
	u32 proto, tx_depth;
>>>>>>> rebase
	int ret;

	gi2c = devm_kzalloc(&pdev->dev, sizeof(*gi2c), GFP_KERNEL);
	if (!gi2c)
		return -ENOMEM;

<<<<<<< HEAD
	if (arr_idx++ < MAX_SE)
		/* Debug purpose */
		gi2c_dev_dbg[arr_idx] = gi2c;

	gi2c->dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -EINVAL;

	wrapper_ph_node = of_parse_phandle(pdev->dev.of_node,
				"qcom,wrapper-core", 0);
	if (IS_ERR_OR_NULL(wrapper_ph_node)) {
		ret = PTR_ERR(wrapper_ph_node);
		dev_err(&pdev->dev, "No wrapper core defined\n");
		return ret;
	}
	wrapper_pdev = of_find_device_by_node(wrapper_ph_node);
	of_node_put(wrapper_ph_node);
	if (IS_ERR_OR_NULL(wrapper_pdev)) {
		ret = PTR_ERR(wrapper_pdev);
		dev_err(&pdev->dev, "Cannot retrieve wrapper device\n");
		return ret;
	}
	gi2c->wrapper_dev = &wrapper_pdev->dev;
	gi2c->i2c_rsc.wrapper_dev = &wrapper_pdev->dev;
	ret = geni_se_resources_init(&gi2c->i2c_rsc, I2C_CORE2X_VOTE,
				     (DEFAULT_SE_CLK * DEFAULT_BUS_WIDTH));
	if (ret) {
		dev_err(gi2c->dev, "geni_se_resources_init\n");
		return ret;
	}

	gi2c->i2c_rsc.ctrl_dev = gi2c->dev;
	gi2c->i2c_rsc.se_clk = devm_clk_get(&pdev->dev, "se-clk");
	if (IS_ERR(gi2c->i2c_rsc.se_clk)) {
		ret = PTR_ERR(gi2c->i2c_rsc.se_clk);
		dev_err(&pdev->dev, "Err getting SE Core clk %d\n", ret);
		return ret;
	}
	gi2c->i2c_rsc.m_ahb_clk = devm_clk_get(&pdev->dev, "m-ahb");
	if (IS_ERR(gi2c->i2c_rsc.m_ahb_clk)) {
		ret = PTR_ERR(gi2c->i2c_rsc.m_ahb_clk);
		dev_err(&pdev->dev, "Err getting M AHB clk %d\n", ret);
		return ret;
	}
	gi2c->i2c_rsc.s_ahb_clk = devm_clk_get(&pdev->dev, "s-ahb");
	if (IS_ERR(gi2c->i2c_rsc.s_ahb_clk)) {
		ret = PTR_ERR(gi2c->i2c_rsc.s_ahb_clk);
		dev_err(&pdev->dev, "Err getting S AHB clk %d\n", ret);
		return ret;
	}

	gi2c->base = devm_ioremap_resource(gi2c->dev, res);
	if (IS_ERR(gi2c->base))
		return PTR_ERR(gi2c->base);

	gi2c->i2c_rsc.geni_pinctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR_OR_NULL(gi2c->i2c_rsc.geni_pinctrl)) {
		dev_err(&pdev->dev, "No pinctrl config specified\n");
		ret = PTR_ERR(gi2c->i2c_rsc.geni_pinctrl);
		return ret;
	}
	gi2c->i2c_rsc.geni_gpio_active =
		pinctrl_lookup_state(gi2c->i2c_rsc.geni_pinctrl,
							PINCTRL_DEFAULT);
	if (IS_ERR_OR_NULL(gi2c->i2c_rsc.geni_gpio_active)) {
		dev_err(&pdev->dev, "No default config specified\n");
		ret = PTR_ERR(gi2c->i2c_rsc.geni_gpio_active);
		return ret;
	}
	gi2c->i2c_rsc.geni_gpio_sleep =
		pinctrl_lookup_state(gi2c->i2c_rsc.geni_pinctrl,
							PINCTRL_SLEEP);
	if (IS_ERR_OR_NULL(gi2c->i2c_rsc.geni_gpio_sleep)) {
		dev_err(&pdev->dev, "No sleep config specified\n");
		ret = PTR_ERR(gi2c->i2c_rsc.geni_gpio_sleep);
		return ret;
	}

	if (of_property_read_bool(pdev->dev.of_node, "qcom,shared")) {
		gi2c->is_shared = true;
		dev_info(&pdev->dev, "Multi-EE usecase\n");
	}

	if (of_property_read_u32(pdev->dev.of_node, "qcom,clk-freq-out",
				&gi2c->i2c_rsc.clk_freq_out)) {
		gi2c->i2c_rsc.clk_freq_out = KHz(400);
	}
	dev_info(&pdev->dev, "Bus frequency is set to %dHz\n",
					gi2c->i2c_rsc.clk_freq_out);

	gi2c->irq = platform_get_irq(pdev, 0);
	if (gi2c->irq < 0) {
		dev_err(gi2c->dev, "IRQ error for i2c-geni\n");
=======
	gi2c->se.dev = &pdev->dev;
	gi2c->se.wrapper = dev_get_drvdata(pdev->dev.parent);
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gi2c->se.base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(gi2c->se.base))
		return PTR_ERR(gi2c->se.base);

	gi2c->se.clk = devm_clk_get(&pdev->dev, "se");
	if (IS_ERR(gi2c->se.clk)) {
		ret = PTR_ERR(gi2c->se.clk);
		dev_err(&pdev->dev, "Err getting SE Core clk %d\n", ret);
		return ret;
	}

	ret = device_property_read_u32(&pdev->dev, "clock-frequency",
							&gi2c->clk_freq_out);
	if (ret) {
		dev_info(&pdev->dev,
			"Bus frequency not specified, default to 100kHz.\n");
		gi2c->clk_freq_out = KHZ(100);
	}

	gi2c->irq = platform_get_irq(pdev, 0);
	if (gi2c->irq < 0) {
		dev_err(&pdev->dev, "IRQ error for i2c-geni\n");
>>>>>>> rebase
		return gi2c->irq;
	}

	ret = geni_i2c_clk_map_idx(gi2c);
	if (ret) {
<<<<<<< HEAD
		dev_err(gi2c->dev, "Invalid clk frequency %d KHz: %d\n",
				gi2c->i2c_rsc.clk_freq_out, ret);
		return ret;
	}

	ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));
	if (ret) {
		ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
		if (ret) {
			dev_err(&pdev->dev, "could not set DMA mask\n");
			return ret;
		}
	}

	gi2c->adap.algo = &geni_i2c_algo;
	init_completion(&gi2c->xfer);
	platform_set_drvdata(pdev, gi2c);
	ret = devm_request_irq(gi2c->dev, gi2c->irq, geni_i2c_irq,
			       IRQF_TRIGGER_HIGH, "i2c_geni", gi2c);
	if (ret) {
		dev_err(gi2c->dev, "Request_irq failed:%d: err:%d\n",
				   gi2c->irq, ret);
		return ret;
	}

	disable_irq(gi2c->irq);
	i2c_set_adapdata(&gi2c->adap, gi2c);
	gi2c->adap.dev.parent = gi2c->dev;
	gi2c->adap.dev.of_node = pdev->dev.of_node;

	strlcpy(gi2c->adap.name, "Geni-I2C", sizeof(gi2c->adap.name));

	pm_runtime_set_suspended(gi2c->dev);
	pm_runtime_set_autosuspend_delay(gi2c->dev, I2C_AUTO_SUSPEND_DELAY);
	pm_runtime_use_autosuspend(gi2c->dev);
	pm_runtime_enable(gi2c->dev);
	ret = i2c_add_adapter(&gi2c->adap);
	if (ret) {
		dev_err(gi2c->dev, "Add adapter failed\n");
		return ret;
	}

	dev_dbg(gi2c->dev, "I2C probed\n");
=======
		dev_err(&pdev->dev, "Invalid clk frequency %d Hz: %d\n",
			gi2c->clk_freq_out, ret);
		return ret;
	}

	gi2c->adap.algo = &geni_i2c_algo;
	init_completion(&gi2c->done);
	spin_lock_init(&gi2c->lock);
	platform_set_drvdata(pdev, gi2c);
	ret = devm_request_irq(&pdev->dev, gi2c->irq, geni_i2c_irq,
			       IRQF_TRIGGER_HIGH, "i2c_geni", gi2c);
	if (ret) {
		dev_err(&pdev->dev, "Request_irq failed:%d: err:%d\n",
			gi2c->irq, ret);
		return ret;
	}
	/* Disable the interrupt so that the system can enter low-power mode */
	disable_irq(gi2c->irq);
	i2c_set_adapdata(&gi2c->adap, gi2c);
	gi2c->adap.dev.parent = &pdev->dev;
	gi2c->adap.dev.of_node = pdev->dev.of_node;
	strlcpy(gi2c->adap.name, "Geni-I2C", sizeof(gi2c->adap.name));

	ret = geni_se_resources_on(&gi2c->se);
	if (ret) {
		dev_err(&pdev->dev, "Error turning on resources %d\n", ret);
		return ret;
	}
	proto = geni_se_read_proto(&gi2c->se);
	tx_depth = geni_se_get_tx_fifo_depth(&gi2c->se);
	if (proto != GENI_SE_I2C) {
		dev_err(&pdev->dev, "Invalid proto %d\n", proto);
		geni_se_resources_off(&gi2c->se);
		return -ENXIO;
	}
	gi2c->tx_wm = tx_depth - 1;
	geni_se_init(&gi2c->se, gi2c->tx_wm, tx_depth);
	geni_se_config_packing(&gi2c->se, BITS_PER_BYTE, PACKING_BYTES_PW,
							true, true, true);
	ret = geni_se_resources_off(&gi2c->se);
	if (ret) {
		dev_err(&pdev->dev, "Error turning off resources %d\n", ret);
		return ret;
	}

	dev_dbg(&pdev->dev, "i2c fifo/se-dma mode. fifo depth:%d\n", tx_depth);

	gi2c->suspended = 1;
	pm_runtime_set_suspended(gi2c->se.dev);
	pm_runtime_set_autosuspend_delay(gi2c->se.dev, I2C_AUTO_SUSPEND_DELAY);
	pm_runtime_use_autosuspend(gi2c->se.dev);
	pm_runtime_enable(gi2c->se.dev);

	ret = i2c_add_adapter(&gi2c->adap);
	if (ret) {
		dev_err(&pdev->dev, "Error adding i2c adapter %d\n", ret);
		pm_runtime_disable(gi2c->se.dev);
		return ret;
	}

>>>>>>> rebase
	return 0;
}

static int geni_i2c_remove(struct platform_device *pdev)
{
	struct geni_i2c_dev *gi2c = platform_get_drvdata(pdev);

<<<<<<< HEAD
	pm_runtime_disable(gi2c->dev);
	i2c_del_adapter(&gi2c->adap);
	if (gi2c->ipcl)
		ipc_log_context_destroy(gi2c->ipcl);
	return 0;
}

static int geni_i2c_resume_noirq(struct device *device)
{
	struct geni_i2c_dev *gi2c = dev_get_drvdata(device);

	GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev, "%s\n", __func__);
	return 0;
}

#ifdef CONFIG_PM
static int geni_i2c_runtime_suspend(struct device *dev)
{
	struct geni_i2c_dev *gi2c = dev_get_drvdata(dev);

	if (gi2c->se_mode == FIFO_SE_DMA)
		disable_irq(gi2c->irq);

	if (gi2c->is_shared) {
		/* Do not unconfigure GPIOs if shared se */
		se_geni_clks_off(&gi2c->i2c_rsc);
	} else {
		se_geni_resources_off(&gi2c->i2c_rsc);
	}
	GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev, "%s\n", __func__);
	return 0;
}

static int geni_i2c_runtime_resume(struct device *dev)
=======
	i2c_del_adapter(&gi2c->adap);
	pm_runtime_disable(gi2c->se.dev);
	return 0;
}

static int __maybe_unused geni_i2c_runtime_suspend(struct device *dev)
>>>>>>> rebase
{
	int ret;
	struct geni_i2c_dev *gi2c = dev_get_drvdata(dev);

<<<<<<< HEAD
	if (!gi2c->ipcl) {
		char ipc_name[I2C_NAME_SIZE];

		snprintf(ipc_name, I2C_NAME_SIZE, "%s", dev_name(gi2c->dev));
		gi2c->ipcl = ipc_log_context_create(2, ipc_name, 0);
	}

	ret = se_geni_resources_on(&gi2c->i2c_rsc);
	if (ret)
		return ret;

	if (gi2c->se_mode == UNINITIALIZED) {
		int proto = get_se_proto(gi2c->base);
		u32 se_mode;

		if (unlikely(proto != I2C)) {
			dev_err(gi2c->dev, "Invalid proto %d\n", proto);
			se_geni_resources_off(&gi2c->i2c_rsc);
			return -ENXIO;
		}

		se_mode = readl_relaxed(gi2c->base +
					GENI_IF_FIFO_DISABLE_RO);
		if (se_mode) {
			gi2c->se_mode = GSI_ONLY;
			geni_se_select_mode(gi2c->base, GSI_DMA);
			GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
					"i2c GSI mode\n");
		} else {
			int gi2c_tx_depth = get_tx_fifo_depth(gi2c->base);

			gi2c->se_mode = FIFO_SE_DMA;
			gi2c->tx_wm = gi2c_tx_depth - 1;
			geni_se_init(gi2c->base, gi2c->tx_wm, gi2c_tx_depth);
			se_config_packing(gi2c->base, 8, 4, true);
			qcom_geni_i2c_conf(gi2c, 0);
			GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
				    "i2c fifo/se-dma mode. fifo depth:%d\n",
				    gi2c_tx_depth);
		}
		GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev, "i2c-%d: %s\n",
			gi2c->adap.nr, dev_name(gi2c->dev));
	}

	if (gi2c->se_mode == FIFO_SE_DMA)
		enable_irq(gi2c->irq);

	GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev, "%s\n", __func__);
	return 0;
}

static int geni_i2c_suspend_noirq(struct device *device)
{
	struct geni_i2c_dev *gi2c = dev_get_drvdata(device);
	int ret;

	/* Make sure no transactions are pending */
	ret = i2c_trylock_bus(&gi2c->adap, I2C_LOCK_SEGMENT);
	if (!ret) {
		GENI_SE_ERR(gi2c->ipcl, true, gi2c->dev,
				"late I2C transaction request\n");
		return -EBUSY;
	}
	if (!pm_runtime_status_suspended(device)) {
		GENI_SE_DBG(gi2c->ipcl, false, gi2c->dev,
			"%s\n", __func__);
		geni_i2c_runtime_suspend(device);
		pm_runtime_disable(device);
		pm_runtime_set_suspended(device);
		pm_runtime_enable(device);
	}
	i2c_unlock_bus(&gi2c->adap, I2C_LOCK_SEGMENT);
	return 0;
}
#else
static int geni_i2c_runtime_suspend(struct device *dev)
{
	return 0;
}

static int geni_i2c_runtime_resume(struct device *dev)
{
	return 0;
}

static int geni_i2c_suspend_noirq(struct device *device)
{
	return 0;
}
#endif

static const struct dev_pm_ops geni_i2c_pm_ops = {
	.suspend_noirq		= geni_i2c_suspend_noirq,
	.resume_noirq		= geni_i2c_resume_noirq,
	.runtime_suspend	= geni_i2c_runtime_suspend,
	.runtime_resume		= geni_i2c_runtime_resume,
};

static const struct of_device_id geni_i2c_dt_match[] = {
	{ .compatible = "qcom,i2c-geni" },
=======
	disable_irq(gi2c->irq);
	ret = geni_se_resources_off(&gi2c->se);
	if (ret) {
		enable_irq(gi2c->irq);
		return ret;

	} else {
		gi2c->suspended = 1;
	}

	return 0;
}

static int __maybe_unused geni_i2c_runtime_resume(struct device *dev)
{
	int ret;
	struct geni_i2c_dev *gi2c = dev_get_drvdata(dev);

	ret = geni_se_resources_on(&gi2c->se);
	if (ret)
		return ret;

	enable_irq(gi2c->irq);
	gi2c->suspended = 0;
	return 0;
}

static int __maybe_unused geni_i2c_suspend_noirq(struct device *dev)
{
	struct geni_i2c_dev *gi2c = dev_get_drvdata(dev);

	if (!gi2c->suspended) {
		geni_i2c_runtime_suspend(dev);
		pm_runtime_disable(dev);
		pm_runtime_set_suspended(dev);
		pm_runtime_enable(dev);
	}
	return 0;
}

static const struct dev_pm_ops geni_i2c_pm_ops = {
	SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(geni_i2c_suspend_noirq, NULL)
	SET_RUNTIME_PM_OPS(geni_i2c_runtime_suspend, geni_i2c_runtime_resume,
									NULL)
};

static const struct of_device_id geni_i2c_dt_match[] = {
	{ .compatible = "qcom,geni-i2c" },
>>>>>>> rebase
	{}
};
MODULE_DEVICE_TABLE(of, geni_i2c_dt_match);

static struct platform_driver geni_i2c_driver = {
	.probe  = geni_i2c_probe,
	.remove = geni_i2c_remove,
	.driver = {
<<<<<<< HEAD
		.name = "i2c_geni",
=======
		.name = "geni_i2c",
>>>>>>> rebase
		.pm = &geni_i2c_pm_ops,
		.of_match_table = geni_i2c_dt_match,
	},
};

module_platform_driver(geni_i2c_driver);

<<<<<<< HEAD
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:i2c_geni");
=======
MODULE_DESCRIPTION("I2C Controller Driver for GENI based QUP cores");
MODULE_LICENSE("GPL v2");
>>>>>>> rebase

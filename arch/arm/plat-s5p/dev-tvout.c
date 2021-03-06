/* linux/arch/arm/plat-s3c/dev-usb.c
 *
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * S5P series device definition for USB host and device
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/gfp.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/platform_device.h>

#include <mach/irqs.h>
#include <mach/map.h>

#include <plat/devs.h>
#include <plat/tvout.h>

/* TVOUT interface */
static struct resource s5p_tvout_resources[] = {
	[0] = {
		.start  = S5P_PA_TVENC,
		.end    = S5P_PA_TVENC + S5P_SZ_TVENC - 1,
		.flags  = IORESOURCE_MEM,
		.name   = "s5p-sdo"
	},
	[1] = {
		.start  = S5P_PA_VP,
		.end    = S5P_PA_VP + S5P_SZ_VP - 1,
		.flags  = IORESOURCE_MEM,
		.name   = "s5p-vp"
	},
	[2] = {
		.start  = S5P_PA_MIXER,
		.end    = S5P_PA_MIXER + S5P_SZ_MIXER - 1,
		.flags  = IORESOURCE_MEM,
		.name   = "s5p-mixer"
	},
	[3] = {
		.start  = S5P_PA_HDMI,
		.end    = S5P_PA_HDMI + S5P_SZ_HDMI - 1,
		.flags  = IORESOURCE_MEM,
		.name   = "s5p-hdmi"
	},
	[4] = {
		.start  = S5P_I2C_HDMI_PHY,
		.end    = S5P_I2C_HDMI_PHY + S5P_I2C_HDMI_SZ_PHY - 1,
		.flags  = IORESOURCE_MEM,
		.name   = "s5p-i2c-hdmi-phy"
	},
	[5] = {
		.start  = IRQ_MIXER,
		.end    = IRQ_MIXER,
		.flags  = IORESOURCE_IRQ,
		.name   = "s5p-mixer"
	},
	[6] = {
		.start  = IRQ_HDMI,
		.end    = IRQ_HDMI,
		.flags  = IORESOURCE_IRQ,
		.name   = "s5p-hdmi"
	},
	[7] = {
		.start  = IRQ_TVENC,
		.end    = IRQ_TVENC,
		.flags  = IORESOURCE_IRQ,
		.name   = "s5p-sdo"
	},
};

static u64 tvout_dma_mask = 0xffffffffUL;

struct platform_device s5p_device_tvout = {
	.name		 = "s5p-tvout",
	.id		 = -1,
	.num_resources	= ARRAY_SIZE(s5p_tvout_resources),
	.resource	= s5p_tvout_resources,
	.dev            = {
                .dma_mask               = &tvout_dma_mask,
                .coherent_dma_mask      = 0xffffffffUL
        }
};
EXPORT_SYMBOL(s5p_device_tvout);

/* CEC */
static struct resource s5p_cec_resources[] = {
	[0] = {
		.start	= S5P_PA_CEC,
		.end	= S5P_PA_CEC + S5P_SZ_CEC - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_CEC,
		.end	= IRQ_CEC,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device s5p_device_cec = {
	.name		= "s5p-tvout-cec",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(s5p_cec_resources),
	.resource	= s5p_cec_resources,
};
EXPORT_SYMBOL(s5p_device_cec);

/* HPD */
static struct resource s5p_hpd_resources[] = {
	[0] = {
		.start	= IRQ_TVOUT_HPD,
		.end	= IRQ_TVOUT_HPD,
		.flags	= IORESOURCE_IRQ,
	},
};
struct platform_device s5p_device_hpd = {
	.name		= "s5p-tvout-hpd",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(s5p_hpd_resources),
	.resource	= s5p_hpd_resources,
};
EXPORT_SYMBOL(s5p_device_hpd);

void __init s5p_hdmi_hpd_set_platdata(struct s5p_platform_hpd *pd)
{
	struct s5p_platform_hpd *npd;
	npd = kmemdup(pd, sizeof(struct s5p_platform_hpd), GFP_KERNEL);
	if (!npd)
		printk(KERN_ERR "%s: no memory for platform data\n", __func__);
	else {
		if (!npd->int_src_ext_hpd)
			npd->int_src_ext_hpd = s5p_int_src_ext_hpd;
		if (!npd->int_src_hdmi_hpd)
			npd->int_src_hdmi_hpd = s5p_int_src_hdmi_hpd;
		if (!npd->read_gpio)
			npd->read_gpio = s5p_hpd_read_gpio;
		s5p_device_hpd.dev.platform_data = npd;
	}
}

void __init s5p_hdmi_cec_set_platdata(struct s5p_platform_cec *pd)
{
	struct s5p_platform_cec *npd;
	npd = kmemdup(pd, sizeof(struct s5p_platform_cec), GFP_KERNEL);
	if (!npd)
		printk(KERN_ERR "%s: no memory for platform data\n", __func__);
	else {
		if (!npd->cfg_gpio)
			npd->cfg_gpio = s5p_cec_cfg_gpio;
		s5p_device_cec.dev.platform_data = npd;
	}
}

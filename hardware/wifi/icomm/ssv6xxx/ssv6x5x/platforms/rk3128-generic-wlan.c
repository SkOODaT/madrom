/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/irq.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/gpio.h>
#include <linux/mmc/host.h>
#include <linux/delay.h>
#include <linux/regulator/consumer.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0))
#include <linux/printk.h>
#include <linux/err.h>
#else
#include <config/printk.h>
#endif
extern int rockchip_wifi_power(int on);
extern int rockchip_wifi_set_carddetect(int val);
#define GPIO_REG_WRITEL(val,reg) do{__raw_writel(val, CTL_PIN_BASE + (reg));}while(0)
static int g_wifidev_registered = 0;
extern int tu_tu_ssvdevice_init(void);
extern void tu_tu_ssvdevice_exit(void);
#ifdef CONFIG_SSV_SUPPORT_AES_ASM
extern int aes_init(void);
extern void aes_fini(void);
extern int sha1_mod_init(void);
extern void sha1_mod_fini(void);
#endif
int initWlan(void)
{
    int ret=0;
    rockchip_wifi_set_carddetect(0);
    msleep(50);
    rockchip_wifi_power(0);
    msleep(50);
    rockchip_wifi_power(1);
    msleep(50);
    rockchip_wifi_set_carddetect(1);
    msleep(120);
    g_wifidev_registered = 1;
    ret = tu_tu_ssvdevice_init();
    return ret;
}
void exitWlan(void)
{
    if (g_wifidev_registered)
    {
        tu_tu_ssvdevice_exit();
        rockchip_wifi_set_carddetect(0);
        rockchip_wifi_power(0);
        g_wifidev_registered = 0;
    }
    return;
}
static __init int tu_tu_generic_wifi_init_module(void)
{
 printk("%s\n", __func__);
#ifdef CONFIG_SSV_SUPPORT_AES_ASM
 sha1_mod_init();
 aes_init();
#endif
 return initWlan();
}
static __exit void tu_tu_generic_wifi_exit_module(void)
{
 printk("%s\n", __func__);
#ifdef CONFIG_SSV_SUPPORT_AES_ASM
        aes_fini();
        sha1_mod_fini();
#endif
 exitWlan();
}
EXPORT_SYMBOL(tu_tu_generic_wifi_init_module);
EXPORT_SYMBOL(tu_tu_generic_wifi_exit_module);
module_init(tu_tu_generic_wifi_init_module);
module_exit(tu_tu_generic_wifi_exit_module);
MODULE_LICENSE("Dual BSD/GPL");

/*
<<<<<<< HEAD
 * LED driver - leds-ktd2692.c
 *
 * Copyright (C) 2020 Sunggeun Yim <sunggeun.yim@samsung.com>
=======
 * LED driver : leds-ktd2692.c
 *
 * Copyright (C) 2015 Samsung Electronics
 * Ingi Kim <ingi2.kim@samsung.com>
>>>>>>> rebase
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/pwm.h>
#include <linux/vmalloc.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/leds-ktd2692.h>
#include <linux/time.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_gpio.h>
#endif

//#define DEBUG_LED_TIME
extern struct class *camera_class; /*sys/class/camera*/
struct device *ktd2692_led_dev;

struct ktd2692_platform_data *global_ktd2692data = NULL;
struct device *global_dev_kt;
bool sysfs_flash_op_kt;

void ktd2692_setGpio(int onoff)
{
	if (onoff) {
		if (gpio_is_valid(global_ktd2692data->flash_control))
			__gpio_set_value(global_ktd2692data->flash_control, 1);
		if (gpio_is_valid(global_ktd2692data->flash_en))
			__gpio_set_value(global_ktd2692data->flash_en, 1);
	} else {
		if (gpio_is_valid(global_ktd2692data->flash_control))
			__gpio_set_value(global_ktd2692data->flash_control, 0);
		if (gpio_is_valid(global_ktd2692data->flash_en))
			__gpio_set_value(global_ktd2692data->flash_en, 0);
	}
}

void ktd2692_set_low_bit(void)
{
#ifdef DEBUG_LED_TIME
	struct timeval start_low, end_low, start_high, end_high;
	unsigned long time_low, time_high;
#endif

#ifdef DEBUG_LED_TIME
	do_gettimeofday(&start_low);
#endif

	if (gpio_is_valid(global_ktd2692data->flash_control))
		__gpio_set_value(global_ktd2692data->flash_control, 0);
	udelay(T_L_LB);

#ifdef DEBUG_LED_TIME
	do_gettimeofday(&end_low);

	do_gettimeofday(&start_high);
#endif

	if (gpio_is_valid(global_ktd2692data->flash_control))
		__gpio_set_value(global_ktd2692data->flash_control, 1);
	udelay(T_H_LB);

#ifdef DEBUG_LED_TIME
	do_gettimeofday(&end_high);

	time_low = (end_low.tv_sec - start_low.tv_sec) * 1000000 + (end_low.tv_usec - start_low.tv_usec);
	time_high = (end_high.tv_sec - start_high.tv_sec) * 1000000 + (end_high.tv_usec - start_high.tv_usec);

	LED_INFO("[ta] LOW BIT: time_low(%lu) / time_high(%lu)\n", time_low, time_high);
	if (time_low <= (time_high*2)) 
		LED_ERROR("[ta] Low Bit: high pulse too long\n");
#endif
}

void ktd2692_set_high_bit(void)
{
#ifdef DEBUG_LED_TIME
	struct timeval start_low, end_low, start_high, end_high;
	unsigned long time_low, time_high;
#endif

#ifdef DEBUG_LED_TIME
	do_gettimeofday(&start_low);
#endif

	if (gpio_is_valid(global_ktd2692data->flash_control))
		__gpio_set_value(global_ktd2692data->flash_control, 0);
	udelay(T_L_HB);

#ifdef DEBUG_LED_TIME
	do_gettimeofday(&end_low);

	do_gettimeofday(&start_high);
#endif

	if (gpio_is_valid(global_ktd2692data->flash_control))
		__gpio_set_value(global_ktd2692data->flash_control, 1);
	udelay(T_H_HB);

#ifdef DEBUG_LED_TIME
	do_gettimeofday(&end_high);

	time_low = (end_low.tv_sec - start_low.tv_sec) * 1000000 + (end_low.tv_usec - start_low.tv_usec);
	time_high = (end_high.tv_sec - start_high.tv_sec) * 1000000 + (end_high.tv_usec - start_high.tv_usec);

	LED_INFO("[ta] HIGHT BIT: time_low(%lu) / time_high(%lu)\n", time_low, time_high);
	if ((time_low*2) >= time_high) 
		LED_ERROR("[ta] HIGH BIT: low pulse too long\n");
#endif
}

static int ktd2692_set_bit(unsigned int bit)
{
	if (bit) {
		ktd2692_set_high_bit();
	} else {
		ktd2692_set_low_bit();
	}
	return 0;
}

static int ktd2692_write_data(unsigned data)
{
	int err = 0;
	unsigned int bit = 0;

	/* Data Start Condition */
	if (gpio_is_valid(global_ktd2692data->flash_control))
		__gpio_set_value(global_ktd2692data->flash_control, 1);
	udelay(T_SOD_KT);

	/* BIT 7*/
	bit = ((data>> 7) & 0x01);
	ktd2692_set_bit(bit);

	/* BIT 6 */
	bit = ((data>> 6) & 0x01);
	ktd2692_set_bit(bit);

	/* BIT 5*/
	bit = ((data>> 5) & 0x01);
	ktd2692_set_bit(bit);

	/* BIT 4 */
	bit = ((data>> 4) & 0x01);
	ktd2692_set_bit(bit);

	/* BIT 3*/
	bit = ((data>> 3) & 0x01);
	ktd2692_set_bit(bit);

	/* BIT 2 */
	bit = ((data>> 2) & 0x01);
	ktd2692_set_bit(bit);

	/* BIT 1*/
	bit = ((data>> 1) & 0x01);
	ktd2692_set_bit(bit);

	/* BIT 0 */
	bit = ((data>> 0) & 0x01);
	ktd2692_set_bit(bit);

	if (gpio_is_valid(global_ktd2692data->flash_control))
		__gpio_set_value(global_ktd2692data->flash_control, 0);
	udelay(T_EOD_L);

	/* Data End Condition */
	if (gpio_is_valid(global_ktd2692data->flash_control))
		__gpio_set_value(global_ktd2692data->flash_control, 1);
	udelay(T_EOD_H);

	return err;
}

ssize_t ktd2692_store(struct device *dev,struct device_attribute *attr, const char *buf,size_t count)
{
	int value = 0;
	int brightness_value = 0;
	int ret = 0;
	unsigned long flags = 0;
	int torch_intensity = -1;
	sysfs_flash_op_kt = 0;

	if ((buf == NULL) || kstrtouint(buf, 10, &value)) {
		LED_INFO("sysfs value :(%d)\n", value);
		return -1;
	}

	LED_INFO("sysfs value :(%d)\n", value);
	
	if (global_ktd2692data == NULL) {
		LED_ERROR("KTD2692(%s) global_ktd2692data is not initialized.\n", __func__);
		return -EFAULT;
	}

	LED_INFO("KTD2692-STORE BEFORE:(%d) (%d) (%d)\n", value,buf,global_ktd2692data->sysfs_input_data);
	global_ktd2692data->sysfs_input_data = value;
	LED_INFO("KTD2692-STORE AFTER:(%d) (%d)\n", value,global_ktd2692data->sysfs_input_data);

	if (gpio_is_valid(global_ktd2692data->flash_control)) {
		ret = gpio_request(global_ktd2692data->flash_control, "ktd2692_led_control");
		if (ret) {
			LED_ERROR("Failed to request ktd2692_led_mode_ctrl\n");
			return -1;
		}
	}
	if (gpio_is_valid(global_ktd2692data->flash_en)) {
		ret = gpio_request(global_ktd2692data->flash_en, "ktd2692_led_en");
		if (ret) {
			gpio_free(global_ktd2692data->flash_control);
			LED_ERROR("Failed to request ktd2692_led_en\n");
		}
	}

	if (value <= 0) {
		sysfs_flash_op_kt = false;

		LED_INFO("KTD2692-TORCH OFF. : E(%d)\n", value);

		global_ktd2692data->mode_status = KTD2692_DISABLES_TORCH_FLASH_MODE;
		spin_lock_irqsave(&global_ktd2692data->int_lock, flags);
		ktd2692_write_data(global_ktd2692data->mode_status|
							KTD2692_ADDR_VIDEO_FLASHMODE_CONTROL);
		spin_unlock_irqrestore(&global_ktd2692data->int_lock, flags);

		ktd2692_setGpio(0);
		global_ktd2692data->is_torch_enable = false;
		LED_INFO("KTD2692-TORCH OFF. : X(%d)\n", value);
	} else {
		sysfs_flash_op_kt = true;

		global_ktd2692data->mode_status = KTD2692_ENABLE_TORCH_MODE;
		global_ktd2692data->is_torch_enable = true;
		spin_lock_irqsave(&global_ktd2692data->int_lock, flags);
		ktd2692_write_data(global_ktd2692data->LVP_Voltage|
							KTD2692_ADDR_LVP_SETTING);
		LED_INFO("KTD2692-TORCH ON VALUE. : X (%d) (%d)\n", value,global_ktd2692data->sysfs_input_data);
#if 0	/* use the internel defualt setting */
		ktd2692_write_data(global_ktd2692data->flash_timeout|
							KTD2692_ADDR_FLASH_TIMEOUT_SETTING);
		LED_INFO("KTD2692_ADDR_FLASH_TIMEOUT_SETTING");
#endif
		if (value == 100 || value == 1) {/* value 1 is getting used for turning on torch using side key */
			LED_INFO("KTD2692-TORCH ON : E(%d), current(%d)\n", value, global_ktd2692data->factory_torch_current_value);
			ktd2692_write_data(global_ktd2692data->factory_torch_current_value|
								KTD2692_ADDR_VIDEO_CURRENT_SETTING);
			LED_INFO("KTD2692-TORCH ON : X(%d), current(%d)\n", value, global_ktd2692data->factory_torch_current_value);
		} else if (1001 <= value && value <= 1010) {
			LED_INFO("KTD2692-TORCH ON. : E(%d)\n", value);
			brightness_value = value - 1001;
			if (global_ktd2692data->torch_table[brightness_value] != 0) {
				torch_intensity = KTD2692_CAL_VIDEO_CURRENT(KTD2692_TORCH_STEP_LEVEL_CURRENT(global_ktd2692data->torch_table[brightness_value], global_ktd2692data->max_current),
					global_ktd2692data->max_current);
			}
			if (torch_intensity < 0) {
				LED_INFO("KTD2692-force to set as default : %d\n", global_ktd2692data->factory_torch_current_value);
				torch_intensity = global_ktd2692data->factory_torch_current_value;
			}
			ktd2692_write_data(torch_intensity|
								KTD2692_ADDR_VIDEO_CURRENT_SETTING);
			LED_INFO("KTD2692-TORCH ON. : X(%d)\n", value);
		} else {
			LED_INFO(" KTD2692-FLASH ON : E(%d), current(%d)\n", value, global_ktd2692data->flash_current_value);
			global_ktd2692data->mode_status = KTD2692_ENABLE_FLASH_MODE;
			/*ktd2692_write_data(global_ktd2692data->flash_current_value|
								KTD2692_ADDR_FLASH_CURRENT_SETTING);*/
			if (gpio_is_valid(global_ktd2692data->flash_en))
				__gpio_set_value(global_ktd2692data->flash_en, 1);
			
			LED_INFO(" KTD2692-FLASH ON. : X(%d), current(%d)\n", value, global_ktd2692data->flash_current_value);
		}
		ktd2692_write_data(global_ktd2692data->mode_status|
							KTD2692_ADDR_VIDEO_FLASHMODE_CONTROL);
		spin_unlock_irqrestore(&global_ktd2692data->int_lock, flags);
	}

	if (gpio_is_valid(global_ktd2692data->flash_control))
		gpio_free(global_ktd2692data->flash_control);
	if (gpio_is_valid(global_ktd2692data->flash_en))
		gpio_free(global_ktd2692data->flash_en);
	return count;
}
EXPORT_SYMBOL(ktd2692_store);

int32_t ktd2692_led_mode_ctrl(int state, u32 intensity)
{
	int ret = 0;
	unsigned long flags = 0;

	if (global_ktd2692data == NULL) {
		LED_ERROR("KTD2692(%s) global_ktd2692data is not initialized.\n", __func__);
		return -EFAULT;
	}

	if (sysfs_flash_op_kt) {
		pr_warn("%s : The camera led control is not allowed"
			"because sysfs led control already used it\n", __FUNCTION__);
		return 0; //no error
	}

	if (gpio_is_valid(global_ktd2692data->flash_control)) {
		ret = gpio_request(global_ktd2692data->flash_control, "ktd2692_led_control");
		if (ret) {
			LED_ERROR("Failed to request ktd2692_led_mode_ctrl\n");
			return -1;
		}
	}

	switch(state) {
		case 0:
			/* FlashLight Mode OFF */
			if (gpio_is_valid(global_ktd2692data->flash_en)) {
				ret = gpio_request(global_ktd2692data->flash_en, "ktd2692_led_en");
				if (ret) {
					gpio_free(global_ktd2692data->flash_control);
					LED_ERROR("Failed to request ktd2692_led_en\n");
				}
			}
			LED_INFO("KTD2692-FLASH OFF E(%d)\n", state);
			global_ktd2692data->mode_status = KTD2692_DISABLES_TORCH_FLASH_MODE;
			spin_lock_irqsave(&global_ktd2692data->int_lock, flags);
			ktd2692_write_data(global_ktd2692data->mode_status|
								KTD2692_ADDR_VIDEO_FLASHMODE_CONTROL);
			spin_unlock_irqrestore(&global_ktd2692data->int_lock, flags);

			ktd2692_setGpio(0);
			global_ktd2692data->is_torch_enable = false;
			if (gpio_is_valid(global_ktd2692data->flash_en))
				gpio_free(global_ktd2692data->flash_en);
			LED_INFO("KTD2692-FLASH OFF X(%d)\n", state);
			break;
		case 1:
			/* FlashLight Mode TORCH */
			/*if (gpio_is_valid(global_ktd2692data->flash_en)) 
				ret = gpio_request(global_ktd2692data->flash_en, "ktd2692_led_en");

				if (ret) {
					gpio_free(global_ktd2692data->flash_control);
					LED_ERROR("Failed to request ktd2692_led_en\n");
				}
			
			if (global_ktd2692data->is_torch_enable == true) {
				LED_INFO("KTD2692-TORCH is already ON\n");
				return 0;
			}*/
			
			global_ktd2692data->mode_status = KTD2692_ENABLE_TORCH_MODE;
			LED_INFO("KTD2692 global video current [%d] [%d] \n", state, global_ktd2692data->video_current_value );
			spin_lock_irqsave(&global_ktd2692data->int_lock, flags);
			ktd2692_write_data(global_ktd2692data->LVP_Voltage|
								KTD2692_ADDR_LVP_SETTING);
			if (intensity > 0) {
				ktd2692_write_data((uint8_t)(KTD2692_VIDEO_CURRENT(intensity, KTD2692_MAX_CURRENT)) |
									KTD2692_ADDR_VIDEO_CURRENT_SETTING);
				LED_INFO("[%s] : Intensity(%u) Current-Index (%d)\n", __func__, intensity,
						 KTD2692_VIDEO_CURRENT(intensity, KTD2692_MAX_CURRENT));
			} else {
				ktd2692_write_data(global_ktd2692data->video_current_value|
									KTD2692_ADDR_VIDEO_CURRENT_SETTING);
				LED_INFO("[%s] : Intensity(%u) Current-Index (%d)\n", __func__, intensity,
						 KTD2692_VIDEO_CURRENT(intensity, KTD2692_MAX_CURRENT))	;				
			}
			
			ktd2692_write_data(global_ktd2692data->mode_status|
								KTD2692_ADDR_VIDEO_FLASHMODE_CONTROL);
										
			/*if (gpio_is_valid(global_ktd2692data->flash_en))
				gpio_free(global_ktd2692data->flash_en);*/
			spin_unlock_irqrestore(&global_ktd2692data->int_lock, flags);
			LED_INFO("KTD2692-TORCH ON X(%d)\n", state);
			break;
		case 2:
			/* FlashLight Mode Flash */
			/*if (gpio_is_valid(global_ktd2692data->flash_en)) {
				ret = gpio_request(global_ktd2692data->flash_en, "ktd2692_led_en");
				LED_INFO("KTD2692-FLASH gpio enabled  (%d)\n",ret );
				if (ret) {
					gpio_free(global_ktd2692data->flash_control);
					LED_ERROR("Failed to request ktd2692_led_en\n");
				}
			}*/
			LED_INFO("KTD2692-FLASH ON E(%d)\n", state);
			global_ktd2692data->mode_status = KTD2692_ENABLE_FLASH_MODE;
			spin_lock_irqsave(&global_ktd2692data->int_lock, flags);
			ktd2692_write_data(global_ktd2692data->LVP_Voltage|
								KTD2692_ADDR_LVP_SETTING);
			/*if (intensity > 0) {
				ktd2692_write_data((uint8_t)(KTD2692_FLASH_CURRENT(intensity, KTD2692_MAX_CURRENT))|
									KTD2692_ADDR_FLASH_CURRENT_SETTING);
				LED_INFO("[%s] : Intensity(%u) Current-Index (%d)\n", __func__, intensity,
						 KTD2692_FLASH_CURRENT(intensity, KTD2692_MAX_CURRENT));
			} else {
				ktd2692_write_data(global_ktd2692data->flash_current_value|
									KTD2692_ADDR_FLASH_CURRENT_SETTING);
			}*/

			ktd2692_write_data(global_ktd2692data->mode_status|
								KTD2692_ADDR_VIDEO_FLASHMODE_CONTROL);
			if (gpio_is_valid(global_ktd2692data->flash_en))
				__gpio_set_value(global_ktd2692data->flash_en, 1);
			/*if (gpio_is_valid(global_ktd2692data->flash_en))
				gpio_free(global_ktd2692data->flash_en);*/
			spin_unlock_irqrestore(&global_ktd2692data->int_lock, flags);
			LED_INFO("KTD2692-FLASH ON X(%d)\n", state);
			break;
		case 3:
			/* FlashLight Mode Flicker */
			LED_INFO("KTD2692-FLASH ON E(%d)\n", state);
			/*global_ktd2692data->mode_status = KTD2692_ENABLE_FLASH_MODE;*/
			spin_lock_irqsave(&global_ktd2692data->int_lock, flags);
			ktd2692_write_data(global_ktd2692data->LVP_Voltage|
								KTD2692_ADDR_LVP_SETTING);
			if (intensity > 0) {
				ktd2692_write_data((uint8_t)(KTD2692_FLASH_CURRENT(intensity, KTD2692_MAX_CURRENT))|
									KTD2692_ADDR_FLASH_CURRENT_SETTING);
				LED_INFO("[%s] : Intensity(%u) Current-Index (%d)\n", __func__, intensity,
						 KTD2692_FLASH_CURRENT(intensity, KTD2692_MAX_CURRENT));
			} else {
				ktd2692_setGpio(0);
			}

			/*ktd2692_write_data(global_ktd2692data->mode_status|
								KTD2692_ADDR_VIDEO_FLASHMODE_CONTROL);*/
			spin_unlock_irqrestore(&global_ktd2692data->int_lock, flags);
			LED_INFO("KTD2692-FLASH ON X(%d)\n", state);
			break;
		default:
			/* FlashLight Mode OFF */
			if (gpio_is_valid(global_ktd2692data->flash_en)) {
				ret = gpio_request(global_ktd2692data->flash_en, "ktd2692_led_en");
				if (ret) {
					gpio_free(global_ktd2692data->flash_control);
					LED_ERROR("Failed to request ktd2692_led_en\n");
				}
			}
			LED_INFO("KTD2692-FLASH OFF E(%d)\n", state);
			global_ktd2692data->mode_status = KTD2692_DISABLES_TORCH_FLASH_MODE;
			spin_lock_irqsave(&global_ktd2692data->int_lock, flags);
			ktd2692_write_data(global_ktd2692data->mode_status|
								KTD2692_ADDR_VIDEO_FLASHMODE_CONTROL);
			spin_unlock_irqrestore(&global_ktd2692data->int_lock, flags);

			ktd2692_setGpio(0);
			if (gpio_is_valid(global_ktd2692data->flash_en))
				gpio_free(global_ktd2692data->flash_en);
			LED_INFO("KTD2692-FLASH OFF X(%d)\n", state);
			break;
	}

	if (gpio_is_valid(global_ktd2692data->flash_control)){
		LED_INFO("KTD2692 free flash_ctrl");
		gpio_free(global_ktd2692data->flash_control);
	}
	return ret;
}
EXPORT_SYMBOL(ktd2692_led_mode_ctrl);

ssize_t ktd2692_show (struct device *dev,struct device_attribute *attr, char *buf)
{
	LED_INFO("KTD2692-SHOW:(%d) (%d)\n", buf,global_ktd2692data->sysfs_input_data);
	return sprintf(buf, "%d\n", global_ktd2692data->sysfs_input_data);
}
EXPORT_SYMBOL(ktd2692_show);

static DEVICE_ATTR(rear_flash, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH,
	ktd2692_show, ktd2692_store);
	
static DEVICE_ATTR(rear_torch_flash, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH,
	ktd2692_show, ktd2692_store);

static int ktd2692_parse_dt(struct device *dev,
                                struct ktd2692_platform_data *pdata)
{
	struct device_node *dnode = dev->of_node;
	u32 buffer = 0;
	int ret = 0;
	u32 torch_table_enable = 0;

	/* Defulat Value */
	pdata->LVP_Voltage = KTD2692_DISABLE_LVP;
	pdata->flash_timeout = KTD2692_TIMER_1049ms;	/* default */
	pdata->min_current_value = KTD2692_MIN_CURRENT_240mA;
	pdata->flash_current_value = KTD2692_FLASH_CURRENT(KTD2692_FLASH_DEFAULT_CURRENT, KTD2692_MAX_CURRENT);
	pdata->video_current_value = KTD2692_VIDEO_CURRENT(KTD2692_VIDEO_DEFAULT_CURRENT, KTD2692_MAX_CURRENT);
	pdata->factory_torch_current_value = KTD2692_VIDEO_CURRENT(KTD2692_FACTORY_TORCH_CURRENT, KTD2692_MAX_CURRENT);
	pdata->mode_status = KTD2692_DISABLES_TORCH_FLASH_MODE;

	/* get gpio */
	pdata->flash_control = of_get_named_gpio(dnode, "flash_control", 0);
	if (!gpio_is_valid(pdata->flash_control)) {
		dev_err(dev, "failed to get flash_control\n");
		return -1;
	}

	/* get gpio */
	pdata->flash_en = of_get_named_gpio(dnode, "flash_en", 0);
	if (!gpio_is_valid(pdata->flash_en)) {
		dev_err(dev, "failed to get flash_enable\n");
	}

	/* get max current value */
	if (of_property_read_u32(dnode, "max_current", &buffer) == 0) {
		dev_info(dev, "max_current = <%d>\n",
			buffer);
		pdata->max_current = buffer;
	}

	/* get flash current value */
	if (of_property_read_u32(dnode, "flash_current", &buffer) == 0) {
		dev_info(dev, "flash_current = <%d><%d>\n",
			buffer, KTD2692_FLASH_CURRENT(buffer, pdata->max_current));
		pdata->flash_current_value = KTD2692_FLASH_CURRENT(buffer, pdata->max_current);
	}

	/* get vdieo current value */
	if (of_property_read_u32(dnode, "video_current", &buffer) == 0) {
		dev_info(dev, "video_current = <%d><%d>\n",
			buffer, KTD2692_VIDEO_CURRENT(buffer, pdata->max_current));
		pdata->video_current_value = KTD2692_VIDEO_CURRENT(buffer, pdata->max_current);
	}

	/* get factory torch current value */
	if (of_property_read_u32(dnode, "factory_torch_current", &buffer) == 0) {
		dev_info(dev, "factory_torch_current = <%d><%d>\n",
			buffer, KTD2692_VIDEO_CURRENT(buffer, pdata->max_current));
		pdata->factory_torch_current_value = KTD2692_VIDEO_CURRENT(buffer, pdata->max_current);
	}

	ret = of_property_read_u32(dnode, "torch_table_enable", &torch_table_enable);
	if (ret) {
		pr_info("%s failed to get a torch_table_enable\n", __func__);
	}
	if (torch_table_enable == 1) {
		pdata->torch_table_enable = torch_table_enable;
		ret = of_property_read_u32_array(dnode, "torch_table", pdata->torch_table, TORCH_STEP);
	} else {
		pdata->torch_table_enable = 0;
	}

	return ret;
}

static int ktd2692_probe(struct platform_device *pdev)
{
	struct ktd2692_platform_data *pdata;
	int ret = 0;

	if (pdev->dev.of_node) {
		pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata) {
			dev_err(&pdev->dev, "Failed to allocate memory\n");
			return -ENOMEM;
		}
		ret = ktd2692_parse_dt(&pdev->dev, pdata);
		if (ret < 0) {
			return -EFAULT;
		}
	} else {
	pdata = pdev->dev.platform_data;
		if (pdata == NULL) {
			return -EFAULT;
		}
	}

	sysfs_flash_op_kt = 0; //default off


	if (camera_class == NULL) {
		camera_class = class_create(THIS_MODULE, "camera");
		if (IS_ERR(camera_class))
			pr_err("%s: failed to create device cam_dev_rear!\n", __func__);
	}
	
	
	if (camera_class != NULL) {
		ktd2692_led_dev = device_create(camera_class, NULL, 0, NULL, "flash");
		if (IS_ERR(ktd2692_led_dev)) {
			pr_err("%s: Failed to create device(flash)!\n", __func__);
			return -ENODEV;
		}

	if (device_create_file(ktd2692_led_dev, &dev_attr_rear_flash) < 0) {
			pr_err("%s: failed to create device file, %s\n",
					__func__, dev_attr_rear_flash.attr.name);
			return -ENOENT;
	}
	

	if (device_create_file(ktd2692_led_dev, &dev_attr_rear_torch_flash) < 0) {
			pr_err("%s: failed to create device file, %s\n",
					__func__, dev_attr_rear_torch_flash.attr.name);
			return -ENOENT;
	}
	}


	global_ktd2692data = pdata;
	global_dev_kt = &pdev->dev;

	LED_INFO("KTD2692_LED Probe\n");

	global_ktd2692data->is_torch_enable = false;

	spin_lock_init(&pdata->int_lock);

	return 0;
}
static int ktd2692_remove(struct platform_device *pdev)
{
=======

#include <linux/delay.h>
#include <linux/err.h>
#include <linux/gpio/consumer.h>
#include <linux/led-class-flash.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

/* Value related the movie mode */
#define KTD2692_MOVIE_MODE_CURRENT_LEVELS	16
#define KTD2692_MM_TO_FL_RATIO(x)		((x) / 3)
#define KTD2962_MM_MIN_CURR_THRESHOLD_SCALE	8

/* Value related the flash mode */
#define KTD2692_FLASH_MODE_TIMEOUT_LEVELS	8
#define KTD2692_FLASH_MODE_TIMEOUT_DISABLE	0
#define KTD2692_FLASH_MODE_CURR_PERCENT(x)	(((x) * 16) / 100)

/* Macro for getting offset of flash timeout */
#define GET_TIMEOUT_OFFSET(timeout, step)	((timeout) / (step))

/* Base register address */
#define KTD2692_REG_LVP_BASE			0x00
#define KTD2692_REG_FLASH_TIMEOUT_BASE		0x20
#define KTD2692_REG_MM_MIN_CURR_THRESHOLD_BASE	0x40
#define KTD2692_REG_MOVIE_CURRENT_BASE		0x60
#define KTD2692_REG_FLASH_CURRENT_BASE		0x80
#define KTD2692_REG_MODE_BASE			0xA0

/* Set bit coding time for expresswire interface */
#define KTD2692_TIME_RESET_US			700
#define KTD2692_TIME_DATA_START_TIME_US		10
#define KTD2692_TIME_HIGH_END_OF_DATA_US	350
#define KTD2692_TIME_LOW_END_OF_DATA_US		10
#define KTD2692_TIME_SHORT_BITSET_US		4
#define KTD2692_TIME_LONG_BITSET_US		12

/* KTD2692 default length of name */
#define KTD2692_NAME_LENGTH			20

enum ktd2692_bitset {
	KTD2692_LOW = 0,
	KTD2692_HIGH,
};

/* Movie / Flash Mode Control */
enum ktd2692_led_mode {
	KTD2692_MODE_DISABLE = 0,	/* default */
	KTD2692_MODE_MOVIE,
	KTD2692_MODE_FLASH,
};

struct ktd2692_led_config_data {
	/* maximum LED current in movie mode */
	u32 movie_max_microamp;
	/* maximum LED current in flash mode */
	u32 flash_max_microamp;
	/* maximum flash timeout */
	u32 flash_max_timeout;
	/* max LED brightness level */
	enum led_brightness max_brightness;
};

struct ktd2692_context {
	/* Related LED Flash class device */
	struct led_classdev_flash fled_cdev;

	/* secures access to the device */
	struct mutex lock;
	struct regulator *regulator;

	struct gpio_desc *aux_gpio;
	struct gpio_desc *ctrl_gpio;

	enum ktd2692_led_mode mode;
	enum led_brightness torch_brightness;
};

static struct ktd2692_context *fled_cdev_to_led(
				struct led_classdev_flash *fled_cdev)
{
	return container_of(fled_cdev, struct ktd2692_context, fled_cdev);
}

static void ktd2692_expresswire_start(struct ktd2692_context *led)
{
	gpiod_direction_output(led->ctrl_gpio, KTD2692_HIGH);
	udelay(KTD2692_TIME_DATA_START_TIME_US);
}

static void ktd2692_expresswire_reset(struct ktd2692_context *led)
{
	gpiod_direction_output(led->ctrl_gpio, KTD2692_LOW);
	udelay(KTD2692_TIME_RESET_US);
}

static void ktd2692_expresswire_end(struct ktd2692_context *led)
{
	gpiod_direction_output(led->ctrl_gpio, KTD2692_LOW);
	udelay(KTD2692_TIME_LOW_END_OF_DATA_US);
	gpiod_direction_output(led->ctrl_gpio, KTD2692_HIGH);
	udelay(KTD2692_TIME_HIGH_END_OF_DATA_US);
}

static void ktd2692_expresswire_set_bit(struct ktd2692_context *led, bool bit)
{
	/*
	 * The Low Bit(0) and High Bit(1) is based on a time detection
	 * algorithm between time low and time high
	 * Time_(L_LB) : Low time of the Low Bit(0)
	 * Time_(H_LB) : High time of the LOW Bit(0)
	 * Time_(L_HB) : Low time of the High Bit(1)
	 * Time_(H_HB) : High time of the High Bit(1)
	 *
	 * It can be simplified to:
	 * Low Bit(0) : 2 * Time_(H_LB) < Time_(L_LB)
	 * High Bit(1) : 2 * Time_(L_HB) < Time_(H_HB)
	 * HIGH  ___           ____    _..     _________    ___
	 *          |_________|    |_..  |____|         |__|
	 * LOW        <L_LB>  <H_LB>     <L_HB>  <H_HB>
	 *          [  Low Bit (0) ]     [  High Bit(1) ]
	 */
	if (bit) {
		gpiod_direction_output(led->ctrl_gpio, KTD2692_LOW);
		udelay(KTD2692_TIME_SHORT_BITSET_US);
		gpiod_direction_output(led->ctrl_gpio, KTD2692_HIGH);
		udelay(KTD2692_TIME_LONG_BITSET_US);
	} else {
		gpiod_direction_output(led->ctrl_gpio, KTD2692_LOW);
		udelay(KTD2692_TIME_LONG_BITSET_US);
		gpiod_direction_output(led->ctrl_gpio, KTD2692_HIGH);
		udelay(KTD2692_TIME_SHORT_BITSET_US);
	}
}

static void ktd2692_expresswire_write(struct ktd2692_context *led, u8 value)
{
	int i;

	ktd2692_expresswire_start(led);
	for (i = 7; i >= 0; i--)
		ktd2692_expresswire_set_bit(led, value & BIT(i));
	ktd2692_expresswire_end(led);
}

static int ktd2692_led_brightness_set(struct led_classdev *led_cdev,
				       enum led_brightness brightness)
{
	struct led_classdev_flash *fled_cdev = lcdev_to_flcdev(led_cdev);
	struct ktd2692_context *led = fled_cdev_to_led(fled_cdev);

	mutex_lock(&led->lock);

	if (brightness == LED_OFF) {
		led->mode = KTD2692_MODE_DISABLE;
		gpiod_direction_output(led->aux_gpio, KTD2692_LOW);
	} else {
		ktd2692_expresswire_write(led, brightness |
					KTD2692_REG_MOVIE_CURRENT_BASE);
		led->mode = KTD2692_MODE_MOVIE;
	}

	ktd2692_expresswire_write(led, led->mode | KTD2692_REG_MODE_BASE);
	mutex_unlock(&led->lock);
>>>>>>> rebase

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_OF
static struct of_device_id ktd2692_dt_ids[] = {
	{ .compatible = "qcom,ktd2692",},
	{},
};
/*MODULE_DEVICE_TABLE(of, ktd2692_dt_ids);*/
#endif

static struct platform_driver ktd2692_driver = {
	.driver = {
		   .name = ktd2692_NAME,
		   .owner = THIS_MODULE,
#ifdef CONFIG_OF
		   .of_match_table = ktd2692_dt_ids,
#endif
		   },
	.probe = ktd2692_probe,
	.remove = ktd2692_remove,
};


#if defined(CONFIG_SEC_R8Q_PROJECT)
static unsigned int system_rev __read_mostly;

static int __init sec_hw_rev_setup(char *p)
{
	int ret;

	ret = kstrtouint(p, 0, &system_rev);
	if (unlikely(ret < 0)) {
		pr_info("%s:androidboot.revision is malformed %s ",__func__, p);
		return -EINVAL;
	}

	pr_info("%s:androidboot.revision %x \n", __func__,system_rev);

	return 0;
}
early_param("androidboot.revision", sec_hw_rev_setup);

static unsigned int sec_hw_rev(void)
{
	return system_rev;
}
#endif

static int __init ktd2692_init(void)
{
	int rc = 0;
	
	#if defined(CONFIG_SEC_R8Q_PROJECT)
	unsigned int board_rev = sec_hw_rev();
	if(board_rev >= 8)
	{
		return platform_driver_register(&ktd2692_driver);
	}
	#else
		return platform_driver_register(&ktd2692_driver);
	#endif

	if (rc < 0) {
		pr_info("%s: platform_driver_register Failed: rc = %d",
			__func__, rc);
		return rc;
	}
	return rc;
}

static void __exit ktd2692_exit(void)
{
	platform_driver_unregister(&ktd2692_driver);
}

module_init(ktd2692_init);
module_exit(ktd2692_exit);

MODULE_DESCRIPTION("KTD2692 driver");
MODULE_LICENSE("GPL");
=======
static int ktd2692_led_flash_strobe_set(struct led_classdev_flash *fled_cdev,
					bool state)
{
	struct ktd2692_context *led = fled_cdev_to_led(fled_cdev);
	struct led_flash_setting *timeout = &fled_cdev->timeout;
	u32 flash_tm_reg;

	mutex_lock(&led->lock);

	if (state) {
		flash_tm_reg = GET_TIMEOUT_OFFSET(timeout->val, timeout->step);
		ktd2692_expresswire_write(led, flash_tm_reg
				| KTD2692_REG_FLASH_TIMEOUT_BASE);

		led->mode = KTD2692_MODE_FLASH;
		gpiod_direction_output(led->aux_gpio, KTD2692_HIGH);
	} else {
		led->mode = KTD2692_MODE_DISABLE;
		gpiod_direction_output(led->aux_gpio, KTD2692_LOW);
	}

	ktd2692_expresswire_write(led, led->mode | KTD2692_REG_MODE_BASE);

	fled_cdev->led_cdev.brightness = LED_OFF;
	led->mode = KTD2692_MODE_DISABLE;

	mutex_unlock(&led->lock);

	return 0;
}

static int ktd2692_led_flash_timeout_set(struct led_classdev_flash *fled_cdev,
					 u32 timeout)
{
	return 0;
}

static void ktd2692_init_movie_current_max(struct ktd2692_led_config_data *cfg)
{
	u32 offset, step;
	u32 movie_current_microamp;

	offset = KTD2692_MOVIE_MODE_CURRENT_LEVELS;
	step = KTD2692_MM_TO_FL_RATIO(cfg->flash_max_microamp)
		/ KTD2692_MOVIE_MODE_CURRENT_LEVELS;

	do {
		movie_current_microamp = step * offset;
		offset--;
	} while ((movie_current_microamp > cfg->movie_max_microamp) &&
		(offset > 0));

	cfg->max_brightness = offset;
}

static void ktd2692_init_flash_timeout(struct led_classdev_flash *fled_cdev,
				       struct ktd2692_led_config_data *cfg)
{
	struct led_flash_setting *setting;

	setting = &fled_cdev->timeout;
	setting->min = KTD2692_FLASH_MODE_TIMEOUT_DISABLE;
	setting->max = cfg->flash_max_timeout;
	setting->step = cfg->flash_max_timeout
			/ (KTD2692_FLASH_MODE_TIMEOUT_LEVELS - 1);
	setting->val = cfg->flash_max_timeout;
}

static void ktd2692_setup(struct ktd2692_context *led)
{
	led->mode = KTD2692_MODE_DISABLE;
	ktd2692_expresswire_reset(led);
	gpiod_direction_output(led->aux_gpio, KTD2692_LOW);

	ktd2692_expresswire_write(led, (KTD2962_MM_MIN_CURR_THRESHOLD_SCALE - 1)
				 | KTD2692_REG_MM_MIN_CURR_THRESHOLD_BASE);
	ktd2692_expresswire_write(led, KTD2692_FLASH_MODE_CURR_PERCENT(45)
				 | KTD2692_REG_FLASH_CURRENT_BASE);
}

static void regulator_disable_action(void *_data)
{
	struct device *dev = _data;
	struct ktd2692_context *led = dev_get_drvdata(dev);
	int ret;

	ret = regulator_disable(led->regulator);
	if (ret)
		dev_err(dev, "Failed to disable supply: %d\n", ret);
}

static int ktd2692_parse_dt(struct ktd2692_context *led, struct device *dev,
			    struct ktd2692_led_config_data *cfg)
{
	struct device_node *np = dev->of_node;
	struct device_node *child_node;
	int ret;

	if (!dev->of_node)
		return -ENXIO;

	led->ctrl_gpio = devm_gpiod_get(dev, "ctrl", GPIOD_ASIS);
	ret = PTR_ERR_OR_ZERO(led->ctrl_gpio);
	if (ret) {
		dev_err(dev, "cannot get ctrl-gpios %d\n", ret);
		return ret;
	}

	led->aux_gpio = devm_gpiod_get(dev, "aux", GPIOD_ASIS);
	ret = PTR_ERR_OR_ZERO(led->aux_gpio);
	if (ret) {
		dev_err(dev, "cannot get aux-gpios %d\n", ret);
		return ret;
	}

	led->regulator = devm_regulator_get(dev, "vin");
	if (IS_ERR(led->regulator))
		led->regulator = NULL;

	if (led->regulator) {
		ret = regulator_enable(led->regulator);
		if (ret) {
			dev_err(dev, "Failed to enable supply: %d\n", ret);
		} else {
			ret = devm_add_action_or_reset(dev,
						regulator_disable_action, dev);
			if (ret)
				return ret;
		}
	}

	child_node = of_get_next_available_child(np, NULL);
	if (!child_node) {
		dev_err(dev, "No DT child node found for connected LED.\n");
		return -EINVAL;
	}

	led->fled_cdev.led_cdev.name =
		of_get_property(child_node, "label", NULL) ? : child_node->name;

	ret = of_property_read_u32(child_node, "led-max-microamp",
				   &cfg->movie_max_microamp);
	if (ret) {
		dev_err(dev, "failed to parse led-max-microamp\n");
		goto err_parse_dt;
	}

	ret = of_property_read_u32(child_node, "flash-max-microamp",
				   &cfg->flash_max_microamp);
	if (ret) {
		dev_err(dev, "failed to parse flash-max-microamp\n");
		goto err_parse_dt;
	}

	ret = of_property_read_u32(child_node, "flash-max-timeout-us",
				   &cfg->flash_max_timeout);
	if (ret) {
		dev_err(dev, "failed to parse flash-max-timeout-us\n");
		goto err_parse_dt;
	}

err_parse_dt:
	of_node_put(child_node);
	return ret;
}

static const struct led_flash_ops flash_ops = {
	.strobe_set = ktd2692_led_flash_strobe_set,
	.timeout_set = ktd2692_led_flash_timeout_set,
};

static int ktd2692_probe(struct platform_device *pdev)
{
	struct ktd2692_context *led;
	struct led_classdev *led_cdev;
	struct led_classdev_flash *fled_cdev;
	struct ktd2692_led_config_data led_cfg;
	int ret;

	led = devm_kzalloc(&pdev->dev, sizeof(*led), GFP_KERNEL);
	if (!led)
		return -ENOMEM;

	fled_cdev = &led->fled_cdev;
	led_cdev = &fled_cdev->led_cdev;

	ret = ktd2692_parse_dt(led, &pdev->dev, &led_cfg);
	if (ret)
		return ret;

	ktd2692_init_flash_timeout(fled_cdev, &led_cfg);
	ktd2692_init_movie_current_max(&led_cfg);

	fled_cdev->ops = &flash_ops;

	led_cdev->max_brightness = led_cfg.max_brightness;
	led_cdev->brightness_set_blocking = ktd2692_led_brightness_set;
	led_cdev->flags |= LED_CORE_SUSPENDRESUME | LED_DEV_CAP_FLASH;

	mutex_init(&led->lock);

	platform_set_drvdata(pdev, led);

	ret = led_classdev_flash_register(&pdev->dev, fled_cdev);
	if (ret) {
		dev_err(&pdev->dev, "can't register LED %s\n", led_cdev->name);
		mutex_destroy(&led->lock);
		return ret;
	}

	ktd2692_setup(led);

	return 0;
}

static int ktd2692_remove(struct platform_device *pdev)
{
	struct ktd2692_context *led = platform_get_drvdata(pdev);

	led_classdev_flash_unregister(&led->fled_cdev);

	mutex_destroy(&led->lock);

	return 0;
}

static const struct of_device_id ktd2692_match[] = {
	{ .compatible = "kinetic,ktd2692", },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, ktd2692_match);

static struct platform_driver ktd2692_driver = {
	.driver = {
		.name  = "ktd2692",
		.of_match_table = ktd2692_match,
	},
	.probe  = ktd2692_probe,
	.remove = ktd2692_remove,
};

module_platform_driver(ktd2692_driver);

MODULE_AUTHOR("Ingi Kim <ingi2.kim@samsung.com>");
MODULE_DESCRIPTION("Kinetic KTD2692 LED driver");
MODULE_LICENSE("GPL v2");
>>>>>>> rebase

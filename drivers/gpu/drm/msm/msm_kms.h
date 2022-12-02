/*
<<<<<<< HEAD
 * Copyright (c) 2016-2019, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2016-2018, The Linux Foundation. All rights reserved.
>>>>>>> rebase
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MSM_KMS_H__
#define __MSM_KMS_H__

#include <linux/clk.h>
#include <linux/regulator/consumer.h>

#include "msm_drv.h"

#define MAX_PLANE	4

<<<<<<< HEAD
#if defined(CONFIG_DISPLAY_SAMSUNG)
enum mdss_intf_events {
	SS_EVENT_FRAME_UPDATE_POST = 0,
	SS_EVENT_FRAME_UPDATE_PRE,
	SS_EVENT_FB_EVENT_CALLBACK,
	SS_EVENT_PANEL_ON,
	SS_EVENT_PANEL_OFF,
	SS_EVENT_PANEL_RECOVERY,
	SS_EVENT_PANEL_ESD_RECOVERY,
	SS_EVENT_CHECK_TE,
	SS_EVENT_SDE_HW_CATALOG_INIT,
	SS_EVENT_MAX,
};
#endif

=======
>>>>>>> rebase
/* As there are different display controller blocks depending on the
 * snapdragon version, the kms support is split out and the appropriate
 * implementation is loaded at runtime.  The kms module is responsible
 * for constructing the appropriate planes/crtcs/encoders/connectors.
 */
struct msm_kms_funcs {
	/* hw initialization: */
	int (*hw_init)(struct msm_kms *kms);
	/* irq handling: */
	void (*irq_preinstall)(struct msm_kms *kms);
	int (*irq_postinstall)(struct msm_kms *kms);
	void (*irq_uninstall)(struct msm_kms *kms);
	irqreturn_t (*irq)(struct msm_kms *kms);
	int (*enable_vblank)(struct msm_kms *kms, struct drm_crtc *crtc);
	void (*disable_vblank)(struct msm_kms *kms, struct drm_crtc *crtc);
	/* modeset, bracketing atomic_commit(): */
<<<<<<< HEAD
	void (*prepare_commit)(struct msm_kms *kms,
				struct drm_atomic_state *state);
	void (*commit)(struct msm_kms *kms, struct drm_atomic_state *state);
	void (*complete_commit)(struct msm_kms *kms,
				struct drm_atomic_state *state);
=======
	void (*prepare_commit)(struct msm_kms *kms, struct drm_atomic_state *state);
	void (*commit)(struct msm_kms *kms, struct drm_atomic_state *state);
	void (*complete_commit)(struct msm_kms *kms, struct drm_atomic_state *state);
>>>>>>> rebase
	/* functions to wait for atomic commit completed on each CRTC */
	void (*wait_for_crtc_commit_done)(struct msm_kms *kms,
					struct drm_crtc *crtc);
	/* get msm_format w/ optional format modifiers from drm_mode_fb_cmd2 */
	const struct msm_format *(*get_format)(struct msm_kms *kms,
					const uint32_t format,
					const uint64_t modifiers);
	/* do format checking on format modified through fb_cmd2 modifiers */
	int (*check_modified_format)(const struct msm_kms *kms,
			const struct msm_format *msm_fmt,
			const struct drm_mode_fb_cmd2 *cmd,
			struct drm_gem_object **bos);
	/* misc: */
	long (*round_pixclk)(struct msm_kms *kms, unsigned long rate,
			struct drm_encoder *encoder);
	int (*set_split_display)(struct msm_kms *kms,
			struct drm_encoder *encoder,
			struct drm_encoder *slave_encoder,
			bool is_cmd_mode);
	void (*set_encoder_mode)(struct msm_kms *kms,
				 struct drm_encoder *encoder,
				 bool cmd_mode);
	/* pm suspend/resume hooks */
	int (*pm_suspend)(struct device *dev);
	int (*pm_resume)(struct device *dev);
	/* cleanup: */
	void (*destroy)(struct msm_kms *kms);
#ifdef CONFIG_DEBUG_FS
	/* debugfs: */
	int (*debugfs_init)(struct msm_kms *kms, struct drm_minor *minor);
#endif
<<<<<<< HEAD

#if defined(CONFIG_DISPLAY_SAMSUNG)
	int (*ss_callback)(int display_ndx,
			enum mdss_intf_events event, void *arg);
#endif
=======
>>>>>>> rebase
};

struct msm_kms {
	const struct msm_kms_funcs *funcs;

	/* irq number to be passed on to drm_irq_install */
	int irq;

	/* mapper-id used to request GEM buffer mapped for scanout: */
	struct msm_gem_address_space *aspace;
};

static inline void msm_kms_init(struct msm_kms *kms,
		const struct msm_kms_funcs *funcs)
{
	kms->funcs = funcs;
}

struct msm_kms *mdp4_kms_init(struct drm_device *dev);
struct msm_kms *mdp5_kms_init(struct drm_device *dev);
struct msm_kms *dpu_kms_init(struct drm_device *dev);

struct msm_mdss_funcs {
	int (*enable)(struct msm_mdss *mdss);
	int (*disable)(struct msm_mdss *mdss);
	void (*destroy)(struct drm_device *dev);
};

struct msm_mdss {
	struct drm_device *dev;
	const struct msm_mdss_funcs *funcs;
};

int mdp5_mdss_init(struct drm_device *dev);
int dpu_mdss_init(struct drm_device *dev);

#endif /* __MSM_KMS_H__ */

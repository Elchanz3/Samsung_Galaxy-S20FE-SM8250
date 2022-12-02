/*
 * Copyright (C) 2015 Red Hat, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef VIRTIO_DRV_H
#define VIRTIO_DRV_H

#include <linux/virtio.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_config.h>
#include <linux/virtio_gpu.h>

#include <drm/drmP.h>
#include <drm/drm_gem.h>
#include <drm/drm_atomic.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_encoder.h>
<<<<<<< HEAD
#include <drm/drm_fb_helper.h>
=======
>>>>>>> rebase
#include <drm/ttm/ttm_bo_api.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_module.h>

#define DRIVER_NAME "virtio_gpu"
#define DRIVER_DESC "virtio GPU"
#define DRIVER_DATE "0"

#define DRIVER_MAJOR 0
<<<<<<< HEAD
#define DRIVER_MINOR 1
#define DRIVER_PATCHLEVEL 0
=======
#define DRIVER_MINOR 0
#define DRIVER_PATCHLEVEL 1
>>>>>>> rebase

/* virtgpu_drm_bus.c */
int drm_virtio_init(struct drm_driver *driver, struct virtio_device *vdev);

<<<<<<< HEAD
struct virtio_gpu_object_params {
	uint32_t format;
	uint32_t width;
	uint32_t height;
	unsigned long size;
	bool dumb;
	/* 3d */
	bool virgl;
	uint32_t target;
	uint32_t bind;
	uint32_t depth;
	uint32_t array_size;
	uint32_t last_level;
	uint32_t nr_samples;
	uint32_t flags;
};

=======
>>>>>>> rebase
struct virtio_gpu_object {
	struct drm_gem_object gem_base;
	uint32_t hw_res_handle;

	struct sg_table *pages;
<<<<<<< HEAD
	uint32_t mapped;
=======
>>>>>>> rebase
	void *vmap;
	bool dumb;
	struct ttm_place                placement_code;
	struct ttm_placement		placement;
	struct ttm_buffer_object	tbo;
	struct ttm_bo_kmap_obj		kmap;
<<<<<<< HEAD
	bool created;
=======
>>>>>>> rebase
};
#define gem_to_virtio_gpu_obj(gobj) \
	container_of((gobj), struct virtio_gpu_object, gem_base)

struct virtio_gpu_vbuffer;
struct virtio_gpu_device;

typedef void (*virtio_gpu_resp_cb)(struct virtio_gpu_device *vgdev,
				   struct virtio_gpu_vbuffer *vbuf);

struct virtio_gpu_fence_driver {
	atomic64_t       last_seq;
	uint64_t         sync_seq;
	uint64_t         context;
	struct list_head fences;
	spinlock_t       lock;
};

struct virtio_gpu_fence {
	struct dma_fence f;
	struct virtio_gpu_fence_driver *drv;
	struct list_head node;
<<<<<<< HEAD
=======
	uint64_t seq;
>>>>>>> rebase
};
#define to_virtio_fence(x) \
	container_of(x, struct virtio_gpu_fence, f)

struct virtio_gpu_vbuffer {
	char *buf;
	int size;

	void *data_buf;
	uint32_t data_size;

	char *resp_buf;
	int resp_size;

	virtio_gpu_resp_cb resp_cb;

	struct list_head list;
};

struct virtio_gpu_output {
	int index;
	struct drm_crtc crtc;
	struct drm_connector conn;
	struct drm_encoder enc;
	struct virtio_gpu_display_one info;
	struct virtio_gpu_update_cursor cursor;
<<<<<<< HEAD
	struct edid *edid;
	int cur_x;
	int cur_y;
	bool enabled;
=======
	int cur_x;
	int cur_y;
>>>>>>> rebase
};
#define drm_crtc_to_virtio_gpu_output(x) \
	container_of(x, struct virtio_gpu_output, crtc)
#define drm_connector_to_virtio_gpu_output(x) \
	container_of(x, struct virtio_gpu_output, conn)
#define drm_encoder_to_virtio_gpu_output(x) \
	container_of(x, struct virtio_gpu_output, enc)

struct virtio_gpu_framebuffer {
	struct drm_framebuffer base;
	int x1, y1, x2, y2; /* dirty rect */
	spinlock_t dirty_lock;
	uint32_t hw_res_handle;
<<<<<<< HEAD
	struct virtio_gpu_fence *fence;
=======
>>>>>>> rebase
};
#define to_virtio_gpu_framebuffer(x) \
	container_of(x, struct virtio_gpu_framebuffer, base)

struct virtio_gpu_mman {
	struct ttm_bo_global_ref        bo_global_ref;
	struct drm_global_reference	mem_global_ref;
	bool				mem_global_referenced;
	struct ttm_bo_device		bdev;
};

<<<<<<< HEAD
=======
struct virtio_gpu_fbdev;

>>>>>>> rebase
struct virtio_gpu_queue {
	struct virtqueue *vq;
	spinlock_t qlock;
	wait_queue_head_t ack_queue;
	struct work_struct dequeue_work;
};

struct virtio_gpu_drv_capset {
	uint32_t id;
	uint32_t max_version;
	uint32_t max_size;
};

struct virtio_gpu_drv_cap_cache {
	struct list_head head;
	void *caps_cache;
	uint32_t id;
	uint32_t version;
	uint32_t size;
	atomic_t is_valid;
};

struct virtio_gpu_device {
	struct device *dev;
	struct drm_device *ddev;

	struct virtio_device *vdev;

	struct virtio_gpu_mman mman;

<<<<<<< HEAD
=======
	/* pointer to fbdev info structure */
	struct virtio_gpu_fbdev *vgfbdev;
>>>>>>> rebase
	struct virtio_gpu_output outputs[VIRTIO_GPU_MAX_SCANOUTS];
	uint32_t num_scanouts;

	struct virtio_gpu_queue ctrlq;
	struct virtio_gpu_queue cursorq;
	struct kmem_cache *vbufs;
	bool vqs_ready;

<<<<<<< HEAD
	struct ida	resource_ida;
=======
	struct idr	resource_idr;
	spinlock_t resource_idr_lock;
>>>>>>> rebase

	wait_queue_head_t resp_wq;
	/* current display info */
	spinlock_t display_info_lock;
	bool display_info_pending;

	struct virtio_gpu_fence_driver fence_drv;

<<<<<<< HEAD
	struct ida	ctx_id_ida;

	bool has_virgl_3d;
	bool has_edid;
=======
	struct idr	ctx_id_idr;
	spinlock_t ctx_id_idr_lock;

	bool has_virgl_3d;
>>>>>>> rebase

	struct work_struct config_changed_work;

	struct virtio_gpu_drv_capset *capsets;
	uint32_t num_capsets;
	struct list_head cap_cache;
};

struct virtio_gpu_fpriv {
	uint32_t ctx_id;
};

/* virtio_ioctl.c */
#define DRM_VIRTIO_NUM_IOCTLS 10
extern struct drm_ioctl_desc virtio_gpu_ioctls[DRM_VIRTIO_NUM_IOCTLS];
<<<<<<< HEAD
int virtio_gpu_object_list_validate(struct ww_acquire_ctx *ticket,
				    struct list_head *head);
void virtio_gpu_unref_list(struct list_head *head);
=======
>>>>>>> rebase

/* virtio_kms.c */
int virtio_gpu_driver_load(struct drm_device *dev, unsigned long flags);
void virtio_gpu_driver_unload(struct drm_device *dev);
int virtio_gpu_driver_open(struct drm_device *dev, struct drm_file *file);
void virtio_gpu_driver_postclose(struct drm_device *dev, struct drm_file *file);

/* virtio_gem.c */
void virtio_gpu_gem_free_object(struct drm_gem_object *gem_obj);
int virtio_gpu_gem_init(struct virtio_gpu_device *vgdev);
void virtio_gpu_gem_fini(struct virtio_gpu_device *vgdev);
int virtio_gpu_gem_create(struct drm_file *file,
			  struct drm_device *dev,
<<<<<<< HEAD
			  struct virtio_gpu_object_params *params,
=======
			  uint64_t size,
>>>>>>> rebase
			  struct drm_gem_object **obj_p,
			  uint32_t *handle_p);
int virtio_gpu_gem_object_open(struct drm_gem_object *obj,
			       struct drm_file *file);
void virtio_gpu_gem_object_close(struct drm_gem_object *obj,
				 struct drm_file *file);
<<<<<<< HEAD
struct virtio_gpu_object*
virtio_gpu_alloc_object(struct drm_device *dev,
			struct virtio_gpu_object_params *params,
			struct virtio_gpu_fence *fence);
=======
struct virtio_gpu_object *virtio_gpu_alloc_object(struct drm_device *dev,
						  size_t size, bool kernel,
						  bool pinned);
>>>>>>> rebase
int virtio_gpu_mode_dumb_create(struct drm_file *file_priv,
				struct drm_device *dev,
				struct drm_mode_create_dumb *args);
int virtio_gpu_mode_dumb_mmap(struct drm_file *file_priv,
			      struct drm_device *dev,
			      uint32_t handle, uint64_t *offset_p);

/* virtio_fb */
<<<<<<< HEAD
=======
#define VIRTIO_GPUFB_CONN_LIMIT 1
int virtio_gpu_fbdev_init(struct virtio_gpu_device *vgdev);
void virtio_gpu_fbdev_fini(struct virtio_gpu_device *vgdev);
>>>>>>> rebase
int virtio_gpu_surface_dirty(struct virtio_gpu_framebuffer *qfb,
			     struct drm_clip_rect *clips,
			     unsigned int num_clips);
/* virtio vg */
int virtio_gpu_alloc_vbufs(struct virtio_gpu_device *vgdev);
void virtio_gpu_free_vbufs(struct virtio_gpu_device *vgdev);
<<<<<<< HEAD
void virtio_gpu_cmd_create_resource(struct virtio_gpu_device *vgdev,
				    struct virtio_gpu_object *bo,
				    struct virtio_gpu_object_params *params,
				    struct virtio_gpu_fence *fence);
void virtio_gpu_cmd_unref_resource(struct virtio_gpu_device *vgdev,
				   uint32_t resource_id);
void virtio_gpu_cmd_transfer_to_host_2d(struct virtio_gpu_device *vgdev,
					struct virtio_gpu_object *bo,
					uint64_t offset,
					__le32 width, __le32 height,
					__le32 x, __le32 y,
					struct virtio_gpu_fence *fence);
=======
void virtio_gpu_resource_id_get(struct virtio_gpu_device *vgdev,
			       uint32_t *resid);
void virtio_gpu_resource_id_put(struct virtio_gpu_device *vgdev, uint32_t id);
void virtio_gpu_cmd_create_resource(struct virtio_gpu_device *vgdev,
				    uint32_t resource_id,
				    uint32_t format,
				    uint32_t width,
				    uint32_t height);
void virtio_gpu_cmd_unref_resource(struct virtio_gpu_device *vgdev,
				   uint32_t resource_id);
void virtio_gpu_cmd_transfer_to_host_2d(struct virtio_gpu_device *vgdev,
					uint32_t resource_id, uint64_t offset,
					__le32 width, __le32 height,
					__le32 x, __le32 y,
					struct virtio_gpu_fence **fence);
>>>>>>> rebase
void virtio_gpu_cmd_resource_flush(struct virtio_gpu_device *vgdev,
				   uint32_t resource_id,
				   uint32_t x, uint32_t y,
				   uint32_t width, uint32_t height);
void virtio_gpu_cmd_set_scanout(struct virtio_gpu_device *vgdev,
				uint32_t scanout_id, uint32_t resource_id,
				uint32_t width, uint32_t height,
				uint32_t x, uint32_t y);
int virtio_gpu_object_attach(struct virtio_gpu_device *vgdev,
			     struct virtio_gpu_object *obj,
<<<<<<< HEAD
			     struct virtio_gpu_fence *fence);
void virtio_gpu_object_detach(struct virtio_gpu_device *vgdev,
			      struct virtio_gpu_object *obj);
=======
			     uint32_t resource_id,
			     struct virtio_gpu_fence **fence);
>>>>>>> rebase
int virtio_gpu_attach_status_page(struct virtio_gpu_device *vgdev);
int virtio_gpu_detach_status_page(struct virtio_gpu_device *vgdev);
void virtio_gpu_cursor_ping(struct virtio_gpu_device *vgdev,
			    struct virtio_gpu_output *output);
int virtio_gpu_cmd_get_display_info(struct virtio_gpu_device *vgdev);
<<<<<<< HEAD
=======
void virtio_gpu_cmd_resource_inval_backing(struct virtio_gpu_device *vgdev,
					   uint32_t resource_id);
>>>>>>> rebase
int virtio_gpu_cmd_get_capset_info(struct virtio_gpu_device *vgdev, int idx);
int virtio_gpu_cmd_get_capset(struct virtio_gpu_device *vgdev,
			      int idx, int version,
			      struct virtio_gpu_drv_cap_cache **cache_p);
<<<<<<< HEAD
int virtio_gpu_cmd_get_edids(struct virtio_gpu_device *vgdev);
=======
>>>>>>> rebase
void virtio_gpu_cmd_context_create(struct virtio_gpu_device *vgdev, uint32_t id,
				   uint32_t nlen, const char *name);
void virtio_gpu_cmd_context_destroy(struct virtio_gpu_device *vgdev,
				    uint32_t id);
void virtio_gpu_cmd_context_attach_resource(struct virtio_gpu_device *vgdev,
					    uint32_t ctx_id,
					    uint32_t resource_id);
void virtio_gpu_cmd_context_detach_resource(struct virtio_gpu_device *vgdev,
					    uint32_t ctx_id,
					    uint32_t resource_id);
void virtio_gpu_cmd_submit(struct virtio_gpu_device *vgdev,
			   void *data, uint32_t data_size,
<<<<<<< HEAD
			   uint32_t ctx_id, struct virtio_gpu_fence *fence);
=======
			   uint32_t ctx_id, struct virtio_gpu_fence **fence);
>>>>>>> rebase
void virtio_gpu_cmd_transfer_from_host_3d(struct virtio_gpu_device *vgdev,
					  uint32_t resource_id, uint32_t ctx_id,
					  uint64_t offset, uint32_t level,
					  struct virtio_gpu_box *box,
<<<<<<< HEAD
					  struct virtio_gpu_fence *fence);
void virtio_gpu_cmd_transfer_to_host_3d(struct virtio_gpu_device *vgdev,
					struct virtio_gpu_object *bo,
					uint32_t ctx_id,
					uint64_t offset, uint32_t level,
					struct virtio_gpu_box *box,
					struct virtio_gpu_fence *fence);
void
virtio_gpu_cmd_resource_create_3d(struct virtio_gpu_device *vgdev,
				  struct virtio_gpu_object *bo,
				  struct virtio_gpu_object_params *params,
				  struct virtio_gpu_fence *fence);
=======
					  struct virtio_gpu_fence **fence);
void virtio_gpu_cmd_transfer_to_host_3d(struct virtio_gpu_device *vgdev,
					uint32_t resource_id, uint32_t ctx_id,
					uint64_t offset, uint32_t level,
					struct virtio_gpu_box *box,
					struct virtio_gpu_fence **fence);
void
virtio_gpu_cmd_resource_create_3d(struct virtio_gpu_device *vgdev,
				  struct virtio_gpu_resource_create_3d *rc_3d,
				  struct virtio_gpu_fence **fence);
>>>>>>> rebase
void virtio_gpu_ctrl_ack(struct virtqueue *vq);
void virtio_gpu_cursor_ack(struct virtqueue *vq);
void virtio_gpu_fence_ack(struct virtqueue *vq);
void virtio_gpu_dequeue_ctrl_func(struct work_struct *work);
void virtio_gpu_dequeue_cursor_func(struct work_struct *work);
void virtio_gpu_dequeue_fence_func(struct work_struct *work);

/* virtio_gpu_display.c */
int virtio_gpu_framebuffer_init(struct drm_device *dev,
				struct virtio_gpu_framebuffer *vgfb,
				const struct drm_mode_fb_cmd2 *mode_cmd,
				struct drm_gem_object *obj);
int virtio_gpu_modeset_init(struct virtio_gpu_device *vgdev);
void virtio_gpu_modeset_fini(struct virtio_gpu_device *vgdev);

/* virtio_gpu_plane.c */
uint32_t virtio_gpu_translate_format(uint32_t drm_fourcc);
struct drm_plane *virtio_gpu_plane_init(struct virtio_gpu_device *vgdev,
					enum drm_plane_type type,
					int index);

/* virtio_gpu_ttm.c */
int virtio_gpu_ttm_init(struct virtio_gpu_device *vgdev);
void virtio_gpu_ttm_fini(struct virtio_gpu_device *vgdev);
int virtio_gpu_mmap(struct file *filp, struct vm_area_struct *vma);

/* virtio_gpu_fence.c */
<<<<<<< HEAD
bool virtio_fence_signaled(struct dma_fence *f);
struct virtio_gpu_fence *virtio_gpu_fence_alloc(
	struct virtio_gpu_device *vgdev);
void virtio_gpu_fence_emit(struct virtio_gpu_device *vgdev,
			  struct virtio_gpu_ctrl_hdr *cmd_hdr,
			  struct virtio_gpu_fence *fence);
=======
int virtio_gpu_fence_emit(struct virtio_gpu_device *vgdev,
			  struct virtio_gpu_ctrl_hdr *cmd_hdr,
			  struct virtio_gpu_fence **fence);
>>>>>>> rebase
void virtio_gpu_fence_event_process(struct virtio_gpu_device *vdev,
				    u64 last_seq);

/* virtio_gpu_object */
int virtio_gpu_object_create(struct virtio_gpu_device *vgdev,
<<<<<<< HEAD
			     struct virtio_gpu_object_params *params,
			     struct virtio_gpu_object **bo_ptr,
			     struct virtio_gpu_fence *fence);
void virtio_gpu_object_kunmap(struct virtio_gpu_object *bo);
int virtio_gpu_object_kmap(struct virtio_gpu_object *bo);
=======
			     unsigned long size, bool kernel, bool pinned,
			     struct virtio_gpu_object **bo_ptr);
int virtio_gpu_object_kmap(struct virtio_gpu_object *bo, void **ptr);
>>>>>>> rebase
int virtio_gpu_object_get_sg_table(struct virtio_gpu_device *qdev,
				   struct virtio_gpu_object *bo);
void virtio_gpu_object_free_sg_table(struct virtio_gpu_object *bo);
int virtio_gpu_object_wait(struct virtio_gpu_object *bo, bool no_wait);

/* virtgpu_prime.c */
<<<<<<< HEAD
=======
int virtgpu_gem_prime_pin(struct drm_gem_object *obj);
void virtgpu_gem_prime_unpin(struct drm_gem_object *obj);
>>>>>>> rebase
struct sg_table *virtgpu_gem_prime_get_sg_table(struct drm_gem_object *obj);
struct drm_gem_object *virtgpu_gem_prime_import_sg_table(
	struct drm_device *dev, struct dma_buf_attachment *attach,
	struct sg_table *sgt);
void *virtgpu_gem_prime_vmap(struct drm_gem_object *obj);
void virtgpu_gem_prime_vunmap(struct drm_gem_object *obj, void *vaddr);
int virtgpu_gem_prime_mmap(struct drm_gem_object *obj,
			   struct vm_area_struct *vma);

static inline struct virtio_gpu_object*
virtio_gpu_object_ref(struct virtio_gpu_object *bo)
{
<<<<<<< HEAD
	ttm_bo_get(&bo->tbo);
=======
	ttm_bo_reference(&bo->tbo);
>>>>>>> rebase
	return bo;
}

static inline void virtio_gpu_object_unref(struct virtio_gpu_object **bo)
{
	struct ttm_buffer_object *tbo;

	if ((*bo) == NULL)
		return;
	tbo = &((*bo)->tbo);
<<<<<<< HEAD
	ttm_bo_put(tbo);
	*bo = NULL;
=======
	ttm_bo_unref(&tbo);
	if (tbo == NULL)
		*bo = NULL;
>>>>>>> rebase
}

static inline u64 virtio_gpu_object_mmap_offset(struct virtio_gpu_object *bo)
{
	return drm_vma_node_offset_addr(&bo->tbo.vma_node);
}

static inline int virtio_gpu_object_reserve(struct virtio_gpu_object *bo,
					 bool no_wait)
{
	int r;

	r = ttm_bo_reserve(&bo->tbo, true, no_wait, NULL);
	if (unlikely(r != 0)) {
		if (r != -ERESTARTSYS) {
			struct virtio_gpu_device *qdev =
				bo->gem_base.dev->dev_private;
			dev_err(qdev->dev, "%p reserve failed\n", bo);
		}
		return r;
	}
	return 0;
}

static inline void virtio_gpu_object_unreserve(struct virtio_gpu_object *bo)
{
	ttm_bo_unreserve(&bo->tbo);
}

/* virgl debufs */
int virtio_gpu_debugfs_init(struct drm_minor *minor);

#endif

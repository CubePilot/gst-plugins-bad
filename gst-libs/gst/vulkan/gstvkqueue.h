/*
 * GStreamer
 * Copyright (C) 2015 Matthew Waters <matthew@centricular.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GST_VULKAN_QUEUE_H__
#define __GST_VULKAN_QUEUE_H__

#include <gst/vulkan/gstvkdevice.h>
#include <gst/vulkan/gstvkcommandpool.h>

#define GST_TYPE_VULKAN_QUEUE         (gst_vulkan_queue_get_type())
#define GST_VULKAN_QUEUE(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), GST_TYPE_VULKAN_QUEUE, GstVulkanQueue))
#define GST_VULKAN_QUEUE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GST_TYPE_VULKAN_QUEUE, GstVulkanQueueClass))
#define GST_IS_VULKAN_QUEUE(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), GST_TYPE_VULKAN_QUEUE))
#define GST_IS_VULKAN_QUEUE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), GST_TYPE_VULKAN_QUEUE))
#define GST_VULKAN_QUEUE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), GST_TYPE_VULKAN_QUEUE, GstVulkanQueueClass))
GST_VULKAN_API
GType gst_vulkan_queue_get_type       (void);

#define GST_VULKAN_QUEUE_CONTEXT_TYPE_STR "gst.vulkan.queue"

struct _GstVulkanQueue
{
  GstObject parent;

  GstVulkanDevice *device;

  VkQueue queue; /* hides a pointer */
  guint32 family;
  guint32 index;
};

struct _GstVulkanQueueClass
{
  GstObjectClass parent_class;
};

GST_VULKAN_API
GstVulkanDevice *   gst_vulkan_queue_get_device (GstVulkanQueue * queue);

GST_VULKAN_API
GstVulkanCommandPool *  gst_vulkan_queue_create_command_pool    (GstVulkanQueue * queue,
                                                                 GError ** error);

GST_VULKAN_API
void                gst_context_set_vulkan_queue                (GstContext * context,
                                                                 GstVulkanQueue * queue);
GST_VULKAN_API
gboolean            gst_context_get_vulkan_queue                (GstContext * context,
                                                                 GstVulkanQueue ** queue);
GST_VULKAN_API
gboolean            gst_vulkan_queue_handle_context_query       (GstElement * element,
                                                                 GstQuery * query,
                                                                 GstVulkanQueue ** queue);
GST_VULKAN_API
gboolean            gst_vulkan_queue_run_context_query          (GstElement * element,
                                                                 GstVulkanQueue ** queue);

#endif /* __GST_VULKAN_QUEUE_H__ */

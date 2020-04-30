/* GStreamer Editing Services
 * Copyright (C) 2009 Edward Hervey <edward.hervey@collabora.co.uk>
 *               2009 Nokia Corporation
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

/**
 * SECTION: gesuriclip
 * @title: GESUriClip
 * @short_description: An object for manipulating media files in a GESTimeline
 *
 * Represents all the output streams from a particular uri. It is assumed that
 * the URI points to a file of some type.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ges-internal.h"
#include "ges-uri-clip.h"
#include "ges-source-clip.h"
#include "ges-video-uri-source.h"
#include "ges-audio-uri-source.h"
#include "ges-uri-asset.h"
#include "ges-track-element-asset.h"
#include "ges-extractable.h"
#include "ges-image-source.h"
#include "ges-audio-test-source.h"
#include "ges-multi-file-source.h"
#include "ges-layer.h"

static void ges_extractable_interface_init (GESExtractableInterface * iface);

#define parent_class ges_uri_clip_parent_class

struct _GESUriClipPrivate
{
  gchar *uri;

  gboolean mute;
  gboolean is_image;
};

enum
{
  PROP_0,
  PROP_URI,
  PROP_MUTE,
  PROP_IS_IMAGE,
  PROP_SUPPORTED_FORMATS,
};

G_DEFINE_TYPE_WITH_CODE (GESUriClip, ges_uri_clip,
    GES_TYPE_SOURCE_CLIP, G_ADD_PRIVATE (GESUriClip)
    G_IMPLEMENT_INTERFACE (GES_TYPE_EXTRACTABLE,
        ges_extractable_interface_init));

static GList *ges_uri_clip_create_track_elements (GESClip *
    clip, GESTrackType type);
static void ges_uri_clip_set_uri (GESUriClip * self, gchar * uri);

gboolean
uri_clip_set_max_duration (GESTimelineElement * element,
    GstClockTime maxduration);

static void
ges_uri_clip_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GESUriClipPrivate *priv = GES_URI_CLIP (object)->priv;

  switch (property_id) {
    case PROP_URI:
      g_value_set_string (value, priv->uri);
      break;
    case PROP_MUTE:
      g_value_set_boolean (value, priv->mute);
      break;
    case PROP_IS_IMAGE:
      g_value_set_boolean (value, priv->is_image);
      break;
    case PROP_SUPPORTED_FORMATS:
      g_value_set_flags (value,
          ges_clip_get_supported_formats (GES_CLIP (object)));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
ges_uri_clip_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GESUriClip *uriclip = GES_URI_CLIP (object);

  switch (property_id) {
    case PROP_URI:
      ges_uri_clip_set_uri (uriclip, g_value_dup_string (value));
      break;
    case PROP_MUTE:
      ges_uri_clip_set_mute (uriclip, g_value_get_boolean (value));
      break;
    case PROP_IS_IMAGE:
      ges_uri_clip_set_is_image (uriclip, g_value_get_boolean (value));
      break;
    case PROP_SUPPORTED_FORMATS:
      ges_clip_set_supported_formats (GES_CLIP (uriclip),
          g_value_get_flags (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
ges_uri_clip_finalize (GObject * object)
{
  GESUriClipPrivate *priv = GES_URI_CLIP (object)->priv;

  if (priv->uri)
    g_free (priv->uri);
  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
ges_uri_clip_class_init (GESUriClipClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GESClipClass *clip_class = GES_CLIP_CLASS (klass);
  GESTimelineElementClass *element_class = GES_TIMELINE_ELEMENT_CLASS (klass);

  object_class->get_property = ges_uri_clip_get_property;
  object_class->set_property = ges_uri_clip_set_property;
  object_class->finalize = ges_uri_clip_finalize;


  /**
   * GESUriClip:uri:
   *
   * The location of the file/resource to use.
   */
  g_object_class_install_property (object_class, PROP_URI,
      g_param_spec_string ("uri", "URI", "uri of the resource", NULL,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  /**
   * GESUriClip:mute:
   *
   * Whether the sound will be played or not.
   */
  g_object_class_install_property (object_class, PROP_MUTE,
      g_param_spec_boolean ("mute", "Mute", "Mute audio track",
          FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

  /**
   * GESUriClip:is-image:
   *
   * Whether this uri clip represents a still image or not. This must be set
   * before create_track_elements is called.
   */
  g_object_class_install_property (object_class, PROP_IS_IMAGE,
      g_param_spec_boolean ("is-image", "Is still image",
          "Whether the clip represents a still image or not",
          FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

  /* Redefine the supported formats property so the default value is UNKNOWN
   * and not AUDIO | VIDEO */
  g_object_class_install_property (object_class, PROP_SUPPORTED_FORMATS,
      g_param_spec_flags ("supported-formats",
          "Supported formats", "Formats supported by the file",
          GES_TYPE_TRACK_TYPE, GES_TRACK_TYPE_UNKNOWN,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

  element_class->set_max_duration = uri_clip_set_max_duration;

  clip_class->create_track_elements = ges_uri_clip_create_track_elements;
}

static gchar *
extractable_check_id (GType type, const gchar * id)
{
  if (gst_uri_is_valid (id))
    return g_strdup (id);

  return NULL;
}

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;       /* Start ignoring GParameter deprecation */

static GParameter *
extractable_get_parameters_from_id (const gchar * id, guint * n_params)
{
  GParameter *params = g_new0 (GParameter, 2);

  params[0].name = "uri";
  g_value_init (&params[0].value, G_TYPE_STRING);
  g_value_set_string (&params[0].value, id);

  *n_params = 1;

  return params;
}

G_GNUC_END_IGNORE_DEPRECATIONS; /* End ignoring GParameter deprecation */

static gchar *
extractable_get_id (GESExtractable * self)
{
  return g_strdup (GES_URI_CLIP (self)->priv->uri);
}

static gboolean
extractable_set_asset (GESExtractable * self, GESAsset * asset)
{
  gboolean res = TRUE, contains_core;
  GESUriClip *uriclip = GES_URI_CLIP (self);
  GESUriClipAsset *uri_clip_asset;
  GESClip *clip = GES_CLIP (self);
  GESLayer *layer = ges_clip_get_layer (clip);
  GList *tmp, *children;
  GHashTable *source_by_track;

  g_return_val_if_fail (GES_IS_URI_CLIP_ASSET (asset), FALSE);

  uri_clip_asset = GES_URI_CLIP_ASSET (asset);
  if (GST_CLOCK_TIME_IS_VALID (GES_TIMELINE_ELEMENT_DURATION (self)) &&
      ges_uri_clip_asset_get_duration (uri_clip_asset) <
      GES_TIMELINE_ELEMENT_INPOINT (self) +
      GES_TIMELINE_ELEMENT_DURATION (self)) {
    GST_INFO_OBJECT (self,
        "Can not set asset to %p as its duration is %" GST_TIME_FORMAT
        " < to inpoint %" GST_TIME_FORMAT " + %" GST_TIME_FORMAT " = %"
        GST_TIME_FORMAT, asset,
        GST_TIME_ARGS (ges_uri_clip_asset_get_duration (uri_clip_asset)),
        GST_TIME_ARGS (GES_TIMELINE_ELEMENT_INPOINT (self)),
        GST_TIME_ARGS (GES_TIMELINE_ELEMENT_DURATION (self)),
        GST_TIME_ARGS (GES_TIMELINE_ELEMENT_INPOINT (self) +
            GES_TIMELINE_ELEMENT_DURATION (self)));


    return FALSE;
  }

  if (GST_CLOCK_TIME_IS_VALID (GES_TIMELINE_ELEMENT_DURATION (clip)) == FALSE)
    _set_duration0 (GES_TIMELINE_ELEMENT (uriclip),
        ges_uri_clip_asset_get_duration (uri_clip_asset));

  ges_uri_clip_set_is_image (uriclip,
      ges_uri_clip_asset_is_image (uri_clip_asset));

  if (ges_clip_get_supported_formats (clip) == GES_TRACK_TYPE_UNKNOWN) {
    ges_clip_set_supported_formats (clip,
        ges_clip_asset_get_supported_formats (GES_CLIP_ASSET (uri_clip_asset)));
  }

  GES_TIMELINE_ELEMENT (uriclip)->asset = asset;

  source_by_track = g_hash_table_new_full (NULL, NULL,
      gst_object_unref, gst_object_unref);
  children = ges_container_get_children (GES_CONTAINER (self), TRUE);

  for (tmp = children; tmp; tmp = tmp->next) {
    GESTrackElement *child = tmp->data;
    GESTrack *track = ges_track_element_get_track (child);
    /* remove our core children */
    if (GES_TRACK_ELEMENT_IS_CORE (child)) {
      if (track)
        g_hash_table_insert (source_by_track, gst_object_ref (track),
            gst_object_ref (child));

      /* removing the track element from its clip whilst it is in a
       * timeline will remove it from its track */
      /* removing the core element will also empty its non-core siblings
       * from the same track */
      ges_container_remove (GES_CONTAINER (self), GES_TIMELINE_ELEMENT (child));
    }
  }
  g_list_free_full (children, g_object_unref);

  contains_core = FALSE;

  if (layer) {
    gst_object_ref (clip);

    ges_layer_remove_clip (layer, clip);
    /* adding back to the layer will trigger the re-creation of the core
     * children */
    res = ges_layer_add_clip (layer, clip);

    /* NOTE: assume that core children in the same tracks correspond to
     * the same source! */
    for (tmp = GES_CONTAINER_CHILDREN (clip); tmp; tmp = tmp->next) {
      GESTrackElement *child = tmp->data;
      if (GES_TRACK_ELEMENT_IS_CORE (child)) {
        GESTrackElement *orig_source = g_hash_table_lookup (source_by_track,
            ges_track_element_get_track (child));
        contains_core = TRUE;

        if (orig_source) {
          ges_track_element_copy_properties (GES_TIMELINE_ELEMENT (orig_source),
              GES_TIMELINE_ELEMENT (child));
          ges_track_element_copy_bindings (orig_source, child,
              GST_CLOCK_TIME_NONE);
        }
      }
    }
    gst_object_unref (clip);
    gst_object_unref (layer);
  }
  g_hash_table_unref (source_by_track);

  if (res) {
    g_free (uriclip->priv->uri);
    uriclip->priv->uri = g_strdup (ges_asset_get_id (asset));
  }

  if (!contains_core)
    ges_timeline_element_set_max_duration (GES_TIMELINE_ELEMENT (uriclip),
        ges_uri_clip_asset_get_max_duration (uri_clip_asset));

  return res;
}

static void
ges_extractable_interface_init (GESExtractableInterface * iface)
{
  iface->asset_type = GES_TYPE_URI_CLIP_ASSET;
  iface->check_id = (GESExtractableCheckId) extractable_check_id;
  iface->get_parameters_from_id = extractable_get_parameters_from_id;
  iface->get_id = extractable_get_id;
  iface->get_id = extractable_get_id;
  iface->can_update_asset = TRUE;
  iface->set_asset_full = extractable_set_asset;
}

static void
ges_uri_clip_init (GESUriClip * self)
{
  self->priv = ges_uri_clip_get_instance_private (self);

  /* Setting the duration to -1 by default. */
  GES_TIMELINE_ELEMENT (self)->duration = GST_CLOCK_TIME_NONE;
}

/**
 * ges_uri_clip_set_mute:
 * @self: the #GESUriClip on which to mute or unmute the audio track
 * @mute: %TRUE to mute @self audio track, %FALSE to unmute it
 *
 * Sets whether the audio track of this clip is muted or not.
 *
 */
void
ges_uri_clip_set_mute (GESUriClip * self, gboolean mute)
{
  GList *tmp;

  GST_DEBUG ("self:%p, mute:%d", self, mute);

  self->priv->mute = mute;

  /* Go over tracked objects, and update 'active' status on all audio objects */
  for (tmp = GES_CONTAINER_CHILDREN (self); tmp; tmp = g_list_next (tmp)) {
    GESTrackElement *trackelement = (GESTrackElement *) tmp->data;
    GESTrack *track = ges_track_element_get_track (trackelement);

    if (track && track->type == GES_TRACK_TYPE_AUDIO)
      ges_track_element_set_active (trackelement, !mute);
  }
}

gboolean
uri_clip_set_max_duration (GESTimelineElement * element,
    GstClockTime maxduration)
{
  gboolean ret =
      GES_TIMELINE_ELEMENT_CLASS (parent_class)->set_max_duration (element,
      maxduration);

  if (ret) {
    GstClockTime limit = ges_clip_get_duration_limit (GES_CLIP (element));
    if (GST_CLOCK_TIME_IS_VALID (limit) && (element->duration == 0))
      _set_duration0 (element, limit);
  }

  return ret;
}

/**
 * ges_uri_clip_set_is_image:
 * @self: the #GESUriClip
 * @is_image: %TRUE if @self is a still image, %FALSE otherwise
 *
 * Sets whether the clip is a still image or not.
 */
void
ges_uri_clip_set_is_image (GESUriClip * self, gboolean is_image)
{
  self->priv->is_image = is_image;
}

/**
 * ges_uri_clip_is_muted:
 * @self: the #GESUriClip
 *
 * Lets you know if the audio track of @self is muted or not.
 *
 * Returns: %TRUE if the audio track of @self is muted, %FALSE otherwise.
 */
gboolean
ges_uri_clip_is_muted (GESUriClip * self)
{
  return self->priv->mute;
}

/**
 * ges_uri_clip_is_image:
 * @self: the #GESUriClip
 *
 * Lets you know if @self is an image or not.
 *
 * Returns: %TRUE if @self is a still image %FALSE otherwise.
 */
gboolean
ges_uri_clip_is_image (GESUriClip * self)
{
  return self->priv->is_image;
}

/**
 * ges_uri_clip_get_uri:
 * @self: the #GESUriClip
 *
 * Get the location of the resource.
 *
 * Returns: The location of the resource.
 */
const gchar *
ges_uri_clip_get_uri (GESUriClip * self)
{
  return self->priv->uri;
}

static GList *
ges_uri_clip_create_track_elements (GESClip * clip, GESTrackType type)
{
  GList *res = NULL;
  const GList *tmp, *stream_assets;
  GESAsset *asset = GES_TIMELINE_ELEMENT (clip)->asset;
  GESUriClipAsset *uri_asset;
  GstClockTime max_duration;

  g_return_val_if_fail (asset, NULL);

  uri_asset = GES_URI_CLIP_ASSET (asset);

  max_duration = ges_uri_clip_asset_get_max_duration (uri_asset);
  stream_assets = ges_uri_clip_asset_get_stream_assets (uri_asset);

  for (tmp = stream_assets; tmp; tmp = tmp->next) {
    GESTrackElementAsset *element_asset = GES_TRACK_ELEMENT_ASSET (tmp->data);

    if (ges_track_element_asset_get_track_type (element_asset) == type) {
      GESTrackElement *element =
          GES_TRACK_ELEMENT (ges_asset_extract (GES_ASSET (element_asset),
              NULL));
      ges_timeline_element_set_max_duration (GES_TIMELINE_ELEMENT (element),
          max_duration);
      res = g_list_prepend (res, element);
    }
  }

  return res;
}

/**
 * ges_uri_clip_new:
 * @uri: the URI the source should control
 *
 * Creates a new #GESUriClip for the provided @uri.
 *
 * Returns: (transfer floating) (nullable): The newly created #GESUriClip, or
 * %NULL if there was an error.
 */
GESUriClip *
ges_uri_clip_new (const gchar * uri)
{
  GESAsset *asset = GES_ASSET (ges_uri_clip_asset_request_sync (uri, NULL));
  GESUriClip *res = NULL;

  if (asset) {
    res = GES_URI_CLIP (ges_asset_extract (asset, NULL));
    gst_object_unref (asset);
  } else
    GST_ERROR ("Could not create asset for uri: %s", uri);

  return res;
}

void
ges_uri_clip_set_uri (GESUriClip * self, gchar * uri)
{
  if (GES_CONTAINER_CHILDREN (self)) {
    /* FIXME handle this case properly */
    GST_WARNING_OBJECT (self, "Can not change uri when already"
        "containing TrackElements");

    return;
  }

  self->priv->uri = uri;
}
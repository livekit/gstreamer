/* GStreamer
 * Copyright (C) 2003 Benjamin Otte <in7y118@public.uni-hamburg.de>
 * Copyright (C) 2005-2009 Tim-Philipp Müller <tim centricular net>
 * Copyright (C) 2009 Sebastian Dröge <sebastian.droege@collabora.co.uk>
 * Copyright (C) 2020 Huawei Technologies Co., Ltd.
 *   @Author: Stéphane Cerveau <scerveau@collabora.com>
 *
 * gsttypefindfunctionsplugin.h: collection of various typefind functions
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


#ifndef __GST_TYPE_FIND_FUNCTIONS_PLUGIN_H__
#define __GST_TYPE_FIND_FUNCTIONS_PLUGIN_H__

#include <gst/gst.h>

GST_DEBUG_CATEGORY_EXTERN (type_find_functions_debug);
#define GST_CAT_DEFAULT type_find_functions_debug

/*** plugin initialization ***/

/*RIFF type find declaration */
GST_TYPE_FIND_REGISTER_DECLARE (avi);
GST_TYPE_FIND_REGISTER_DECLARE (qcp);
GST_TYPE_FIND_REGISTER_DECLARE (cdxa);
GST_TYPE_FIND_REGISTER_DECLARE (riff_mid);
GST_TYPE_FIND_REGISTER_DECLARE (wav);
GST_TYPE_FIND_REGISTER_DECLARE (webp);

/*'Start with' type find declaration */
GST_TYPE_FIND_REGISTER_DECLARE (asf);
GST_TYPE_FIND_REGISTER_DECLARE (vcd);
GST_TYPE_FIND_REGISTER_DECLARE (imelody);
GST_TYPE_FIND_REGISTER_DECLARE (scc);
#if 0
GST_TYPE_FIND_REGISTER_DECLARE (smoke);
#endif
GST_TYPE_FIND_REGISTER_DECLARE (rmf);
GST_TYPE_FIND_REGISTER_DECLARE (ram);
GST_TYPE_FIND_REGISTER_DECLARE (flv);
GST_TYPE_FIND_REGISTER_DECLARE (nist);
GST_TYPE_FIND_REGISTER_DECLARE (voc);
GST_TYPE_FIND_REGISTER_DECLARE (w64);
GST_TYPE_FIND_REGISTER_DECLARE (rf64);
GST_TYPE_FIND_REGISTER_DECLARE (gif);
GST_TYPE_FIND_REGISTER_DECLARE (png);
GST_TYPE_FIND_REGISTER_DECLARE (mve);
GST_TYPE_FIND_REGISTER_DECLARE (amr);
GST_TYPE_FIND_REGISTER_DECLARE (amr_wb);
GST_TYPE_FIND_REGISTER_DECLARE (sid);
GST_TYPE_FIND_REGISTER_DECLARE (xcf);
GST_TYPE_FIND_REGISTER_DECLARE (mng);
GST_TYPE_FIND_REGISTER_DECLARE (jng);
GST_TYPE_FIND_REGISTER_DECLARE (xpm);
GST_TYPE_FIND_REGISTER_DECLARE (ras);
GST_TYPE_FIND_REGISTER_DECLARE (bz2);
GST_TYPE_FIND_REGISTER_DECLARE (gz);
GST_TYPE_FIND_REGISTER_DECLARE (zip);
GST_TYPE_FIND_REGISTER_DECLARE (z);
GST_TYPE_FIND_REGISTER_DECLARE (elf);
GST_TYPE_FIND_REGISTER_DECLARE (spc);
GST_TYPE_FIND_REGISTER_DECLARE (caf);
GST_TYPE_FIND_REGISTER_DECLARE (rar);
GST_TYPE_FIND_REGISTER_DECLARE (nsf);
GST_TYPE_FIND_REGISTER_DECLARE (gym);
GST_TYPE_FIND_REGISTER_DECLARE (ay);
GST_TYPE_FIND_REGISTER_DECLARE (gbs);
GST_TYPE_FIND_REGISTER_DECLARE (vgm);
GST_TYPE_FIND_REGISTER_DECLARE (sap);
GST_TYPE_FIND_REGISTER_DECLARE (ivf);
GST_TYPE_FIND_REGISTER_DECLARE (kss);
GST_TYPE_FIND_REGISTER_DECLARE (pdf);
GST_TYPE_FIND_REGISTER_DECLARE (doc);
/* Mac OS X .DS_Store files tend to be taken for video/mpeg */
GST_TYPE_FIND_REGISTER_DECLARE (ds_store);
GST_TYPE_FIND_REGISTER_DECLARE (psd);
GST_TYPE_FIND_REGISTER_DECLARE (xi);
GST_TYPE_FIND_REGISTER_DECLARE (dmp);

/*Type find declaration by functions */
GST_TYPE_FIND_REGISTER_DECLARE (musepack);
GST_TYPE_FIND_REGISTER_DECLARE (au);
GST_TYPE_FIND_REGISTER_DECLARE (mcc);
GST_TYPE_FIND_REGISTER_DECLARE (mid);
GST_TYPE_FIND_REGISTER_DECLARE (mxmf);
GST_TYPE_FIND_REGISTER_DECLARE (flx);
GST_TYPE_FIND_REGISTER_DECLARE (id3v2);
GST_TYPE_FIND_REGISTER_DECLARE (id3v1);
GST_TYPE_FIND_REGISTER_DECLARE (apetag);
GST_TYPE_FIND_REGISTER_DECLARE (tta);
GST_TYPE_FIND_REGISTER_DECLARE (mod);
GST_TYPE_FIND_REGISTER_DECLARE (mp3);
GST_TYPE_FIND_REGISTER_DECLARE (ac3);
GST_TYPE_FIND_REGISTER_DECLARE (dts);
GST_TYPE_FIND_REGISTER_DECLARE (gsm);
GST_TYPE_FIND_REGISTER_DECLARE (mpeg_sys);
GST_TYPE_FIND_REGISTER_DECLARE (mpeg_ts);
GST_TYPE_FIND_REGISTER_DECLARE (ogganx);
GST_TYPE_FIND_REGISTER_DECLARE (mpeg_video_stream);
GST_TYPE_FIND_REGISTER_DECLARE (mpeg4_video);
GST_TYPE_FIND_REGISTER_DECLARE (h263_video);
GST_TYPE_FIND_REGISTER_DECLARE (h264_video);
GST_TYPE_FIND_REGISTER_DECLARE (h265_video);
GST_TYPE_FIND_REGISTER_DECLARE (nuv);
GST_TYPE_FIND_REGISTER_DECLARE (m4a);
GST_TYPE_FIND_REGISTER_DECLARE (q3gp);
GST_TYPE_FIND_REGISTER_DECLARE (qt);
GST_TYPE_FIND_REGISTER_DECLARE (qtif);
GST_TYPE_FIND_REGISTER_DECLARE (jp2);
GST_TYPE_FIND_REGISTER_DECLARE (jpc);
GST_TYPE_FIND_REGISTER_DECLARE (mj2);
GST_TYPE_FIND_REGISTER_DECLARE (html);
GST_TYPE_FIND_REGISTER_DECLARE (swf);
GST_TYPE_FIND_REGISTER_DECLARE (xges);
GST_TYPE_FIND_REGISTER_DECLARE (xmeml);
GST_TYPE_FIND_REGISTER_DECLARE (fcpxml);
GST_TYPE_FIND_REGISTER_DECLARE (otio);
GST_TYPE_FIND_REGISTER_DECLARE (dash_mpd);
GST_TYPE_FIND_REGISTER_DECLARE (mss_manifest);
GST_TYPE_FIND_REGISTER_DECLARE (utf8);
GST_TYPE_FIND_REGISTER_DECLARE (utf16);
GST_TYPE_FIND_REGISTER_DECLARE (utf32);
GST_TYPE_FIND_REGISTER_DECLARE (uri);
GST_TYPE_FIND_REGISTER_DECLARE (itc);
GST_TYPE_FIND_REGISTER_DECLARE (hls);
GST_TYPE_FIND_REGISTER_DECLARE (sdp);
GST_TYPE_FIND_REGISTER_DECLARE (smil);
GST_TYPE_FIND_REGISTER_DECLARE (ttml_xml);
GST_TYPE_FIND_REGISTER_DECLARE (xml);
GST_TYPE_FIND_REGISTER_DECLARE (aiff);
GST_TYPE_FIND_REGISTER_DECLARE (svx);
GST_TYPE_FIND_REGISTER_DECLARE (paris);
GST_TYPE_FIND_REGISTER_DECLARE (sds);
GST_TYPE_FIND_REGISTER_DECLARE (ircam);
GST_TYPE_FIND_REGISTER_DECLARE (shn);
GST_TYPE_FIND_REGISTER_DECLARE (ape);
GST_TYPE_FIND_REGISTER_DECLARE (jpeg);
GST_TYPE_FIND_REGISTER_DECLARE (bmp);
GST_TYPE_FIND_REGISTER_DECLARE (tiff);
GST_TYPE_FIND_REGISTER_DECLARE (exr);
GST_TYPE_FIND_REGISTER_DECLARE (pnm);
GST_TYPE_FIND_REGISTER_DECLARE (matroska);
GST_TYPE_FIND_REGISTER_DECLARE (mxf);
GST_TYPE_FIND_REGISTER_DECLARE (dv);
GST_TYPE_FIND_REGISTER_DECLARE (ilbc);
GST_TYPE_FIND_REGISTER_DECLARE (sbc);
GST_TYPE_FIND_REGISTER_DECLARE (kate);
GST_TYPE_FIND_REGISTER_DECLARE (webvtt);
GST_TYPE_FIND_REGISTER_DECLARE (flac);
GST_TYPE_FIND_REGISTER_DECLARE (vorbis);
GST_TYPE_FIND_REGISTER_DECLARE (theora);
GST_TYPE_FIND_REGISTER_DECLARE (ogmvideo);
GST_TYPE_FIND_REGISTER_DECLARE (ogmaudio);
GST_TYPE_FIND_REGISTER_DECLARE (ogmtext);
GST_TYPE_FIND_REGISTER_DECLARE (speex);
GST_TYPE_FIND_REGISTER_DECLARE (celt);
GST_TYPE_FIND_REGISTER_DECLARE (oggskel);
GST_TYPE_FIND_REGISTER_DECLARE (cmml);
GST_TYPE_FIND_REGISTER_DECLARE (aac);
GST_TYPE_FIND_REGISTER_DECLARE (wavpack_wvp);
GST_TYPE_FIND_REGISTER_DECLARE (wavpack_wvc);
GST_TYPE_FIND_REGISTER_DECLARE (postscript);
GST_TYPE_FIND_REGISTER_DECLARE (svg);
GST_TYPE_FIND_REGISTER_DECLARE (tar);
GST_TYPE_FIND_REGISTER_DECLARE (ar);
GST_TYPE_FIND_REGISTER_DECLARE (msdos);
GST_TYPE_FIND_REGISTER_DECLARE (dirac);
GST_TYPE_FIND_REGISTER_DECLARE (multipart);
GST_TYPE_FIND_REGISTER_DECLARE (mmsh);
GST_TYPE_FIND_REGISTER_DECLARE (vivo);
GST_TYPE_FIND_REGISTER_DECLARE (wbmp);
GST_TYPE_FIND_REGISTER_DECLARE (y4m);
GST_TYPE_FIND_REGISTER_DECLARE (windows_icon);
#ifdef USE_GIO
GST_TYPE_FIND_REGISTER_DECLARE (xdgmime);
#endif
GST_TYPE_FIND_REGISTER_DECLARE (degas);
GST_TYPE_FIND_REGISTER_DECLARE (dvdiso);
GST_TYPE_FIND_REGISTER_DECLARE (ssa);
GST_TYPE_FIND_REGISTER_DECLARE (pva);
GST_TYPE_FIND_REGISTER_DECLARE (aa);
GST_TYPE_FIND_REGISTER_DECLARE (tap);

#endif //__GST_TYPE_FIND_FUNCTIONS_PLUGIN_H__
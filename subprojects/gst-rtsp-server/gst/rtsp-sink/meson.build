rtspsink_sources = [
  'gstrtspclientsink.c',
  'plugin.c',
]

rtspsink = library('gstrtspclientsink',
  rtspsink_sources,
  c_args : rtspserver_args + ['-DG_LOG_DOMAIN="GStreamer-rtspclientsink"'],
  include_directories : rtspserver_incs,
  dependencies : [gstrtsp_dep, gstsdp_dep, gst_rtsp_server_dep],
  install : true,
  install_dir : plugins_install_dir)
plugins += [rtspsink]

gstvideocrop = library('gstvideocrop',
  'gstvideocrop.c', 'gstaspectratiocrop.c', 'gstvideocropplugin.c',
  c_args : gst_plugins_good_args,
  include_directories : [configinc, libsinc],
  dependencies : [gst_dep, gstbase_dep, gstvideo_dep],
  install : true,
  install_dir : plugins_install_dir,
)
plugins += [gstvideocrop]

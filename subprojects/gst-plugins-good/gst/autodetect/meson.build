autodetect_sources = [
  'gstautoaudiosink.c',
  'gstautoaudiosrc.c',
  'gstautodetect.c',
  'gstautodetectplugin.c',
  'gstautodetectelement.c',
  'gstautovideosink.c',
  'gstautovideosrc.c',
]

gstautodetect = library('gstautodetect',
  autodetect_sources,
  c_args : gst_plugins_good_args,
  include_directories : [configinc],
  dependencies : [gst_dep],
  install : true,
  install_dir : plugins_install_dir,
)
plugins += [gstautodetect]

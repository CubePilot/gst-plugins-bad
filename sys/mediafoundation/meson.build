mf_sources = [
  'plugin.c',
  'gstmfutils.cpp',
  'gstmftransform.cpp',
  'gstmfvideoenc.cpp',
  'gstmfh264enc.cpp',
  'gstmfh265enc.cpp',
  'gstmfvideosrc.c',
  'gstmfsourceobject.c',
  'gstmfdevice.c',
  'gstmfaudioenc.cpp',
  'gstmfaacenc.cpp',
  'gstmfmp3enc.cpp',
]

mf_desktop_sources = [
  'gstmfsourcereader.cpp',
]

mf_app_sources = [
  'gstmfcapturewinrt.cpp',
  'mediacapturewrapper.cpp',
]

mf_header_deps = [
  'mfidl.h',
  'mfapi.h',
  'mfreadwrite.h',
  'mferror.h',
  'strmif.h',
  'mfobjects.h',
  'codecapi.h',
]

winapi_desktop = false
winapi_app = false
have_capture_engine = false
mf_lib_deps = []
mf_config = configuration_data()

mf_option = get_option('mediafoundation')
if host_system != 'windows' or mf_option.disabled()
  subdir_done()
endif

if cc.get_id() != 'msvc'
  if mf_option.enabled()
    error('mediafoundation plugin can only be built with MSVC')
  endif
  subdir_done()
endif

mf_lib = cc.find_library('mf', required : mf_option)
mfplat_lib = cc.find_library('mfplat', required : mf_option)
mfreadwrite_lib = cc.find_library('mfreadwrite', required : mf_option)
mfuuid_lib = cc.find_library('mfuuid', required : mf_option)
strmiids_lib = cc.find_library('strmiids', required : mf_option)
ole32_dep = cc.find_library('ole32', required : mf_option)
runtimeobject_lib = cc.find_library('runtimeobject', required : false)

have_mf_lib = mf_lib.found() and mfplat_lib.found() and mfreadwrite_lib.found() and mfuuid_lib.found() and strmiids_lib.found() and ole32_dep.found()
if not have_mf_lib
  if mf_option.enabled()
    error('The mediafoundation plugin was enabled explicitly, but required libraries were not found.')
  endif
  subdir_done()
endif

mf_lib_deps += [mf_lib, mfplat_lib, mfreadwrite_lib, mfuuid_lib, strmiids_lib, ole32_dep]

have_mf_header = true
foreach h: mf_header_deps
  if have_mf_header
    have_mf_header = cc.has_header(h)
  endif
endforeach

if not have_mf_header
  if mf_option.enabled()
    error('The mediafoundation plugin was enabled explicitly, but required headers were not found.')
  endif
  subdir_done()
endif

winapi_desktop = cxx.compiles('''#include <winapifamily.h>
    #include <wrl.h>
    #if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
    #error "not win32"
    #endif''',
    dependencies: mf_lib_deps,
    name: 'checking if building for Win32')

if runtimeobject_lib.found()
  winapi_app = cxx.compiles('''#include <winapifamily.h>
      #include <windows.applicationmodel.core.h>
      #include <wrl.h>
      #if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
      #error "not winrt"
      #endif''',
      dependencies: [mf_lib_deps, runtimeobject_lib],
      name: 'checking if building for WinRT')
endif

if not winapi_desktop and not winapi_app
  error('Neither Desktop partition nor App partition')
endif

winapi_app_only = winapi_app and not winapi_desktop
if winapi_app_only
  mf_sources += mf_app_sources
  mf_lib_deps += [runtimeobject_lib]
else
  mf_sources += mf_desktop_sources
  have_capture_engine = cc.has_header('mfcaptureengine.h')
  if have_capture_engine
    mf_sources += ['gstmfcaptureengine.cpp']
  endif
endif

mf_config.set10('GST_MF_HAVE_CAPTURE_ENGINE', have_capture_engine)
mf_config.set10('GST_MF_WINAPI_ONLY_APP', winapi_app_only)

configure_file(
  output: 'gstmfconfig.h',
  configuration: mf_config,
)

gstmediafoundation = library('gstmediafoundation',
  mf_sources,
  c_args : gst_plugins_bad_args + ['-DCOBJMACROS'],
  cpp_args : gst_plugins_bad_args,
  include_directories : [configinc],
  dependencies : [gstbase_dep, gstvideo_dep, gstaudio_dep, gstpbutils_dep] + mf_lib_deps,
  install : true,
  install_dir : plugins_install_dir,
)
pkgconfig.generate(gstmediafoundation, install_dir : plugins_pkgconfig_install_dir)
plugins += [gstmediafoundation]

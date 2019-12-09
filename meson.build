project('xiphos', 'c', 'cpp',
  version : '4.1.0',
  license : 'LGPLv2.1+',
  default_options : [
	  'warning_level=1',
	  'buildtype=debugoptimized',
	  ],
  meson_version : '>=0.41.0'
)

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('GETTEXT_PACKAGE', meson.project_name())
conf.set_quoted('PACKAGE', meson.project_name())
conf.set_quoted('PACKAGE_NAME', meson.project_name())

conf.set_quoted('GNOMELOCALEDIR', join_paths(get_option('prefix'), get_option('localedir')))
conf.set_quoted('LOCALEDIR', join_paths(get_option('prefix'), get_option('localedir')))
conf.set_quoted('INSTALL_PREFIX', get_option('prefix'))
conf.set_quoted('PACKAGE_TARNAME', meson.project_name())
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_APPDATA_DIR', join_paths(get_option('prefix'), get_option('datadir'), 'appdata'))
conf.set_quoted('PACKAGE_BUGREPORT', 'https://github.com/crosswire/xiphos/issues')

conf.set_quoted('PACKAGE_DATA_DIR', join_paths(get_option('prefix'), get_option('datadir'), meson.project_name()))
conf.set_quoted('PACKAGE_DOC_DIR', join_paths(get_option('prefix'), get_option('datadir'), 'doc', meson.project_name()))
conf.set_quoted('PACKAGE_HELP_DIR', join_paths(get_option('prefix'), get_option('datadir'), 'gnome', 'help', meson.project_name()))
conf.set_quoted('PACKAGE_LOCALE_DIR', join_paths(get_option('prefix'), get_option('localedir')))
conf.set_quoted('PACKAGE_MENU_DIR', join_paths(get_option('prefix'), get_option('datadir'), 'applications'))
conf.set_quoted('PACKAGE_PIXMAPS_DIR', join_paths(get_option('prefix'), get_option('datadir'), meson.project_name()))

conf.set_quoted('PACKAGE_LIBDIR', join_paths(get_option('prefix'), get_option('libdir')))
conf.set_quoted('PACKAGE_BINDIR', join_paths(get_option('prefix'), get_option('bindir')))
conf.set_quoted('PACKAGE_SYSCONFDIR', join_paths(get_option('prefix'), get_option('sysconfdir')))
conf.set_quoted('PACKAGE_LOCALSTATEDIR', join_paths(get_option('prefix'), get_option('localstatedir')))
conf.set_quoted('PACKAGE_LIBEXECDIR', join_paths(get_option('prefix'), get_option('libexecdir')))
conf.set_quoted('SHARE_DIR', join_paths(get_option('prefix'), get_option('datadir'), meson.project_name()))

conf.set10('HAVE_STRCASESTR', true)
# disable dbus here
# conf.set10('HAVE_DBUS', true)

add_global_arguments([
  '-DHAVE_CONFIG_H',
  '-I' + meson.build_root(),
], language: 'c')

add_global_arguments([
  '-DHAVE_CONFIG_H',
  '-I' + meson.build_root(),
], language: 'cpp')

if host_machine.system() == 'windows'
  rootdir_win32 = 'C:\msys'
endif

if get_option('dbus')
  dbus_glib_dep = dependency('dbus-glib-1', required: false)
endif

# Common Depends Libs
gconf_dep = dependency('gconf-2.0')
gmodule_dep = dependency('gmodule-2.0')
glib_dep = dependency('glib-2.0')
libgsf_dep = dependency('libgsf-1', version: '>= 1.14')
libxml_dep = dependency('libxml-2.0')

common_deps = [
    gconf_dep,
    gmodule_dep,
    glib_dep,
    libgsf_dep,
    libxml_dep,
]

conf.set10('WEBKIT', true)

# check gtk version
gtk_ver = get_option('gtk')
gtk3_dep = dependency('gtk+-3.0', required: false)
if gtk_ver == 'auto' and gtk3_dep.found()
  gtk_ver = '3'
else
  gtk_ver = '2'
endif

if gtk_ver == '3'
  common_deps += [ gtk3_dep ]
  conf.set10('USE_GTK_3', true)
  conf.set10('USE_GTKBUILDER', true)
else
  common_deps += [
    dependency('gtk+-2.0'),
    dependency('libglade-2.0'),
    ]
  conf.set10('GTK2', true)
endif

# check html editor lib
# gtk3: webkit2gtk => webkitgtk => gtkhtml
# gtk2:               webkitgtk => gtkhtml
editor = get_option('editor')
webkit2gtk_dep = dependency('webkit2gtk-4.0', required: false)
webkitgtk_dep = dependency('webkitgtk-3.0', required: false)
gtkhtml_dep = dependency('gtkhtml-editor-4.0', required: false)
if editor == 'auto'
  if gtk_ver == '3'
    if webkit2gtk_dep.found()
      editor = 'webkit2gtk'
    elif webkitgtk_dep.found()
      editor = 'webkitgtk'
    elif gtkhtml_dep.found()
      editor = 'gtkhtml'
    else
      editor = 'disable'
    endif
  elif gtk_ver == '2'
    if webkitgtk_dep.found()
      editor = 'webkitgtk'
    elif gtkhtml_dep.found()
      editor = 'gtkhtml'
    else
      editor = 'disable'
    endif
  endif
endif

if editor == 'webkit2gtk'
  conf.set('USE_WEBKIT2_EDITOR', 1)
  common_deps += [ webkit2gtk_dep ]
elif editor == 'webkitgtk'
  conf.set('USE_WEBKIT_EDITOR', 1)
  common_deps += [ webkitgtk_dep ]
elif editor == 'gtkhtml'
  common_deps += [ gtkhtml_dep ]
else
  message('disable editor')
endif
#####################
#      common_deps += [ webkitgtk_dep ]
#      dependency('webkitgtk-3.0')]
#    if get_option('webkit2') and webkit2gtk_dep.found()
#    if dependency('webkitgtk-3.0').found()
#      common_deps += [ dependency('webkitgtk-3.0')]
#    if dependency('gtk+-3.0').found()
#      gtk_ver = '3'
#  elif gtk_ver = '2'
#  elif dependency('gtk+-2.0', version: '>= 2.24').found()
#    gtk_ver = '2'
#  else
#    message('gtk version should greate 2.24')
#  endif
#endif
#  if get_option('webkit-editor')
#    conf.set('USE_WEBKIT_EDITOR', 1)
#    common_deps += dependency('gtk+-2.0')
#  elif dependency('gtkhtml-editor-3.14', required: false).found()
#    gtkhtml_editor_dep = dependency('gtkhtml-editor-3.14')
#    common_deps += [gtk2_dep, gtkhtml_editor_dep, dependency('libgtkhtml-3.14', version: '>= 3.23')]
#  endif

unix_print_dep = dependency('gtk+-unix-print-2.0')
sword_dep = dependency('sword')
i18n_dep = dependency('icu-i18n')
biblesync_dep = dependency('biblesync', version: '>=1.1.2')

all_deps = [common_deps,
    unix_print_dep,
    sword_dep,
    i18n_dep,
    biblesync_dep,
]

if host_machine.system() != 'windows'
uuid_dep = dependency('uuid')
all_deps += [uuid_dep]
endif

conf.set10('STDC_HEADERS', 1)
conf.set10('TIME_WITH_SYS_TIME', 1)
conf.set10('ENABLE_NLS', 1)
conf.set10('HAVE_BIND_TEXTDOMAIN_CODESET', 1)
conf.set10('HAVE_GETTEXT', 1)
conf.set10('HAVE_DCGETTEXT', 1)

if get_option('chatty')
  conf.set10('CHATTY', 1)
endif

configure_file(
  output : 'config.h',
  configuration : conf
)

root_dir = include_directories('.')
src_dir = include_directories('./src')

subdir('po')
subdir('ui')
subdir('pixmaps')
subdir('src/backend')
subdir('src/editor')
subdir('src/main')
subdir('src/xiphos_html')
subdir('src/webkit')
subdir('src/gtk')
subdir('desktop')
subdir('help')

doc_files = [
  'AUTHORS',
  'COPYING',
  'ChangeLog',
  'INSTALL.md',
  'README.md',
  'RELEASE-NOTES',
  'TODO',
  'doc/i18n reminder for developers.md',
  'doc/targets for developpers.md',
  'doc/Translating-Xiphos.md',
  'doc/Writing Xiphos Manual.md',
  'doc/Xiphos.ogg'
]

install_data(doc_files,
  install_dir : join_paths(get_option('datadir'), 'doc', meson.project_name()),
  install_mode : 'rw-r--r--',
)

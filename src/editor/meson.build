if get_option('webkit-editor')
    source = [
	'link_dialog.c',
	'editor.c',
	'webkit_editor.c',
	]
else
sources = [
    'slib-editor.c',
    'link_dialog.c',
]
endif

if host_machine.system() == 'windows'
  x11_include = '../../win32/include'
else
  x11_include = ' '
endif

# bld.new_task_gen(
#     features='cc cxx',
#     includes='. ../.. ../../src ../../src/editor %s %s' % (bld.env['GECKO_INCLUDE'], x11_include),
#     source=source,
#     target='editor',
#     #uselib='GLIB GNOMEUI SWORD GTKMOZEMBED GTKMOZEMBED_UNSTABLE GTKHTML GTKHTML_EDITOR'
#     uselib= bld.env['ALL_LIBS'],
# )

editor_lib = static_library('editor',
    sources,
    include_directories : src_dir,
    dependencies : all_deps,
)

if build_machine.system() != 'windows'
	install_data('xiphos.appdata.xml',
	install_dir : join_paths(get_option('datadir'), 'appdata'),
	install_mode : 'rw-r--r--',
)

desktop_conf = configuration_data()
desktop_conf.set('PROJECT_NAME', 'xiphos')

generated = configure_file(
	input: 'xiphos.desktop.in',
	output : 'xiphos.desktop.in',
	configuration : desktop_conf,
)

i18n.merge_file(
	input: generated,
	output : 'xiphos.desktop',
	type: 'desktop',
	po_dir: join_paths(meson.source_root(), 'po'),
	install: true,
	install_dir: join_paths(get_option('datadir'), 'applications')
)
endif

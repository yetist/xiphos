png_sources = ['about.png',
'book_closed.png',
'book_closed_rtol.png',
'book_open.png',
'book_open_rtol.png',
'chat_icon-16.png',
'dlg-un-16.png',
'dlg-un.png',
'epiphany-bookmark-page.png',
'epiphany-bookmarks.png',
'epiphany-secure.png',
'epiphany-unsecure.png',
'gnome-session.png',
'gnome-window-manager.png',
'gnome-windows.png',
'gs2-48x48-old.png',
'gs2-48x48.png',
'indexed-16.png',
'indexed.png',
'init-settings.png',
'logo.png',
'mailing_list_icon-16.png',
'mozilla-icon.png',
'new_tab_button.png',
'silk-edit-bookmarks.png',
'splash.png',
'sword-big.png',
'sword_icon-16.png',
'sword.png',
'translation_icon-16.png',
'tt.png',
'window-close.png',
'xiphos-button-125.png',
'xiphos-x-16.png',
]

ico_sources = [
    'gs2-48x48.ico',
    'gs2-48x48-old.ico',
]

xpm_sources = [
    'gs2.xpm',
    'xiphos.xpm',
]

svg_sources = ['xiphos.svg']

foreach src : [png_sources, ico_sources, xpm_sources]
	install_data(src,
	install_dir : join_paths(get_option('datadir'), meson.project_name()),
	install_mode : 'rw-r--r--',
)
endforeach

install_data(svg_sources,
install_dir : join_paths(get_option('datadir'), 'icons', 'hicolor', 'scalable', 'apps'),
install_mode : 'rw-r--r--',
)

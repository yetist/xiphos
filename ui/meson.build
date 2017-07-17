png_files = [
  'arrow_down_box.png',
  'arrow_down.png',
  'arrow_up_box.png',
  'arrow_up.png',
  'biblesync-v1-30.png',
  'biblesync-v1-50.png',
  'chat_icon-16.png',
  'dlg-un-16.png',
  'epiphany-bookmarks.png',
  'gnome-session.png',
  'gnome-sound-properties.png',
  'gnome-windows.png',
  'gs2-48x48-old.png',
  'logo.png',
  'mailing_list_icon-16.png',
  'new_tab_button.png',
  'silk-edit-bookmarks.png',
  'sword3.png',
  'sword_icon-16.png',
  'sword.png',
  'translation_icon-16.png',
  'xiphos-x-16.png',
  ]

glade_files = [
  'bookmarks.glade',
  'export-dialog.glade',
  'markverse.glade',
  'module-manager.glade',
  'navbar_versekey.glade',
  'prefs.glade',
  'search-dialog.glade',
  'selector-prefs.glade',
  'xi-export-bookmarks.glade',
  'xi-menus.glade',
  ]

gtkbuilder_files = [
  'bookmarks.gtkbuilder',
  'bookmarks_old.gtkbuilder',
  'export-dialog.gtkbuilder',
  'export-dialog_old.gtkbuilder',
  'markverse.gtkbuilder',
  'markverse_old.gtkbuilder',
  'module-manager.gtkbuilder',
  'module-manager_old.gtkbuilder',
  'navbar_versekey.gtkbuilder',
  'navbar_versekey_old.gtkbuilder',
  'prefs.gtkbuilder',
  'prefs_old.gtkbuilder',
  'search-dialog.gtkbuilder',
  'search-dialog_old.gtkbuilder',
  'selector-prefs.gtkbuilder',
  'selector-prefs_old.gtkbuilder',
  'xi-menus.gtkbuilder',
  'xi-menus_old.gtkbuilder',
  'xi-menus-popup.gtkbuilder',
  'xi-menus-popup_old.gtkbuilder',
  'xi-splash.gtkbuilder',
  'xi-splash_old.gtkbuilder',
  'editor_link_dialog.gtkbuilder',
  'editor_link_dialog_old.gtkbuilder',
]

ui_files = [
  'gtk2_webedit.ui',
  'gtk_webedit.ui',
]

other_files = [
  'languages',
  'editor_studypad.xml',
  'editor_note.xml',
]

foreach file : [png_files, glade_files, gtkbuilder_files, ui_files, other_files]
  install_data(file,
    install_dir : join_paths(get_option('datadir'), meson.project_name()),
    install_mode : 'rw-r--r--',
    )
endforeach

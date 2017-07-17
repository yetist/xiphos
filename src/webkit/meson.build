gnome = import('gnome')
wk_marshallers = gnome.genmarshal('marshal',
    sources : 'marshal.list',
    prefix : 'wk_marshal',
    )
wk_marshaller_c = wk_marshallers[0]
wk_marshaller_h = wk_marshallers[1]

sources = [
    'wk-html.c',
    'wk-html.h',
    wk_marshaller_h,
    wk_marshaller_c,
    ]
webkit_lib = static_library('webkit',
    sources,
    include_directories : src_dir,
    dependencies : all_deps,
    )

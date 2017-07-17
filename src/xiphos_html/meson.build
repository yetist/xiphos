#gnome = import('gnome')
#marshallers = gnome.genmarshal('xihtml_marshal',
#    sources : 'marshal2.list',
#    prefix : 'xihtml_marshal',
#    )
#marshaller_c = marshallers[0]
#marshaller_h = marshallers[1]

sources = [
    'xiphos_html.c',
    'xiphos_html.h',
    #marshaller_c,
    ]
xiphos_html_lib = static_library('xiphos_html',
    sources,
    include_directories : src_dir,
    dependencies : all_deps,
    )

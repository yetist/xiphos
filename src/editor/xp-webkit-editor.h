/* vi: set sw=4 ts=4 wrap ai: */
/*
 * xp-webkit-editor.h: This file is part of ____
 *
 * Copyright (C) 2018 yetist <yetist@yetibook>
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * */

#ifndef __XP_WEBKIT_EDITOR_H__ 
#define __XP_WEBKIT_EDITOR_H__  1

//#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

G_BEGIN_DECLS

#define XP_TYPE_WEBKIT_EDITOR              (xp_webkit_editor_get_type ())
#define XP_WEBKIT_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), XP_TYPE_WEBKIT_EDITOR, XpWebkitEditor))
#define XP_WEBKIT_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XP_TYPE_WEBKIT_EDITOR, XpWebkitEditorClass))
#define XP_IS_WEBKIT_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XP_TYPE_WEBKIT_EDITOR))
#define XP_IS_WEBKIT_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XP_TYPE_WEBKIT_EDITOR))
#define XP_WEBKIT_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XP_TYPE_WEBKIT_EDITOR, XpWebkitEditorClass))

typedef struct _XpWebkitEditor             XpWebkitEditor;
typedef struct _XpWebkitEditorClass        XpWebkitEditorClass;

struct _XpWebkitEditor
{
  WebKitWebView parent_instance;
  XpWebkitEditorPrivate * priv;
};

struct _XpWebkitEditorClass
{
  WebKitWebViewClass parent_class;
};

GType               xp_webkit_editor_get_type           (void) G_GNUC_CONST;
XpWebkitEditor*     xp_webkit_editor_new                (void);
//XpWebkitEditor*     xp_webkit_editor_new (EditorObj *note);
void                xp_webkit_editor_apply_format       (XpWebkitEditor *self, gint format);
gboolean            xp_webkit_editor_has_selection      (XpWebkitEditor *self);
const gchar*        xp_webkit_editor_get_selection      (XpWebkitEditor *self);
void                xp_webkit_editor_cut                (XpWebkitEditor *self);
void                xp_webkit_editor_copy               (XpWebkitEditor *self);
void                xp_webkit_editor_paste              (XpWebkitEditor *self);
void                xp_webkit_editor_undo               (XpWebkitEditor *self);
void                xp_webkit_editor_redo               (XpWebkitEditor *self);
void                xp_webkit_editor_set_font           (XpWebkitEditor *self, gchar *font);

G_END_DECLS

#endif /* __XP_WEBKIT_EDITOR_H__ */

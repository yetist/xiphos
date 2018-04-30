/* vi: set sw=4 ts=4 wrap ai: */
/*
 * xp-editor-selection.h: This file is part of ____
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

#ifndef __XP_EDITOR_SELECTION_H__ 
#define __XP_EDITOR_SELECTION_H__  1

#include <glib-object.h>
#include <webkit2/webkit2.h>

G_BEGIN_DECLS

#define XP_TYPE_EDITOR_SELECTION              (xp_editor_selection_get_type ())
#define XP_EDITOR_SELECTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), XP_TYPE_EDITOR_SELECTION, XpEditorSelection))
#define XP_EDITOR_SELECTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XP_TYPE_EDITOR_SELECTION, XpEditorSelectionClass))
#define XP_IS_EDITOR_SELECTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XP_TYPE_EDITOR_SELECTION))
#define XP_IS_EDITOR_SELECTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XP_TYPE_EDITOR_SELECTION))
#define XP_EDITOR_SELECTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XP_TYPE_EDITOR_SELECTION, XpEditorSelectionClass))

typedef struct _XpEditorSelection             XpEditorSelection;
typedef struct _XpEditorSelectionClass        XpEditorSelectionClass;

struct _XpEditorSelection
{
	GObject      object;
	XpEditorSelectionPrivate *priv;
};

struct _XpEditorSelectionClass
{
	GObjectClass     parent_class;
};

GType                  xp_editor_selection_get_type           (void) G_GNUC_CONST;
XpEditorSelection*	   xp_editor_selection_new		          (WebKitWebView *parent_view);
void			       xp_editor_selection_set_bold	          (XpEditorSelection *selection, gboolean bold);
gboolean		       xp_editor_selection_get_bold	          (XpEditorSelection *selection);

void			       xp_editor_selection_set_italic	      (XpEditorSelection *selection, gboolean italic);
gboolean		       xp_editor_selection_get_italic	      (XpEditorSelection *selection);

void			       xp_editor_selection_set_strike_through (XpEditorSelection *selection, gboolean strike_through);
gboolean		       xp_editor_selection_get_strike_through (XpEditorSelection *selection);

G_END_DECLS

#endif /* __XP_EDITOR_SELECTION_H__ */

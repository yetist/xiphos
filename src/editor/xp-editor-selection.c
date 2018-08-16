/* vi: set sw=4 ts=4 wrap ai: */
/*
 * xp-editor-selection.c: This file is part of ____
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

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "xp-editor-selection.h"

enum {
	LAST_SIGNAL
};

enum {
	PROP_0,
	PROP_WEBVIEW,
	PROP_BOLD,
	PROP_ITALIC,
	PROP_STRIKE_THROUGH,
};

//#define XP_EDITOR_SELECTION_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE((obj), XP_EDITOR_SELECTION, XpEditorSelectionPrivate))

struct _XpEditorSelectionPrivate
{
	WebKitWebView *webview;
	WebKitEditorTypingAttributes attrs;
};

static void xp_editor_selection_set_property  (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void xp_editor_selection_get_property  (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

G_DEFINE_TYPE (XpEditorSelection, xp_editor_selection, G_TYPE_OBJECT);

static void check_and_update_typing_attr (XpEditorSelection *selection, WebKitEditorTypingAttributes attrs, unsigned attr, const char *poperty_name)
{
	if (attrs & attr) {
		if (!(selection->priv->attrs & attr)) {
			selection->priv->attrs |= attr;
			g_object_notify (G_OBJECT (selection), poperty_name);
		}
	} else if (!(attrs & attr)) {
		if (selection->priv->attrs & attr) {
			selection->priv->attrs &= ~attr;
			g_object_notify (G_OBJECT (selection), poperty_name);
		}
	}
}

static void webview_typing_attributes_changed (WebKitEditorState *editor, GParamSpec *spec, XpEditorSelection *selection)
{
	WebKitEditorTypingAttributes attrs;

	attrs = webkit_editor_state_get_typing_attributes (editor);

	g_object_freeze_notify (G_OBJECT (selection));
	check_and_update_typing_attr (selection, attrs, WEBKIT_EDITOR_TYPING_ATTRIBUTE_BOLD, "bold");
	check_and_update_typing_attr (selection, attrs, WEBKIT_EDITOR_TYPING_ATTRIBUTE_ITALIC, "italic");
	check_and_update_typing_attr (selection, attrs, WEBKIT_EDITOR_TYPING_ATTRIBUTE_STRIKETHROUGH, "strike-through");
	g_object_thaw_notify (G_OBJECT (selection));
}

static void editor_selection_set_webview (XpEditorSelection *selection, WebKitWebView *webview)
{
	WebKitEditorState *editor;

	g_clear_object (&selection->priv->webview);
	selection->priv->webview = g_object_ref (webview);

	editor = webkit_web_view_get_editor_state (webview);
	selection->priv->attrs = webkit_editor_state_get_typing_attributes (editor);
	g_signal_connect (
			editor, "notify::typing-attributes",
			G_CALLBACK (webview_typing_attributes_changed), selection);
}


static void xp_editor_selection_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
	XpEditorSelection *selection = XP_EDITOR_SELECTION (object);

	switch (property_id) {
		case PROP_BOLD:
			g_value_set_boolean (value, xp_editor_selection_get_bold (selection));
			break;

		case PROP_ITALIC:
			g_value_set_boolean (value, xp_editor_selection_get_italic (selection));
			break;

		case PROP_STRIKE_THROUGH:
			g_value_set_boolean (value, xp_editor_selection_get_strike_through (selection));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
	}
}

static void xp_editor_selection_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
	XpEditorSelection *selection = XP_EDITOR_SELECTION (object);

	switch (property_id) {
		case PROP_WEBVIEW:
			editor_selection_set_webview (selection, g_value_get_object (value));
			break;

		case PROP_BOLD:
			xp_editor_selection_set_bold (selection, g_value_get_boolean (value));
			break;

		case PROP_ITALIC:
			xp_editor_selection_set_italic (selection, g_value_get_boolean (value));
			break;

		case PROP_STRIKE_THROUGH:
			xp_editor_selection_set_strike_through (selection, g_value_get_boolean (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	}
}

static void xp_editor_selection_finalize (GObject *object)
{
	XpEditorSelection *selection = XP_EDITOR_SELECTION (object);

	g_object_unref (selection->priv->webview);

	G_OBJECT_CLASS (xp_editor_selection_parent_class)->finalize (object);
}

static void xp_editor_selection_class_init (XpEditorSelectionClass *klass)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);
	object_class->get_property = xp_editor_selection_get_property;
	object_class->set_property = xp_editor_selection_set_property;
	object_class->finalize = xp_editor_selection_finalize;

	g_object_class_install_property (
			object_class,
			PROP_WEBVIEW,
			g_param_spec_object (
				"webview",
				NULL,
				NULL,
				WEBKIT_TYPE_WEB_VIEW,
				G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (
			object_class,
			PROP_BOLD,
			g_param_spec_boolean (
				"bold",
				NULL,
				NULL,
				FALSE,
				G_PARAM_READWRITE));

	g_object_class_install_property (
			object_class,
			PROP_ITALIC,
			g_param_spec_boolean (
				"italic",
				NULL,
				NULL,
				FALSE,
				G_PARAM_READWRITE));

	g_object_class_install_property (
			object_class,
			PROP_STRIKE_THROUGH,
			g_param_spec_boolean (
				"strike-through",
				NULL,
				NULL,
				FALSE,
				G_PARAM_READWRITE));
	g_type_class_add_private (klass, sizeof (XpEditorSelectionPrivate));
}


static void xp_editor_selection_init (XpEditorSelection *selection)
{
	//selection->priv = XP_EDITOR_SELECTION_GET_PRIVATE (selection);
}

XpEditorSelection * xp_editor_selection_new (WebKitWebView *parent_view)
{
	g_return_val_if_fail (WEBKIT_IS_WEB_VIEW (parent_view), NULL);

	return g_object_new (
			XP_TYPE_EDITOR_SELECTION,
			"webview", parent_view, NULL);
}

gboolean xp_editor_selection_get_bold (XpEditorSelection *selection)
{
	g_return_val_if_fail (XP_IS_EDITOR_SELECTION (selection), FALSE);

	return selection->priv->attrs & WEBKIT_EDITOR_TYPING_ATTRIBUTE_BOLD;
}

void xp_editor_selection_set_bold (XpEditorSelection *selection, gboolean bold)
{
	g_return_if_fail (XP_IS_EDITOR_SELECTION (selection));

	if ((xp_editor_selection_get_bold (selection) ? TRUE : FALSE)
			== (bold ? TRUE : FALSE)) {
		return;
	}

	webkit_web_view_execute_editing_command (selection->priv->webview, "Bold");
}

gboolean xp_editor_selection_get_italic (XpEditorSelection *selection)
{
	g_return_val_if_fail (XP_IS_EDITOR_SELECTION (selection), FALSE);

	return selection->priv->attrs & WEBKIT_EDITOR_TYPING_ATTRIBUTE_ITALIC;
}

void xp_editor_selection_set_italic (XpEditorSelection *selection, gboolean italic)
{
	g_return_if_fail (XP_IS_EDITOR_SELECTION (selection));

	if ((xp_editor_selection_get_italic (selection) ? TRUE : FALSE)
			== (italic ? TRUE : FALSE)) {
		return;
	}

	webkit_web_view_execute_editing_command (selection->priv->webview, "Italic");
}

gboolean xp_editor_selection_get_strike_through (XpEditorSelection *selection)
{
	g_return_val_if_fail (XP_IS_EDITOR_SELECTION (selection), FALSE);

	return selection->priv->attrs & WEBKIT_EDITOR_TYPING_ATTRIBUTE_STRIKETHROUGH;
}

void xp_editor_selection_set_strike_through (XpEditorSelection *selection, gboolean strike_through)
{
	g_return_if_fail (XP_IS_EDITOR_SELECTION (selection));

	if ((xp_editor_selection_get_strike_through (selection) ? TRUE : FALSE)
			== (strike_through? TRUE : FALSE)) {
		return;
	}

	webkit_web_view_execute_editing_command (selection->priv->webview, "Strikethrough");
}

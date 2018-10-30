/* vi: set sw=4 ts=4 wrap ai: */
/*
 * xp-webkit-editor.c: This file is part of ____
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

#include <JavaScriptCore/JavaScript.h>
#include <glib-object.h>
#include "xp-webkit-editor.h"
#include "xp-editor-selection.h"
#include <jsc/jsc.h>

#define DATADIR "/tmp"

/* Signals */
enum {
	EDITOR_CLOSED,
	CONTENT_CHANGED,
	LAST_SIGNAL
};

/* Prop */
enum {
	PROP_0,
	PROP_NOTE,
	NUM_PROP
};

enum {
	PROP_BOLD,
	PROP_ITALIC,
	PROP_UNDERLINE,
	PROP_STRIKE,
	XP_BULLET_LIST,
	XP_ORDER_LIST,
};

/* Block Format */
typedef enum {
	BLOCK_FORMAT_NONE,
	BLOCK_FORMAT_UNORDERED_LIST,
	BLOCK_FORMAT_ORDERED_LIST
} BlockFormat;

static guint xp_editor_signals [LAST_SIGNAL] = { 0 };

static GParamSpec *properties[NUM_PROP] = { NULL, };

typedef gboolean GetFormatFunc (XpEditorSelection*);
typedef void     SetFormatFunc (XpEditorSelection*, gboolean);

struct _XpWebkitEditor
{
	WebKitWebView parent;
	//XpNoteObj *note;
	gulong content_changed;
	gulong color_changed;
	gboolean has_text;
	gchar *selected_text;
	BlockFormat block_format;
	gboolean first_load;
	XpEditorSelection *sel;
};

G_DEFINE_TYPE (XpWebkitEditor, xp_webkit_editor, WEBKIT_TYPE_WEB_VIEW);

static void xp_webkit_editor_set_property  (GObject          *object,
		guint             prop_id,
		const GValue     *value,
		GParamSpec       *pspec);
static void xp_webkit_editor_get_property  (GObject          *object,
		guint             prop_id,
		GValue           *value,
		GParamSpec       *pspec);


gboolean xp_webkit_editor_has_selection (XpWebkitEditor *self)
{
	//XpWebkitEditorPrivate *priv = self->priv;

	return self->has_text && self->selected_text && *self->selected_text;
}

const gchar * xp_webkit_editor_get_selection (XpWebkitEditor *self)
{
	return self->selected_text;
}

static WebKitWebContext * xp_webkit_editor_get_web_context (void)
{
	static WebKitWebContext *web_context = NULL;

	if (!web_context)
	{
		web_context = webkit_web_context_get_default ();
		webkit_web_context_set_cache_model (web_context, WEBKIT_CACHE_MODEL_DOCUMENT_VIEWER);
		webkit_web_context_set_process_model (web_context, WEBKIT_PROCESS_MODEL_SHARED_SECONDARY_PROCESS);
		webkit_web_context_set_spell_checking_enabled (web_context, TRUE);
	}

	return web_context;
}

static WebKitSettings* xp_webkit_editor_get_web_settings (void)
{
	static WebKitSettings *settings = NULL;

	if (!settings)
	{
		settings = webkit_settings_new_with_settings (
				"enable-page-cache", FALSE,
				"enable-plugins", FALSE,
				"enable-tabs-to-links", FALSE,
				"allow-file-access-from-file-urls", TRUE,
				NULL);
	}

	return settings;
}

static void xp_toggle_format (XpEditorSelection *sel, GetFormatFunc get_format, SetFormatFunc set_format)
{
	set_format (sel, !get_format (sel));
}

static void xp_toggle_block_format (XpWebkitEditor *self, BlockFormat block_format)
{
	/* insert commands toggle the formatting */
	switch (block_format)
	{
		case BLOCK_FORMAT_NONE:
			break;
		case BLOCK_FORMAT_UNORDERED_LIST:
			webkit_web_view_execute_editing_command (WEBKIT_WEB_VIEW (self), "insertUnorderedList");
			break;
		case BLOCK_FORMAT_ORDERED_LIST:
			webkit_web_view_execute_editing_command (WEBKIT_WEB_VIEW (self), "insertOrderedList");
			break;
		default:
			g_assert_not_reached ();
	}
}

void xp_webkit_editor_apply_format (XpWebkitEditor *self, gint format)
{
	if (self->has_text)
	{
		switch (format)
		{
			case PROP_BOLD:
				xp_toggle_format (self->sel, xp_editor_selection_get_bold,
						xp_editor_selection_set_bold);
				break;

			case PROP_ITALIC:
				xp_toggle_format (self->sel, xp_editor_selection_get_italic,
						xp_editor_selection_set_italic);
				break;

			case PROP_UNDERLINE:
				xp_toggle_format (self->sel, xp_editor_selection_get_underline,
						xp_editor_selection_set_underline);
				break;

			case PROP_STRIKE:
				xp_toggle_format (self->sel, xp_editor_selection_get_strike_through,
						xp_editor_selection_set_strike_through);
				break;

			case XP_BULLET_LIST:
				xp_toggle_block_format (self, BLOCK_FORMAT_UNORDERED_LIST);
				break;

			case XP_ORDER_LIST:
				xp_toggle_block_format (self, BLOCK_FORMAT_ORDERED_LIST);
				break;

			default:
				g_warning ("xp_webkit_editor_apply_format : Invalid format");
		}
	}
}

void xp_webkit_editor_cut (XpWebkitEditor *self)
{
	webkit_web_view_execute_editing_command (WEBKIT_WEB_VIEW (self), WEBKIT_EDITING_COMMAND_CUT);
}

void xp_webkit_editor_copy (XpWebkitEditor *self)
{
	webkit_web_view_execute_editing_command (WEBKIT_WEB_VIEW (self), WEBKIT_EDITING_COMMAND_COPY);
}

void xp_webkit_editor_paste (XpWebkitEditor *self)
{
	webkit_web_view_execute_editing_command (WEBKIT_WEB_VIEW (self), WEBKIT_EDITING_COMMAND_PASTE);
}

void xp_webkit_editor_undo (XpWebkitEditor *self)
{
	webkit_web_view_execute_editing_command (WEBKIT_WEB_VIEW (self), WEBKIT_EDITING_COMMAND_UNDO);
}

void xp_webkit_editor_redo (XpWebkitEditor *self)
{
	webkit_web_view_execute_editing_command (WEBKIT_WEB_VIEW (self), WEBKIT_EDITING_COMMAND_REDO);
}

void xp_webkit_editor_exec_command (XpWebkitEditor *self, const gchar* command, const gchar* argument)
{
	if (argument == NULL) {
		webkit_web_view_execute_editing_command (self, command);
	} else {
		webkit_web_view_execute_editing_command_with_argument(self, command, argument);
	}
}

static void set_editor_color (WebKitWebView *w, GdkRGBA *col)
{
	g_autofree gchar *script = NULL;

	webkit_web_view_set_background_color (w, col);
	script = g_strdup_printf ("document.getElementById('editable').style.color = '%s';",
			col->red < 0.5 ? "white" : "black");
	webkit_web_view_run_javascript (w, script, NULL, NULL, NULL);
}

void xp_webkit_editor_set_font (XpWebkitEditor *self, gchar *font)
{
	PangoFontDescription *font_desc;
	const gchar *family;
	gint size;
	GdkScreen *screen;
	double dpi;
	guint font_size;

	/* parse : but we only parse font properties we'll be able
	 * to transfer to webkit editor
	 * Maybe is there a better way than webkitSettings,
	 * eg applying format to the whole body */
	font_desc = pango_font_description_from_string (font);
	family = pango_font_description_get_family (font_desc);
	size = pango_font_description_get_size (font_desc);

	if (!pango_font_description_get_size_is_absolute (font_desc))
		size /= PANGO_SCALE;

	screen = gtk_widget_get_screen (GTK_WIDGET (self));
	dpi = screen ? gdk_screen_get_resolution (screen) : 96.0;
	font_size = size / 72. * dpi;

	/* Set */
	g_object_set (xp_webkit_editor_get_web_settings (),
			"default-font-family", family,
			"default-font-size", font_size,
			NULL);

	pango_font_description_free (font_desc);
}

static void xp_webkit_editor_init (XpWebkitEditor *self)
{
	//XpWebkitEditorPrivate *priv;

	//self->priv = XP_WEBKIT_EDITOR_GET_PRIVATE (self);

}

static void xp_webkit_editor_finalize (GObject *object)
{
	XpWebkitEditor *self = XP_WEBKIT_EDITOR (object);

	g_free (self->selected_text);

//	if (priv->note != NULL) {
//		g_object_remove_weak_pointer (G_OBJECT (priv->note), (gpointer*) &priv->note);
//		g_signal_handler_disconnect (priv->note, priv->color_changed);
//	}

	G_OBJECT_CLASS (xp_webkit_editor_parent_class)->finalize (object);
}

static void xp_webkit_editor_content_changed (XpWebkitEditor *self, const char *html, const char *text)
{
	gchar **rows;

	//xp_note_obj_set_html (note, (char *)html);
	//xp_note_obj_set_raw_text (note, (char *)text);
	g_print("html:\n%s\n", html);
	g_print("text:\n%s\n", html);

	g_signal_emit (self, xp_editor_signals[CONTENT_CHANGED], 0, NULL);

	/* Now tries to update title */

	rows = g_strsplit (text, "\n", 2);

	if (rows && rows[0])
	{
		gchar *title;
		g_autofree gchar *unique_title = NULL;

		title = rows[0];

		g_print("title=%s\n", title);
		//if (g_strcmp0 (title, xp_item_get_title (XP_ITEM (note))) != 0)
		//{
		//	unique_title = xp_manager_get_unique_title (xp_item_get_manager (XP_ITEM (note)),
		//			title);

		//	xp_note_obj_set_title (note, unique_title);
		//}
	}

	g_strfreev (rows);

	//xp_note_obj_set_mtime (note, g_get_real_time () / G_USEC_PER_SEC);
	//xp_note_obj_save_note (note);
}


#if 0
static void on_note_color_changed (XpNoteObj *note, XpWebkitEditor *self)
{
	GdkRGBA color;

	if (xp_note_obj_get_rgba(note,&color))
		set_editor_color (WEBKIT_WEB_VIEW (self), &color);
}
#endif

static gboolean on_navigation_request (WebKitWebView *web_view, WebKitPolicyDecision *decision, WebKitPolicyDecisionType decision_type, gpointer user_data)
{
	WebKitNavigationPolicyDecision *navigation_decision;
	WebKitNavigationAction *action;
	const char *requested_uri;
	GtkWidget *toplevel;
	GError *error = NULL;

	if (decision_type != WEBKIT_POLICY_DECISION_TYPE_NAVIGATION_ACTION)
		return FALSE;

	navigation_decision = WEBKIT_NAVIGATION_POLICY_DECISION (decision);
	action = webkit_navigation_policy_decision_get_navigation_action (navigation_decision);
	requested_uri = webkit_uri_request_get_uri (webkit_navigation_action_get_request (action));
	if (g_strcmp0 (webkit_web_view_get_uri (web_view), requested_uri) == 0)
		return FALSE;

	toplevel = gtk_widget_get_toplevel (GTK_WIDGET (web_view));
	g_return_val_if_fail (gtk_widget_is_toplevel (toplevel), FALSE);

	gtk_show_uri_on_window (GTK_WINDOW (toplevel),
			requested_uri,
			GDK_CURRENT_TIME,
			&error);

	if (error)
	{
		g_warning ("%s", error->message);
		g_error_free (error);
	}

	webkit_policy_decision_ignore (decision);
	return TRUE;
}

static void on_load_change (WebKitWebView  *web_view, WebKitLoadEvent event)
{
	GdkRGBA color;

	if (event != WEBKIT_LOAD_FINISHED)
		return;

	///* Apply color */
	//if (xp_note_obj_get_rgba (priv->note, &color))
	//	set_editor_color (web_view, &color);

	//if (!priv->color_changed)
	//{
	//	priv->color_changed = g_signal_connect (priv->note,
	//			"color-changed",
	//			G_CALLBACK (on_note_color_changed),
	//			web_view);
	//}
}

static char * get_js_property_string (JSGlobalContextRef js_context, JSObjectRef js_object, const char *property_name)
{
	JSStringRef js_property_name;
	JSValueRef js_property_value;
	JSStringRef js_string_value;
	size_t max_size;
	char *property_value = NULL;

	js_property_name = JSStringCreateWithUTF8CString (property_name);
	js_property_value = JSObjectGetProperty (js_context, js_object, js_property_name, NULL);
	JSStringRelease (js_property_name);

	if (!js_property_value || !JSValueIsString (js_context, js_property_value))
		return NULL;

	js_string_value = JSValueToStringCopy (js_context, js_property_value, NULL);
	if (!js_string_value)
		return NULL;

	max_size = JSStringGetMaximumUTF8CStringSize (js_string_value);
	if (max_size)
	{
		property_value = g_malloc (max_size);
		JSStringGetUTF8CString (js_string_value, property_value, max_size);
	}
	JSStringRelease (js_string_value);

	return property_value;
}

static gboolean get_js_property_boolean (JSGlobalContextRef js_context, JSObjectRef js_object, const char *property_name)
{
	JSStringRef js_property_name;
	JSValueRef js_property_value;

	js_property_name = JSStringCreateWithUTF8CString (property_name);
	js_property_value = JSObjectGetProperty (js_context, js_object, js_property_name, NULL);
	JSStringRelease (js_property_name);

	if (!js_property_value || !JSValueIsBoolean (js_context, js_property_value))
		return FALSE;

	return JSValueToBoolean (js_context, js_property_value);
}

static void xp_webkit_editor_handle_contents_update (XpWebkitEditor *self, JSCValue *js_value)

{
	g_autoptr (JSCValue) js_outer_html = NULL;
	g_autoptr (JSCValue) js_inner_text = NULL;
	g_autofree gchar *html = NULL;
	g_autofree gchar *text = NULL;

	js_outer_html = jsc_value_object_get_property (js_value, "outerHTML");
	html = jsc_value_to_string (js_outer_html);
	if (!html)
		return;

	js_inner_text = jsc_value_object_get_property (js_value, "innerText");
	text = jsc_value_to_string (js_inner_text);
	if (!text)
		return;

	xp_webkit_editor_content_changed (self, html, text);
}

static void xp_webkit_editor_handle_selection_change (XpWebkitEditor *self, JSCValue *js_value)

{
	printf("selected changed\n");
	g_autoptr (JSCValue) js_has_text     = NULL;
	g_autoptr (JSCValue) js_text         = NULL;
	g_autoptr (JSCValue) js_block_format = NULL;
	g_autofree char *block_format_str = NULL;

	js_has_text = jsc_value_object_get_property (js_value, "hasText");
	self->has_text = jsc_value_to_boolean (js_has_text);

	js_text = jsc_value_object_get_property (js_value, "text");
	g_free (self->selected_text);
	self->selected_text = jsc_value_to_string (js_text);

	printf("selected text:%s\n", self->selected_text);
	js_block_format = jsc_value_object_get_property (js_value, "blockFormat");
	block_format_str = jsc_value_to_string (js_block_format);
	if (g_strcmp0 (block_format_str, "UL") == 0)
		self->block_format = BLOCK_FORMAT_UNORDERED_LIST;
	else if (g_strcmp0 (block_format_str, "OL") == 0)
		self->block_format = BLOCK_FORMAT_ORDERED_LIST;
	else
		self->block_format = BLOCK_FORMAT_NONE;
}

static void on_script_message (WebKitUserContentManager *user_content, WebKitJavascriptResult *message, XpWebkitEditor *self)
{
  JSCValue             *js_value        = NULL;
  g_autoptr (JSCValue)  js_message_name = NULL;
  g_autofree char *message_name = NULL;

  js_value = webkit_javascript_result_get_js_value (message);
  g_assert (jsc_value_is_object (js_value));

  js_message_name = jsc_value_object_get_property (js_value, "messageName");
  message_name = jsc_value_to_string (js_message_name);
  if (g_strcmp0 (message_name, "ContentsUpdate") == 0)
    {
      if (self->first_load)
        self->first_load = FALSE;
      else
	    xp_webkit_editor_handle_contents_update (self, js_value);
    }
  else if (g_strcmp0 (message_name, "SelectionChange") == 0)
	  xp_webkit_editor_handle_selection_change (self, js_value);
}


static void xp_webkit_editor_constructed (GObject *obj)
{
	XpWebkitEditor *self;
	WebKitWebView *view;
	WebKitUserContentManager *user_content;
	GBytes *html_data = NULL;
	gchar *body;

	self = XP_WEBKIT_EDITOR (obj);
	view = WEBKIT_WEB_VIEW (self);
	self->first_load = TRUE;

	G_OBJECT_CLASS (xp_webkit_editor_parent_class)->constructed (obj);

	user_content = webkit_web_view_get_user_content_manager (view);
	webkit_user_content_manager_register_script_message_handler (user_content, "xiphos");
	g_signal_connect (user_content, "script-message-received::xiphos", G_CALLBACK (on_script_message), self);

	self->sel = xp_editor_selection_new (view);

	webkit_web_view_set_editable (view, TRUE);

	/* Do not segfault at finalize
	 * if the note died */
	//g_object_add_weak_pointer (G_OBJECT (priv->note), (gpointer*) &priv->note);

	//body = xp_note_obj_get_html (priv->note);

	//if (!body)
	//	body = html_from_plain_text ("");
	//body = "<h1>hi</h1>";
	body = g_strconcat ("<html xmlns=\"http://www.w3.org/1999/xhtml\">",
			"<head>",
			//"<link rel='stylesheet' href='Default.css' type='text/css'/>",
			"<script language='javascript' src='xiphos.js'></script>"
			"</head>",
			"<body contenteditable='true' id='editable'>",
			"</body></html>", NULL);


	html_data = g_bytes_new_take (body, strlen (body));
	webkit_web_view_load_bytes (view, html_data, "application/xhtml+xml", NULL,
			"file://" DATADIR G_DIR_SEPARATOR_S "xiphos" G_DIR_SEPARATOR_S);

	/* Do not be a browser */
	g_signal_connect (view, "decide-policy", G_CALLBACK (on_navigation_request), NULL);
	g_signal_connect (view, "load-changed", G_CALLBACK (on_load_change), NULL);
}

static void xp_webkit_editor_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	XpWebkitEditor *webkit_editor;

	webkit_editor = XP_WEBKIT_EDITOR (object);

	switch (prop_id)
	{
//		case PROP_NOTE:
//			webkit_editor->priv->note = g_value_get_object (value);
//			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void xp_webkit_editor_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	XpWebkitEditor *webkit_editor;

	webkit_editor = XP_WEBKIT_EDITOR (object);

	switch (prop_id)
	{
//		case PROP_NOTE:
//			g_value_set_object (value, webkit_editor->priv->note);
//			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void xp_webkit_editor_class_init (XpWebkitEditorClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	object_class->constructed = xp_webkit_editor_constructed;
	object_class->finalize = xp_webkit_editor_finalize;
	object_class->get_property = xp_webkit_editor_get_property;
	object_class->set_property = xp_webkit_editor_set_property;

//	properties[PROP_NOTE] = g_param_spec_object ("note",
//			"Note",
//			"Xp Note Obj",
//			XP_TYPE_NOTE_OBJ,
//			G_PARAM_READWRITE  |
//			G_PARAM_CONSTRUCT |
//			G_PARAM_STATIC_STRINGS);

	g_object_class_install_property (object_class, PROP_NOTE, properties[PROP_NOTE]);

	xp_editor_signals[EDITOR_CLOSED] = g_signal_new ("closed",
			G_OBJECT_CLASS_TYPE (klass),
			G_SIGNAL_RUN_FIRST,
			0,
			NULL,
			NULL,
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE,
			0);
	xp_editor_signals[CONTENT_CHANGED] = g_signal_new ("content-changed",
			G_OBJECT_CLASS_TYPE (klass),
			G_SIGNAL_RUN_LAST,
			0,
			NULL,
			NULL,
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE,
			0);

	//g_type_class_add_private (klass, sizeof (XpWebkitEditorPrivate));
}

//XpWebkitEditor * xp_webkit_editor_new (XpNoteObj *note)
XpWebkitEditor * xp_webkit_editor_new (void)
{
	//return g_object_new (XP_TYPE_WEBKIT_EDITOR, NULL);
	WebKitUserContentManager *manager = webkit_user_content_manager_new ();

	return g_object_new (XP_TYPE_WEBKIT_EDITOR,
			"web-context", xp_webkit_editor_get_web_context (),
			"settings", xp_webkit_editor_get_web_settings (),
			"user-content-manager", manager,
			//"note", note,
			NULL);

	//g_object_unref (manager);
}

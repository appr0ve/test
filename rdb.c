/* SPDX-License: Apache-2.0
 * SPDX-FileCopyrightText: 2024 appr0ve
 */

/* Internalization */
#include <glib/gi18n.h>
#include <locale.h>

/* JSON parse */
#include <json-glib/json-glib.h>

/* Not found solution for GLib
 * Source code contain somethink like stdin
 * but compiled test "not working" like getchar.
 * src: glib/gio/tests/basic-application.c +182
 * (from glib2-2.82.2-alt1-81940df)
 * */
#include <stdio.h>

/* RDB API shared library */
#include <rdb/rdb.h>

/* Options related stuff */
/* Stub for help option
 * Only demonstrative purpose.
 * GNU standards say following:
 *   All programs should support two standard options:
 *   --version and --help
 * See more in `print_version`.
 * Notice: GOptionContext generate help option itself.
 */
gpointer print_version ();
gpointer print_branches (RdbApi*);
gchar * get_property (RdbApi *api, gchar *property);

static gboolean print = FALSE;
static gboolean list_arches, list_branches = FALSE;
static gboolean compare_packages, compare_versions = FALSE;
static gchar * control = NULL;
static gchar * target = NULL;

static const GOptionEntry entries[] =
{
  { "version", 'v', 0,
    G_OPTION_ARG_NONE, &print,
    N_("Print version"), NULL },
  { "list-branches", 'b', 0,
    G_OPTION_ARG_NONE, &list_branches,
    N_("List availaible branches"), NULL },
  { "list-arches", 'a', 0,
    G_OPTION_ARG_NONE, &list_arches,
    N_("List CPU arch-s available"), NULL },
  { "compare-packages", 'p', 0,
    G_OPTION_ARG_NONE, &compare_packages,
    N_("Compare packages beetwen two branches"), NULL },
  { "compare-versions", 'u', 0,
    G_OPTION_ARG_NONE, &compare_versions,
    N_("Compare version beetwen two branches"), NULL },
  { "control", '1', 0, 
    G_OPTION_ARG_STRING, &control,
    N_("Control branch"), N_("BRANCH") },
  { "target", '2', 0,
    G_OPTION_ARG_STRING, &target,
    N_("Target branch"), N_("BRANCH") },
  { NULL }
};

int main (int argc, gchar *argv[]) {
  GError *error = NULL;
  /* We are using that for parsing cli options */
  GOptionContext *context;

  RdbApi *api = g_object_new (RDB_TYPE_API, NULL);

  const gchar * RDB_API_URL = g_getenv("RDB_API_URL");
  if(RDB_API_URL)
    api->url = (gchar*) RDB_API_URL;

  /* Initialize gettext routines */
  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  /* Register new context */
  context = g_option_context_new N_("- Show how to use rdb client");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);
  g_option_context_set_translation_domain (context, GETTEXT_PACKAGE);

  /* Validate recieved options */
  if (!g_option_context_parse (context, &argc, &argv, &error))
  {
    g_print ("%s: %s\n", _("Option parsing failed"), error->message);
    exit (1);
  }

  if (argc < 2) {
    g_print ("%s\n", g_option_context_get_help (context, FALSE, NULL));
    return EXIT_FAILURE;
  }

  if (print)
  {
    print_version();
  }
  if
  (control && target) {
    if (g_strcmp0(control, target)) {
      g_print("%s: %s\n", _("Using following url"), get_property(api, "url"));
      rdb_api_cache_check (api, &error, control, target);
      if (api->control_status) {
	g_print ("%s %s %s",
	       _("Cache for branch"),
	         control,
	       _("already exist, redownload?\n"));
	char decision;
        g_print ("[y/n] ");
	while (!(decision == 'y' ||
		 decision == 'n' ||
		 decision == 'Y' ||
		 decision == 'N' )) {
	  scanf ("%c", &decision);
          if (decision == 'y' || decision == 'Y') {
            api->control_overwrite = TRUE;
          }
	}
      }
      if (api->target_status) {
        g_print ("%s %s %s",
	       _("Cache for branch"),
	         target,
	       _("already exist, redownload?\n"));
        g_print ("[y/n] ");
	char decision = '0';
	while (!(decision == 'y' ||
		 decision == 'n' ||
		 decision == 'Y' ||
		 decision == 'N' )) {
	  scanf ("%c", &decision);
          if (decision == 'y' || decision == 'Y') {
            api->target_overwrite = TRUE;
          }
	}
      }

      rdb_api_get_binaries (api, &error, control, target);
      rdb_api_compare_binary(api, &error, control, target);
    } else {
      g_print (_("Branches must be differ!\n"));
      return EXIT_FAILURE;
    }
  }
  if (list_branches) {
    rdb_api_get_branches(api, &error);
  }
  if (list_arches) {
    rdb_api_get_arches(api, &error);
  }

  g_option_context_free (context);

  return EXIT_SUCCESS;
}

/* Print version number
 * options: -v --version
 */
gpointer print_version () {
  g_print ("%s %s\n", GETTEXT_PACKAGE, VERSION);
  return NULL;
}

gchar * get_property (RdbApi *api, gchar *property) {
  gchar *value;
  GValue val = G_VALUE_INIT;
  g_value_init (&val, G_TYPE_STRING);
  g_object_get_property(G_OBJECT (api), property, &val);
  g_assert (G_VALUE_HOLDS_STRING (&val));
  value = g_strdup (g_value_get_string(&val));
  g_value_unset (&val);
  return value;
}

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
#include <rdb.h>

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
gpointer print_branches (RdbApi *);
gchar *get_property (RdbApi * api, gchar * property);

static gboolean print = FALSE;
static gboolean list_arches, list_branches = FALSE;
static gboolean compare_packages,
  compare_packages_reverse, compare_versions = FALSE;
static gchar *control = NULL;
static gchar *target = NULL;
static gchar *arch = NULL;

static const GOptionEntry entries[] = {
  {"version", 'v', 0,
   G_OPTION_ARG_NONE, &print,
   N_("Print version"), NULL},
  {"list-branches", 'b', 0,
   G_OPTION_ARG_NONE, &list_branches,
   N_("List availaible branches"), NULL},
  {"list-arches", 'a', 0,
   G_OPTION_ARG_NONE, &list_arches,
   N_("List CPU arch-s available"), NULL},
  {"compare-packages", 'p', 0,
   G_OPTION_ARG_NONE, &compare_packages,
   N_("Show difference existence between packages within two branches"),
   NULL},
  {"compare-packages-reverse", 'r', 0,
   G_OPTION_ARG_NONE, &compare_packages_reverse,
   N_("Reverse-order mode"), NULL},
  {"compare-versions", 'u', 0,
   G_OPTION_ARG_NONE, &compare_versions,
   N_("Show difference version between packages within two branches"),
   NULL},
  {"control", '1', 0,
   G_OPTION_ARG_STRING, &control,
   N_("Control branch"), N_("BRANCH")},
  {"target", '2', 0,
   G_OPTION_ARG_STRING, &target,
   N_("Target branch"), N_("BRANCH")},
  {"target", '3', 0,
   G_OPTION_ARG_STRING, &arch,
   N_("Architecture"), N_("ARCH")},
  {NULL}
};

int
main (int argc, gchar * argv[])
{
  GError *error = NULL;
  /* We are using that for parsing cli options */
  GOptionContext *context;

  RdbApi *api = g_object_new (RDB_TYPE_API, NULL);

  const gchar *RDB_API_URL = g_getenv ("RDB_API_URL");
  if (RDB_API_URL)
    api->url = (gchar *) RDB_API_URL;

  /* Initialize gettext routines */
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (PACKAGE, "UTF-8");
  textdomain (PACKAGE);

  /* Register new context */
  context = g_option_context_new N_("- Show how to use rdb client");
  g_option_context_add_main_entries (context, entries, PACKAGE);
  g_option_context_set_translation_domain (context, PACKAGE);

  /* Validate recieved options */
  if (!g_option_context_parse (context, &argc, &argv, &error))
    {
      g_print ("%s: %s\n", _("Option parsing failed"), error->message);
      exit (1);
    }

  /* Handle options */
  if (print)
    {
      print_version ();
    }
  if (control && target && arch)
    {
      if (g_strcmp0 (control, target))
	{
	  if (compare_packages ||
	      compare_packages_reverse || compare_versions)
	    {
	      api->compare_packages = compare_packages;
              api->compare_packages_reverse = compare_packages_reverse;
              api->compare_versions = compare_versions;
	      g_print ("%s: %s\n", _("Using following url"),
		       api->url);
	      rdb_api_compare_binary (api, &error, control, target, arch);
	    }
	  else
	    g_print (_("Select what to do with branches!\n"));
	}
      else
	{
	  g_print (_("Branches must be differ!\n"));
	  return EXIT_FAILURE;
	}
    }
  if (list_branches)
    {
      rdb_api_get_branches (api, &error);
    }
  if (list_arches)
    {
      rdb_api_get_arches (api, &error);
    }

  g_option_context_free (context);

  return EXIT_SUCCESS;
}

/* Print version number
 * options: -v --version
 */
gpointer
print_version ()
{
  g_print ("%s %s\n", PACKAGE, VERSION);
  return NULL;
}

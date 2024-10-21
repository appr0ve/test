/* SPDX-License: Apache-2.0
 * SPDX-FileCopyrightText: 2024 appr0ve
 */
#define GETTEXT_PACKAGE "rdb"

/* Internalization */
#include <glib/gi18n.h>
#include <locale.h>

/* JSON parse */
#include <json-glib/json-glib.h>

/* Options related stuff */
/* Stub for help option
 * Only demonstrative purpose.
 * GNU standards say following:
 *   All programs should support two standard options:
 *   --version and --help
 * See more in `print_version`.
 * Notice: GOptionContext generate help option itself.
 */
#define VERSION 1
gpointer print_version ();

static gboolean print = FALSE;

static GOptionEntry entries[] =
{
  { "version", 'v', 0, G_OPTION_ARG_NONE, &print, "Print version", NULL },
  { NULL }
};

gchar* assign_var(const gchar *, const gchar *);

int main (int argc, gchar *argv[]) {
  GError *error = NULL;
  /* We are using that for parsing cli options */
  GOptionContext *context;

  /* Assign LOCALEDIR for gettext
   * (testing-purpose)
   */
  const gchar * LOCALEDIR = assign_var ("LOCALEDIR", "/usr/share/locale");
  const gchar * RDB_API_URL = assign_var ("RDB_API_URL", "https://rdb.altlinux.org/api/");
  //TODO

  /* Initialize gettext routines */
  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  /* Register new context */
  context = g_option_context_new ("Interact with RDB API");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);
  g_option_context_set_translation_domain (context, GETTEXT_PACKAGE);

  /* Validate recieved options */
  if (!g_option_context_parse (context, &argc, &argv, &error))
  {
    g_print ("%s: %s\n", _("Option parsing failed"), error->message);
    exit (1);
  }
  /* TODO */
  if (print) {
    print_version();
  }

  return 0;
}

/* Print version number
 * options: -v --version
 */
gpointer print_version () {
  g_print ("%s %d\n", GETTEXT_PACKAGE, VERSION);
  return NULL;
}

gchar * assign_var (const gchar * var, const gchar * def) {
  if(!g_getenv(var))
    return (gchar *) def;
  else
    return (gchar *) g_getenv(var);
}

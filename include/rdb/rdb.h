/* SPDX-License: Apache-2.0
 * SPDX-FileCopyrightText: 2024 appr0ve
 */
#ifndef __RDB_API_H_
#define __RDB_API_H_
#include <glib-object.h>

G_BEGIN_DECLS
#define RDB_TYPE_API rdb_api_get_type ()
G_DECLARE_FINAL_TYPE (RdbApi, rdb_api, RDB, API, GObject)
struct _RdbApi
{
  GObject parent_instance;

  /* url - URI path for RDB API instance */
  gchar *url;
  gchar *method_binary;
  gchar *method_branch;
  gchar *method_arch;
  goffset download_counter;
  gboolean compare_packages;
  gboolean compare_packages_reverse;
  gboolean compare_versions;
};

void rdb_api_get_branches (RdbApi * self, GError ** error);
void rdb_api_get_arches (RdbApi * self, GError ** error);
void rdb_api_compare_binary (
  RdbApi * self,
  GError ** error,
  gchar * control,
  gchar * target,
  gchar * arch
);

G_END_DECLS
#endif

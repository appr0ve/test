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

  void (*setup_url) (RdbApi  *self,
                GError **error);
  /* url - URI path for RDB API instance */
  gchar *url;
  gchar *method_binary;
  gchar *method_branch;
  void (*print_url)(RdbApi*);
};

void rdb_api_get_branches(RdbApi* self,
                          GError **error);
void rdb_api_get_arches  (RdbApi* self,
                          GError **error);
void rdb_api_get_binary  (RdbApi* self,
                          GError **error);
void rdb_api_compare_binary  (RdbApi* self,
                          GError **error);

G_END_DECLS

#endif

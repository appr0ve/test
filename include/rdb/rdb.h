/* SPDX-License: Apache-2.0
 * SPDX-FileCopyrightText: 2024 appr0ve
 */
#include <glib-object.h>

#define RDB_TYPE_API rdb_api_get_type ()
G_DECLARE_FINAL_TYPE (RdbApi, rdb_api, RDB, API, GObject)

struct _RdbApi
{
  GObject parent_instance;

  /* url - URI path for RDB API instance */
  gchar *url;
};

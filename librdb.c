/* SPDX-License: Apache-2.0
 * SPDX-FileCopyrightText: 2024 appr0ve
 */
#include <rdb/rdb.h>

G_DEFINE_TYPE (RdbApi, rdb_api, G_TYPE_OBJECT)

static void
rdb_api_constructed (GObject *obj)
{
  G_OBJECT_CLASS (rdb_api_parent_class)->constructed (obj);
}

static void
rdb_api_finalize (GObject *obj)
{
  RdbApi *self = RDB_API (obj);

  g_free (self->url);

  G_OBJECT_CLASS (rdb_api_parent_class)->finalize (obj);
}

static void
rdb_api_class_init (RdbApiClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->constructed = rdb_api_constructed;
  object_class->finalize = rdb_api_finalize;
}

static void
rdb_api_init (RdbApi *self)
{
  /* export/branch_binary_packages
   * errata/errata_branches
   */
}

static void rdb_api_setup_url()
{
}

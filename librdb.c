/* SPDX-License: Apache-2.0
 * SPDX-FileCopyrightText: 2024 appr0ve
 */
#include <rdb/rdb.h>
#include <gio/gio.h>
#include <json-glib/json-glib.h>

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

typedef enum
{
  RDB_URL = 1,
  METHOD_BINARY,
  METHOD_BRANCH,
  N_PROPERTIES
} RdbApiProperty;

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void
rdb_api_set_property (GObject      *object,
                      guint         property_id,
                      const GValue *value,
                      GParamSpec   *pspec)
{
  RdbApi *self = RDB_API (object);

  switch ((RdbApiProperty) property_id)
  {
    case RDB_URL:
      g_free (self->url);
      self->url = g_value_dup_string (value);
      break;
     case METHOD_BINARY:
      g_free (self->method_binary);
      self->method_binary = g_value_dup_string (value);
      break;
    case METHOD_BRANCH:
      g_free (self->method_branch);
      self->method_branch = g_value_dup_string (value);
      break;
  default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
rdb_api_get_property (GObject      *object,
                      guint         property_id,
                      GValue *value,
                      GParamSpec   *pspec)
{
  RdbApi *self = RDB_API (object);

  switch ((RdbApiProperty) property_id)
  {
    case RDB_URL:
      g_value_set_string ((GValue*)value, self->url);
      break;
    case METHOD_BINARY:
      g_value_set_string ((GValue*)value, self->method_binary);
      break;
    case METHOD_BRANCH:
      g_value_set_string ((GValue*)value, self->method_branch);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}


static void
rdb_api_class_init (RdbApiClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = rdb_api_set_property;
  object_class->get_property = rdb_api_get_property;
  obj_properties[RDB_URL] =
    g_param_spec_string ("url",
                         "URL",
                         "RDB URL used for get requests",
                         "https://rdb.altlinux.org/api/",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_properties[METHOD_BINARY] =
    g_param_spec_string ("method-binary",
                         "BINARY",
                         "Method name for get binary",
                         "export/branch_binary_packages",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_properties[METHOD_BRANCH] =
    g_param_spec_string ("method-branch",
                         "BRANCH",
                         "Method name for get list of available branches",
                         "errata/errata_branches",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties (object_class,
                                     N_PROPERTIES,
                                     obj_properties);

  object_class->constructed = rdb_api_constructed;
  object_class->finalize = rdb_api_finalize;
}

static void
rdb_api_init (RdbApi *self)
{
}

void
rdb_api_get_branches(RdbApi *self, GError **error)
{
  g_return_if_fail (RDB_IS_API (self));
  g_return_if_fail (error == NULL || *error == NULL);

  GFile * branches;
  g_assert (self->url != NULL);
  const gchar * errata = g_strconcat(self->url, "errata/errata_branches");
  g_assert (errata != NULL);
  branches = g_file_new_for_uri (errata);
  GError * err = NULL;
  GFileInputStream *stream = g_file_read (branches, NULL, &err);
  g_assert_no_error (err);

  JsonParser * parser;
  JsonNode   * node;

  parser = json_parser_new ();
  json_parser_load_from_stream (parser, G_INPUT_STREAM (stream), NULL, &err);
  g_assert_no_error (err);
  g_assert (stream != NULL);

  node = json_parser_get_root (parser);
  g_assert (node != NULL);
  g_assert (JSON_NODE_HOLDS_OBJECT(node));

  JsonObject * obj = json_node_get_object (node);

  JsonArray * array = json_object_get_array_member (obj, "branches");

  gint count = json_array_get_length(array);
  gint i;
  for (i = 0; i < count; i++) {
    g_print ("%s\n", json_array_get_string_element(array, i));
  }
}

void
rdb_api_get_arches(RdbApi *self, GError **error)
{
  g_return_if_fail (RDB_IS_API (self));
  g_return_if_fail (error == NULL || *error == NULL);

  GFile * arches;
  const gchar * errata = g_strconcat(self->url, "site/all_pkgset_archs?branch=p10");
  arches = g_file_new_for_uri (errata);
  GError * err = NULL;
  GFileInputStream *stream = g_file_read (arches, NULL, &err);
  g_assert_no_error (err);

  JsonParser * parser;
  JsonNode   * node;

  parser = json_parser_new ();
  json_parser_load_from_stream (parser, G_INPUT_STREAM (stream), NULL, &err);
  g_assert_no_error (err);
  g_assert (stream != NULL);

  node = json_parser_get_root (parser);
  g_assert (node != NULL);
  g_assert (JSON_NODE_HOLDS_OBJECT(node));

  JsonObject * obj = json_node_get_object (node);

  JsonArray * array = json_object_get_array_member (obj, "archs");
  g_assert (array != NULL);

  gint count = json_array_get_length(array);
  gint i;
  JsonNode * nod[count];
  JsonObject * objj[count];
  JsonArray * arr[count];
  for (i = 0; i < count; i++) {
    nod[i] = json_array_get_element (array, i);
    g_assert (JSON_NODE_HOLDS_OBJECT (nod[i]));
    objj[i] = json_node_get_object (nod[i]);
    g_print ("%s\n", json_object_get_string_member (objj[i], "arch"));
  }
}

/* SPDX-License: Apache-2.0
 * SPDX-FileCopyrightText: 2024 appr0ve
 */
/* Initiate structure for RdbApi object */
#include <rdb/rdb.h>
/* GFile routins */
#include <gio/gio.h>
/* Library for manipulating JSON data */
#include <json-glib/json-glib.h>

/* Define own object inherated GObject from GLib
 * In that case this not neccesery, but using
 * for demonstrative purpose.
 */
G_DEFINE_TYPE (RdbApi, rdb_api, G_TYPE_OBJECT)
     static void rdb_api_constructed (GObject * obj)
{
  G_OBJECT_CLASS (rdb_api_parent_class)->constructed (obj);
}

static void
rdb_api_finalize (GObject * obj)
{
  RdbApi *self = RDB_API (obj);

  g_free (self->url);

  G_OBJECT_CLASS (rdb_api_parent_class)->finalize (obj);
}

/* Define properties inside RdbApi object.
 * Zero member reserved by GLib.
 */
typedef enum
{
  RDB_URL = 1,
  METHOD_BINARY,
  METHOD_BRANCH,
  METHOD_ARCH,
  N_PROPERTIES
} RdbApiProperty;

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void
rdb_api_set_property (GObject * object,
		      guint property_id,
		      const GValue * value, GParamSpec * pspec)
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
    case METHOD_ARCH:
      g_free (self->method_arch);
      self->method_arch = g_value_dup_string (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
rdb_api_get_property (GObject * object,
		      guint property_id, GValue * value, GParamSpec * pspec)
{
  RdbApi *self = RDB_API (object);

  switch ((RdbApiProperty) property_id)
    {
    case RDB_URL:
      g_value_set_string ((GValue *) value, self->url);
      break;
    case METHOD_BINARY:
      g_value_set_string ((GValue *) value, self->method_binary);
      break;
    case METHOD_BRANCH:
      g_value_set_string ((GValue *) value, self->method_branch);
      break;
    case METHOD_ARCH:
      g_value_set_string ((GValue *) value, self->method_arch);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

/* Initiate default properties */
static void
rdb_api_class_init (RdbApiClass * klass)
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
  obj_properties[METHOD_ARCH] =
    g_param_spec_string ("method-arch",
			 "ARCH",
			 "Method name for get list of available branches",
			 "site/all_pkgset_archs?branch=p11",
			 G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties (object_class,
				     N_PROPERTIES, obj_properties);

  object_class->constructed = rdb_api_constructed;
  object_class->finalize = rdb_api_finalize;
}

/* Initiate states used by download logic */
static void
rdb_api_init (RdbApi * self)
{
  self->compare_packages = FALSE;
  self->compare_packages_reverse = FALSE;
  self->compare_versions = FALSE;
}

goffset progress_counter;

GFileProgressCallback
progress_callback (goffset current_num_bytes,
		   goffset total_num_bytes, gpointer user_data)
{
  progress_counter = current_num_bytes / 1024;
  total_num_bytes /= 1024;
  g_print ("%s: %ld / %ld\r", (gchar *) user_data, progress_counter,
	   total_num_bytes);
  return 0;
}

GInputStream *
rdb_api_get_binary (RdbApi * self, gchar * method, gchar * data)
{
  GFile *binary_data;
  GFile *output_data;
  gchar *path, *tmp_path, *value;
  /* value */
  GValue val = G_VALUE_INIT;
  g_value_init (&val, G_TYPE_STRING);
  g_object_get_property (G_OBJECT (self), method, &val);
  g_assert (G_VALUE_HOLDS_STRING (&val));
  value = g_strdup (g_value_get_string (&val));
  g_value_unset (&val);
  /* end value */
  path =
    g_build_path ("/", self->url, value, data,
		  NULL);
  g_message (path);
  binary_data = g_file_new_for_uri (path);
  GError *err = NULL;
  tmp_path =
    g_build_path ("/", "/tmp", "test", NULL);
  g_assert (tmp_path != NULL);

  output_data = g_file_new_for_path (tmp_path);

  if (data) {
    progress_counter = (goffset) & self->download_counter;
    g_file_copy (binary_data, output_data, G_FILE_COPY_OVERWRITE,
	       NULL, (GFileProgressCallback) progress_callback, data, &err);
  } else {
    g_file_copy (binary_data, output_data, G_FILE_COPY_OVERWRITE,
	       NULL, NULL, data, &err);
  }
  g_assert_no_error (err);

  GFileInputStream *stream = g_file_read (output_data, NULL, &err);
  g_assert_no_error (err);

  g_object_unref (binary_data);
  return (GInputStream*) stream;
}

JsonArray *
rdb_api_get_array_packages
(JsonParser *parser, GInputStream * stream, GError  *** error)
{
  JsonNode *node;
  JsonObject *obj;
  JsonArray *array;
  json_parser_load_from_stream (
    parser, G_INPUT_STREAM (stream), NULL, (GError **) error);
  node = json_parser_get_root (parser);
  g_assert (node != NULL);
  g_assert (JSON_NODE_HOLDS_OBJECT (node));
  obj = json_node_get_object (node);
  array = json_object_get_array_member (obj, "packages");

  return array;
}

void
  rdb_api_compare_binary
  (RdbApi * self, GError ** error, gchar * control, gchar * target, gchar * arch)
{
  g_return_if_fail (RDB_IS_API (self));
  g_return_if_fail (error == NULL || *error == NULL);

  GInputStream * control_binary =
    rdb_api_get_binary (self, "method-binary",
      g_strconcat(control, "?arch=", arch, NULL));
  GInputStream * target_binary =
    rdb_api_get_binary (self, "method-binary",
      g_strconcat(target, "?arch=", arch, NULL));
  g_assert (control_binary != NULL);
  g_assert (target_binary != NULL);

  JsonArray *control_array, *target_array;

  JsonParser *parser, *parser2;
  parser = json_parser_new ();
  parser2 = json_parser_new ();

  control_array =
    rdb_api_get_array_packages (parser, control_binary, &error);
  target_array =
    rdb_api_get_array_packages (parser2, target_binary, &error);

  JsonBuilder *builder = json_builder_new ();
  json_builder_begin_object (builder);
  json_builder_set_member_name (builder, "");
  json_builder_begin_array (builder);

  if (self->compare_versions) {
  gint count = json_array_get_length (control_array);
  gint i;
  JsonNode *nod[count];
  JsonObject *objj[count];
  for (i = 0; i < count; i++)
    {
      nod[i] = json_array_get_element (control_array, i);
      g_assert (JSON_NODE_HOLDS_OBJECT (nod[i]));
      objj[i] = json_node_get_object (nod[i]);
      g_print ("%s\n", json_object_get_string_member (objj[i], "name"));
    }
  }
}

/* Method used for showing available branches */
void
rdb_api_get_branches (RdbApi * self, GError ** error)
{
  g_return_if_fail (RDB_IS_API (self));
  g_return_if_fail (error == NULL || *error == NULL);
  GInputStream *stream;
  stream = rdb_api_get_binary (self, "method-branch", NULL);

  JsonParser *parser;
  JsonNode *node;

  parser = json_parser_new ();
  json_parser_load_from_stream (parser, G_INPUT_STREAM (stream), NULL, error);
  g_assert (stream != NULL);

  node = json_parser_get_root (parser);
  g_assert (node != NULL);
  g_assert (JSON_NODE_HOLDS_OBJECT (node));

  JsonObject *obj = json_node_get_object (node);

  JsonArray *array = json_object_get_array_member (obj, "branches");

  gint count = json_array_get_length (array);
  gint i;
  for (i = 0; i < count; i++)
    {
      g_print ("%s\n", json_array_get_string_element (array, i));
    }
}

/* Method used for showing available arches */
void
rdb_api_get_arches (RdbApi * self, GError ** error)
{
  g_return_if_fail (RDB_IS_API (self));
  g_return_if_fail (error == NULL || *error == NULL);
  GInputStream *stream;
  stream = rdb_api_get_binary (self, "method-arch", NULL);

  JsonParser *parser;
  JsonNode *node;

  parser = json_parser_new ();
  json_parser_load_from_stream (parser, G_INPUT_STREAM (stream), NULL, error);
  g_assert (stream != NULL);

  node = json_parser_get_root (parser);
  g_assert (node != NULL);
  g_assert (JSON_NODE_HOLDS_OBJECT (node));

  JsonObject *obj = json_node_get_object (node);

  JsonArray *array = json_object_get_array_member (obj, "archs");
  g_assert (array != NULL);

  gint count = json_array_get_length (array);
  gint i;
  JsonNode *nod[count];
  JsonObject *objj[count];
  for (i = 0; i < count; i++)
    {
      nod[i] = json_array_get_element (array, i);
      g_assert (JSON_NODE_HOLDS_OBJECT (nod[i]));
      objj[i] = json_node_get_object (nod[i]);
      g_print ("%s\n", json_object_get_string_member (objj[i], "arch"));
    }
}

/*  caki-node.cc - caki NodeJs bindings.
    Copyright (C) 2018 Renato Alencar <renatoalencar.73@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <node_api.h>
#include <assert.h>

#ifdef __MACH__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

extern "C"
{
  #include "caki.h"
}


namespace caki
{
  napi_value caki_node_to_object(napi_env env, CakiNode *node);

  napi_value caki_node_to_value(napi_env env, CakiNode *node)
  {
    napi_value value;
    napi_status status;

    switch (node->type) {
      case CAKI_TYPE_INT:
        status = napi_create_int32(env, *((int32_t *) node->v_ptr), &value);
        assert(status == napi_ok);
        break;
      case CAKI_TYPE_STR:
        status = napi_create_string_utf8(env, (char *) node->v_ptr, NAPI_AUTO_LENGTH, &value);
        assert(status == napi_ok);
        break;
      case CAKI_TYPE_NODE:
        value = caki_node_to_object(env, (CakiNode *) node->v_ptr);
        break;
      case CAKI_TYPE_FLOAT:
        status = napi_create_double(env, *((float *) node->v_ptr), &value);
        assert(status == napi_ok);
        break;
      default:
        return nullptr;
        break;
    }

    return value;
  }

  napi_value caki_node_to_object(napi_env env, CakiNode *node)
  {
    napi_value object;
    napi_value value;
    napi_value key;
    napi_status status;
    CakiNode *i;

    status = napi_create_object(env, &object);
    assert(status == napi_ok);

    for (i = node; i != NULL; i = i->next) {
      value = caki_node_to_value(env, i);

      status = napi_create_string_utf8(env, i->name, NAPI_AUTO_LENGTH, &key);
      assert(status == napi_ok);
      status = napi_set_property(env, object, key, value);
      assert(status == napi_ok);
    }
    
    return object;
  }

  napi_value parse(napi_env env, napi_callback_info info)
  {
    napi_status status;
    napi_value filename_value;
    napi_value root_object;
    CakiNode *root;
    char *filename;
    size_t filename_length;
    size_t argc = 1;

    status = napi_get_cb_info(env, info, &argc, &filename_value, nullptr, nullptr);
    assert(status == napi_ok);

    status = napi_get_value_string_utf8(env, filename_value, nullptr, 0, &filename_length);
    assert(status == napi_ok);

    filename = (char *) malloc(filename_length + 1);

    status = napi_get_value_string_utf8(env, filename_value, filename, filename_length + 1, nullptr);
    assert(status == napi_ok);

    root = caki_parse(filename);
    root_object = caki_node_to_object(env, root);
    caki_free(root);

    return root_object;
  }

  napi_value init(napi_env env, napi_value exports)
  {
    napi_status status;
    napi_value fn;

    status = napi_create_function(env, "parse", 0, parse, nullptr, &fn);
    if (status != napi_ok) return nullptr;

    status = napi_set_named_property(env, exports, "parse", fn);
    if (status != napi_ok) return nullptr;

    return exports;
  }

  NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
}
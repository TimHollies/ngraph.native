#include <node_api.h>
#include <assert.h>
#include <stdio.h>
napi_value Add(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 3;
  napi_value args[3];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);

  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "Wrong number of arguments");
    return nullptr;
  }

  napi_valuetype valuetype0;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);

  napi_valuetype valuetype1;
  status = napi_typeof(env, args[1], &valuetype1);
  assert(status == napi_ok);

  napi_valuetype valuetype2;
  status = napi_typeof(env, args[2], &valuetype2);
  assert(status == napi_ok);

  if (valuetype0 != napi_number || valuetype1 != napi_number || valuetype2 != napi_object) {
    napi_throw_type_error(env, nullptr, "Wrong arguments");
    return nullptr;
  }

  bool istypedarray;
  status = napi_is_typedarray(env, args[2], &istypedarray);
  assert(status == napi_ok);

  if(!istypedarray) {
    napi_throw_type_error(env, nullptr, "Arg 3 must be a typed array");
    return nullptr;
  }

  napi_typedarray_type value2type;
  size_t value2length;
  void* value2data;
  size_t offset;
  napi_value arraybuffer;
  status = napi_get_typedarray_info(env, args[2], &value2type, &value2length, &value2data, &arraybuffer, &offset);
  assert(status == napi_ok);

  if(value2type != napi_int32_array) {
    napi_throw_type_error(env, nullptr, "Arg 3 must be an int32array");
    return nullptr;
  }

  int32_t* values = static_cast<int32_t*>(value2data);

//napi_int32_array  

  double value0;
  status = napi_get_value_double(env, args[0], &value0);
  assert(status == napi_ok);

  double value1;
  status = napi_get_value_double(env, args[1], &value1);
  assert(status == napi_ok);

  napi_value sum;
  status = napi_create_double(env, value0 + values[0], &sum);
  assert(status == napi_ok);

  napi_value result_array_buffer;
  status = napi_create_arraybuffer(env, value2length * 4, &values, &result_array_buffer);
  assert(status == napi_ok);

  void* result_data;
  size_t result_size;
  status = napi_get_arraybuffer_info(env, result_array_buffer, &result_data, &result_size);
  assert(status == napi_ok);

  int32_t* resultValues = static_cast<int32_t*>(result_data);
  resultValues[0] = 100;

  napi_value result;
  status = napi_create_typedarray(env, napi_int32_array, value2length, result_array_buffer, offset, &result);
  assert(status == napi_ok);

  return result;
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("add", Add);
  status = napi_define_properties(env, exports, 1, &addDescriptor);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(addon, Init)

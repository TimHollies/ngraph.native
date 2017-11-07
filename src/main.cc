#include <node_api.h>
#include <assert.h>
#include <vector>
#include <stdio.h>
#include <iostream>

#include "layout.h"

napi_value RunLayout(napi_env env, napi_callback_info info) {
  napi_status status;

  #pragma region validateInput

  size_t argc = 2;
  napi_value args[2];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "Wrong number of arguments");
    return nullptr;
  }

  napi_valuetype valuetype0;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);

  napi_valuetype valuetype1;
  status = napi_typeof(env, args[1], &valuetype1);
  assert(status == napi_ok);

  if (valuetype0 != napi_object || valuetype1 != napi_object) {
    napi_throw_type_error(env, nullptr, "Wrong arguments");
    return nullptr;
  }

  bool istypedarray0;
  status = napi_is_typedarray(env, args[0], &istypedarray0);
  assert(status == napi_ok);

  bool istypedarray1;
  status = napi_is_typedarray(env, args[1], &istypedarray1);
  assert(status == napi_ok);

  if(!istypedarray0 || !istypedarray1) {
    napi_throw_type_error(env, nullptr, "Both arguments must be int32array");
    return nullptr;
  }

  napi_typedarray_type nodesTaType;
  size_t nodesLength;
  void* nodesDataRaw;
  size_t nodesOffset;
  napi_value nodesArraybuffer;
  status = napi_get_typedarray_info(env, args[0], &nodesTaType, &nodesLength, &nodesDataRaw, &nodesArraybuffer, &nodesOffset);
  assert(status == napi_ok);

  napi_typedarray_type edgesTaType;
  size_t edgesLength;
  void* edgesDataRaw;
  size_t edgesOffset;
  napi_value edgesArraybuffer;
  status = napi_get_typedarray_info(env, args[1], &edgesTaType, &edgesLength, &edgesDataRaw, &edgesArraybuffer, &edgesOffset);
  assert(status == napi_ok);

  if(nodesTaType != napi_int32_array || edgesTaType != napi_int32_array) {
    napi_throw_type_error(env, nullptr, "Both arguments must be int32array");
    return nullptr;
  }

  #pragma endregion validate input

  int32_t* nodesData = static_cast<int32_t*>(nodesDataRaw);
  int32_t* edgesData = static_cast<int32_t*>(edgesDataRaw);

  Layout graphLayout;
  graphLayout.init(nodesData, nodesLength, edgesData, edgesLength);

  for (int i = 0; i < 10000; ++i) {
    bool done = graphLayout.step();
    if (done) {
        break;
    }
  }

  napi_value result_array_buffer;
  void* result_data;
  status = napi_create_arraybuffer(env, nodesLength * sizeof(int32_t) * 3, &result_data, &result_array_buffer);
  assert(status == napi_ok);
  
  int32_t* resultValues = static_cast<int32_t*>(result_data);

  std::vector<Body>* bodies = graphLayout.getBodies();

  for(int i=0; i<nodesLength; i+=1) {
    resultValues[i*3] = nodesData[i];
    resultValues[(i*3) + 1] = (int)std::round((*bodies)[i].pos.x);
    resultValues[(i*3) + 2] = (int)std::round((*bodies)[i].pos.y);
  }

  #pragma region createOutput

  
  napi_value result;
  status = napi_create_typedarray(env, napi_int32_array, nodesLength * 3, result_array_buffer, nodesOffset, &result);
  assert(status == napi_ok);

  #pragma endregion createOutput

  return result;
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("runLayout", RunLayout);
  status = napi_define_properties(env, exports, 1, &addDescriptor);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(addon, Init)

#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <tensorflow/lite/c/c_api.h>
#include <edgetpu_c.h>

#if 1
#define ARENA_SAVE \
  int ai = mrb_gc_arena_save(mrb);
#define ARENA_RESTORE \
  mrb_gc_arena_restore(mrb, ai);
#else
#define ARENA_SAVE
#define ARENA_RESTORE
#endif

static void
mrb_tflite_edgetpu_delegate_free(mrb_state *mrb, void *p) {
  edgetpu_free_delegate((TfLiteDelegate*)p);
}

static const struct mrb_data_type mrb_tflite_edgetpu_delegate_type = {
  "mrb_tflite_edgetpu_delegate", mrb_tflite_edgetpu_delegate_free,
};

static mrb_value
mrb_tflite_edgetpu_devices(mrb_state *mrb, mrb_value self) {
  mrb_value arr;
  int n;
  size_t numDevices = 0;
  struct edgetpu_device *devices;
  devices = edgetpu_list_devices(&numDevices);
  arr = mrb_ary_new(mrb);
  for (n = 0; n < numDevices; n++) {
    mrb_value item = mrb_ary_new(mrb);
    mrb_ary_push(mrb, item, mrb_fixnum_value(devices[n].type));
    mrb_ary_push(mrb, item, mrb_str_new_cstr(mrb, devices[n].path));
    mrb_ary_push(mrb, arr, item);
  }
  edgetpu_free_devices(devices);
  return arr;
}

static mrb_value
mrb_tflite_edgetpu_init(mrb_state *mrb, mrb_value self) {
  TfLiteDelegate* delegate;
  mrb_value item;

  mrb_get_args(mrb, "o", &item);
  if (!mrb_array_p(item) || RARRAY_LEN(item) != 2) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "invalid argument");
  }
  mrb_int type = mrb_fixnum(mrb_ary_ref(mrb, item, 0));
  const char* path = mrb_string_cstr(mrb, mrb_ary_ref(mrb, item, 1));
  delegate = edgetpu_create_delegate(type, path, NULL, 0);
  if (delegate == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot create delegate");
  }
  DATA_TYPE(self) = &mrb_tflite_edgetpu_delegate_type;
  DATA_PTR(self) = delegate;
  return self;
}
void
mrb_mruby_tflite_edgetpu_gem_init(mrb_state* mrb) {
  struct RClass *_class_tflite, *_class_tflite_edgetpu;
  ARENA_SAVE;

  _class_tflite = mrb_define_module(mrb, "TfLite");
  _class_tflite_edgetpu = mrb_define_class_under(mrb, _class_tflite, "EdgeTPU", mrb->object_class);

  mrb_define_method(mrb, _class_tflite_edgetpu, "initialize", mrb_tflite_edgetpu_init, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, _class_tflite_edgetpu, "devices", mrb_tflite_edgetpu_devices, MRB_ARGS_NONE());
  ARENA_RESTORE;
}

void
mrb_mruby_tflite_edgetpu_gem_final(mrb_state* mrb) {
}

/* vim:set et ts=2 sts=2 sw=2 tw=0: */

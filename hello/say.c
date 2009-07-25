#include "ruby.h"

void hello();

VALUE wrap_hello(self)
     VALUE self;
{
  hello();
  return Qnil;
}

void Init_say()
{
  VALUE module;

  module = rb_define_module("Say");
  rb_define_module_function(module, "hello", wrap_hello, 0);
}


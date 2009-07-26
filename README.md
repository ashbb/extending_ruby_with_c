Study Note: Extending Ruby with C on Windows
============================================

This is my study note. (July 26th, 2009)


Environments
------------
- Windows XP SP3
- Ruby 1.8.6
- Microsoft Visual C++ 2008 Express Edition with SP1


Install C compiler
------------------
Installed [Microsoft Visual C++ 2008 Express Edition with SP1](http://www.microsoft.com/japan/msdn/vstudio/express/) (Japan site).

**Note**: At first I attempted using [Borland C++ Compiler version 5.5](http://edn.embarcadero.com/jp/article/20633). BCC worked well to compile my tiny C code (hellomain.c), but Ruby `mkmf` couldn't create `Makefile` for BCC.


Hello World in C
----------------
Write the C code: **hellomain.c**

	#include <stdio.h>
	
	int main(){
	    printf("hello, world!\n");
	    return 0;
	}

Open Visual Studio 2008 command prompt console window:

- start --> all programs --> Microsoft Visual C++ 2008 Express Edition --> Visula Studio Tools --> Visula Studio 2008 command prompt

<pre>
Setting environment for using Microsoft Visual Studio 2008 x86 tools.

C:\Program Files\Microsoft Visual Studio 9.0\VC>
</pre>

Do the following on the console:

C:\rubyprograms\c\hellomain>**dir**

<pre>
2009/07/25  13:57    <DIR>          .
2009/07/25  13:57    <DIR>          ..
2009/07/25  09:46                85 hellomain.c
</pre>

C:\rubyprograms\c\hellomain>**cl hellomain.c**

<pre>
Microsoft(R) 32-bit C/C++ Optimizing Compiler Version 15.00.30729.01 for 80x86
Copyright (C) Microsoft Corporation.  All rights reserved.

hellomain.c
Microsoft (R) Incremental Linker Version 9.00.30729.01
Copyright (C) Microsoft Corporation.  All rights reserved.

/out:hellomain.exe
hellomain.obj
</pre>

C:\rubyprograms\c\hellomain>**dir**

<pre>
2009/07/25  13:59    <DIR>          .
2009/07/25  13:59    <DIR>          ..
2009/07/25  09:46                85 hellomain.c
2009/07/25  13:59            48,128 hellomain.exe
2009/07/25  13:59               644 hellomain.obj
</pre>

C:\rubyprograms\c\hellomain>**hellomain.exe**

<pre>
hello, world!
</pre>


Hello World in Ruby with C extention
------------------------------------
Write the following two C codes.

**hello.c** :

	#include <stdio.h>
	void hello()
	{
	  printf("Hello World!\n");
	}

**say.c** : 

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

To do `make`, we need to create `Makefile`. We can use Ruby `mkmf` library like this.

**extconf.rb** :

	require "mkmf"
	create_makefile("say")

Open Visual Studio 2008 command prompt console window and do the following:

**BUT, BEFORE THAT**, you had to edit the following two files:

**C:\ruby\lib\ruby\1.8\i386-mswin32\config.h**, top line:

	#if _MSC_VER != 1200   ====>   #if _MSC_VER < 1200

**C:\ruby\lib\ruby\1.8\i386-mswin32\rbconfig.rb**, line 25:

	CONFIG["CFLAGS"] = "-MD -Zi -O2b2xg- -G6"   ====>   CONFIG["CFLAGS"] = "-MT -Zi "


C:\rubyprograms\c\extending_ruby_with_c\hello>**dir**

<pre>
2009/07/26  10:43    <DIR>          .
2009/07/26  10:43    <DIR>          ..
2009/07/25  12:34                42 extconf.rb
2009/07/25  12:35                71 hello.c
2009/07/25  12:34               259 say.c
</pre>

C:\rubyprograms\c\extending_ruby_with_c\hello>**ruby extconf.rb**

<pre>
creating Makefile
</pre>

C:\rubyprograms\c\extending_ruby_with_c\hello>**dir**

<pre>
2009/07/26  10:46    <DIR>          .
2009/07/26  10:46    <DIR>          ..
2009/07/25  12:34                42 extconf.rb
2009/07/25  12:35                71 hello.c
2009/07/26  10:46             4,664 Makefile
2009/07/25  12:34               259 say.c
</pre>

C:\rubyprograms\c\extending_ruby_with_c\hello>**nmake**

<pre>
Microsoft(R) Program Maintenance Utility Version 9.00.30729.01
Copyright (C) Microsoft Corporation.  All rights reserved.

        c:\ruby\bin\ruby -e "puts 'EXPORTS', 'Init_say'"  > say-i386-mswin32.def

        cl -nologo -I. -Ic:/ruby/lib/ruby/1.8/i386-mswin32 -Ic:/ruby/lib/ruby/1.
8/i386-mswin32 -I. -MT -Zi  -c -Tchello.c
hello.c
        cl -nologo -I. -Ic:/ruby/lib/ruby/1.8/i386-mswin32 -Ic:/ruby/lib/ruby/1.
8/i386-mswin32 -I. -MT -Zi  -c -Tcsay.c
say.c
        cl -nologo -LD -Fesay.so hello.obj say.obj msvcrt-ruby18.lib  oldnames.l
ib user32.lib advapi32.lib ws2_32.lib  -link -incremental:no -debug -opt:ref -op
t:icf -dll -libpath:"." -libpath:"c:/ruby/lib" -def:say-i386-mswin32.def -implib
:say-i386-mswin32.lib -pdb:say-i386-mswin32.pdb
   library say-i386-mswin32.lib and object say-i386-mswin32.exp making...
</pre>

C:\rubyprograms\c\extending_ruby_with_c\hello>**dir**

<pre>
2009/07/26  10:47    <DIR>          .
2009/07/26  10:47    <DIR>          ..
2009/07/25  12:34                42 extconf.rb
2009/07/25  12:35                71 hello.c
2009/07/26  10:47             2,907 hello.obj
2009/07/26  10:46             4,664 Makefile
2009/07/26  10:47                19 say-i386-mswin32.def
2009/07/26  10:47               720 say-i386-mswin32.exp
2009/07/26  10:47             1,648 say-i386-mswin32.lib
2009/07/26  10:47           871,424 say-i386-mswin32.pdb
2009/07/25  12:34               259 say.c
2009/07/26  10:47            18,732 say.obj
2009/07/26  10:47            53,248 say.so
2009/07/26  10:47            77,824 vc90.pdb
</pre>

Done! We could get `say.so` C extention library. Now, we can use that in my tiny Ruby app.

**hello_world.rb** :

	require 'say'
	Say.hello

C:\rubyprograms\c\extending_ruby_with_c\hello_world>**dir**

<pre>
2009/07/26  11:05    <DIR>          .
2009/07/26  11:05    <DIR>          ..
2009/07/25  12:34                24 hello_world.rb
2009/07/26  10:47            53,248 say.so
</pre>

C:\rubyprograms\c\extending_ruby_with_c\hello_world>**ruby hello_world.rb**

<pre>
Hello World!
</pre>

Cool! :-D


References
----------
- [Tutorial of Extending Ruby](http://ruby.gfd-dennou.org/tutorial/ruby-ext/) (in Japanese)
- [Install Ruby-pg on Windows](http://ronspace.cocolog-nifty.com/blog/2009/02/ruby-pg-windows.html) (in Japanese)


@c
@c  COPYRIGHT (c) 1988-1998.
@c  On-Line Applications Research Corporation (OAR).
@c  All rights reserved.
@c
@c  $Id$
@c

@chapter Makefiles

This chapter discusses the Makefiles associated with a BSP.  It does not 
describe the process of configuring, building, and installing RTEMS.  
This chapter will not provide detailed information about this process.
Nonetheless, it is important to remember that the general process consists
of three parts:

@itemize @bullet
@item configure
@item build
@item install
@end itemize

During the configure phase, a number of files are generated.  These 
generated files are tailored for the specific host/target combination
by the configure script.  This set of files includes the Makefiles used
to actually compile and install RTEMS.

During the build phase, the source files are compiled into object files
and libraries are built.

During the install phase, the libraries, header files, and other support
files are copied to the BSP specific installation point.  After installation
is successfully completed, the files generated by the configure and build
phases may be removed.

@section Makefiles Used During The BSP Building Process

There is a file named @code{Makefile.in} in each directory of a BSP.
RTEMS uses the @b{GNU autoconf} automatic configuration package. 
This tool specializes the @code{Makefile.in} files at the time that RTEMS
is configured for a specific development host and target.  Makefiles
are automatically generated from the @code{Makefile.in} files.  It is
necessary for the BSP developer to provide these files.  Most of the
time, it is possible to copy the @code{Makefile.in} from another
similar directory and edit it.

The @code{Makefile} files generated are processed when building
RTEMS for a given BSP.

The BSP developer is responsible for generating @code{Makefile.in} 
files which properly build all the files associated with their BSP.
There are generally three types of Makefiles in a BSP source tree:


@itemize @bullet
@item Directory Makefiles
@item Source Directory Makefiles
@item Wrapup Makefile
@end itemize

@subsection Directory Makefiles

The Directory class of Makefiles directs the build process through
a set of subdirectories in a particular order.  This order is usually
chosen to insure that build dependencies are properly processed. 
Most BSPs only have one Directory class Makefile.  The @code{Makefile.in}
in the BSP root directory (@code{c/src/lib/libbsp/CPU/BSP}) specifies
which directories are to be built for this BSP. For example, the
following Makefile fragment shows how a BSP would only build the
networking device driver if HAS_NETWORKING was defined:

@example
NETWORKING_DRIVER_yes_V = network
NETWORKING_DRIVER = $(NETWORKING_DRIVER_$(HAS_NETWORKING)_V) 

[...]

SUB_DIRS=include start340 startup clock console timer \
    $(NETWORKING_DRIVER) wrapup
@end example

This fragment states that all the directories have to be processed,
except for the @code{network} directory which is included only if the
user configured networking.

@subsection Source Directory Makefiles

There is a @code{Makefile.in} in most of the directories in a BSP. This
class of Makefile lists the files to be built as part of the driver.
When adding new files to an existing directory, Do not forget to add
the new files to the list of files to be built in the @code{Makefile.in}.

@b{NOTE:} The @code{Makefile.in} files are ONLY processed during the configure
process of a RTEMS build. Therefore, when developing 
a BSP and adding a new file to a @code{Makefile.in}, the 
already generated @code{Makefile} will not include the new references.
This results in the new file not being be taken into account! 
The @code{configure} script must be run again or the @code{Makefile}
(the result of the configure process) modified by hand.  This file will
be in a directory such as the following:

@example
MY_BUILD_DIR/c/src/lib/libbsp/CPU/BSP/DRIVER
@end example

@subsection Wrapup Makefile

This class of Makefiles produces a library.  The BSP wrapup Makefile
is responsible for producing the library @code{libbsp.a} which is later
merged into the @code{librtemsall.a} library.  This Makefile specifies
which BSP components are to be placed into the library as well as which
components from the CPU dependent support code library.  For example,
this component may choose to use a default exception handler from the
CPU dependent support code or provide its own.

This Makefile makes use of the @code{foreach} construct in @b{GNU make}
to pick up the required components:

@example
BSP_PIECES=startup clock console timer
CPU_PIECES=
GENERIC_PIECES=

# bummer; have to use $foreach since % pattern subst rules only replace 1x
OBJS=$(foreach piece, $(BSP_PIECES), ../$(piece)/$(ARCH)/$(piece).rel) \
   $(foreach piece, $(CPU_PIECES), \
       ../../../../libcpu/$(RTEMS_CPU)/$(piece)/$(ARCH)/$(piece).rel) \
   $(wildcard \
  ../../../../libcpu/$(RTEMS_CPU)/$(RTEMS_CPU_MODEL)/fpsp/$(ARCH)/fpsp.rel) \
   $(foreach piece, $(GENERIC_PIECES), ../../../$(piece)/$(ARCH)/$(piece).rel)
@end example

The variable @code{OBJS} is the list of "pieces" expanded to include
path information to the appropriate object files.  The @code{wildcard}
function is used on pieces of @code{libbsp.a} which are optional and
may not be present based upon the configuration options.

@section Makefiles Used Both During The BSP Design and its Use

When building a BSP or an application using that BSP, it is necessary
to tailor the compilation arguments to account for compiler flags, use
custom linker scripts, include the RTEMS libraries, etc..  The BSP
must be built using this information.  Later, once the BSP is installed
with the toolset, this same information must be used when building the
application.  So a BSP must include a build configuration file.  The
configuration file is @code{make/custom/BSP.cfg}.

The configuration file is taken into account when building one's
application using the RTEMS template Makefiles (@code{make/templates}). 
It is strongly advised to use these template Makefiles since they 
encapsulate a number of build rules along with the compile and link
time options necessary to execute on the target board.

There is a template Makefile provided for each of class of RTEMS
Makefiles.  The purpose of each class of RTEMS Makefile is to:

@itemize @bullet
@item call recursively the makefiles in the directories beneath
the current one,

@item build a library, or

@item build an executable.

@end itemize

The following is a shortened and heavily commented version of the
make customization file for the gen68340 BSP.  The original source
for this file can be found in the @code{make/custom} directory.

@example

# The RTEMS CPU Family and Model
RTEMS_CPU=m68k
RTEMS_CPU_MODEL=mcpu32

include $(RTEMS_ROOT)/make/custom/default.cfg

# The name of the BSP directory used for the actual source code.
# This allows for build variants of the same BSP source.
RTEMS_BSP_FAMILY=gen68340

# CPU flag to pass to GCC
CPU_CFLAGS = -mcpu32

# optimization flag to pass to GCC
CFLAGS_OPTIMIZE_V=-O4 -fomit-frame-pointer

# The name of the start file to be linked with.  This file is the first
# part of the BSP which executes.
START_BASE=start340

[...]

# This make-exe macro is used in template makefiles to build the
# final executable. Any other commands to follow, just as using
# objcopy to build a PROM image or converting the executable to binary. 

ifeq ($(RTEMS_USE_GCC272),yes)
# This has rules to link an application if an older version of GCC is 
# to be used with this BSP.  It is not required for a BSP to support
# older versions of GCC.  This option is supported in some of the
# BSPs which already had this support.
[...]
else
# This has rules to link an application using gcc 2.8 or newer or any
# egcs version.  All BSPs should support this.  This version is required
# to support GNAT/RTEMS.
define make-exe
	$(CC) $(CFLAGS) $(CFLAGS_LD) -o $(basename $@@).exe $(LINK_OBJS)
	$(NM) -g -n $(basename $@@).exe > $(basename $@@).num
	$(SIZE) $(basename $@@).exe
endif
@end example

@subsection Creating a New BSP Make Customization File

The basic steps for creating a @code{make/custom} file for a new BSP
are as follows:

@itemize @bullet

@item copy any @code{.cfg} file to @code{BSP.cfg}

@item modify RTEMS_CPU, RTEMS_CPU_MODEL, RTEMS_BSP_FAMILY,
RTEMS_BSP, CPU_CFLAGS, START_BASE, and make-exe rules.

@end itemize

It is generally easier to copy a @code{make/custom} file from a
BSP similar to the one being developed.



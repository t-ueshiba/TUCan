#
#  $Id: Makefile,v 1.10 2009-07-08 01:10:18 ueshiba Exp $
#
#################################
#  User customizable macros	#
#################################
DEST		= $(PREFIX)/lib
INCDIR		= $(PREFIX)/include/TU
INCDIRS		= -I. -I$(PREFIX)/include

NAME		= $(shell basename $(PWD))

CPPFLAGS	=
CFLAGS		= -g
ifeq ($(CCC), icpc)
  ifeq ($(OSTYPE), darwin)
    CPPFLAGS   += -DSSE3
    CFLAGS	= -O3 -axP
  else
    CPPFLAGS   += -DSSSE3
    CFLAGS	= -O3 -xN
  endif
endif
CCFLAGS		= $(CFLAGS)

LINKER		= $(CCC)

#########################
#  Macros set by mkmf	#
#########################
SUFFIX		= .cc:sC
EXTHDRS		= /home/ueshiba/include/TU/Array++.h \
		/home/ueshiba/include/TU/Vector++.h \
		/home/ueshiba/include/TU/types.h \
		TU/Can++.h
HDRS		= Can++.h
SRCS		= Can.cc \
		Manus.cc
OBJS		= Can.o \
		Manus.o

#########################
#  Macros used by RCS	#
#########################
REV		= $(shell echo $Revision: 1.10 $	|		\
		  sed 's/evision://'		|		\
		  awk -F"."					\
		  '{						\
		      for (count = 1; count < NF; count++)	\
			  printf("%d.", $$count);		\
		      printf("%d", $$count + 1);		\
		  }')

include $(PROJECT)/lib/l.mk
###
Can.o: TU/Can++.h /home/ueshiba/include/TU/Vector++.h \
	/home/ueshiba/include/TU/Array++.h /home/ueshiba/include/TU/types.h
Manus.o: TU/Can++.h /home/ueshiba/include/TU/Vector++.h \
	/home/ueshiba/include/TU/Array++.h /home/ueshiba/include/TU/types.h

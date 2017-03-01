SUBDIR	= TUTools++		\
	  TUv++			\
	  TUOgl++		\
	  TUIeee1394++		\
	  TUIIDC++		\
	  TUvIIDC++		\
	  TUObject++		\
	  TUBrep++		\
	  TUCollection++	\
	  TUVision++		\
	  TUXv++		\
	  TUV4L2++		\
	  TUvV4L2++		\
#	  TUCuda++		\
#	  TUUSB++		\
#	  TUHRP2++

TARGETS	= all install clean depend

all:

$(TARGETS):
	@for d in $(SUBDIR); do				\
	  echo "";					\
	  echo "*** Current directory: $$d ***";	\
	  cd $$d;					\
	  $(MAKE) NAME=$$d $@;				\
	  cd ..;					\
	done

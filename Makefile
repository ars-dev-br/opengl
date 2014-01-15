SUBDIRS := $(wildcard */.)
.PHONY : all $(SUBDIRS)

# Build all samples in subdirectories
all : $(SUBDIRS)

$(SUBDIRS) :
	$(MAKE) -C $@

clean :
	@for d in $(SUBDIRS); do   \
		$(MAKE) -C $$d clean;  \
	done;

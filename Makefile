include $(BR2_MKENTRY)
ifeq ($(skip-makefile),)

cflags-y += -I$(srctree) -I$(srctree)/argent-cores/include

target-y += libargent_loader.a

obj-$(BR2_PACKAGE_ARGENT_LOADER) += argent_loader.o

all: $(target-y)

argent_loader.o: $(srctree)/argent_loader.c
	$(Q)$(CC) $(cflags-y) $(CFLAGS) -c $< -o $@

libargent_loader.a: argent_loader.o
	$(Q)$(AR) -rc $@ $^

install: FORCE
	install -d -m 0775 $(STAGING_DIR)/usr/include   
	cp -fr $(srctree)/argent_loader.h $(STAGING_DIR)/usr/include/
	install -d -m 0775 $(STAGING_DIR)/usr/lib
	install -m 0664 $(target-y) $(STAGING_DIR)/usr/lib/
	cp -r $(srctree)/argent-cores/include/* $(STAGING_DIR)/usr/include/

clean: FORCE
	$(Q)$(MAKE) $(clean)=.
	-rm -f libargent_loader.a

.PHONY: FORCE

FORCE:

endif
include ../version.mk

PA_HEADERS?=../3rdparty/pulseaudio-headers

DESTDIR?= # for PKGBUILD and the like
PREFIX?=/usr/local
LIBDIR?=$(PREFIX)/lib
INCLUDEDIR?=$(PREFIX)/include

LIBS=glib-2.0
CFLAGS+=$(shell pkg-config --cflags $(LIBS))
LDFLAGS+=$(shell pkg-config --libs $(LIBS))

.PHONY: all install clean libs pcs cmakes
all: libs pcs cmakes

libs: libpulse.so libpulse-simple.so libpulse-mainloop-glib.so

pcs: libpulse.pc libpulse-simple.pc libpulse-mainloop-glib.pc

cmakes: PulseAudioConfig.cmake PulseAudioConfigVersion.cmake

libpulse-simple.so libpulse-mainloop-glib.so: libpulse.so
	@ln -s $< $@.0
	@ln -s $< $@

lib%.so: %.c nopulse.h
	@$(CC) -shared -fPIC -I$(PA_HEADERS) $(CFLAGS) $(LDFLAGS) -o $@.0 $<
	@ln -s $@.0 $@

nopulse.h: nopulse.h.in
	@sed "s%@CA_VERSION@%$(CA_VERSION)%g" $< > $@

%: %.in
	@sed "s%@PREFIX@%$(PREFIX)%g; s%@LIBDIR@%$(LIBDIR)%g; s%@INCLUDEDIR@%$(INCLUDEDIR)%g; s%@PA_VERSION_MAJOR@%$(PA_VERSION_MAJOR)%g; s%@PA_VERSION_MINOR@%$(PA_VERSION_MINOR)%g; s%@PA_VERSION@%$(PA_VERSION)%g" $< > $@

install: all
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp -a *.so *.so.0 $(DESTDIR)$(LIBDIR)
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -a $(PA_HEADERS)/pulse $(DESTDIR)$(INCLUDEDIR)
	@mkdir -p $(DESTDIR)$(LIBDIR)/pkgconfig
	@cp -a *.pc $(DESTDIR)$(LIBDIR)/pkgconfig
	@mkdir -p $(DESTDIR)$(LIBDIR)/cmake/PulseAudio
	@cp -a *.cmake $(DESTDIR)$(LIBDIR)/cmake/PulseAudio

clean:
	rm -f *.so *.so.0 *.pc *.cmake

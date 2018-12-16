PA_HEADERS?=../3rdparty/pulseaudio-headers
PREFIX?=/usr/local
LIBDIR?=/lib
DESTDIR?= # test

LIBS=glib-2.0
CFLAGS+=$(shell pkg-config --cflags $(LIBS))
LDFLAGS+=$(shell pkg-config --libs $(LIBS))

.PHONY: all install clean libs pcs cmakes
all: libs pcs cmakes

libs: libpulse.so libpulse-simple.so libpulse-mainloop-glib.so

libpulse-simple.so libpulse-mainloop-glib.so: libpulse.so
	@ln -s $< $@.0
	@ln -s $< $@

lib%.so: %.c
	@$(CC) -shared -fPIC -I$(PA_HEADERS) $(CFLAGS) $(LDFLAGS) -o $@.0 $<
	@ln -s $@.0 $@

pcs: libpulse.pc libpulse-simple.pc libpulse-mainloop-glib.pc

lib%.pc: %.pc.in
	@sed "s%@PREFIX@%$(PREFIX)%g" $< > $@

cmakes: PulseAudioConfig.cmake PulseAudioConfigVersion.cmake

%.cmake: %.cmake.in
	@sed "s%@PREFIX@%$(PREFIX)%g" $< > $@

install: all
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp -a $(PA_HEADERS)/pulse $(DESTDIR)$(PREFIX)/include
	@mkdir -p $(DESTDIR)$(PREFIX)${LIBDIR}
	@cp -a *.so *.so.0 $(DESTDIR)$(PREFIX)${LIBDIR}
	@mkdir -p $(DESTDIR)$(PREFIX)${LIBDIR}/pkgconfig
	@cp -a *.pc $(DESTDIR)$(PREFIX)${LIBDIR}/pkgconfig
	@mkdir -p $(DESTDIR)$(PREFIX)${LIBDIR}/cmake/PulseAudio
	@cp -a *.cmake $(DESTDIR)$(PREFIX)${LIBDIR}/cmake/PulseAudio

clean:
	rm -f *.so *.so.0 *.pc *.cmake

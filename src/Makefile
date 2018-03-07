PA_HEADERS?=../3rdparty/pulseaudio-headers
PREFIX?=/usr/local

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
	@mkdir -p $(PREFIX)/include
	@cp -a $(PA_HEADERS)/pulse $(PREFIX)/include
	@mkdir -p $(PREFIX)/lib
	@cp -a *.so *.so.0 $(PREFIX)/lib
	@mkdir -p $(PREFIX)/lib/pkgconfig
	@cp -a *.pc $(PREFIX)/lib/pkgconfig
	@mkdir -p $(PREFIX)/lib/cmake/PulseAudio
	@cp -a *.cmake $(PREFIX)/lib/cmake/PulseAudio

clean:
	rm -f *.so *.so.0 *.pc *.cmake

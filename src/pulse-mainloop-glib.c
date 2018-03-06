#include "nopulse.h"
#include <pulse/xmalloc.h>
#include <string.h>

#include <pulse/glib-mainloop.h>

struct pa_glib_mainloop {
    pa_mainloop_api api;
};

pa_glib_mainloop* pa_glib_mainloop_new(GMainContext *c) {
    pa_glib_mainloop* m = pa_xnew(pa_glib_mainloop, 1);
    memset(&(m->api), 0, sizeof(pa_mainloop_api));
    return m;
}

void pa_glib_mainloop_free(pa_glib_mainloop *m) DO_NOTHING

pa_mainloop_api* pa_glib_mainloop_get_api(pa_glib_mainloop *m) {
    return &m->api;
}

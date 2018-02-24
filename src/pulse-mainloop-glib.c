#include "nopulse.h"
#include <pulse/glib-mainloop.h>

pa_glib_mainloop * pa_glib_mainloop_new(GMainContext *c) DO_ABORT

void pa_glib_mainloop_free(pa_glib_mainloop *g) DO_ABORT

pa_mainloop_api * pa_glib_mainloop_get_api(pa_glib_mainloop *g) DO_ABORT

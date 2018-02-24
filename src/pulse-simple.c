#include "nopulse.h"
#include <pulse/simple.h>

// NOTE: "Simple", LOL
pa_simple* pa_simple_new(const char *server, const char *name, pa_stream_direction_t dir, const char *dev, const char *stream_name, const pa_sample_spec *ss, const pa_channel_map *map, const pa_buffer_attr *attr, int *error) FAIL_NULL

void pa_simple_free(pa_simple *s) DO_NOTHING

// NOTE: The doc says it's like read(2), blocking semantics is different from read(2)
int pa_simple_read(pa_simple *s, void *data, size_t bytes, int *error) FAIL_NOTIMPL

int pa_simple_drain(pa_simple *s, int *error) FAIL_NOTIMPL

// NOTE: Same thing
int pa_simple_write(pa_simple *s, const void *data, size_t bytes, int *error) FAIL_NOTIMPL

int pa_simple_flush(pa_simple *s, int *error) FAIL_NOTIMPL

pa_usec_t pa_simple_get_latency(pa_simple *s, int *error) RETURN_ZERO

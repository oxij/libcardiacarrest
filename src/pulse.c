/*
 * Copyright (C) 2018. Jan Malakhovski
 *
 * This file is a part of libcardiacarrest project.
 *
 * It may be used under the GNU LGPL version 2.1 or any later version.
 */

#include "nopulse.h"

// --------------------- "Does not belong here" parts-------------------------

#include <pulse/xmalloc.h>

// Right, let's implement our own slightly different version of glib
// gmalloc. Doesn't matter that we already link against glib. Why not?
// Right?

#include <malloc.h>
#include <string.h>

void* pa_xmalloc(size_t size)
{
    void * r = malloc(size);
    if (!r) abort();
    return r;
}

// NOTE(pa_xnew): The above function gets used in pa_xnew static
// fuction defined in PA headers (i.e. we can't change its
// implemetation even if we wanted). The fun fact is, pa_xnew requires
// the size param to be at least one byte or else it will fail with
// division by zero. Why? Because it makes integer division, of
// course! Seriously. Go behold that code yourself.

void* pa_xmalloc0(size_t size) {
    void * r = calloc(size, 1);
    if (!r) abort();
    return r;
}

void* pa_xrealloc(void *ptr, size_t size) {
    void * r = realloc(ptr, size);
    if (!r) abort();
    return r;
}

void pa_xfree(void *p) {
    free(p);
}


void* pa_xmemdup(const void *p, size_t size) {
    if (!p)
        return NULL;

    char *r = pa_xmalloc(size);
    memcpy(r, p, size);
    return r;
}

char* pa_xstrdup(const char *s) {
    if (!s)
        return NULL;

    return pa_xmemdup(s, strlen(s)+1);
}

char* pa_xstrndup(const char *s, size_t l) {
    if (!s)
        return NULL;

    char *e;
    if ((e = memchr(s, 0, l)))
        return pa_xmemdup(s, (size_t) (e-s+1));

    // ugly
    char *r = pa_xmalloc(l+1);
    memcpy(r, s, l);
    r[l] = 0;
    return r;
}

#include <pulse/utf8.h>

// Right, let's put iconv in here too. Why not? Right? Right?
// It's an audio library after all.

#define RETURN_STRDUP { \
    return pa_xstrdup(str); \
}

// NOTE: the following is not my invention to simplify the
// implementation, it is exactly the expected semantics of the
// corresponding PA API. Predicates that return newly allocated
// strings. LOL.
char* pa_utf8_valid(const char *str) RETURN_STRDUP
char* pa_ascii_valid(const char *str) RETURN_STRDUP

char* pa_utf8_filter(const char *str) RETURN_STRDUP
char* pa_ascii_filter(const char *str) RETURN_STRDUP
char* pa_utf8_to_locale (const char *str) RETURN_STRDUP
char* pa_locale_to_utf8 (const char *str) RETURN_STRDUP

#include <pulse/util.h>

// ... and our own netdb.h

char* pa_get_user_name(char *s, size_t l) RETURN_NULL
char* pa_get_host_name(char *s, size_t l) RETURN_NULL
char* pa_get_fqdn(char *s, size_t l) RETURN_NULL
char* pa_get_home_dir(char *s, size_t l) RETURN_NULL

// openal-soft uses these two...
char* pa_get_binary_name(char *s, size_t l) RETURN_NULL

// NOTE: this call is designed for generating memory leaks
// just look at its uses
char* pa_path_get_filename(const char *p) RETURN_NULL

int pa_msleep(unsigned long t) DO_ABORT

#include <pulse/timeval.h>

// ... and our own time.h

struct timeval *pa_gettimeofday(struct timeval *tv) DO_ABORT
pa_usec_t pa_timeval_age(const struct timeval *tv) DO_ABORT

pa_usec_t pa_timeval_diff(const struct timeval *a, const struct timeval *b) DO_ABORT
int pa_timeval_cmp(const struct timeval *a, const struct timeval *b) DO_ABORT

struct timeval* pa_timeval_add(struct timeval *tv, pa_usec_t v) DO_ABORT
struct timeval* pa_timeval_sub(struct timeval *tv, pa_usec_t v) DO_ABORT
struct timeval* pa_timeval_store(struct timeval *tv, pa_usec_t v) DO_ABORT
pa_usec_t pa_timeval_load(const struct timeval *tv) DO_ABORT

#include <pulse/rtclock.h>

pa_usec_t pa_rtclock_now(void) DO_ABORT

// --------------------- Utils -------------------------

#include <pulse/version.h>

const char* pa_get_library_version(void) {
    return CA_LIBRARY_VERSION;
}

#include <pulse/error.h>

#define CA_ERROR_NO_SERVICE -2

const char* pa_strerror(int error) {
    if (error == CA_ERROR_NO_SERVICE)
        return "libcardiacarrest refuses to connect to PulseAudio daemon for ethical and security reasons";

    return "libcardiacarrest does not implement this";
}

#include <pulse/operation.h>

pa_operation* pa_operation_ref(pa_operation *dest) RETURN_DEST
void pa_operation_unref(pa_operation *o) DO_NOTHING
void pa_operation_cancel(pa_operation *o) DO_NOTHING

pa_operation_state_t pa_operation_get_state(pa_operation *o) {
    // no INVALID or FAILED state in the API, so we're done
    return PA_OPERATION_DONE;
}

void pa_operation_set_state_callback(pa_operation *o, pa_operation_notify_cb_t cb, void *userdata) DO_NOTHING

#define RETURN_NOOP DO_ABORT

// --------------------- Easy parts-------------------------

#include <pulse/direction.h>

int pa_direction_valid(pa_direction_t direction) RETURN_TRUE
const char *pa_direction_to_string(pa_direction_t direction) RETURN_INVALID

#include <pulse/sample.h>

pa_sample_spec* pa_sample_spec_init(pa_sample_spec *dest) RETURN_DEST

int pa_sample_spec_valid(const pa_sample_spec *spec) RETURN_TRUE
int pa_sample_spec_equal(const pa_sample_spec*a, const pa_sample_spec*b) RETURN_TRUE

char* pa_sample_spec_snprint(char *s, size_t l, const pa_sample_spec *spec) SNPRINTF_UNKNOWN

size_t pa_bytes_per_second(const pa_sample_spec *spec) RETURN_ONE
size_t pa_frame_size(const pa_sample_spec *spec) RETURN_ONE
size_t pa_sample_size(const pa_sample_spec *spec) RETURN_ONE
size_t pa_sample_size_of_format(pa_sample_format_t f) RETURN_ONE
pa_usec_t pa_bytes_to_usec(uint64_t length, const pa_sample_spec *spec) RETURN_ONE
size_t pa_usec_to_bytes(pa_usec_t t, const pa_sample_spec *spec) RETURN_ONE

char* pa_bytes_snprint(char *s, size_t l, unsigned v) {
    snprintf(s, l, "1");
    return s;
}

int pa_sample_format_valid(unsigned format) RETURN_TRUE // NOTE: type error in the original?
int pa_sample_rate_valid(uint32_t rate) RETURN_TRUE
int pa_channels_valid(uint8_t channels) RETURN_TRUE

const char *pa_sample_format_to_string(pa_sample_format_t f) RETURN_INVALID

pa_sample_format_t pa_parse_sample_format(const char *format) {
    return PA_SAMPLE_INVALID;
}

int pa_sample_format_is_le(pa_sample_format_t f) RETURN_MINUSONE
int pa_sample_format_is_be(pa_sample_format_t f) RETURN_MINUSONE

#include <pulse/format.h>

const char *pa_encoding_to_string(pa_encoding_t e) RETURN_INVALID

pa_encoding_t pa_encoding_from_string(const char *encoding) {
    return PA_ENCODING_INVALID;
}

pa_format_info* pa_format_info_new(void) DO_ABORT
pa_format_info* pa_format_info_copy(const pa_format_info *src) DO_ABORT
void pa_format_info_free(pa_format_info *f) DO_NOTHING

int pa_format_info_valid(const pa_format_info *f) RETURN_TRUE
int pa_format_info_is_pcm(const pa_format_info *f) RETURN_TRUE
int pa_format_info_is_compatible(const pa_format_info *first, const pa_format_info *second) RETURN_TRUE

char* pa_format_info_snprint(char *s, size_t l, const pa_format_info *f) SNPRINTF_UNKNOWN

pa_format_info* pa_format_info_from_string(const char *str) DO_ABORT
pa_format_info* pa_format_info_from_sample_spec(const pa_sample_spec *ss, const pa_channel_map *map) DO_ABORT
int pa_format_info_to_sample_spec(const pa_format_info *f, pa_sample_spec *ss, pa_channel_map *map) RETURN_MINUSONE

pa_prop_type_t pa_format_info_get_prop_type(const pa_format_info *f, const char *key) {
    return PA_PROP_TYPE_INVALID;
}

int pa_format_info_get_prop_int(const pa_format_info *f, const char *key, int *v) RETURN_MINUSONE
int pa_format_info_get_prop_int_range(const pa_format_info *f, const char *key, int *min, int *max) RETURN_MINUSONE
int pa_format_info_get_prop_int_array(const pa_format_info *f, const char *key, int **values, int *n_values) RETURN_MINUSONE
int pa_format_info_get_prop_string(const pa_format_info *f, const char *key, char **v) RETURN_MINUSONE
int pa_format_info_get_prop_string_array(const pa_format_info *f, const char *key, char ***values, int *n_values) RETURN_MINUSONE
void pa_format_info_free_string_array(char **values, int n_values) DO_NOTHING

void pa_format_info_set_prop_int(pa_format_info *f, const char *key, int value) DO_NOTHING
void pa_format_info_set_prop_int_array(pa_format_info *f, const char *key, const int *values, int n_values) DO_NOTHING
void pa_format_info_set_prop_int_range(pa_format_info *f, const char *key, int min, int max) DO_NOTHING
void pa_format_info_set_prop_string(pa_format_info *f, const char *key, const char *value) DO_NOTHING
void pa_format_info_set_prop_string_array(pa_format_info *f, const char *key, const char **values, int n_values) DO_NOTHING
void pa_format_info_set_sample_format(pa_format_info *f, pa_sample_format_t sf) DO_NOTHING
void pa_format_info_set_rate(pa_format_info *f, int rate) DO_NOTHING
void pa_format_info_set_channels(pa_format_info *f, int channels) DO_NOTHING
void pa_format_info_set_channel_map(pa_format_info *f, const pa_channel_map *map) DO_NOTHING

#include <pulse/channelmap.h>

pa_channel_map* pa_channel_map_init(pa_channel_map *dest) RETURN_DEST
pa_channel_map* pa_channel_map_init_mono(pa_channel_map *dest) RETURN_DEST
pa_channel_map* pa_channel_map_init_stereo(pa_channel_map *dest) RETURN_DEST
pa_channel_map* pa_channel_map_init_auto(pa_channel_map *dest, unsigned channels, pa_channel_map_def_t def) RETURN_DEST
pa_channel_map* pa_channel_map_init_extend(pa_channel_map *dest, unsigned channels, pa_channel_map_def_t def) RETURN_DEST

const char* pa_channel_position_to_string(pa_channel_position_t pos) RETURN_UNKNOWN
const char* pa_channel_position_to_pretty_string(pa_channel_position_t pos) RETURN_UNKNOWN
char * pa_channel_map_snprint(char *s, size_t l, const pa_channel_map *map) SNPRINTF_UNKNOWN

pa_channel_position_t pa_channel_position_from_string(const char *s)
{
    return PA_CHANNEL_POSITION_INVALID;
}

pa_channel_map* pa_channel_map_parse(pa_channel_map *map, const char *s) DO_ABORT

int pa_channel_map_has_position(const pa_channel_map *map, pa_channel_position_t p) RETURN_TRUE
int pa_channel_map_equal(const pa_channel_map *a, const pa_channel_map *b) RETURN_TRUE
int pa_channel_map_valid(const pa_channel_map *map) RETURN_TRUE
int pa_channel_map_compatible(const pa_channel_map *map, const pa_sample_spec *ss) RETURN_TRUE
int pa_channel_map_superset(const pa_channel_map *a, const pa_channel_map *b) RETURN_TRUE
int pa_channel_map_can_balance(const pa_channel_map *map) RETURN_TRUE
int pa_channel_map_can_lfe_balance(const pa_channel_map *map) RETURN_TRUE
int pa_channel_map_can_fade(const pa_channel_map *map) RETURN_TRUE

const char* pa_channel_map_to_name(const pa_channel_map *map) RETURN_UNKNOWN
const char* pa_channel_map_to_pretty_name(const pa_channel_map *map) RETURN_UNKNOWN

pa_channel_position_mask_t pa_channel_map_mask(const pa_channel_map *map) DO_ABORT

#include <pulse/volume.h>

pa_cvolume* pa_cvolume_init(pa_cvolume *dest) RETURN_DEST
pa_cvolume* pa_cvolume_set(pa_cvolume *dest, unsigned channels, pa_volume_t v) RETURN_DEST

int pa_cvolume_valid(const pa_cvolume *v) RETURN_TRUE
int pa_cvolume_equal(const pa_cvolume *a, const pa_cvolume *b) RETURN_TRUE
int pa_cvolume_channels_equal_to(const pa_cvolume *a, pa_volume_t v) RETURN_TRUE
int pa_cvolume_compatible(const pa_cvolume *v, const pa_sample_spec *ss) RETURN_TRUE
int pa_cvolume_compatible_with_channel_map(const pa_cvolume *v, const pa_channel_map *cm) RETURN_TRUE

pa_cvolume* pa_sw_cvolume_multiply(pa_cvolume *dest, const pa_cvolume *a, const pa_cvolume *b) RETURN_DEST
pa_cvolume* pa_sw_cvolume_multiply_scalar(pa_cvolume *dest, const pa_cvolume *a, pa_volume_t b) RETURN_DEST
pa_cvolume* pa_sw_cvolume_divide(pa_cvolume *dest, const pa_cvolume *a, const pa_cvolume *b) RETURN_DEST
pa_cvolume* pa_sw_cvolume_divide_scalar(pa_cvolume *dest, const pa_cvolume *a, pa_volume_t b) RETURN_DEST
pa_cvolume* pa_cvolume_remap(pa_cvolume *dest, const pa_channel_map *from, const pa_channel_map *to) RETURN_DEST
pa_cvolume* pa_cvolume_scale(pa_cvolume *dest, pa_volume_t max) RETURN_DEST
pa_cvolume* pa_cvolume_scale_mask(pa_cvolume *dest, pa_volume_t max, pa_channel_map *cm, pa_channel_position_mask_t mask) RETURN_DEST
pa_cvolume* pa_cvolume_merge(pa_cvolume *dest, const pa_cvolume *a, const pa_cvolume *b) RETURN_DEST
pa_cvolume* pa_cvolume_inc_clamp(pa_cvolume *dest, pa_volume_t inc, pa_volume_t limit) RETURN_DEST
pa_cvolume* pa_cvolume_inc(pa_cvolume *dest, pa_volume_t inc) RETURN_DEST
pa_cvolume* pa_cvolume_dec(pa_cvolume *dest, pa_volume_t dec) RETURN_DEST

pa_volume_t pa_cvolume_avg(const pa_cvolume *a) RETURN_ZERO
pa_volume_t pa_cvolume_avg_mask(const pa_cvolume *a, const pa_channel_map *cm, pa_channel_position_mask_t mask) RETURN_ZERO
pa_volume_t pa_cvolume_max(const pa_cvolume *a) RETURN_ZERO
pa_volume_t pa_cvolume_max_mask(const pa_cvolume *a, const pa_channel_map *cm, pa_channel_position_mask_t mask) RETURN_ZERO
pa_volume_t pa_cvolume_min(const pa_cvolume *a) RETURN_ZERO
pa_volume_t pa_cvolume_min_mask(const pa_cvolume *a, const pa_channel_map *cm, pa_channel_position_mask_t mask) RETURN_ZERO
pa_volume_t pa_sw_volume_multiply(pa_volume_t a, pa_volume_t b) RETURN_ZERO
pa_volume_t pa_sw_volume_divide(pa_volume_t a, pa_volume_t b) RETURN_ZERO

pa_volume_t pa_cvolume_get_position(pa_cvolume *cv, const pa_channel_map *map, pa_channel_position_t t) RETURN_ZERO
pa_cvolume* pa_cvolume_set_position(pa_cvolume *dest, const pa_channel_map *map, pa_channel_position_t t, pa_volume_t v) RETURN_DEST

float pa_cvolume_get_balance(const pa_cvolume *v, const pa_channel_map *map) RETURN_ZERO
pa_cvolume* pa_cvolume_set_balance(pa_cvolume *dest, const pa_channel_map *map, float new_balance) RETURN_DEST

float pa_cvolume_get_fade(const pa_cvolume *v, const pa_channel_map *map) RETURN_ZERO
pa_cvolume* pa_cvolume_set_fade(pa_cvolume *dest, const pa_channel_map *map, float new_fade) RETURN_DEST

float pa_cvolume_get_lfe_balance(const pa_cvolume *v, const pa_channel_map *map) RETURN_ZERO
pa_cvolume* pa_cvolume_set_lfe_balance(pa_cvolume *dest, const pa_channel_map *map, float new_balance) RETURN_DEST

pa_volume_t pa_sw_volume_from_dB(double f) RETURN_ZERO
double pa_sw_volume_to_dB(pa_volume_t v) RETURN_ZERO

pa_volume_t pa_sw_volume_from_linear(double v) RETURN_ZERO
double pa_sw_volume_to_linear(pa_volume_t v) RETURN_ZERO

char *pa_volume_snprint(char *s, size_t l, pa_volume_t v) SNPRINTF_UNKNOWN
char *pa_volume_snprint_verbose(char *s, size_t l, pa_volume_t v, int print_dB) SNPRINTF_UNKNOWN

char *pa_sw_cvolume_snprint_dB(char *s, size_t l, const pa_cvolume *c) SNPRINTF_UNKNOWN
char *pa_sw_volume_snprint_dB(char *s, size_t l, pa_volume_t v) SNPRINTF_UNKNOWN

char *pa_cvolume_snprint(char *s, size_t l, const pa_cvolume *c) SNPRINTF_UNKNOWN
char *pa_cvolume_snprint_verbose(char *s, size_t l, const pa_cvolume *c, const pa_channel_map *map, int print_dB) SNPRINTF_UNKNOWN

// --------------------- Proplists -------------------------

#include <pulse/proplist.h>

// Right, let's reimplement parts of glib slightly differently just for
// fun.

int pa_proplist_key_valid(const char *key) RETURN_TRUE

// See NOTE(pa_xnew)
// Also, I would have expected to see _t here
struct pa_proplist {
    unsigned size;
};

pa_proplist* pa_proplist_new(void) {
    pa_proplist* p = pa_xnew(pa_proplist, 1);
    p->size = 0;
    return p;
}

pa_proplist* pa_proplist_copy(const pa_proplist *p) DO_ABORT

void pa_proplist_free(pa_proplist* p) {
    free(p);
}

void pa_proplist_clear(pa_proplist *p) {
    p->size = 0;
}

char* pa_proplist_to_string(pa_proplist *p) DO_ABORT
char* pa_proplist_to_string_sep(pa_proplist *p, const char *sep) DO_ABORT
pa_proplist *pa_proplist_from_string(const char *str) DO_ABORT

// NOTE: Eh? Not size_t?
unsigned pa_proplist_size(pa_proplist *p) {
    return p->size;
}

int pa_proplist_isempty(pa_proplist *p) {
    return p->size != 0;
}

int pa_proplist_equal(pa_proplist *a, pa_proplist *b) {
    return a->size == b->size;
}

int pa_proplist_contains(pa_proplist *p, const char *key) RETURN_TRUE

int pa_proplist_get(pa_proplist *p, const char *key, const void **data, size_t *nbytes) DO_ABORT
const char* pa_proplist_gets(pa_proplist *p, const char *key) DO_ABORT

int pa_proplist_set(pa_proplist *p, const char *key, const void *data, size_t nbytes) RETURN_ZERO
int pa_proplist_sets(pa_proplist *p, const char *key, const char *value) RETURN_ZERO
int pa_proplist_setp(pa_proplist *p, const char *pair) RETURN_ZERO
int pa_proplist_setf(pa_proplist *p, const char *key, const char *format, ...) RETURN_ZERO

int pa_proplist_unset(pa_proplist *p, const char *key) DO_ABORT
int pa_proplist_unset_many(pa_proplist *p, const char * const keys[]) DO_ABORT

void pa_proplist_update(pa_proplist *p, pa_update_mode_t mode, const pa_proplist *other) DO_ABORT

const char* pa_proplist_iterate(pa_proplist *p, void **state) DO_ABORT

// --------------------- Hard parts -------------------------

#include <pulse/context.h>

// Where's the _t?
struct pa_context {
    unsigned counter;
};

pa_context *pa_context_new(pa_mainloop_api *mainloop, const char *name) {
    return pa_xnew(pa_context, 1);
}

pa_context *pa_context_new_with_proplist(pa_mainloop_api *mainloop, const char *name, pa_proplist *proplist) {
    return pa_context_new(mainloop, name);
}

pa_context* pa_context_ref(pa_context *c) {
    c->counter++;
}

void pa_context_unref(pa_context *c) {
    if (c->counter == 1)
        free(c);
    c->counter--;
}

// All of this file does is makes the PA lib users to finally call this function.
int pa_context_connect(pa_context *c, const char *server, pa_context_flags_t flags, const pa_spawn_api *api) {
    return CA_ERROR_NO_SERVICE;
}

void pa_context_disconnect(pa_context *c) DO_NOTHING

int pa_context_is_local(pa_context *c) RETURN_MINUSONE
int pa_context_is_pending(pa_context *c) RETURN_FALSE

void pa_context_set_state_callback(pa_context *c, pa_context_notify_cb_t cb, void *userdata) DO_NOTHING
void pa_context_set_event_callback(pa_context *p, pa_context_event_cb_t cb, void *userdata) DO_NOTHING

pa_context_state_t pa_context_get_state(pa_context *c) {
    return PA_CONTEXT_FAILED;
}

int pa_context_errno(pa_context *c) {
    return CA_ERROR_NO_SERVICE;
}

pa_operation* pa_context_drain(pa_context *c, pa_context_notify_cb_t cb, void *userdata) RETURN_NULL

// Go read the description of this function and admire the design. LOL.
pa_operation* pa_context_exit_daemon(pa_context *c, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_set_default_sink(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_default_source(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_name(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

const char* pa_context_get_server(pa_context *c) RETURN_UNKNOWN

uint32_t pa_context_get_protocol_version(pa_context *c) {
    return 13;
}

uint32_t pa_context_get_server_protocol_version(pa_context *c) {
    return 13;
}

uint32_t pa_context_get_index(pa_context *s) DO_ABORT // NOTE: No errors possible? LOL API

pa_time_event* pa_context_rttime_new(pa_context *c, pa_usec_t usec, pa_time_event_cb_t cb, void *userdata) DO_ABORT
void pa_context_rttime_restart(pa_context *c, pa_time_event *e, pa_usec_t usec) DO_ABORT
size_t pa_context_get_tile_size(pa_context *c, const pa_sample_spec *ss) DO_ABORT
int pa_context_load_cookie_from_file(pa_context *c, const char *cookie_file_path) DO_ABORT

pa_operation* pa_context_proplist_update(pa_context *c, pa_update_mode_t mode, pa_proplist *p, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_proplist_remove(pa_context *c, const char *const keys[], pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

#include <pulse/introspect.h>

pa_operation* pa_context_get_sink_info_by_name(pa_context *c, const char *name, pa_sink_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_sink_info_by_index(pa_context *c, uint32_t idx, pa_sink_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_sink_info_list(pa_context *c, pa_sink_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_sink_volume_by_index(pa_context *c, uint32_t idx, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_sink_volume_by_name(pa_context *c, const char *name, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_sink_mute_by_index(pa_context *c, uint32_t idx, int mute, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_sink_mute_by_name(pa_context *c, const char *name, int mute, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_suspend_sink_by_name(pa_context *c, const char *sink_name, int suspend, pa_context_success_cb_t cb, void* userdata) RETURN_NOOP
pa_operation* pa_context_suspend_sink_by_index(pa_context *c, uint32_t idx, int suspend,  pa_context_success_cb_t cb, void* userdata) RETURN_NOOP
pa_operation* pa_context_set_sink_port_by_index(pa_context *c, uint32_t idx, const char*port, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_sink_port_by_name(pa_context *c, const char*name, const char*port, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_get_source_info_by_name(pa_context *c, const char *name, pa_source_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_source_info_by_index(pa_context *c, uint32_t idx, pa_source_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_source_info_list(pa_context *c, pa_source_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_source_volume_by_index(pa_context *c, uint32_t idx, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_source_volume_by_name(pa_context *c, const char *name, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_source_mute_by_index(pa_context *c, uint32_t idx, int mute, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_source_mute_by_name(pa_context *c, const char *name, int mute, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_suspend_source_by_name(pa_context *c, const char *source_name, int suspend, pa_context_success_cb_t cb, void* userdata) RETURN_NOOP
pa_operation* pa_context_suspend_source_by_index(pa_context *c, uint32_t idx, int suspend, pa_context_success_cb_t cb, void* userdata) RETURN_NOOP
pa_operation* pa_context_set_source_port_by_index(pa_context *c, uint32_t idx, const char*port, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_source_port_by_name(pa_context *c, const char*name, const char*port, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_get_server_info(pa_context *c, pa_server_info_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_get_module_info(pa_context *c, uint32_t idx, pa_module_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_module_info_list(pa_context *c, pa_module_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_load_module(pa_context *c, const char*name, const char *argument, pa_context_index_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_unload_module(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_get_client_info(pa_context *c, uint32_t idx, pa_client_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_client_info_list(pa_context *c, pa_client_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_kill_client(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_get_card_info_by_index(pa_context *c, uint32_t idx, pa_card_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_card_info_by_name(pa_context *c, const char *name, pa_card_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_card_info_list(pa_context *c, pa_card_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_card_profile_by_index(pa_context *c, uint32_t idx, const char*profile, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_card_profile_by_name(pa_context *c, const char*name, const char*profile, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_set_port_latency_offset(pa_context *c, const char *card_name, const char *port_name, int64_t offset, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_get_sink_input_info(pa_context *c, uint32_t idx, pa_sink_input_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_sink_input_info_list(pa_context *c, pa_sink_input_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_move_sink_input_by_name(pa_context *c, uint32_t idx, const char *sink_name, pa_context_success_cb_t cb, void* userdata) RETURN_NOOP
pa_operation* pa_context_move_sink_input_by_index(pa_context *c, uint32_t idx, uint32_t sink_idx, pa_context_success_cb_t cb, void* userdata) RETURN_NOOP
pa_operation* pa_context_set_sink_input_volume(pa_context *c, uint32_t idx, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_sink_input_mute(pa_context *c, uint32_t idx, int mute, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_kill_sink_input(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_get_source_output_info(pa_context *c, uint32_t idx, pa_source_output_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_source_output_info_list(pa_context *c, pa_source_output_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_move_source_output_by_name(pa_context *c, uint32_t idx, const char *source_name, pa_context_success_cb_t cb, void* userdata) RETURN_NOOP
pa_operation* pa_context_move_source_output_by_index(pa_context *c, uint32_t idx, uint32_t source_idx, pa_context_success_cb_t cb, void* userdata) RETURN_NOOP
pa_operation* pa_context_set_source_output_volume(pa_context *c, uint32_t idx, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_set_source_output_mute(pa_context *c, uint32_t idx, int mute, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_kill_source_output(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_stat(pa_context *c, pa_stat_info_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_get_sample_info_by_name(pa_context *c, const char *name, pa_sample_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_sample_info_by_index(pa_context *c, uint32_t idx, pa_sample_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_sample_info_list(pa_context *c, pa_sample_info_cb_t cb, void *userdata) RETURN_NOOP

pa_operation* pa_context_get_autoload_info_by_name(pa_context *c, const char *name, pa_autoload_type_t type, pa_autoload_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_autoload_info_by_index(pa_context *c, uint32_t idx, pa_autoload_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_get_autoload_info_list(pa_context *c, pa_autoload_info_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_add_autoload(pa_context *c, const char *name, pa_autoload_type_t type, const char *module, const char *argument, pa_context_index_cb_t cb, void* userdata) RETURN_NOOP // NOTE: unnamed param in the original
pa_operation* pa_context_remove_autoload_by_name(pa_context *c, const char *name, pa_autoload_type_t type, pa_context_success_cb_t cb, void* userdata) RETURN_NOOP
pa_operation* pa_context_remove_autoload_by_index(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void* userdata) RETURN_NOOP

#include <pulse/subscribe.h>

pa_operation* pa_context_subscribe(pa_context *c, pa_subscription_mask_t m, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
void pa_context_set_subscribe_callback(pa_context *c, pa_context_subscribe_cb_t cb, void *userdata) DO_NOTHING

#include <pulse/stream.h>

pa_stream* pa_stream_new(pa_context *c, const char *name, const pa_sample_spec *ss, const pa_channel_map *map) DO_ABORT
pa_stream* pa_stream_new_with_proplist(pa_context *c, const char *name, const pa_sample_spec *ss, const pa_channel_map *map, pa_proplist *p) DO_ABORT
pa_stream *pa_stream_new_extended(pa_context *c, const char *name, pa_format_info * const * formats, unsigned int n_formats, pa_proplist *p) DO_ABORT

pa_stream *pa_stream_ref(pa_stream *dest) RETURN_DEST
void pa_stream_unref(pa_stream *s) DO_NOTHING

pa_stream_state_t pa_stream_get_state(pa_stream *p) {
    return PA_STREAM_FAILED;
}

pa_context* pa_stream_get_context(pa_stream *p) DO_ABORT
uint32_t pa_stream_get_index(pa_stream *s) DO_ABORT
uint32_t pa_stream_get_device_index(pa_stream *s) DO_ABORT
const char *pa_stream_get_device_name(pa_stream *s) RETURN_UNKNOWN
int pa_stream_is_suspended(pa_stream *s) RETURN_FALSE
int pa_stream_is_corked(pa_stream *s) RETURN_FALSE

int pa_stream_connect_playback(pa_stream *s, const char *dev, const pa_buffer_attr *attr, pa_stream_flags_t flags, const pa_cvolume *volume, pa_stream *sync_stream) RETURN_MINUSONE

int pa_stream_connect_record(pa_stream *s, const char *dev, const pa_buffer_attr *attr, pa_stream_flags_t flags) RETURN_MINUSONE

int pa_stream_disconnect(pa_stream *s) RETURN_MINUSONE

int pa_stream_begin_write(pa_stream *p, void **data, size_t *nbytes) RETURN_MINUSONE
int pa_stream_cancel_write(pa_stream *p) RETURN_MINUSONE
int pa_stream_write(pa_stream *p, const void *data, size_t nbytes, pa_free_cb_t free_cb, int64_t offset, pa_seek_mode_t seek) RETURN_MINUSONE
int pa_stream_write_ext_free(pa_stream *p, const void *data, size_t nbytes, pa_free_cb_t free_cb, void *free_cb_data, int64_t offset, pa_seek_mode_t seek) RETURN_MINUSONE
int pa_stream_peek(pa_stream *p, const void **data, size_t *nbytes) RETURN_MINUSONE
int pa_stream_drop(pa_stream *p) RETURN_MINUSONE

size_t pa_stream_writable_size(pa_stream *p) RETURN_ZERO
size_t pa_stream_readable_size(pa_stream *p) RETURN_ZERO

pa_operation* pa_stream_drain(pa_stream *s, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_stream_update_timing_info(pa_stream *p, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP

void pa_stream_set_state_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata) DO_NOTHING

void pa_stream_set_write_callback(pa_stream *p, pa_stream_request_cb_t cb, void *userdata) DO_NOTHING
void pa_stream_set_read_callback(pa_stream *p, pa_stream_request_cb_t cb, void *userdata) DO_NOTHING
void pa_stream_set_overflow_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata) DO_NOTHING
int64_t pa_stream_get_underflow_index(pa_stream *p) RETURN_MINUSONE
void pa_stream_set_underflow_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata) DO_NOTHING
void pa_stream_set_started_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata) DO_NOTHING
void pa_stream_set_latency_update_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata) DO_NOTHING
void pa_stream_set_moved_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata) DO_NOTHING
void pa_stream_set_suspended_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata) DO_NOTHING
void pa_stream_set_event_callback(pa_stream *p, pa_stream_event_cb_t cb, void *userdata) DO_NOTHING
void pa_stream_set_buffer_attr_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata) DO_NOTHING

pa_operation* pa_stream_cork(pa_stream *s, int b, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_stream_flush(pa_stream *s, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_stream_prebuf(pa_stream *s, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_stream_trigger(pa_stream *s, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_stream_set_name(pa_stream *s, const char *name, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP

int pa_stream_get_time(pa_stream *s, pa_usec_t *r_usec) RETURN_ZERO

int pa_stream_get_latency(pa_stream *s, pa_usec_t *r_usec, int *negative) RETURN_ZERO

const pa_timing_info* pa_stream_get_timing_info(pa_stream *s) DO_ABORT

const pa_sample_spec* pa_stream_get_sample_spec(pa_stream *s) DO_ABORT

const pa_channel_map* pa_stream_get_channel_map(pa_stream *s) DO_ABORT

const pa_format_info* pa_stream_get_format_info(pa_stream *s) DO_ABORT

const pa_buffer_attr* pa_stream_get_buffer_attr(pa_stream *s) DO_ABORT

pa_operation *pa_stream_set_buffer_attr(pa_stream *s, const pa_buffer_attr *attr, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_stream_update_sample_rate(pa_stream *s, uint32_t rate, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_stream_proplist_update(pa_stream *s, pa_update_mode_t mode, pa_proplist *p, pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_stream_proplist_remove(pa_stream *s, const char *const keys[], pa_stream_success_cb_t cb, void *userdata) RETURN_NOOP

int pa_stream_set_monitor_stream(pa_stream *s, uint32_t sink_input_idx) RETURN_MINUSONE

uint32_t pa_stream_get_monitor_stream(pa_stream *s) RETURN_ZERO // NOTE: No errors possible? LOL API

#include <pulse/scache.h>

int pa_stream_connect_upload(pa_stream *s, size_t length) RETURN_MINUSONE
int pa_stream_finish_upload(pa_stream *s) RETURN_MINUSONE

pa_operation* pa_context_remove_sample(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_play_sample(pa_context *c, const char *name, const char *dev, pa_volume_t volume, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation* pa_context_play_sample_with_proplist(pa_context *c, const char *name, const char *dev, pa_volume_t volume, pa_proplist *proplist, pa_context_play_sample_cb_t cb, void *userdata) RETURN_NOOP

#include <pulse/ext-device-manager.h>

pa_operation *pa_ext_device_manager_test(pa_context *c, pa_ext_device_manager_test_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_device_manager_read(pa_context *c, pa_ext_device_manager_read_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_device_manager_set_device_description(pa_context *c, const char* device, const char* description, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_device_manager_delete(pa_context *c, const char *const s[], pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_device_manager_enable_role_device_priority_routing(pa_context *c, int enable, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_device_manager_reorder_devices_for_role(pa_context *c, const char* role, const char** devices, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_device_manager_subscribe(pa_context *c, int enable, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

void pa_ext_device_manager_set_subscribe_cb(pa_context *c, pa_ext_device_manager_subscribe_cb_t cb, void *userdata) DO_NOTHING

#include <pulse/ext-device-restore.h>

pa_operation *pa_ext_device_restore_test(pa_context *c, pa_ext_device_restore_test_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_device_restore_subscribe(pa_context *c, int enable, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

void pa_ext_device_restore_set_subscribe_cb(pa_context *c, pa_ext_device_restore_subscribe_cb_t cb, void *userdata) DO_NOTHING

pa_operation *pa_ext_device_restore_read_formats_all(pa_context *c, pa_ext_device_restore_read_device_formats_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_device_restore_read_formats(pa_context *c, pa_device_type_t type, uint32_t idx, pa_ext_device_restore_read_device_formats_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_device_restore_save_formats(pa_context *c, pa_device_type_t type, uint32_t idx, uint8_t n_formats, pa_format_info **formats, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

#include <pulse/ext-stream-restore.h>

pa_operation *pa_ext_stream_restore_test(pa_context *c, pa_ext_stream_restore_test_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_stream_restore_read(pa_context *c, pa_ext_stream_restore_read_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_stream_restore_write(pa_context *c, pa_update_mode_t mode, const pa_ext_stream_restore_info data[], unsigned n, int apply_immediately, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_stream_restore_delete(pa_context *c, const char *const s[], pa_context_success_cb_t cb, void *userdata) RETURN_NOOP
pa_operation *pa_ext_stream_restore_subscribe(pa_context *c, int enable, pa_context_success_cb_t cb, void *userdata) RETURN_NOOP

void pa_ext_stream_restore_set_subscribe_cb(pa_context *c, pa_ext_stream_restore_subscribe_cb_t cb, void *userdata) DO_NOTHING

#include <pulse/mainloop-api.h>

void pa_mainloop_api_once(pa_mainloop_api *m, void (*callback)(pa_mainloop_api *m, void *userdata), void *userdata) DO_ABORT

#include <pulse/mainloop.h>

// See NOTE(pa_xnew)
struct pa_mainloop {
    pa_mainloop_api api;
};

pa_mainloop *pa_mainloop_new(void) {
    pa_mainloop* m = pa_xnew(pa_mainloop, 1);
    memset(&(m->api), 0, sizeof(pa_mainloop_api));
    return m;
}

void pa_mainloop_free(pa_mainloop* m) {
    free(m);
}

pa_mainloop_api* pa_mainloop_get_api(pa_mainloop* m) {
    return &m->api;
}

int pa_mainloop_prepare(pa_mainloop *m, int timeout) RETURN_MINUSONE
int pa_mainloop_poll(pa_mainloop *m) RETURN_MINUSONE
int pa_mainloop_dispatch(pa_mainloop *m) RETURN_MINUSONE
int pa_mainloop_get_retval(pa_mainloop *m) RETURN_ZERO

// NOTE: These should never be called as `pa_context_connect` comes
// before them and fails. But some apps fail to fail on
// `pa_context_connect` fail and then consume 100% of CPU time by
// calling these indefinitely. Let's abort.
int pa_mainloop_iterate(pa_mainloop *m, int block, int *retval) DO_ABORT
int pa_mainloop_run(pa_mainloop *m, int *retval) DO_ABORT

void pa_mainloop_quit(pa_mainloop *m, int retval) DO_NOTHING
void pa_mainloop_wakeup(pa_mainloop *m) DO_NOTHING
void pa_mainloop_set_poll_func(pa_mainloop *m, pa_poll_func poll_func, void *userdata) DO_NOTHING

#include <pulse/thread-mainloop.h>

// And a whole API for what can be done with five lines of pthread code ...

// See NOTE(pa_xnew)
struct pa_threaded_mainloop {
    pa_mainloop* m;
};

pa_threaded_mainloop* pa_threaded_mainloop_new(void) {
    pa_threaded_mainloop* m = pa_xnew(pa_threaded_mainloop, 1);
    m->m = pa_mainloop_new();
    return m;
}

void pa_threaded_mainloop_free(pa_threaded_mainloop* m) {
    pa_mainloop_free(m->m);
    pa_xfree(m);
}

pa_mainloop_api* pa_threaded_mainloop_get_api(pa_threaded_mainloop* m) {
    return &m->m->api;
}

void pa_threaded_mainloop_set_name(pa_threaded_mainloop *m, const char *name) DO_NOTHING

int pa_threaded_mainloop_start(pa_threaded_mainloop *m) RETURN_ZERO
void pa_threaded_mainloop_stop(pa_threaded_mainloop *m) DO_NOTHING // NOTE: No errors possible? LOL API

void pa_threaded_mainloop_lock(pa_threaded_mainloop *m) DO_NOTHING
void pa_threaded_mainloop_unlock(pa_threaded_mainloop *m) DO_NOTHING
void pa_threaded_mainloop_wait(pa_threaded_mainloop *m) DO_NOTHING
void pa_threaded_mainloop_signal(pa_threaded_mainloop *m, int wait_for_accept) DO_NOTHING
void pa_threaded_mainloop_accept(pa_threaded_mainloop *m) DO_NOTHING

int pa_threaded_mainloop_get_retval(pa_threaded_mainloop *m) RETURN_ZERO

// I fear that if I return either 0 or 1 from this somebody would f*ck something up.
// Let's abort.
int pa_threaded_mainloop_in_thread(pa_threaded_mainloop *m) DO_ABORT

#include <pulse/mainloop-signal.h>

int pa_signal_init(pa_mainloop_api *api) DO_ABORT
void pa_signal_done(void) DO_NOTHING

pa_signal_event* pa_signal_new(int sig, pa_signal_cb_t callback, void *userdata) DO_ABORT
void pa_signal_free(pa_signal_event *e) DO_NOTHING
void pa_signal_set_destroy(pa_signal_event *e, pa_signal_destroy_cb_t callback) DO_NOTHING

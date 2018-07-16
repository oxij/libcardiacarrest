/*
 * Copyright (C) 2018. Jan Malakhovski
 *
 * This file is a part of libcardiacarrest project.
 *
 * It may be used under the GNU LGPL version 2.1 or any later version.
 */

#ifndef NOPULSE__NOPULSE_H
#define NOPULSE__NOPULSE_H

#define _GNU_SOURCE
#include <pulse/def.h>

#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define CA_LIBRARY_VERSION "12.1.0"

#define DO_NOTHING { return; }

#define DO_ABORT { \
    fprintf(stderr, "libcardiacarrest: %s: not implemented\n", __func__); \
    abort(); \
}

#define RETURN_NULL { \
    return NULL; \
}

#define RETURN_FALSE { \
    return 0; \
}

#define RETURN_TRUE { \
    return 1; \
}

#define RETURN_MINUSONE { \
    return -1; \
}

#define RETURN_ZERO { \
    return 0; \
}

#define RETURN_ONE { \
    return 1; \
}

#define RETURN_DEST { \
    return dest; \
}

#define FAIL_NULL { \
    if (error) *error = -1; *error = -1; \
    return NULL; \
}

#define FAIL_NOTIMPL { \
    if (error) *error = -1; *error = -1; \
    return -1; \
}

#define RETURN_INVALID { \
    return "invalid"; \
}

#define RETURN_UNKNOWN { \
    return "unknown"; \
}

#define SNPRINTF_INVALID { \
    snprintf(s, l, "invalid"); \
    return s; \
}

#define SNPRINTF_UNKNOWN { \
    snprintf(s, l, "unknown"); \
    return s; \
}

#endif // NOPULSE__NOPULSE_H

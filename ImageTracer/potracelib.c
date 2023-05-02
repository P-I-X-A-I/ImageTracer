/* Copyright (C) 2001-2015 Peter Selinger.
   This file is part of Potrace. It is free software and it is covered
   by the GNU General Public License. See the file COPYING for details. */

#include <stdlib.h>
#include <string.h>

#include "potracelib.h"
#include "curve.h"
#include "decompose.h"
#include "trace.h"
#include "progress.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* default parameters */
static const potrace_param_t param_default = {
  2,                             /* turdsize */
  POTRACE_TURNPOLICY_MINORITY,   /* turnpolicy */
  1.0,                           /* alphamax */
  1,                             /* opticurve */
  0.2,                           /* opttolerance */
  {
    NULL,                        /* callback function */
    NULL,                        /* callback data */
    0.0, 1.0,                    /* progress range */
    0.0,                         /* granularity */
  },
};

/* Return a fresh copy of the set of default parameters, or NULL on
   failure with errno set. */
potrace_param_t *potrace_param_default(void) {
  potrace_param_t *p;

  p = (potrace_param_t *) malloc(sizeof(potrace_param_t));
  if (!p) {
    return NULL;
  }
  memcpy(p, &param_default, sizeof(potrace_param_t));
  return p;
}

/* On success, returns a Potrace state st with st->status ==
   POTRACE_STATUS_OK. On failure, returns NULL if no Potrace state
   could be created (with errno set), or returns an incomplete Potrace
   state (with st->status == POTRACE_STATUS_INCOMPLETE, and with errno
   set). Complete or incomplete Potrace state can be freed with
   potrace_state_free(). */
potrace_state_t *potrace_trace(const potrace_param_t *param, const potrace_bitmap_t *bm) {
  int r;
  path_t *plist = NULL;
  potrace_state_t *st;
  progress_t prog;
  progress_t subprog;
  
  /* prepare private progress bar state */
  prog.callback = param->progress.callback;
  prog.data = param->progress.data;
  prog.min = param->progress.min;
  prog.max = param->progress.max;
  prog.epsilon = param->progress.epsilon;
  prog.d_prev = param->progress.min;

  /* allocate state object */
  st = (potrace_state_t *)malloc(sizeof(potrace_state_t));
  if (!st) {
    return NULL;
  }

  progress_subrange_start(0.0, 0.1, &prog, &subprog);

  /* process the image */
  r = bm_to_pathlist(bm, &plist, param, &subprog);
  if (r) {
    free(st);
    return NULL;
  }

  st->status = POTRACE_STATUS_OK;
  st->plist = plist;
  st->priv = NULL;  /* private state currently unused */

  progress_subrange_end(&prog, &subprog);

  progress_subrange_start(0.1, 1.0, &prog, &subprog);

  /* partial success. */
  r = process_path(plist, param, &subprog);
  if (r) {
    st->status = POTRACE_STATUS_INCOMPLETE;
  }

  progress_subrange_end(&prog, &subprog);

  return st;
}

/* free a Potrace state, without disturbing errno. */
void potrace_state_free(potrace_state_t *st) {
  pathlist_free(st->plist);
  free(st);
}

/* free a parameter list, without disturbing errno. */
void potrace_param_free(potrace_param_t *p) {
  free(p);
}

char *potrace_version(void) {
  return "potracelib ???";
}






static void progress_subrange_start(double a, double b, const progress_t *prog, progress_t *sub) {
  double min, max;

  if (prog == NULL || prog->callback == NULL) {
    sub->callback = NULL;
    return;
  }

  min = prog->min * (1-a) + prog->max * a;
  max = prog->min * (1-b) + prog->max * b;

  if (max - min < prog->epsilon) {
    sub->callback = NULL;    /* no further progress info in subrange */
    sub->b = b;
    return;
  }
  sub->callback = prog->callback;
  sub->data = prog->data;
  sub->epsilon = prog->epsilon;
  sub->min = min;
  sub->max = max;
  sub->d_prev = prog->d_prev;
  return;
}

static void progress_subrange_end(progress_t *prog, progress_t *sub) {
  if (prog != NULL && prog->callback != NULL) {
    if (sub->callback == NULL) {
      progress_update(sub->b, prog);
    } else {
      prog->d_prev = sub->d_prev;
    }
  }    
}



static void progress_update(double d, progress_t *prog) {
  double d_scaled;

  if (prog != NULL && prog->callback != NULL) {
    d_scaled = prog->min * (1-d) + prog->max * d;
    if (d == 1.0 || d_scaled >= prog->d_prev + prog->epsilon) {
      prog->callback(prog->min * (1-d) + prog->max * d, prog->data);
      prog->d_prev = d_scaled;
    }
  }
}

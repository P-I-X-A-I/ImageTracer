#include "bitmap.h"

void bm_free(potrace_bitmap_t *bm) {
  if (bm) {
    free(bm->map);
  }
  free(bm);
}



potrace_bitmap_t *bm_new(int w, int h) {
  potrace_bitmap_t *bm;
  int dy = w == 0 ? 0 : (w - 1) / BM_WORDBITS + 1;
  ssize_t size = (ssize_t)dy * (ssize_t)h * (ssize_t)BM_WORDSIZE;

  /* check for overflow error */
  if (size < 0 || size / h / dy != BM_WORDSIZE) {
    errno = ENOMEM;
    return NULL;
  }

  bm = (potrace_bitmap_t *) malloc(sizeof(potrace_bitmap_t));
  if (!bm) {
    return NULL;
  }
  bm->w = w;
  bm->h = h;
  bm->dy = dy;
  bm->map = (potrace_word *) malloc(size);
  if (!bm->map) {
    free(bm);
    return NULL;
  }
  return bm;
}




void bm_clear(potrace_bitmap_t *bm, int c) {
  
  ssize_t size = (ssize_t)bm->dy * (ssize_t)bm->h * (ssize_t)BM_WORDSIZE;
  memset(bm->map, c ? -1 : 0, size);
}

potrace_bitmap_t *bm_dup(const potrace_bitmap_t *bm) {
  potrace_bitmap_t *bm1 = bm_new(bm->w, bm->h);
  ssize_t size = (ssize_t)bm->dy * (ssize_t)bm->h * (ssize_t)BM_WORDSIZE;
  if (!bm1) {
    return NULL;
  }
  memcpy(bm1->map, bm->map, size);
  return bm1;
}


void bm_invert(potrace_bitmap_t *bm) {
  ssize_t i;
  ssize_t size = (ssize_t)bm->dy * (ssize_t)bm->h;

  for (i = 0; i < size; i++) {
    bm->map[i] ^= BM_ALLBITS;
  }
}
#include "smvf_memory.h"
#include "internal.h"

/* 실제 구조. 공개 헤더에는 없다. */
typedef struct {
    uint64_t     base;
    uint64_t     n_words;
    smvf_word_t *storage;
} mem_impl_t;

/* 불투명 저장소가 실제 구조를 담을 수 있는지 컴파일 타임에 확인한다.
 * 나중에 필드를 추가하다 넘치면 여기서 빌드가 깨진다. */
_Static_assert (sizeof (mem_impl_t) <= sizeof (smvf_memory_t),
                "opaque storage too small for mem_impl_t");
_Static_assert (_Alignof (mem_impl_t) <= _Alignof (smvf_memory_t),
                "opaque storage alignment insufficient");

/* 접근 폭과 워드 크기가 어긋나면 주소 계산이 전부 틀어진다. */
_Static_assert (sizeof (smvf_word_t) == SMVF_ACCESS_WIDTH_BYTES,
                "smvf_word_t size must match access width");

static mem_impl_t *
impl (smvf_memory_t *m)
{
    return (mem_impl_t *)m;
}

static const mem_impl_t *
impl_const (const smvf_memory_t *m)
{
    return (const mem_impl_t *)m;
}

/* 주소 -> 인덱스.
 *
 * base를 더해 비교하지 않고 빼서 비교한다.
 * 더하기로 검사하면 base + n*8 이 64비트를 넘길 때 랩어라운드가 일어나
 * 범위 검사가 통째로 무력화된다.
 */
static smvf_rc_t
addr_to_idx (const mem_impl_t *m, uint64_t addr, uint64_t *idx)
{
    uint64_t off;

    if (addr < m->base) {
        return SMVF_E_RANGE;
    }
    off = addr - m->base;

    if ((off % SMVF_ACCESS_WIDTH_BYTES) != 0u) {
        return SMVF_E_ALIGN;
    }
    off /= SMVF_ACCESS_WIDTH_BYTES;

    if (off >= m->n_words) {
        return SMVF_E_RANGE;
    }

    *idx = off;
    return SMVF_OK;
}

smvf_rc_t
smvf_mem_init (smvf_memory_t *mem, smvf_word_t *storage,
               uint64_t n_words, uint64_t base)
{
    mem_impl_t *m;
    uint64_t    i;

    if ((mem == NULL) || (storage == NULL)) {
        return SMVF_E_NULL;
    }
    if (n_words == 0u) {
        return SMVF_E_EMPTY;
    }
    if ((base % SMVF_ACCESS_WIDTH_BYTES) != 0u) {
        return SMVF_E_ALIGN;
    }

    m = impl (mem);
    m->base    = base;
    m->n_words = n_words;
    m->storage = storage;

    /* memset을 쓰지 않는다 — core는 표준 라이브러리 함수를 호출하지 않는다. */
    for (i = 0u; i < n_words; i++) {
        m->storage[i] = SMVF_POISON;
    }

    return SMVF_OK;
}

smvf_rc_t
smvf_mem_read (smvf_memory_t *mem, uint64_t addr, smvf_word_t *out)
{
    mem_impl_t *m;
    uint64_t    idx;
    smvf_rc_t   rc;

    if ((mem == NULL) || (out == NULL)) {
        return SMVF_E_NULL;
    }

    m  = impl (mem);
    rc = addr_to_idx (m, addr, &idx);
    if (rc != SMVF_OK) {
        return rc;
    }

    *out = m->storage[idx];
    return SMVF_OK;
}

smvf_rc_t
smvf_mem_write (smvf_memory_t *mem, uint64_t addr, smvf_word_t val)
{
    mem_impl_t *m;
    uint64_t    idx;
    smvf_rc_t   rc;

    if (mem == NULL) {
        return SMVF_E_NULL;
    }

    m  = impl (mem);
    rc = addr_to_idx (m, addr, &idx);
    if (rc != SMVF_OK) {
        return rc;
    }

    m->storage[idx] = val;
    return SMVF_OK;
}

smvf_rc_t
smvf_mem_span (const smvf_memory_t *mem, smvf_span_t *out)
{
    const mem_impl_t *m;

    if ((mem == NULL) || (out == NULL)) {
        return SMVF_E_NULL;
    }

    m = impl_const (mem);
    out->base      = m->base;
    out->len_bytes = m->n_words * SMVF_ACCESS_WIDTH_BYTES;

    return SMVF_OK;
}

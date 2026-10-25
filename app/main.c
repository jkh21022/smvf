/* M1.1 데모 — core 바깥. 출력은 여기서만 한다. */
#include <stdio.h>
#include "smvf_memory.h"

#define N_WORDS   16u
#define BASE_ADDR 0x80000000ull

static const char *
rc_name (smvf_rc_t rc)
{
    switch (rc) {
    case SMVF_OK:      return "OK";
    case SMVF_E_NULL:  return "E_NULL";
    case SMVF_E_ALIGN: return "E_ALIGN";
    case SMVF_E_RANGE: return "E_RANGE";
    case SMVF_E_EMPTY: return "E_EMPTY";
    default:           return "?";
    }
}

int
main (void)
{
    smvf_memory_t mem;
    smvf_word_t   storage[N_WORDS];
    smvf_span_t   span;
    smvf_word_t   v;
    smvf_rc_t     rc;

    rc = smvf_mem_init (&mem, storage, N_WORDS, BASE_ADDR);
    printf ("init                     : %s\n", rc_name (rc));

    rc = smvf_mem_span (&mem, &span);
    printf ("span                     : base=0x%016llx len=%llu bytes\n",
            (unsigned long long)span.base,
            (unsigned long long)span.len_bytes);

    /* 초기 내용은 0이 아니어야 한다 */
    rc = smvf_mem_read (&mem, BASE_ADDR, &v);
    printf ("read  @base (poison)     : %s 0x%016llx\n",
            rc_name (rc), (unsigned long long)v);

    /* 쓰고 되읽기 */
    rc = smvf_mem_write (&mem, BASE_ADDR + 8, 0xFFFFFFFFFFFFFFFFull);
    printf ("write @base+8            : %s\n", rc_name (rc));
    rc = smvf_mem_read (&mem, BASE_ADDR + 8, &v);
    printf ("read  @base+8            : %s 0x%016llx\n",
            rc_name (rc), (unsigned long long)v);

    /* 정렬 위반 */
    rc = smvf_mem_read (&mem, BASE_ADDR + 4, &v);
    printf ("read  @base+4 (misalign) : %s\n", rc_name (rc));

    /* 구간 위 */
    rc = smvf_mem_read (&mem, BASE_ADDR + (N_WORDS * 8), &v);
    printf ("read  @base+end (over)   : %s\n", rc_name (rc));

    /* 구간 아래 */
    rc = smvf_mem_read (&mem, BASE_ADDR - 8, &v);
    printf ("read  @base-8 (under)    : %s\n", rc_name (rc));

    /* base 정렬 위반 */
    rc = smvf_mem_init (&mem, storage, N_WORDS, BASE_ADDR + 4);
    printf ("init with bad base       : %s\n", rc_name (rc));

    return 0;
}

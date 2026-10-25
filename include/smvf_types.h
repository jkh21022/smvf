/* SMVF — 공통 어휘
 *
 * 이 헤더는 다른 SMVF 헤더에 의존하지 않는다.
 * 표준 헤더도 타입 정의만 가져온다 (함수 호출 없음).
 */
#ifndef SMVF_TYPES_H
#define SMVF_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* 접근 폭. v1 고정.
 *
 * TODO(v2): 파라미터화한다.
 *   - DQ x4/x8/x16, burst length 대응
 *   - 정렬(alignment)을 접근 폭과 분리할 것.
 *     버스트나 캐시 라인 때문에 폭은 8인데 정렬은 64가 필요할 수 있다.
 */
#define SMVF_ACCESS_WIDTH_BYTES  8u

/* 메모리 한 워드. 접근 폭과 크기가 같아야 한다. */
typedef uint64_t smvf_word_t;

/* 논리 시간 — 수행한 연산의 누적 횟수 */
typedef uint64_t smvf_tick_t;

/* 프레임워크 오류.
 *
 * 검사 대상의 결함과는 다른 축이다.
 * 이 값이 SMVF_OK가 아니면 "도구가 제대로 돌지 못했다"는 뜻이고,
 * 대상이 멀쩡한지 여부는 별도로 판단한다.
 */
typedef enum {
    SMVF_OK = 0,
    SMVF_E_NULL,     /* 널 포인터 */
    SMVF_E_ALIGN,    /* 주소 또는 길이가 접근 폭의 배수가 아님 */
    SMVF_E_RANGE,    /* 구간 밖 접근 */
    SMVF_E_EMPTY     /* 워드 0개 */
} smvf_rc_t;

/* 검사 대상 구간.
 *
 * 인덱스가 아니라 물리 주소로 표현한다.
 * base는 0이 아니어도 되며, 실제로 0이 아닌 값을 쓴다.
 */
typedef struct {
    uint64_t base;       /* 첫 바이트 주소 */
    uint64_t len_bytes;  /* 구간 길이 */
} smvf_span_t;

#endif /* SMVF_TYPES_H */

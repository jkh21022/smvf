/* SMVF — 메모리 모델 (무정책 저장소)
 *
 * 이 모듈은 값을 담고 자기 주소 범위를 알 뿐이다.
 * 알고리즘도, 결함도, 판정도 모른다.
 */
#ifndef SMVF_MEMORY_H
#define SMVF_MEMORY_H

#include "smvf_types.h"

/* 불투명 저장소.
 *
 * 호출자는 이 타입을 스택에 잡을 수 있지만 내부를 해석할 수 없다.
 * 실제 구조는 memory.c 안에만 있고, 크기가 모자라면 컴파일이 실패한다.
 *
 * 저장소 내용을 밖에서 직접 들여다볼 수 없어야, 검출자가 정답을
 * 조회하는 구조가 생기지 않는다.
 */
typedef struct {
    uint64_t _opaque[8];
} smvf_memory_t;

/* 초기화.
 *
 * storage  : 호출자가 잡은 워드 배열 (힙을 쓰지 않는다)
 * n_words  : 배열 길이
 * base     : 첫 워드의 물리 주소. SMVF_ACCESS_WIDTH_BYTES의 배수여야 한다.
 *
 * 저장소는 불확정 패턴으로 채워진다. 0이 아니다.
 */
smvf_rc_t smvf_mem_init (smvf_memory_t *mem,
                         smvf_word_t   *storage,
                         uint64_t       n_words,
                         uint64_t       base);

/* 읽기 / 쓰기.
 *
 * addr는 바이트 주소이며 접근 폭으로 정렬되어 있어야 한다.
 *
 * read가 const를 받지 않는 이유: 실제 DRAM의 읽기는 파괴적이고,
 * 이후 버전에서 접근 경로가 내부 상태를 갱신하게 된다.
 */
smvf_rc_t smvf_mem_read  (smvf_memory_t *mem, uint64_t addr, smvf_word_t *out);
smvf_rc_t smvf_mem_write (smvf_memory_t *mem, uint64_t addr, smvf_word_t  val);

/* 구간 조회 — 순수 정보 */
smvf_rc_t smvf_mem_span (const smvf_memory_t *mem, smvf_span_t *out);

#endif /* SMVF_MEMORY_H */

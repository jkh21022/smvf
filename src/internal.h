/* SMVF — 내부 공용. 공개 헤더가 아니다 (include/ 에 두지 않는다). */
#ifndef SMVF_INTERNAL_H
#define SMVF_INTERNAL_H

#include "smvf_types.h"

/* 초기 저장소 패턴.
 *
 * 0으로 채우지 않는 이유: 실제 메모리의 전원 인가 직후 내용은 불확정이다.
 * 0으로 두면 첫 element가 w0일 때, 그 element가 아무 일도 안 해도
 * 결과가 같아져서 고장이 드러나지 않는다.
 */
#define SMVF_POISON  0xA5A5A5A5A5A5A5A5ull

/* 인덱스 -> 주소 변환.
 *
 * 이 프로젝트에서 유일한 변환 지점이다.
 * 여기저기 흩어지면 v2에서 접근 폭을 파라미터화할 때 전부 찾아다녀야 한다.
 *
 * 상태가 없으므로 헤더의 static inline으로 둬도 안전하다.
 */
static inline uint64_t
smvf_idx_to_addr (uint64_t base, uint64_t idx)
{
    return base + idx * SMVF_ACCESS_WIDTH_BYTES;
}

#endif /* SMVF_INTERNAL_H */

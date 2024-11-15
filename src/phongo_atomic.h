/*
 * Copyright 2024-present MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Note: this file was derived from libbson's bson-atomic.h */

#ifndef PHONGO_ATOMIC_H
#define PHONGO_ATOMIC_H

#include "bson/bson.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

enum phongo_memory_order {
	phongo_memory_order_seq_cst,
	phongo_memory_order_acquire,
	phongo_memory_order_release,
	phongo_memory_order_relaxed,
	phongo_memory_order_acq_rel,
	phongo_memory_order_consume,
};

#if defined(_M_ARM) /* MSVC memorder atomics are only avail on ARM */
#define MSVC_MEMORDER_SUFFIX(X) X
#else
#define MSVC_MEMORDER_SUFFIX(X)
#endif

#if defined(USE_LEGACY_GCC_ATOMICS) || (!defined(__clang__) && __GNUC__ == 4) || defined(__xlC__)
#define PHONGO_USE_LEGACY_GCC_ATOMICS
#else
#undef PHONGO_USE_LEGACY_GCC_ATOMICS
#endif

/* Not all GCC-like compilers support the current __atomic built-ins.  Older
 * GCC (pre-5) used different built-ins named with the __sync prefix.  When
 * compiling with such older GCC versions, it is necessary to use the applicable
 * functions, which requires redefining BSON_IF_GNU_LIKE and defining the
 * additional PHONGO_IF_GNU_LEGACY_ATOMICS macro here. */
#ifdef PHONGO_USE_LEGACY_GCC_ATOMICS
#undef BSON_IF_GNU_LIKE
#define BSON_IF_GNU_LIKE(...)
#define BSON_IF_MSVC(...)
#define PHONGO_IF_GNU_LEGACY_ATOMICS(...) __VA_ARGS__
#else
#define PHONGO_IF_GNU_LEGACY_ATOMICS(...)
#endif

/* CDRIVER-4229 zSeries with gcc 4.8.4 produces illegal instructions for int and
 * int32 atomic intrinsics. */
#if defined(__s390__) || defined(__s390x__) || defined(__zarch__)
#define PHONGO_EMULATE_INT32
#endif

/* clang-format off */

#define DEF_ATOMIC_OP(MSVC_Intrinsic, GNU_Intrinsic, GNU_Legacy_Intrinsic, Order, ...)                 \
	do {                                                                                                \
		switch (Order) {                                                                                 \
			case phongo_memory_order_acq_rel:                                                             \
				BSON_IF_MSVC(return MSVC_Intrinsic(__VA_ARGS__);)                                          \
				BSON_IF_GNU_LIKE(return GNU_Intrinsic(__VA_ARGS__, __ATOMIC_ACQ_REL);)                     \
				PHONGO_IF_GNU_LEGACY_ATOMICS(return GNU_Legacy_Intrinsic(__VA_ARGS__);)                    \
			case phongo_memory_order_seq_cst:                                                             \
				BSON_IF_MSVC(return MSVC_Intrinsic(__VA_ARGS__);)                                          \
				BSON_IF_GNU_LIKE(return GNU_Intrinsic(__VA_ARGS__, __ATOMIC_SEQ_CST);)                     \
				PHONGO_IF_GNU_LEGACY_ATOMICS(return GNU_Legacy_Intrinsic(__VA_ARGS__);)                    \
			case phongo_memory_order_acquire:                                                             \
				BSON_IF_MSVC(return BSON_CONCAT(MSVC_Intrinsic, MSVC_MEMORDER_SUFFIX(_acq))(__VA_ARGS__);) \
				BSON_IF_GNU_LIKE(return GNU_Intrinsic(__VA_ARGS__, __ATOMIC_ACQUIRE);)                     \
				PHONGO_IF_GNU_LEGACY_ATOMICS(return GNU_Legacy_Intrinsic(__VA_ARGS__);)                    \
			case phongo_memory_order_consume:                                                             \
				BSON_IF_MSVC(return BSON_CONCAT(MSVC_Intrinsic, MSVC_MEMORDER_SUFFIX(_acq))(__VA_ARGS__);) \
				BSON_IF_GNU_LIKE(return GNU_Intrinsic(__VA_ARGS__, __ATOMIC_CONSUME);)                     \
				PHONGO_IF_GNU_LEGACY_ATOMICS(return GNU_Legacy_Intrinsic(__VA_ARGS__);)                    \
			case phongo_memory_order_release:                                                             \
				BSON_IF_MSVC(return BSON_CONCAT(MSVC_Intrinsic, MSVC_MEMORDER_SUFFIX(_rel))(__VA_ARGS__);) \
				BSON_IF_GNU_LIKE(return GNU_Intrinsic(__VA_ARGS__, __ATOMIC_RELEASE);)                     \
				PHONGO_IF_GNU_LEGACY_ATOMICS(return GNU_Legacy_Intrinsic(__VA_ARGS__);)                    \
			case phongo_memory_order_relaxed:                                                             \
				BSON_IF_MSVC(return BSON_CONCAT(MSVC_Intrinsic, MSVC_MEMORDER_SUFFIX(_nf))(__VA_ARGS__);)  \
				BSON_IF_GNU_LIKE(return GNU_Intrinsic(__VA_ARGS__, __ATOMIC_RELAXED);)                     \
				PHONGO_IF_GNU_LEGACY_ATOMICS(return GNU_Legacy_Intrinsic(__VA_ARGS__);)                    \
			default:                                                                                      \
				BSON_UNREACHABLE("Invalid phongo_memory_order value");                                     \
		}                                                                                                \
	} while (0)

#define DEF_ATOMIC_CMPEXCH_STRONG(VCSuffix1, VCSuffix2, GNU_MemOrder, Ptr, ExpectActualVar, NewValue)      \
	do {                                                                                                    \
		BSON_IF_MSVC(ExpectActualVar = BSON_CONCAT3(_InterlockedCompareExchange, VCSuffix1, VCSuffix2)(      \
						 Ptr, NewValue, ExpectActualVar);)                                                       \
		BSON_IF_GNU_LIKE((void) __atomic_compare_exchange_n(Ptr,                                             \
															&ExpectActualVar,                                             \
															NewValue,                                                     \
															false, /* Not weak */                                         \
															GNU_MemOrder,                                                 \
															GNU_MemOrder);)                                               \
		PHONGO_IF_GNU_LEGACY_ATOMICS(__typeof__(ExpectActualVar) _val;                                       \
									 _val            = __sync_val_compare_and_swap(Ptr, ExpectActualVar, NewValue); \
									 ExpectActualVar = _val;)                                                       \
	} while (0)

#define DEF_ATOMIC_CMPEXCH_WEAK(VCSuffix1, VCSuffix2, GNU_MemOrder, Ptr, ExpectActualVar, NewValue)        \
	do {                                                                                                    \
		BSON_IF_MSVC(ExpectActualVar = BSON_CONCAT3(_InterlockedCompareExchange, VCSuffix1, VCSuffix2)(      \
						 Ptr, NewValue, ExpectActualVar);)                                                       \
		BSON_IF_GNU_LIKE((void) __atomic_compare_exchange_n(Ptr,                                             \
															&ExpectActualVar,                                             \
															NewValue,                                                     \
															true, /* Yes weak */                                          \
															GNU_MemOrder,                                                 \
															GNU_MemOrder);)                                               \
		PHONGO_IF_GNU_LEGACY_ATOMICS(__typeof__(ExpectActualVar) _val;                                       \
									 _val            = __sync_val_compare_and_swap(Ptr, ExpectActualVar, NewValue); \
									 ExpectActualVar = _val;)                                                       \
	} while (0)

#define DECL_ATOMIC_INTEGRAL(NamePart, Type, VCIntrinSuffix)                                                            \
	static BSON_INLINE Type phongo_atomic_##NamePart##_fetch_add(                                                        \
		Type volatile* a, Type addend, enum phongo_memory_order ord)                                                      \
	{                                                                                                                    \
		DEF_ATOMIC_OP(BSON_CONCAT(_InterlockedExchangeAdd, VCIntrinSuffix),                                               \
					  __atomic_fetch_add,                                                                                    \
					  __sync_fetch_and_add,                                                                                  \
					  ord,                                                                                                   \
					  a,                                                                                                     \
					  addend);                                                                                               \
	}                                                                                                                    \
                                                                                                                        \
	static BSON_INLINE Type phongo_atomic_##NamePart##_fetch_sub(                                                        \
		Type volatile* a, Type subtrahend, enum phongo_memory_order ord)                                                  \
	{                                                                                                                    \
		/* MSVC doesn't have a subtract intrinsic, so just reuse addition    */                                           \
		BSON_IF_MSVC(return phongo_atomic_##NamePart##_fetch_add(a, -subtrahend, ord);)                                   \
		BSON_IF_GNU_LIKE(DEF_ATOMIC_OP(~, __atomic_fetch_sub, ~, ord, a, subtrahend);)                                    \
		PHONGO_IF_GNU_LEGACY_ATOMICS(DEF_ATOMIC_OP(~, ~, __sync_fetch_and_sub, ord, a, subtrahend);)                      \
	}                                                                                                                    \
                                                                                                                        \
	static BSON_INLINE Type phongo_atomic_##NamePart##_fetch(Type volatile const* a, enum phongo_memory_order order)     \
	{                                                                                                                    \
		/* MSVC doesn't have a load intrinsic, so just add zero */                                                        \
		BSON_IF_MSVC(return phongo_atomic_##NamePart##_fetch_add((Type volatile*) a, 0, order);)                          \
		/* GNU doesn't want RELEASE order for the fetch operation, so we can't                                            \
       * just use DEF_ATOMIC_OP. */                                                                                     \
		BSON_IF_GNU_LIKE(switch (order) {                                                                                 \
			case phongo_memory_order_release: /* Fall back to seqcst */                                                    \
			case phongo_memory_order_acq_rel: /* Fall back to seqcst */                                                    \
			case phongo_memory_order_seq_cst:                                                                              \
				return __atomic_load_n(a, __ATOMIC_SEQ_CST);                                                                \
			case phongo_memory_order_acquire:                                                                              \
				return __atomic_load_n(a, __ATOMIC_ACQUIRE);                                                                \
			case phongo_memory_order_consume:                                                                              \
				return __atomic_load_n(a, __ATOMIC_CONSUME);                                                                \
			case phongo_memory_order_relaxed:                                                                              \
				return __atomic_load_n(a, __ATOMIC_RELAXED);                                                                \
			default:                                                                                                       \
				BSON_UNREACHABLE("Invalid phongo_memory_order value");                                                      \
		})                                                                                                                \
		PHONGO_IF_GNU_LEGACY_ATOMICS({                                                                                    \
			BSON_UNUSED(order);                                                                                            \
			__sync_synchronize();                                                                                          \
			return *a;                                                                                                     \
		})                                                                                                                \
	}                                                                                                                    \
                                                                                                                        \
	static BSON_INLINE Type phongo_atomic_##NamePart##_exchange(                                                         \
		Type volatile* a, Type value, enum phongo_memory_order ord)                                                       \
	{                                                                                                                    \
		BSON_IF_MSVC(DEF_ATOMIC_OP(BSON_CONCAT(_InterlockedExchange, VCIntrinSuffix), ~, ~, ord, a, value);)              \
		/* GNU doesn't want CONSUME order for the exchange operation, so we                                               \
       * cannot use DEF_ATOMIC_OP. */                                                                                   \
		BSON_IF_GNU_LIKE(switch (ord) {                                                                                   \
			case phongo_memory_order_acq_rel:                                                                              \
				return __atomic_exchange_n(a, value, __ATOMIC_ACQ_REL);                                                     \
			case phongo_memory_order_release:                                                                              \
				return __atomic_exchange_n(a, value, __ATOMIC_RELEASE);                                                     \
			case phongo_memory_order_seq_cst:                                                                              \
				return __atomic_exchange_n(a, value, __ATOMIC_SEQ_CST);                                                     \
			case phongo_memory_order_consume: /* Fall back to acquire */                                                   \
			case phongo_memory_order_acquire:                                                                              \
				return __atomic_exchange_n(a, value, __ATOMIC_ACQUIRE);                                                     \
			case phongo_memory_order_relaxed:                                                                              \
				return __atomic_exchange_n(a, value, __ATOMIC_RELAXED);                                                     \
			default:                                                                                                       \
				BSON_UNREACHABLE("Invalid phongo_memory_order value");                                                      \
		})                                                                                                                \
		PHONGO_IF_GNU_LEGACY_ATOMICS(BSON_UNUSED(ord); return __sync_val_compare_and_swap(a, *a, value);)                 \
	}                                                                                                                    \
                                                                                                                        \
	static BSON_INLINE Type phongo_atomic_##NamePart##_compare_exchange_strong(                                          \
		Type volatile* a, Type expect, Type new_value, enum phongo_memory_order ord)                                      \
	{                                                                                                                    \
		Type actual = expect;                                                                                             \
		switch (ord) {                                                                                                    \
			case phongo_memory_order_release:                                                                              \
			case phongo_memory_order_acq_rel:                                                                              \
			case phongo_memory_order_seq_cst:                                                                              \
				DEF_ATOMIC_CMPEXCH_STRONG(VCIntrinSuffix, , __ATOMIC_SEQ_CST, a, actual, new_value);                        \
				break;                                                                                                      \
			case phongo_memory_order_acquire:                                                                              \
				DEF_ATOMIC_CMPEXCH_STRONG(                                                                                  \
					VCIntrinSuffix, MSVC_MEMORDER_SUFFIX(_acq), __ATOMIC_ACQUIRE, a, actual, new_value);                     \
				break;                                                                                                      \
			case phongo_memory_order_consume:                                                                              \
				DEF_ATOMIC_CMPEXCH_STRONG(                                                                                  \
					VCIntrinSuffix, MSVC_MEMORDER_SUFFIX(_acq), __ATOMIC_CONSUME, a, actual, new_value);                     \
				break;                                                                                                      \
			case phongo_memory_order_relaxed:                                                                              \
				DEF_ATOMIC_CMPEXCH_STRONG(                                                                                  \
					VCIntrinSuffix, MSVC_MEMORDER_SUFFIX(_nf), __ATOMIC_RELAXED, a, actual, new_value);                      \
				break;                                                                                                      \
			default:                                                                                                       \
				BSON_UNREACHABLE("Invalid phongo_memory_order value");                                                      \
		}                                                                                                                 \
		return actual;                                                                                                    \
	}                                                                                                                    \
                                                                                                                        \
	static BSON_INLINE Type phongo_atomic_##NamePart##_compare_exchange_weak(                                            \
		Type volatile* a, Type expect, Type new_value, enum phongo_memory_order ord)                                      \
	{                                                                                                                    \
		Type actual = expect;                                                                                             \
		switch (ord) {                                                                                                    \
			case phongo_memory_order_release:                                                                              \
			case phongo_memory_order_acq_rel:                                                                              \
			case phongo_memory_order_seq_cst:                                                                              \
				DEF_ATOMIC_CMPEXCH_WEAK(VCIntrinSuffix, , __ATOMIC_SEQ_CST, a, actual, new_value);                          \
				break;                                                                                                      \
			case phongo_memory_order_acquire:                                                                              \
				DEF_ATOMIC_CMPEXCH_WEAK(                                                                                    \
					VCIntrinSuffix, MSVC_MEMORDER_SUFFIX(_acq), __ATOMIC_ACQUIRE, a, actual, new_value);                     \
				break;                                                                                                      \
			case phongo_memory_order_consume:                                                                              \
				DEF_ATOMIC_CMPEXCH_WEAK(                                                                                    \
					VCIntrinSuffix, MSVC_MEMORDER_SUFFIX(_acq), __ATOMIC_CONSUME, a, actual, new_value);                     \
				break;                                                                                                      \
			case phongo_memory_order_relaxed:                                                                              \
				DEF_ATOMIC_CMPEXCH_WEAK(VCIntrinSuffix, MSVC_MEMORDER_SUFFIX(_nf), __ATOMIC_RELAXED, a, actual, new_value); \
				break;                                                                                                      \
			default:                                                                                                       \
				BSON_UNREACHABLE("Invalid phongo_memory_order value");                                                      \
		}                                                                                                                 \
		return actual;                                                                                                    \
	}

/* clang-format on */

#define DECL_ATOMIC_STDINT(Name, VCSuffix) DECL_ATOMIC_INTEGRAL(Name, Name##_t, VCSuffix)

#if defined(_MSC_VER) || defined(PHONGO_USE_LEGACY_GCC_ATOMICS)
/* MSVC and GCC require built-in types (not typedefs) for their atomic
 * intrinsics. */
#if defined(_MSC_VER)
#define DECL_ATOMIC_INTEGRAL_INT8 char
#define DECL_ATOMIC_INTEGRAL_INT32 long
#else
#define DECL_ATOMIC_INTEGRAL_INT8 signed char
#define DECL_ATOMIC_INTEGRAL_INT32 int
#endif
DECL_ATOMIC_INTEGRAL(int8, DECL_ATOMIC_INTEGRAL_INT8, 8)
#if !defined(PHONGO_EMULATE_INT32)
DECL_ATOMIC_INTEGRAL(int32, DECL_ATOMIC_INTEGRAL_INT32, )
#endif
#else
/* Other compilers that we support provide generic intrinsics */
DECL_ATOMIC_STDINT(int8, 8)
#if !defined(PHONGO_EMULATE_INT32)
DECL_ATOMIC_STDINT(int32, )
#endif
#endif

#ifndef DECL_ATOMIC_INTEGRAL_INT32
#define DECL_ATOMIC_INTEGRAL_INT32 int32_t
#endif

int32_t _phongo_emul_atomic_int32_fetch_add(int32_t volatile* val, int32_t v, enum phongo_memory_order);

int32_t _phongo_emul_atomic_int32_exchange(int32_t volatile* val, int32_t v, enum phongo_memory_order);

int32_t _phongo_emul_atomic_int32_compare_exchange_strong(
	int32_t volatile* val,
	int32_t           expect_value,
	int32_t           new_value,
	enum phongo_memory_order);

int32_t _phongo_emul_atomic_int32_compare_exchange_weak(
	int32_t volatile* val,
	int32_t           expect_value,
	int32_t           new_value,
	enum phongo_memory_order);

#if defined(PHONGO_EMULATE_INT32)
static BSON_INLINE int32_t phongo_atomic_int32_fetch(const int32_t volatile* val, enum phongo_memory_order order)
{
	return _phongo_emul_atomic_int32_fetch_add((int32_t volatile*) val, 0, order);
}

static BSON_INLINE int32_t phongo_atomic_int32_fetch_add(int32_t volatile* val, int32_t v, enum phongo_memory_order order)
{
	return _phongo_emul_atomic_int32_fetch_add(val, v, order);
}

static BSON_INLINE int32_t phongo_atomic_int32_fetch_sub(int32_t volatile* val, int32_t v, enum phongo_memory_order order)
{
	return _phongo_emul_atomic_int32_fetch_add(val, -v, order);
}

static BSON_INLINE int32_t phongo_atomic_int32_exchange(int32_t volatile* val, int32_t v, enum phongo_memory_order order)
{
	return _phongo_emul_atomic_int32_exchange(val, v, order);
}

static BSON_INLINE int32_t phongo_atomic_int32_compare_exchange_strong(
	int32_t volatile*        val,
	int32_t                  expect_value,
	int32_t                  new_value,
	enum phongo_memory_order order)
{
	return _phongo_emul_atomic_int32_compare_exchange_strong(val, expect_value, new_value, order);
}

static BSON_INLINE int32_t phongo_atomic_int32_compare_exchange_weak(
	int32_t volatile*        val,
	int32_t                  expect_value,
	int32_t                  new_value,
	enum phongo_memory_order order)
{
	return _phongo_emul_atomic_int32_compare_exchange_weak(val, expect_value, new_value, order);
}
#endif /* PHONGO_EMULATE_INT32 */

#undef DECL_ATOMIC_STDINT
#undef DECL_ATOMIC_INTEGRAL
#undef DEF_ATOMIC_OP
#undef DEF_ATOMIC_CMPEXCH_STRONG
#undef DEF_ATOMIC_CMPEXCH_WEAK
#undef MSVC_MEMORDER_SUFFIX

/* TODO: Redefine BSON_IF_MSVC is necessary (see: CDRIVER-5813) */
#ifdef PHONGO_USE_LEGACY_GCC_ATOMICS
#undef BSON_IF_GNU_LIKE
#define BSON_IF_GNU_LIKE(...) __VA_ARGS__
#endif
#undef PHONGO_IF_GNU_LEGACY_ATOMICS
#undef PHONGO_USE_LEGACY_GCC_ATOMICS

#undef PHONGO_EMULATE_INT32

#endif /* PHONGO_ATOMIC_H */

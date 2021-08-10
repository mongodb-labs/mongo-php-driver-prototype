/*
 * Copyright 2015-present MongoDB, Inc.
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

#ifndef PHONGO_COMPAT_H
#define PHONGO_COMPAT_H

#include <php.h>
#include <Zend/zend_string.h>
#include <Zend/zend_portability.h>

#ifdef PHP_WIN32
#include "config.w32.h"
#else
#include <php_config.h>
#endif

#ifndef PHP_FE_END
#define PHP_FE_END       \
	{                    \
		NULL, NULL, NULL \
	}
#endif

#ifndef HASH_KEY_NON_EXISTENT
#define HASH_KEY_NON_EXISTENT HASH_KEY_NON_EXISTANT
#endif

#if defined(__GNUC__)
#define ARG_UNUSED __attribute__((unused))
#else
#define ARG_UNUSED
#endif

#if defined(__GNUC__)
#define PHONGO_GNUC_CHECK_VERSION(major, minor) \
	((__GNUC__ > (major)) ||                    \
	 ((__GNUC__ == (major)) && (__GNUC_MINOR__ >= (minor))))
#else
#define PHONGO_GNUC_CHECK_VERSION(major, minor) 0
#endif

#if PHONGO_GNUC_CHECK_VERSION(7, 0)
#define PHONGO_BREAK_INTENTIONALLY_MISSING __attribute__((fallthrough));
#elif defined(__clang__) && __clang_major__ >= 12
#define PHONGO_BREAK_INTENTIONALLY_MISSING __attribute__((fallthrough));
#else
#define PHONGO_BREAK_INTENTIONALLY_MISSING
#endif

#if PHP_VERSION_ID >= 80000
#define PHONGO_COMPAT_OBJ_P(val) Z_OBJ_P(val)
#define phongo_compat_object_handler_type zend_object
#define PHONGO_COMPAT_GET_OBJ(val) val
#define PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(type) php_phongo_handler_##type.compare = php_phongo_##type##_compare_objects;
#else /* PHP_VERSION_ID < 80000 */
#define PHONGO_COMPAT_OBJ_P(val) val
#define phongo_compat_object_handler_type zval
#define PHONGO_COMPAT_GET_OBJ(val) Z_OBJ_P(val)
#define PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(type) php_phongo_handler_##type.compare_objects = php_phongo_##type##_compare_objects;
#define ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2)
#endif /* PHP_VERSION_ID >= 80000 */

#if SIZEOF_ZEND_LONG == 8
#define PHONGO_LONG_FORMAT PRId64
#elif SIZEOF_ZEND_LONG == 4
#define PHONGO_LONG_FORMAT PRId32
#else
#error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif
#define PHONGO_ALLOC_OBJECT_T(_obj_t, _class_type) (_obj_t*) ecalloc(1, sizeof(_obj_t) + zend_object_properties_size(_class_type))
#define ADD_ASSOC_STRING(_zv, _key, _value) add_assoc_string_ex(_zv, ZEND_STRL(_key), (char*) (_value));
#define ADD_ASSOC_STRINGL(_zv, _key, _value, _len) add_assoc_stringl_ex(_zv, ZEND_STRL(_key), (char*) (_value), _len);
#define ADD_ASSOC_STRING_EX(_zv, _key, _key_len, _value, _value_len) add_assoc_stringl_ex(_zv, _key, _key_len, (char*) (_value), _value_len);
#define ADD_ASSOC_LONG_EX(_zv, _key, _value) add_assoc_long_ex(_zv, ZEND_STRL(_key), _value);
#define ADD_ASSOC_ZVAL_EX(_zv, _key, _value) add_assoc_zval_ex(_zv, ZEND_STRL(_key), _value);
#define ADD_ASSOC_ZVAL(_zv, _key, _value) add_assoc_zval(_zv, _key, _value);
#define ADD_ASSOC_NULL_EX(_zv, _key) add_assoc_null_ex(_zv, ZEND_STRL(_key));
#define ADD_ASSOC_BOOL_EX(_zv, _key, _value) add_assoc_bool_ex(_zv, ZEND_STRL(_key), _value);
#define ZVAL_INT64_STRING(_zv, _value)                              \
	do {                                                            \
		char tmp[24];                                               \
		int  tmp_len;                                               \
		tmp_len = snprintf(tmp, sizeof(tmp), "%" PRId64, (_value)); \
		ZVAL_STRINGL((_zv), tmp, tmp_len);                          \
	} while (0)
#define ADD_ASSOC_INT64_AS_STRING(_zv, _key, _value) \
	do {                                             \
		zval z_int;                                  \
		ZVAL_INT64_STRING(&z_int, (_value));         \
		ADD_ASSOC_ZVAL_EX((_zv), (_key), &z_int);    \
	} while (0)
#define ADD_NEXT_INDEX_STRINGL(_zv, _value, _len) add_next_index_stringl(_zv, _value, _len);
#define PHONGO_RETVAL_SMART_STR(val) RETVAL_STRINGL(ZSTR_VAL((val).s), ZSTR_LEN((val).s));
#define ZVAL_STATIC_INIT \
	{                    \
		{                \
			0            \
		}                \
	}

#if SIZEOF_ZEND_LONG == 8
#define ADD_INDEX_INT64(_zv, _index, _value) add_index_long((_zv), (_index), (_value))
#define ADD_NEXT_INDEX_INT64(_zv, _value) add_next_index_long((_zv), (_value))
#define ADD_ASSOC_INT64(_zv, _key, _value) add_assoc_long((_zv), (_key), (_value))
#define ZVAL_INT64(_zv, _value) ZVAL_LONG((_zv), (_value))
#elif SIZEOF_ZEND_LONG == 4
#define ADD_INDEX_INT64(_zv, _index, _value)            \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		zval zchild;                                    \
		php_phongo_bson_new_int64(&zchild, (_value));   \
		add_index_zval((_zv), (_index), &zchild);       \
	} else {                                            \
		add_index_long((_zv), (_index), (_value));      \
	}
#define ADD_NEXT_INDEX_INT64(_zv, _value)               \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		zval zchild;                                    \
		php_phongo_bson_new_int64(&zchild, (_value));   \
		add_next_index_zval((_zv), &zchild);            \
	} else {                                            \
		add_next_index_long((_zv), (_value));           \
	}
#define ADD_ASSOC_INT64(_zv, _key, _value)              \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		zval zchild;                                    \
		php_phongo_bson_new_int64(&zchild, (_value));   \
		add_assoc_zval((_zv), (_key), &zchild);         \
	} else {                                            \
		add_assoc_long((_zv), (_key), (_value));        \
	}
#define ZVAL_INT64(_zv, _value)                         \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		php_phongo_bson_new_int64((_zv), (_value));     \
	} else {                                            \
		ZVAL_LONG((_zv), (_value));                     \
	}
#else /* SIZEOF_ZEND_LONG != 8 && SIZEOF_ZEND_LONG != 4 */
#error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif /* SIZEOF_ZEND_LONG */

#if PHP_VERSION_ID < 70300
#define ZVAL_COPY_DEREF(z, v)                     \
	do {                                          \
		zval* _z3 = (v);                          \
		if (Z_OPT_REFCOUNTED_P(_z3)) {            \
			if (UNEXPECTED(Z_OPT_ISREF_P(_z3))) { \
				_z3 = Z_REFVAL_P(_z3);            \
				if (Z_OPT_REFCOUNTED_P(_z3)) {    \
					Z_ADDREF_P(_z3);              \
				}                                 \
			} else {                              \
				Z_ADDREF_P(_z3);                  \
			}                                     \
		}                                         \
		ZVAL_COPY_VALUE(z, _z3);                  \
	} while (0)
#endif /* PHP_VERSION_ID < 70300 */

/* Compatibility macros to override error handling logic */
#define PHONGO_PARSE_PARAMETERS_START(min_num_args, max_num_args)               \
	do {                                                                        \
		zend_error_handling error_handling;                                     \
		zend_replace_error_handling(                                            \
			EH_THROW,                                                           \
			phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), \
			&error_handling);                                                   \
	ZEND_PARSE_PARAMETERS_START(min_num_args, max_num_args)

#define PHONGO_PARSE_PARAMETERS_END()                 \
	ZEND_PARSE_PARAMETERS_END_EX(                     \
		zend_restore_error_handling(&error_handling); \
		return );                                     \
	zend_restore_error_handling(&error_handling);     \
	}                                                 \
	while (0)

#ifndef ZEND_PARSE_PARAMETERS_NONE
#define PHONGO_PARSE_PARAMETERS_NONE()                                          \
	do {                                                                        \
		zend_error_handling error_handling;                                     \
		zend_replace_error_handling(                                            \
			EH_THROW,                                                           \
			phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), \
			&error_handling);                                                   \
		if (zend_parse_parameters_none() == FAILURE) {                          \
			zend_restore_error_handling(&error_handling);                       \
			return;                                                             \
		}                                                                       \
		zend_restore_error_handling(&error_handling);                           \
	} while (0)
#else
#define PHONGO_PARSE_PARAMETERS_NONE()                                          \
	do {                                                                        \
		zend_error_handling error_handling;                                     \
		zend_replace_error_handling(                                            \
			EH_THROW,                                                           \
			phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), \
			&error_handling);                                                   \
		if (UNEXPECTED(ZEND_NUM_ARGS() != 0)) {                                 \
			zend_wrong_parameters_none_error();                                 \
			zend_restore_error_handling(&error_handling);                       \
			return;                                                             \
		}                                                                       \
		zend_restore_error_handling(&error_handling);                           \
	} while (0)
#endif

/* Per https://wiki.php.net/rfc/internal_method_return_types, "Non-final
 * internal method return types - when possible - are declared tentatively in
 * PHP 8.1, and they will become enforced in PHP 9.0." This can be revisited
 * when more general typing improvements are made in PHPC-1709. */
#ifndef ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX
#define ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
	ZEND_BEGIN_ARG_INFO_EX(name, 0, return_reference, required_num_args)
#endif

void      phongo_add_exception_prop(const char* prop, int prop_len, zval* value);
zend_bool php_phongo_zend_hash_apply_protection_begin(HashTable* ht);
zend_bool php_phongo_zend_hash_apply_protection_end(HashTable* ht);

#endif /* PHONGO_COMPAT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

/*
 * Copyright 2014-present MongoDB, Inc.
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

#include <php.h>
#include <zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_util.h"

zend_class_entry* php_phongo_cursorid_ce;

/* Initialize the object from a numeric string and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_cursorid_init_from_string(php_phongo_cursorid_t* intern, const char* s_id, size_t s_id_len) /* {{{ */
{
	int64_t id;

	if (!php_phongo_parse_int64(&id, s_id, s_id_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit id for %s initialization", s_id, ZSTR_VAL(php_phongo_cursorid_ce->name));
		return false;
	}

	intern->id          = id;
	intern->initialized = true;
	return true;
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_cursorid_init_from_hash(php_phongo_cursorid_t* intern, HashTable* props) /* {{{ */
{
	zval* value;

	if ((value = zend_hash_str_find(props, "id", sizeof("id") - 1)) && Z_TYPE_P(value) == IS_STRING) {
		return php_phongo_cursorid_init_from_string(intern, Z_STRVAL_P(value), Z_STRLEN_P(value));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"id\" string field", ZSTR_VAL(php_phongo_cursorid_ce->name));
	return false;
} /* }}} */

static HashTable* php_phongo_cursorid_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp, bool is_serialize) /* {{{ */
{
	php_phongo_cursorid_t* intern;
	HashTable*             props;

	intern = Z_OBJ_CURSORID(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	if (!intern->initialized) {
		return props;
	}

	{
		zval value;

		if (is_serialize) {
			ZVAL_INT64_STRING(&value, intern->id);
		} else {
#if SIZEOF_ZEND_LONG == 4
			if (intern->id > INT32_MAX || intern->id < INT32_MIN) {
				ZVAL_INT64_STRING(&value, intern->id);
			} else {
				ZVAL_LONG(&value, intern->id);
			}
#else
			ZVAL_LONG(&value, intern->id);
#endif
		}
		zend_hash_str_update(props, "id", sizeof("id") - 1, &value);
	}

	return props;
} /* }}} */

/* {{{ proto MongoDB\Driver\CursorId MongoDB\Driver\CursorId::__set_state(array $properties)
*/
static PHP_METHOD(CursorId, __set_state)
{
	zend_error_handling    error_handling;
	php_phongo_cursorid_t* intern;
	HashTable*             props;
	zval*                  array;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &array) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	object_init_ex(return_value, php_phongo_cursorid_ce);

	intern = Z_CURSORID_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_cursorid_init_from_hash(intern, props);
} /* }}} */

/* {{{ proto string MongoDB\Driver\CursorId::__toString()
   Returns the string representation of the CursorId */
static PHP_METHOD(CursorId, __toString)
{
	zend_error_handling    error_handling;
	php_phongo_cursorid_t* intern;
	char*                  tmp;
	int                    tmp_len;

	intern = Z_CURSORID_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	tmp_len = spprintf(&tmp, 0, "%" PRId64, intern->id);
	RETVAL_STRINGL(tmp, tmp_len);
	efree(tmp);
} /* }}} */

/* {{{ proto string MongoDB\Driver\CursorId::serialize()
*/
static PHP_METHOD(CursorId, serialize)
{
	zend_error_handling    error_handling;
	php_phongo_cursorid_t* intern;
	zval                   retval;
	php_serialize_data_t   var_hash;
	smart_str              buf = { 0 };

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	intern = Z_CURSORID_OBJ_P(getThis());

	array_init_size(&retval, 1);
	ADD_ASSOC_INT64_AS_STRING(&retval, "id", intern->id);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\Driver\CursorId::unserialize(string $serialized)
*/
static PHP_METHOD(CursorId, unserialize)
{
	zend_error_handling    error_handling;
	php_phongo_cursorid_t* intern;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_CURSORID_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_cursorid_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_cursorid_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ proto array MongoDB\Driver\CursorId::__serialize()
*/
static PHP_METHOD(CursorId, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_cursorid_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true, true));
} /* }}} */

/* {{{ proto void MongoDB\Driver\CursorId::__unserialize(array $data)
*/
static PHP_METHOD(CursorId, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_cursorid_init_from_hash(Z_CURSORID_OBJ_P(getThis()), Z_ARRVAL_P(data));
} /* }}} */

/* {{{ MongoDB\Driver\CursorId function entries */
/* clang-format off */
ZEND_BEGIN_ARG_INFO_EX(ai_CursorId___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(ai_CursorId___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_CursorId___unserialize, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_CursorId_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_CursorId_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_cursorid_me[] = {
	PHP_ME(CursorId, __serialize, ai_CursorId_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CursorId, __set_state, ai_CursorId___set_state, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(CursorId, __toString, ai_CursorId___toString, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CursorId, __unserialize, ai_CursorId___unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CursorId, serialize, ai_CursorId_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CursorId, unserialize, ai_CursorId_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_CursorId_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_FE_END
};
/* clang-format on */
/* }}} */

/* {{{ MongoDB\Driver\CursorId object handlers */
static zend_object_handlers php_phongo_handler_cursorid;

static void php_phongo_cursorid_free_object(zend_object* object) /* {{{ */
{
	php_phongo_cursorid_t* intern = Z_OBJ_CURSORID(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
} /* }}} */

static zend_object* php_phongo_cursorid_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_cursorid_t* intern = zend_object_alloc(sizeof(php_phongo_cursorid_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_cursorid;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_cursorid_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_cursorid_get_properties_hash(object, true, false);
} /* }}} */
/* }}} */

static HashTable* php_phongo_cursorid_get_properties(phongo_compat_object_handler_type* object) /* {{{ */
{
	return php_phongo_cursorid_get_properties_hash(object, false, false);
} /* }}} */

void php_phongo_cursorid_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "CursorId", php_phongo_cursorid_me);
	php_phongo_cursorid_ce                = zend_register_internal_class(&ce);
	php_phongo_cursorid_ce->create_object = php_phongo_cursorid_create_object;
	PHONGO_CE_FINAL(php_phongo_cursorid_ce);

	zend_class_implements(php_phongo_cursorid_ce, 1, zend_ce_serializable);

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_cursorid_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_cursorid, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_cursorid.get_debug_info = php_phongo_cursorid_get_debug_info;
	php_phongo_handler_cursorid.get_properties = php_phongo_cursorid_get_properties;
	php_phongo_handler_cursorid.free_obj       = php_phongo_cursorid_free_object;
	php_phongo_handler_cursorid.offset         = XtOffsetOf(php_phongo_cursorid_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

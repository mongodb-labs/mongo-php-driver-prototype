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
#include <Zend/zend_interfaces.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_writeerror_ce;

/* {{{ proto integer MongoDB\Driver\WriteError::getCode()
   Returns the MongoDB error code */
static PHP_METHOD(WriteError, getCode)
{
	zend_error_handling      error_handling;
	php_phongo_writeerror_t* intern;

	intern = Z_WRITEERROR_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	RETURN_LONG(intern->code);
} /* }}} */

/* {{{ proto integer MongoDB\Driver\WriteError::getIndex()
   Returns the index of the operation in the BulkWrite to which this WriteError
   corresponds. */
static PHP_METHOD(WriteError, getIndex)
{
	zend_error_handling      error_handling;
	php_phongo_writeerror_t* intern;

	intern = Z_WRITEERROR_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	RETURN_LONG(intern->index);
} /* }}} */

/* {{{ proto string MongoDB\Driver\WriteError::getMessage()
   Returns the actual error message from the server */
static PHP_METHOD(WriteError, getMessage)
{
	zend_error_handling      error_handling;
	php_phongo_writeerror_t* intern;

	intern = Z_WRITEERROR_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	RETURN_STRING(intern->message);
} /* }}} */

/* {{{ proto object|null MongoDB\Driver\WriteError::getInfo()
   Returns additional metadata for the error */
static PHP_METHOD(WriteError, getInfo)
{
	zend_error_handling      error_handling;
	php_phongo_writeerror_t* intern;

	intern = Z_WRITEERROR_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if (!Z_ISUNDEF(intern->info)) {
		RETURN_ZVAL(&intern->info, 1, 0);
	}
} /* }}} */

/* {{{ MongoDB\Driver\WriteError function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_WriteError_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_writeerror_me[] = {
	/* clang-format off */
	PHP_ME(WriteError, getCode, ai_WriteError_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteError, getIndex, ai_WriteError_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteError, getMessage, ai_WriteError_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteError, getInfo, ai_WriteError_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_WriteError_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_WriteError_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\WriteError object handlers */
static zend_object_handlers php_phongo_handler_writeerror;

static void php_phongo_writeerror_free_object(zend_object* object) /* {{{ */
{
	php_phongo_writeerror_t* intern = Z_OBJ_WRITEERROR(object);

	zend_object_std_dtor(&intern->std);

	if (intern->message) {
		efree(intern->message);
	}

	if (!Z_ISUNDEF(intern->info)) {
		zval_ptr_dtor(&intern->info);
	}
} /* }}} */

static zend_object* php_phongo_writeerror_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_writeerror_t* intern = zend_object_alloc(sizeof(php_phongo_writeerror_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_writeerror;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_writeerror_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_writeerror_t* intern;
	zval                     retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_OBJ_WRITEERROR(PHONGO_COMPAT_GET_OBJ(object));

	array_init_size(&retval, 3);
	ADD_ASSOC_STRING(&retval, "message", intern->message);
	ADD_ASSOC_LONG_EX(&retval, "code", intern->code);
	ADD_ASSOC_LONG_EX(&retval, "index", intern->index);
	if (!Z_ISUNDEF(intern->info)) {
		Z_ADDREF(intern->info);
		ADD_ASSOC_ZVAL_EX(&retval, "info", &intern->info);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "info");
	}

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_writeerror_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteError", php_phongo_writeerror_me);
	php_phongo_writeerror_ce                = zend_register_internal_class(&ce);
	php_phongo_writeerror_ce->create_object = php_phongo_writeerror_create_object;
	PHONGO_CE_FINAL(php_phongo_writeerror_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_writeerror_ce);

	memcpy(&php_phongo_handler_writeerror, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeerror.get_debug_info = php_phongo_writeerror_get_debug_info;
	php_phongo_handler_writeerror.free_obj       = php_phongo_writeerror_free_object;
	php_phongo_handler_writeerror.offset         = XtOffsetOf(php_phongo_writeerror_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

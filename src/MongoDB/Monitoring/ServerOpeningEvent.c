/*
 * Copyright 2021-present MongoDB, Inc.
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

zend_class_entry* php_phongo_serveropeningevent_ce;

/* {{{ proto string ServerOpeningEvent::getHost()
   Returns this event's host */
static PHP_METHOD(ServerOpeningEvent, getHost)
{
	php_phongo_serveropeningevent_t* intern = Z_SERVEROPENINGEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING((intern->host)->host);
} /* }}} */

/* {{{ proto MongoDB\BSON\ObjectId ServerOpeningEvent::getTopologyId()
   Returns this event's topology id */
static PHP_METHOD(ServerOpeningEvent, getTopologyId)
{
	php_phongo_objectid_t*           topology_id;
	php_phongo_serveropeningevent_t* intern = Z_SERVEROPENINGEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	object_init_ex(return_value, php_phongo_objectid_ce);

	topology_id = Z_OBJECTID_OBJ_P(return_value);
	bson_oid_to_string(&intern->topology_id, topology_id->oid);
	topology_id->initialized = true;
} /* }}} */

/* {{{ MongoDB\Driver\Monitoring\ServerOpeningEvent function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ServerOpeningEvent_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_serveropeningevent_me[] = {
	/* clang-format off */
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_ServerOpeningEvent_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_ME(ServerOpeningEvent, getHost, ai_ServerOpeningEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerOpeningEvent, getTopologyId, ai_ServerOpeningEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_ServerOpeningEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Monitoring\ServerOpeningEvent object handlers */
static zend_object_handlers php_phongo_handler_serveropeningevent;

static void php_phongo_serveropeningevent_free_object(zend_object* object) /* {{{ */
{
	php_phongo_serveropeningevent_t* intern = Z_OBJ_SERVEROPENINGEVENT(object);

	zend_object_std_dtor(&intern->std);
} /* }}} */

static zend_object* php_phongo_serveropeningevent_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_serveropeningevent_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_serveropeningevent_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_serveropeningevent;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_serveropeningevent_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_serveropeningevent_t* intern;
	zval                             retval = ZVAL_STATIC_INIT;
	char                             topology_id[25];

	intern   = Z_OBJ_SERVEROPENINGEVENT(PHONGO_COMPAT_GET_OBJ(object));
	*is_temp = 1;
	array_init_size(&retval, 2);

	ADD_ASSOC_STRING(&retval, "host", (intern->host)->host);

	bson_oid_to_string(&intern->topology_id, topology_id);
	ADD_ASSOC_STRING(&retval, "topologyId", topology_id);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_serveropeningevent_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void) type;
	(void) module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerOpeningEvent", php_phongo_serveropeningevent_me);
	php_phongo_serveropeningevent_ce                = zend_register_internal_class(&ce);
	php_phongo_serveropeningevent_ce->create_object = php_phongo_serveropeningevent_create_object;
	PHONGO_CE_FINAL(php_phongo_serveropeningevent_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_serveropeningevent_ce);

	memcpy(&php_phongo_handler_serveropeningevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serveropeningevent.get_debug_info = php_phongo_serveropeningevent_get_debug_info;
	php_phongo_handler_serveropeningevent.free_obj       = php_phongo_serveropeningevent_free_object;
	php_phongo_handler_serveropeningevent.offset         = XtOffsetOf(php_phongo_serveropeningevent_t, std);

	return;
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

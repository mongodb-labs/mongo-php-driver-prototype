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

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_long.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_error.h"

#include "BSON/Document.h"
#include "MongoDB/Server.h"
#include "MongoDB/WriteConcernError.h"
#include "MongoDB/WriteError.h"
#include "BulkWriteCommandResult_arginfo.h"

#define PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED(method) \
	if (!intern->is_acknowledged) { \
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "MongoDB\\Driver\\BulkWriteCommandResult::" method "() should not be called for an unacknowledged write result"); \
		return; \
	}

zend_class_entry* php_phongo_bulkwritecommandresult_ce;

/* Populates return_value with a list of WriteConcernError objects. Returns true
 * on success; otherwise, false is returned and an exception is thrown. */
static bool phongo_bulkwritecommandresult_get_writeconcernerrors(php_phongo_bulkwritecommandresult_t* intern, zval* return_value)
{
	bson_iter_t iter;

	array_init(return_value);

	if (intern->write_concern_errors && bson_iter_init(&iter, intern->write_concern_errors)) {
		while (bson_iter_next(&iter)) {
			bson_t         bson;
			uint32_t       len;
			const uint8_t* data;
			zval           write_concern_error;

			if (!BSON_ITER_HOLDS_DOCUMENT(&iter)) {
				continue;
			}

			bson_iter_document(&iter, &len, &data);

			if (!bson_init_static(&bson, data, len)) {
				continue;
			}

			if (!phongo_writeconcernerror_init(&write_concern_error, &bson)) {
				/* Exception already thrown */
				zval_ptr_dtor(&write_concern_error);
				return false;
			}

			add_next_index_zval(return_value, &write_concern_error);
		}
	}

	return true;
}

/* Populates return_value with a map of WriteError objects indexed by the offset
 * of the corresponding operation. Returns true on success; otherwise, false is
 * returned and an exception is thrown. */
static bool phongo_bulkwritecommandresult_get_writeerrors(php_phongo_bulkwritecommandresult_t* intern, zval* return_value)
{
	bson_iter_t iter;

	array_init(return_value);

	if (intern->write_errors && bson_iter_init(&iter, intern->write_errors)) {
		while (bson_iter_next(&iter)) {
			bson_t         bson;
			uint32_t       len;
			const uint8_t* data;
			zval           write_error;
			zend_ulong     index;

			if (!BSON_ITER_HOLDS_DOCUMENT(&iter)) {
				continue;
			}

			bson_iter_document(&iter, &len, &data);

			if (!bson_init_static(&bson, data, len)) {
				continue;
			}

			index = (zend_ulong) ZEND_STRTOUL(bson_iter_key(&iter), NULL, 10);

			if (!phongo_writeerror_init_ex(&write_error, &bson, (int32_t) index)) {
				/* Exception already thrown */
				zval_ptr_dtor(&write_error);
				return false;
			}

			add_index_zval(return_value, index, &write_error);
		}
	}

	return true;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_BulkWriteCommandResult)

/* Returns the number of documents that were inserted */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getInsertedCount)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getInsertedCount");

	RETURN_LONG(intern->inserted_count);
}

/* Returns the number of documents that matched the update criteria */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getMatchedCount)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getMatchedCount");

	RETURN_LONG(intern->matched_count);
}

/* Returns the number of documents that were actually modified by an update */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getModifiedCount)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getModifiedCount");

	RETURN_LONG(intern->modified_count);
}

/* Returns the number of documents that were deleted */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getDeletedCount)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getDeletedCount");

	RETURN_LONG(intern->deleted_count);
}

/* Returns the number of documents that were upserted */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getUpsertedCount)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getUpsertedCount");

	RETURN_LONG(intern->upserted_count);
}

/* Returns the last Server used to execute a command for the bulk write */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getServer)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getServer");

	// TODO: null handling
	phongo_server_init(return_value, &intern->manager, intern->server_id);
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getInsertResults)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getInsertResults");

	if (intern->insert_results) {
		phongo_document_new(return_value, intern->insert_results, true);
	}
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getUpdateResults)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getUpdateResults");

	if (intern->update_results) {
		phongo_document_new(return_value, intern->update_results, true);
	}
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getDeleteResults)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getDeleteResults");

	if (intern->delete_results) {
		phongo_document_new(return_value, intern->delete_results, true);
	}
}

/* Return any write concern errors that occurred */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getWriteConcernErrors)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_bulkwritecommandresult_get_writeconcernerrors(intern, return_value);
}

/* Returns any write errors that occurred */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getWriteErrors)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_bulkwritecommandresult_get_writeerrors(intern, return_value);
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getErrorReply)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (intern->error_reply) {
		phongo_document_new(return_value, intern->error_reply, true);
	}
}

/* Returns whether the write operation was acknowledged (based on the write
   concern). */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, isAcknowledged)
{
	php_phongo_bulkwritecommandresult_t* intern;

	intern = Z_BULKWRITECOMMANDRESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(intern->is_acknowledged);
}

/* MongoDB\Driver\BulkWriteCommandResult object handlers */
static zend_object_handlers php_phongo_handler_bulkwritecommandresult;

static void php_phongo_bulkwritecommandresult_free_object(zend_object* object)
{
	php_phongo_bulkwritecommandresult_t* intern = Z_OBJ_BULKWRITECOMMANDRESULT(object);

	zend_object_std_dtor(&intern->std);

	bson_destroy(intern->insert_results);
	bson_destroy(intern->update_results);
	bson_destroy(intern->delete_results);
	bson_destroy(intern->error_reply);
	bson_destroy(intern->write_errors);
	bson_destroy(intern->write_concern_errors);

	if (!Z_ISUNDEF(intern->manager)) {
		zval_ptr_dtor(&intern->manager);
	}
}

static zend_object* php_phongo_bulkwritecommandresult_create_object(zend_class_entry* class_type)
{
	php_phongo_bulkwritecommandresult_t* intern = zend_object_alloc(sizeof(php_phongo_bulkwritecommandresult_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_bulkwritecommandresult;

	return &intern->std;
}

static HashTable* php_phongo_bulkwritecommandresult_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_bulkwritecommandresult_t* intern;
	zval                      retval = ZVAL_STATIC_INIT;

	intern   = Z_OBJ_BULKWRITECOMMANDRESULT(object);
	*is_temp = 1;
	array_init_size(&retval, 12);

	ADD_ASSOC_BOOL_EX(&retval, "isAcknowledged", intern->is_acknowledged);
	ADD_ASSOC_LONG_EX(&retval, "insertedCount", intern->inserted_count);
	ADD_ASSOC_LONG_EX(&retval, "matchedCount", intern->matched_count);
	ADD_ASSOC_LONG_EX(&retval, "modifiedCount", intern->modified_count);
	ADD_ASSOC_LONG_EX(&retval, "upsertedCount", intern->upserted_count);
	ADD_ASSOC_LONG_EX(&retval, "deletedCount", intern->deleted_count);

	if (intern->insert_results) {
		zval insert_results;

		phongo_document_new(&insert_results, intern->insert_results, true);
		ADD_ASSOC_ZVAL_EX(&retval, "insertResults", &insert_results);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "insertResults");
	}

	if (intern->update_results) {
		zval update_results;

		phongo_document_new(&update_results, intern->update_results, true);
		ADD_ASSOC_ZVAL_EX(&retval, "updateResults", &update_results);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "updateResults");
	}

	if (intern->delete_results) {
		zval delete_results;

		phongo_document_new(&delete_results, intern->delete_results, true);
		ADD_ASSOC_ZVAL_EX(&retval, "deleteResults", &delete_results);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "deleteResults");
	}

	{
		zval writeerrors;

		phongo_bulkwritecommandresult_get_writeerrors(intern, &writeerrors);
		ADD_ASSOC_ZVAL_EX(&retval, "writeErrors", &writeerrors);
	}

	{
		zval writeconcernerrors;

		phongo_bulkwritecommandresult_get_writeconcernerrors(intern, &writeconcernerrors);
		ADD_ASSOC_ZVAL_EX(&retval, "writeConcernErrors", &writeconcernerrors);
	}

	if (intern->error_reply) {
		zval error_reply;

		phongo_document_new(&error_reply, intern->error_reply, true);
		ADD_ASSOC_ZVAL_EX(&retval, "errorReply", &error_reply);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "errorReply");
	}

	if (intern->server_id) {
		zval server;

		phongo_server_init(&server, &intern->manager, intern->server_id);
		ADD_ASSOC_ZVAL_EX(&retval, "server", &server);
	} else {
		/* TODO: Determine if this path is only reached when a partial result is
		 * attached to a BulkWriteCommandException on an unacknowledged write. */
		ADD_ASSOC_NULL_EX(&retval, "server");
	}

	return Z_ARRVAL(retval);
}

void php_phongo_bulkwritecommandresult_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_bulkwritecommandresult_ce                = register_class_MongoDB_Driver_BulkWriteCommandResult();
	php_phongo_bulkwritecommandresult_ce->create_object = php_phongo_bulkwritecommandresult_create_object;

	memcpy(&php_phongo_handler_bulkwritecommandresult, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_bulkwritecommandresult.get_debug_info = php_phongo_bulkwritecommandresult_get_debug_info;
	php_phongo_handler_bulkwritecommandresult.free_obj       = php_phongo_bulkwritecommandresult_free_object;
	php_phongo_handler_bulkwritecommandresult.offset         = XtOffsetOf(php_phongo_bulkwritecommandresult_t, std);
}

static inline bson_t* _bson_copy_or_null (const bson_t* bson)
{
	return bson_empty0(bson) ? NULL : bson_copy(bson);
}

php_phongo_bulkwritecommandresult_t* phongo_bulkwritecommandresult_init(zval* return_value, mongoc_bulkwritereturn_t* bw_ret, zval* manager)
{
	php_phongo_bulkwritecommandresult_t* bwcr;

	object_init_ex(return_value, php_phongo_bulkwritecommandresult_ce);

	bwcr            = Z_BULKWRITECOMMANDRESULT_OBJ_P(return_value);
	bwcr->is_acknowledged = !!bw_ret->res;

	// Copy mongoc_bulkwriteresult_t fields
	if (bw_ret->res) {
		bwcr->inserted_count = mongoc_bulkwriteresult_insertedcount(bw_ret->res);
		bwcr->upserted_count = mongoc_bulkwriteresult_upsertedcount(bw_ret->res);
		bwcr->matched_count = mongoc_bulkwriteresult_matchedcount(bw_ret->res);
		bwcr->modified_count = mongoc_bulkwriteresult_modifiedcount(bw_ret->res);
		bwcr->deleted_count = mongoc_bulkwriteresult_deletedcount(bw_ret->res);

		bwcr->insert_results = _bson_copy_or_null(mongoc_bulkwriteresult_insertresults(bw_ret->res));
		bwcr->update_results = _bson_copy_or_null(mongoc_bulkwriteresult_updateresults(bw_ret->res));
		bwcr->delete_results = _bson_copy_or_null(mongoc_bulkwriteresult_deleteresults(bw_ret->res));

		bwcr->server_id = mongoc_bulkwriteresult_serverid(bw_ret->res);
	}

	// Copy mongoc_bulkwriteexception_t fields
	if (bw_ret->exc) {
		bwcr->error_reply = _bson_copy_or_null(mongoc_bulkwriteexception_errorreply(bw_ret->exc));
		bwcr->write_errors = _bson_copy_or_null(mongoc_bulkwriteexception_writeerrors(bw_ret->exc));
		bwcr->write_concern_errors = _bson_copy_or_null(mongoc_bulkwriteexception_writeconcernerrors(bw_ret->exc));
	}

	ZVAL_ZVAL(&bwcr->manager, manager, 1, 0);

	return bwcr;
}

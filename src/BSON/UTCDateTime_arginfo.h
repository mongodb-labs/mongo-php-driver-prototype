/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 20b80ad0636d3d0726362f2183eceb560c169f4c */

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___construct, 0, 0, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, milliseconds, DateTime|DateTimeImmutable, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_DOUBLE|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___construct, 0, 0, 0)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, milliseconds, "null")
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_toDateTime, 0, 0, DateTime, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___set_state, 0, 1, MongoDB\\BSON\\\125TCDateTime, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_UTCDateTime_serialize arginfo_class_MongoDB_BSON_UTCDateTime___toString

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __construct);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __construct);
#endif
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, toDateTime);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __toString);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __set_state);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, serialize);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, unserialize);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __unserialize);
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, __serialize);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_UTCDateTime, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_UTCDateTime_methods[] = {
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_UTCDateTime, __construct, arginfo_class_MongoDB_BSON_UTCDateTime___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __construct, arginfo_class_MongoDB_BSON_UTCDateTime___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_UTCDateTime, toDateTime, arginfo_class_MongoDB_BSON_UTCDateTime_toDateTime, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __toString, arginfo_class_MongoDB_BSON_UTCDateTime___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __set_state, arginfo_class_MongoDB_BSON_UTCDateTime___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(MongoDB_BSON_UTCDateTime, serialize, arginfo_class_MongoDB_BSON_UTCDateTime_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, unserialize, arginfo_class_MongoDB_BSON_UTCDateTime_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __unserialize, arginfo_class_MongoDB_BSON_UTCDateTime___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_UTCDateTime, __serialize, arginfo_class_MongoDB_BSON_UTCDateTime___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_UTCDateTime, jsonSerialize, arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_UTCDateTime, jsonSerialize, arginfo_class_MongoDB_BSON_UTCDateTime_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_UTCDateTime(zend_class_entry *class_entry_MongoDB_BSON_UTCDateTimeInterface, zend_class_entry *class_entry_MongoDB_BSON_JsonSerializable, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Serializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "UTCDateTime", class_MongoDB_BSON_UTCDateTime_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 4, class_entry_MongoDB_BSON_UTCDateTimeInterface, class_entry_MongoDB_BSON_JsonSerializable, class_entry_MongoDB_BSON_Type, class_entry_Serializable);

	return class_entry;
}

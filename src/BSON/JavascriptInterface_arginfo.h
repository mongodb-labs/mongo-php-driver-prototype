/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d2335273fc900e95ba3ce8731fa241792beeec88 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_JavascriptInterface_getCode, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_JavascriptInterface_getScope, 0, 0, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_JavascriptInterface___toString arginfo_class_MongoDB_BSON_JavascriptInterface_getCode




static const zend_function_entry class_MongoDB_BSON_JavascriptInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_JavascriptInterface, getCode, arginfo_class_MongoDB_BSON_JavascriptInterface_getCode, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_JavascriptInterface, getScope, arginfo_class_MongoDB_BSON_JavascriptInterface_getScope, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(MongoDB_BSON_JavascriptInterface, __toString, arginfo_class_MongoDB_BSON_JavascriptInterface___toString, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_JavascriptInterface(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "JavascriptInterface", class_MongoDB_BSON_JavascriptInterface_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}

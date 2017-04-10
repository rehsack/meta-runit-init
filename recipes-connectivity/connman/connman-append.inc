FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/connmand.run \
		"
RUNIT_SERVICES += "connmand.log"

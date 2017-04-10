FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/modutils.run \
		"
RUNIT_SERVICES += "modutils.once.log"

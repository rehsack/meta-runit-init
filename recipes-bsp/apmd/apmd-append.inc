FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
		file://runit-init/apmd.run \
		"
RUNIT_SERVICES += "apmd.once.log"

inherit runit-services

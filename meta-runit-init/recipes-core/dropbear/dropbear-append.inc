FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/dropbear.run \
		"
RUNIT_SERVICES += "dropbear.log"

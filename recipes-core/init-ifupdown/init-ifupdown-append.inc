FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/networking.run \
		"
RUNIT_SERVICES += "networking.once.log"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/fbsetup.run \
		"
RUNIT_SERVICES += "fbsetup.once.log"

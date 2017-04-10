FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/run-postinsts.run \
		"
RUNIT_SERVICES += "run-postinsts.once.log"

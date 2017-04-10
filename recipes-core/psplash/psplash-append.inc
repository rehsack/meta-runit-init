FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/psplash.run \
		"
RUNIT_SERVICES += "psplash.once"

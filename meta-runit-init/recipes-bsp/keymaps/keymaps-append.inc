FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/keymaps.run \
		"
RUNIT_SERVICES += "keymaps.once"

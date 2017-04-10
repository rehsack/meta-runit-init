FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append += " \
		file://runit-init/dnsmasq.run \
		"

RUNIT_SERVICES += "dnsmasq.log"


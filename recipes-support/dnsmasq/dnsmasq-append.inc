FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append += " \
		file://persistent \
		file://runit-init/dnsmasq.run \
		"

RUNIT_SERVICES += "dnsmasq.log"

do_install_append() {
	install -d ${D}${sysconfdir}/default/persistent
	install -m 0644 ${WORKDIR}/persistent ${D}${sysconfdir}/default/persistent/50_dnsmasq
}

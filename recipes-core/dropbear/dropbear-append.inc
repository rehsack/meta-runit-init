FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://persistent \
		file://default \
		file://runit-init/dropbear.run \
		"
RUNIT_SERVICES += "dropbear"

do_install_append() {
	rm -f ${D}/${sysconfdir}/default/dropbear
	install -d ${D}${sysconfdir}/default/persistent
	install -m 0644 ${WORKDIR}/default ${D}${sysconfdir}/default/dropbear
	install -m 0644 ${WORKDIR}/persistent ${D}${sysconfdir}/default/persistent/50_dropbear
}

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/bluetoothd.run \
		"
RUNIT_SERVICES += "bluetoothd.log"

do_install_append() {
	sed -i -e 's%@LIBEXECDIR@%${libexecdir}%g' ${D}${sysconfdir}/sv/service/bluetoothd.run
}

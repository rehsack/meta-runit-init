FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/bluetoothd.run \
		"
RUNIT_SERVICES += "bluetoothd.log"

do_install_append() {
	if ${@bb.utils.contains('VIRTUAL-RUNTIME_init_manager', 'busybox-runit-init', 'true', 'false', d)}; then
		sed -i -e 's%@LIBEXECDIR@%${libexecdir}%g' ${D}${sysconfdir}/sv/service/bluetoothd.run
	fi
}

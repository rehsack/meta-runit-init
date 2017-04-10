FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/alsa-state.run \
		"
RUNIT_SERVICES += "alsa-state.once"

do_install_append() {
	if ${@bb.utils.contains('VIRTUAL-RUNTIME_init_manager', 'busybux-runit-init', 'true', 'false', d)}; then
		sed -i -e 's%#STATEDIR#%${localstatedir}%' ${D}${sysconfdir}/sv/service/alsa-state.run
	fi
}

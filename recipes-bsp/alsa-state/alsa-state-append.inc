FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/alsa-state.run \
		"
RUNIT_SERVICES += "alsa-state.once"

do_install_append() {
	sed -i -e 's%#STATEDIR#%${localstatedir}%' ${D}${sysconfdir}/sv/service/alsa-state.run
}

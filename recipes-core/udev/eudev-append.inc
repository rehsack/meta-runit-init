FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/udev.run \
		file://runit-init/udev-trigger.run \
		file://trigger \
		"
RUNIT_SERVICES += "udev.log udev-trigger.once.log"

do_install_append() {
     sed -i s%@UDEVD@%${base_sbindir}/udevd% ${D}${sysconfdir}/sv/service/udev.run
}

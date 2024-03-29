FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/dbus.run \
		file://runit-init/dbus-events.run \
		file://dbus-events.init \
		"
RUNIT_SERVICES += "dbus.log dbus-events.once.log"

do_install_append() {
     if ${@bb.utils.contains("VIRTUAL-RUNTIME_init_manager", "busybox-runit-init", "true", "false", d)}; then
         sed -i s%@bindir@%${bindir}% ${D}${sysconfdir}/sv/service/dbus.run
         sed -i s%@bindir@%${bindir}% ${D}${sysconfdir}/sv/service/dbus-events.run
     fi
}

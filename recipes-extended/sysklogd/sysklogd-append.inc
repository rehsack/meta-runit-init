FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit runit-services

SRC_URI_append = " \
		file://runit-init/syslog.run \
		file://runit-init/klogd.run \
		"
RUNIT_SERVICES += "syslog klogd"

ALTERNATIVE_${PN} += "syslog-runit-init"

ALTERNATIVE_LINK_NAME[syslog-runit-init] = "${sysconfdir}/sv/service/syslog.run"
ALTERNATIVE_TARGET[syslog-runit-init] = "${sysconfdir}/sv/service/syslog.run.${PN}"

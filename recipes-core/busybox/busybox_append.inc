
SRC_URI_append = " \
	file://runit-init/crond.run\
	file://runit-init/doneall.run \
	file://runit-init/donesingle.run \
	file://runit-init/functions-ri \
	file://runit-init/klogd.run \
	file://runit-init/getty-tty.run \
	file://runit-init/init \
	file://runit-init/initsig \
	file://runit-init/mdev.run \
	file://runit-init/ntpd.run \
	file://runit-init/rc.local.sample \
	file://runit-init/rclocal.run \
	file://runit-init/rcS-default \
	file://runit-init/runsvlog.run \
        file://runit-init/service_progress.run \
	file://runit-init/sulogin.sh \
	file://runit-init/syslog.run \
	file://runit-init/system-stop.sh \
	file://runit-init/telinit \
	${@['','file://runit.cfg'][(d.getVar('VIRTUAL-RUNTIME_runit', True) == 'busybox')]} \
	${@['','file://runit.cfg'][(d.getVar('VIRTUAL-RUNTIME_init_manager', True) == 'busybox-runit-init')]} \
	${@['','file://syslogd.cfg'][(d.getVar('VIRTUAL-RUNTIME_syslog', True) == 'busybox-syslog')]} \
	"

RDEPENDS_busybox-runit-init =+ "${VIRTUAL-RUNTIME_syslog} initscripts"
RRECOMMENDS_busybox-runit-init =+ "kernel-module-overlay"

PACKAGES =+ "${@['','busybox-runit-init'][(d.getVar('VIRTUAL-RUNTIME_init_manager', True) == 'busybox-runit-init')]}"

FILES_${PN}-runit-init = "${base_sbindir}/init ${base_sbindir}/init.${BPN} ${base_sbindir}/telinit ${base_sbindir}/telinit.${BPN} ${base_sbindir}/initsig ${sysconfdir}/rc.local ${sysconfdir}/default ${sysconfdir}/sv ${libexecdir}/getty-tty ${base_sbindir}/runlevel ${localstatedir}/lib/urandom ${sysconfdir}/etc/init.d/system-stop.sh ${base_sbindir}/sulogin.sh"

inherit update-alternatives runit-services

ALTERNATIVE_${PN}_append += "init_runit telinit_runit"
ALTERNATIVE_LINK_NAME[init_runit] = "${base_sbindir}/init"
ALTERNATIVE_TARGET[init_runit] = "${base_sbindir}/init.${PN}-runit-init"
ALTERNATIVE_PRIORITY[init_runit] = "70"
ALTERNATIVE_LINK_NAME[telinit_runit] = "${base_sbindir}/telinit"
ALTERNATIVE_PRIORITY[telinit_runit] = "70"
ALTERNATIVE_TARGET[telinit_runit] = "${base_sbindir}/telinit.${PN}-runit-init"

CONFFILES_${PN}-runit-init = "${sysconfdir}/default/rcS"

RUNIT_SERVICES=\
	" \
	${@bb.utils.contains('PACKAGES', '${PN}-cron', 'crond.log', '', d)} \
	donesingle.once \
	doneall.once \
	${@bb.utils.contains('PACKAGES', '${PN}-syslog', 'klogd', '', d)} \
	${@bb.utils.contains('VIRTUAL-RUNTIME_dev_manager', 'busybox-mdev', 'mdev.once.log', '', d)} \
	${@bb.utils.contains('PACKAGES', '${PN}-ntpd', 'ntpd.log', '', d)} \
	rclocal.once \
	${@bb.utils.contains('PACKAGES', '${PN}-syslog', 'syslog', '', d)} \
	"

do_install_append() {
	if ${@bb.utils.contains('PACKAGES', 'busybox-runit-init', "true", "false", d)}; then
		install -d ${D}${base_sbindir}
		install -d ${D}${sysconfdir}
		install -d ${D}${sysconfdir}/default
		install -d ${D}${libexecdir}/getty-tty
		install -d ${D}${localstatedir}/service
		# Holds state information pertaining to urandom
		install -d ${D}${localstatedir}/lib/urandom

		install -m 0755 ${WORKDIR}/runit-init/init ${D}${base_sbindir}/init.${BPN}-runit-init
		install -m 0755 ${WORKDIR}/runit-init/telinit ${D}${base_sbindir}/telinit.${BPN}-runit-init
		install -m 0755 ${WORKDIR}/runit-init/sulogin.sh ${D}${base_sbindir}/sulogin.sh
		install -m 0755 ${WORKDIR}/runit-init/rc.local.sample ${D}${sysconfdir}/rc.local
		install -m 0644 ${WORKDIR}/runit-init/rcS-default ${D}${sysconfdir}/default/rcS
		install -m 0755 ${WORKDIR}/runlevel ${D}${base_sbindir}/runlevel
		install -m 0755 ${WORKDIR}/runit-init/initsig ${D}${base_sbindir}/initsig
		for iscript in ${D}${base_sbindir}/init.${BPN}-runit-init ${D}${base_sbindir}/initsig; do
			sed -e "s,/var,${localstatedir}," $iscript
		done

		install -d ${D}${sysconfdir}/init.d
		install -m 0755 ${WORKDIR}/runit-init/functions-ri ${D}${sysconfdir}/init.d/
		install -m 0755 ${WORKDIR}/runit-init/system-stop.sh ${D}${sysconfdir}/init.d/

		sed -i ${D}${sysconfdir}/init.d/system-stop.sh -e 's,/var/,${localstatedir}/,g;s,/etc/,${sysconfdir}/,g'

		install -d ${D}${sysconfdir}/service
		install -d ${D}${sysconfdir}/sv/service
		install -d ${D}${localstatedir}/service
		install -m 0755 ${WORKDIR}/runit-init/getty-tty.run ${D}${sysconfdir}/sv/service
		install -m 0755 ${WORKDIR}/runit-init/service_progress.run ${D}${sysconfdir}/sv/service

		consoles="115200;tty1 115200;tty2 115200;tty3 115200;tty4 ${SERIAL_CONSOLES}"
		for console in $consoles; do
			oIFS="$IFS"
			IFS=";"
			set -- $console
			IFS="$oIFS"
			install -d ${D}${sysconfdir}/service/5-getty-$2-$1
			ln -sf ${sysconfdir}/sv/service/getty-tty.run ${D}${sysconfdir}/service/5-getty-$2-$1/run
		done
		install -d ${D}${sysconfdir}/service/0-service_progress
		ln -sf ${sysconfdir}/sv/service/service_progress.run ${D}${sysconfdir}/service/0-service_progress/run

		# Common log service for services that need it.
		install -d ${D}${sysconfdir}/sv
		install -m 0755 ${WORKDIR}/runit-init/runsvlog.run ${D}${sysconfdir}/sv/service

		# Make sure timestamp exists
		touch ${D}${sysconfdir}/timestamp
		if ${@bb.utils.contains('DISTRO_FEATURES', 'read-only-rootfs', 'false', 'true', d)}; then
			for sv in $(ls -A ${D}${sysconfdir}/service); do
				mkdir -p ${D}${localstatedir}/service/$sv
				ln -sf ${sysconfdir}/service/$sv/run ${D}${localstatedir}/service/$sv/
			done
		fi
	fi
}

python() {
    if d.getVar('VIRTUAL-RUNTIME_init_manager', True) == 'busybox-runit-init':
        pn = d.getVar('PN', True)
        d.appendVar('ALTERNATIVE_%s-cron' % (pn), ' crond-init')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'crond-init', '%s/init.d/crond' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'crond-init', '%s/init.d/crond.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))
        d.appendVar('ALTERNATIVE_%s-ntpd' % (pn), ' ntpd-init')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'ntpd-init', '%s/init.d/ntpd' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'ntpd-init', '%s/init.d/ntpd.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))
        d.appendVar('ALTERNATIVE_%s-ntpd' % (pn), ' ntpd-conf')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'ntpd-conf', '%s/ntpd.conf' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'ntpd-conf', '%s/ntpd.conf.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))
        d.appendVar('ALTERNATIVE_%s-syslog' % (pn), ' syslog-init')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'syslog-init', '%s/init.d/syslog' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'syslog-init', '%s/init.d/syslog.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))
        d.appendVar('ALTERNATIVE_%s-syslog' % (pn), ' syslog-startup-conf')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'syslog-startup-conf', '%s/syslog-startup.conf' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'syslog-startup-conf', '%s/syslog-startup.conf.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))

    if bb.utils.contains('DISTRO_FEATURES', 'sysvinit', True, False, d):
        pn = d.getVar('PN', True)
        d.appendVar('ALTERNATIVE_%s-syslog' % (pn), ' syslog-init')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'syslog-init', '%s/init.d/syslog' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'syslog-init', '%s/init.d/syslog.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))
        d.appendVar('ALTERNATIVE_%s-syslog' % (pn), ' syslog-startup-conf')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'syslog-startup-conf', '%s/syslog-startup.conf' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'syslog-startup-conf', '%s/syslog-startup.conf.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))
        d.appendVar('ALTERNATIVE_%s-cron' % (pn), ' busybox-cron')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'busybox-cron', '%s/init.d/crond' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'busybox-cron', '%s/init.d/crond.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))
        d.appendVar('ALTERNATIVE_%s-ntpd' % (pn), ' busybox-ntpd')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'busybox-ntpd', '%s/init.d/ntpd' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'busybox-ntpd', '%s/init.d/ntpd.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))
        d.appendVar('ALTERNATIVE_%s-ntpd' % (pn), ' busybox-ntpd-conf')
        d.setVarFlag('ALTERNATIVE_LINK_NAME', 'busybox-ntpd-conf', '%s/ntpd.conf' % (d.getVar('sysconfdir', True)))
        d.setVarFlag('ALTERNATIVE_TARGET', 'busybox-ntpd-conf', '%s/ntpd.conf.%s' % (d.getVar('sysconfdir', True), d.getVar('BPN', True)))
}

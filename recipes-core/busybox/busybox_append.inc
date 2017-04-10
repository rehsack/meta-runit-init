
SRC_URI_append = " \
	file://runit-init/banner.run \
	file://runit-init/crond.run\
	file://runit-init/dmesglog.run \
	file://runit-init/donesingle.run \
	file://runit-init/functions-ri \
	file://runit-init/hostname.run \
	file://runit-init/klogd.run \
	file://runit-init/loadmodules.run \
	file://runit-init/localmounts.run \
	file://runit-init/logrotate-dmesg.conf \
	file://runit-init/getty-tty.run \
	file://runit-init/init \
	file://runit-init/initsig \
	file://runit-init/mdev.run \
	file://runit-init/ntpd.run \
	file://runit-init/persistent \
	file://runit-init/rc.local.sample \
	file://runit-init/rclocal.run \
	file://runit-init/rcS-default \
	file://runit-init/rootfs.run \
	file://runit-init/runsvlog.run \
	file://runit-init/sulogin.sh \
	file://runit-init/sushell \
	file://runit-init/syslog.run \
	file://runit-init/system-stop.sh \
	file://runit-init/telinit \
	file://runit-init/urandom.run \
	${@bb.utils.contains('DISTRO_FEATURES','selinux','file://runit-init/sushell','',d)} \
	${@['','file://runit.cfg'][(d.getVar('VIRTUAL-RUNTIME_runit', True) == 'busybox')]} \
	${@['','file://runit.cfg'][(d.getVar('VIRTUAL-RUNTIME_init_manager', True) == 'busybox-runit-init')]} \
	"

RRECOMMENDS_${PN}-runit-init += "${PN}-mdev ${PN}-syslog"

PACKAGES =+ "${@['','busybox-runit-init'][(d.getVar('VIRTUAL-RUNTIME_init_manager', True) == 'busybox-runit-init')]}"

FILES_${PN}-runit-init = "${base_sbindir}/init ${base_sbindir}/init.${PN} ${base_sbindir}/telinit ${base_sbindir}/telinit.${PN} ${base_sbindir}/initsig ${sysconfdir}/rc.local ${sysconfdir}/default ${sysconfdir}/sv ${libexecdir}/getty-tty ${sysconfdir}/default/persistent ${sysconfdir}/logrotate-dmesg.conf ${base_sbindir}/runlevel ${localstatedir}/lib/urandom ${sysconfdir}/etc/init.d/system-stop.sh ${base_sbindir}/sulogin.sh ${base_sbindir}/sushell"

inherit update-alternatives

ALTERNATIVE_${PN}_append = "init telinit poweroff halt reboot"
ALTERNATIVE_LINK_NAME[init] = "${base_sbindir}/init"
ALTERNATIVE_PRIORITY[init] = "15"
ALTERNATIVE_LINK_NAME[telinit] = "${base_sbindir}/telinit"
ALTERNATIVE_PRIORITY[telinit] = "15"
ALTERNATIVE_LINK_NAME[poweroff] = "${base_sbindir}/poweroff"
ALTERNATIVE_PRIORITY[poweroff] = "15"
ALTERNATIVE_LINK_NAME[halt] = "${base_sbindir}/halt"
ALTERNATIVE_PRIORITY[halt] = "15"
ALTERNATIVE_LINK_NAME[reboot] = "${base_sbindir}/reboot"
ALTERNATIVE_PRIORITY[reboot] = "15"

ALTERNATIVE_${PN}_append += "sulogin"
ALTERNATIVE_PRIORITY[init] = "70"
ALTERNATIVE_PRIORITY[telinit] = "70"

CONFFILES_${PN}-runit-init = "${sysconfdir}/default/persistent/20_runit_init ${sysconfdir}/default/rcS ${sysconfdir}/default/devpts ${sysconfdir}/logrotate-dmesg.conf"

RUNIT_SERVICES=\
	" \
	banner.once \
	${@bb.utils.contains('BUSYBOX_EXTRA_PACKAGES', '${PN}-cron', 'crond.log', '', d)} \
	dmesglog.once \
	donesingle \
	hostname.once \
	${@bb.utils.contains('BUSYBOX_EXTRA_PACKAGES', '${PN}-syslog', 'klogd', '', d)} \
	loadmodules.once \
	localmounts.once.log \
	${@bb.utils.contains('BUSYBOX_EXTRA_PACKAGES', '${PN}-mdev', 'mdev.once.log', '', d)} \
	${@bb.utils.contains('BUSYBOX_EXTRA_PACKAGES', '${PN}-ntpd', 'ntpd.log', '', d)} \
	rclocal \
	rootfs.once \
	${@bb.utils.contains('BUSYBOX_EXTRA_PACKAGES', '${PN}-syslog', 'syslog', '', d)} \
	urandom.once.log \
	"

inherit runit-services

do_install_append() {
	if ${@bb.utils.contains('PACKAGES', 'busybox-runit-init', "true", "false", d)}; then
		install -d ${D}${base_sbindir}
		install -d ${D}${sysconfdir}
		install -d ${D}${sysconfdir}/default
		install -d ${D}${sysconfdir}/default/persistent
		install -d ${D}${libexecdir}/getty-tty
		# Holds state information pertaining to urandom
		install -d ${D}${localstatedir}/lib/urandom

		install -m 0755 ${WORKDIR}/runit-init/init ${D}${base_sbindir}/init.${BPN}
		install -m 0755 ${WORKDIR}/runit-init/telinit ${D}${base_sbindir}/telinit.${BPN}
		install -m 0755 ${WORKDIR}/runit-init/sulogin.sh ${D}${base_sbindir}/sulogin.sh
		install -m 0755 ${WORKDIR}/runit-init/sushell ${D}${base_sbindir}/sushell
		install -m 0755 ${WORKDIR}/runit-init/rc.local.sample ${D}${sysconfdir}/rc.local
		install -m 0644 ${WORKDIR}/runit-init/rcS-default ${D}${sysconfdir}/default/rcS
		install -m 0755 ${WORKDIR}/runlevel ${D}${base_sbindir}/runlevel
		install -m 0755 ${WORKDIR}/runit-init/initsig ${D}${base_sbindir}/initsig
		for iscript in ${D}${base_sbindir}/init.${BPN} ${D}${base_sbindir}/initsig; do
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

		consoles="115200;tty1 115200;tty2 115200;tty3 115200;tty4 ${SERIAL_CONSOLES}"
		for console in $consoles; do
			oIFS="$IFS"
			IFS=";"
			set -- $console
			IFS="$oIFS"
			install -d ${D}${sysconfdir}/service/getty-$2-$1
			ln -sf ${sysconfdir}/sv/service/getty-tty.run ${D}${sysconfdir}/service/getty-$2-$1/run
		done

		# Common log service for services that need it.
		install -d ${D}${sysconfdir}/sv
		install -m 0755 ${WORKDIR}/runit-init/runsvlog.run ${D}${sysconfdir}/sv/service

		install -m 0644 ${WORKDIR}/runit-init/persistent ${D}${sysconfdir}/default/persistent/15_busybox
		install -m 0644 ${WORKDIR}/runit-init/logrotate-dmesg.conf ${D}${sysconfdir}/

		# Make sure timestamp exists
		touch ${D}${sysconfdir}/timestamp
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

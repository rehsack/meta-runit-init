RUNIT_SERVICES ?= ""

FILES_${PN} =+ "${sysconfdir}/sv ${sysconfdir}/service ${localstatedir}/service"

do_install_append() {
	if ${@bb.utils.contains('VIRTUAL-RUNTIME_init_manager', 'busybox-runit-init', "true", "false", d)}; then
		install -d ${D}${sysconfdir}/sv/service
		install -d ${D}${sysconfdir}/service
		install -d ${D}${localstatedir}/service
		for service in ${RUNIT_SERVICES}; do
			log=0
			once=""
			oncename="$(basename "$service" .log)"
			if [ "$oncename" != "$service" ]; then
				log=1
			fi
			servicename="$(basename "$oncename" .once)"
			if [ "$servicename" != "$oncename" ]; then
				once=1-
                        else
				once=5-
			fi
			install -d ${D}${sysconfdir}/service/${once}${servicename}
			install -m 0755 ${WORKDIR}/runit-init/${servicename}.run ${D}${sysconfdir}/sv/service
			ln -sf ${sysconfdir}/sv/service/${servicename}.run ${D}${sysconfdir}/service/${once}${servicename}/run

			if [ "$log" = "1" ]; then
				install -d ${D}${sysconfdir}/service/${once}${servicename}/log
				ln -sf ${sysconfdir}/sv/service/runsvlog.run ${D}${sysconfdir}/service/${once}${servicename}/log/run
			fi
		done
		if ${@bb.utils.contains('DISTRO_FEATURES', 'read-only-rootfs', 'false', 'true', d)}; then
			for sv in $(ls -A ${D}${sysconfdir}/service); do
				mkdir -p ${D}${localstatedir}/service/$sv
				ln -sf ${sysconfdir}/service/$sv/run ${D}${localstatedir}/service/$sv/
			done
		fi
	fi
}

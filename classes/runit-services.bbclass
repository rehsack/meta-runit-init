RUNIT_SERVICES ?= ""

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
				once=1
			fi
			install -d ${D}${sysconfdir}/service/${once:+1-}${servicename}
			install -m 0755 ${WORKDIR}/runit-init/${servicename}.run ${D}${sysconfdir}/sv/service
			ln -sf ${sysconfdir}/sv/service/${servicename}.run ${D}${sysconfdir}/service/${once:+1-}${servicename}/run

			if [ "$log" = "1" ]; then
       	                	install -d ${D}${sysconfdir}/service/${once:+1-}${servicename}/log
       	                	ln -sf ${sysconfdir}/sv/service/runsvlog.run ${D}${sysconfdir}/service/${once:+1-}${servicename}/log/run
			fi
		done
	fi
}

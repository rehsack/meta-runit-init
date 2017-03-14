#FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

ALTERNATIVE_${PN} = "init telinit halt poweroff shutdown"
ALTERNATIVE_PRIORITY[init] = "80"
ALTERNATIVE_LINK_NAME[init] = "${base_sbindir}/init"
ALTERNATIVE_PRIORITY[telinit] = "80"
ALTERNATIVE_LINK_NAME[telinit] = "${base_sbindir}/sulogin"
ALTERNATIVE_PRIORITY[halt] = "80"
ALTERNATIVE_LINK_NAME[halt] = "${base_sbindir}/halt"
ALTERNATIVE_PRIORITY[poweroff] = "80"
ALTERNATIVE_LINK_NAME[poweroff] = "${base_sbindir}/poweroff"
ALTERNATIVE_PRIORITY[reboot] = "80"
ALTERNATIVE_LINK_NAME[reboot] = "${base_sbindir}/reboot"
ALTERNATIVE_PRIORITY[shutdown] = "80"
ALTERNATIVE_LINK_NAME[shutdown] = "${base_sbindir}/shutdown"
ALTERNATIVE_${PN}-sulogin = "sulogin"
ALTERNATIVE_PRIORITY_${PN}-sulogin[sulogin] = "80"
ALTERNATIVE_LINK_NAME_${PN}-sulogin[sulogin] = "${base_sbindir}/sulogin"


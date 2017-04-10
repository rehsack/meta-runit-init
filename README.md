# meta-runit-init

## 0. About

An alternative to standard rc-scripts (e.g. busybox-init or sysvinit)
or systemd (which is rather large), this layer allow the use of the
busybox-equivalents of the 'runit' package as the main init system.
(NB: runit itself is not in Yocto/OE, but busybox version and
daemontools are).

## 1. Prerequisites

Before runit-init can run, /var/service must be mounted read-write.
This can be managed by a related layer (meta-earlyinit) or in an
initramfs.  If this is not the case then runit-init will mount a
tmpfs overlay on /var/service so that runsv and friends can succeed.

## 2. Usage

1.  In the init script:

    a.  If you have an rc.d initscript modify it to detect running
        as runit script (if [ "$(basename "${0}")" = "run" ]; then),
    b.  Include the helper functions (. /etc/init.d/functions-ri),
    c.  In the script, Verify any prerequisites (e.g.
        ``prereqok 5-syslog || exit 1``
        (which verify syslog is running an exit with error if it
        is not, which mean runsv will retry running the script until
        the condition is met).)
    d.  If the script on only meant to be automatically run at startup
        (rather than respawning daemons on exit), then end the script
        with the 'markran' helper function.
    e.  NB: single run scripts will be prefixed by 1- and scripts
        that monitor a process will be prefixed by 5- (e.g. where
        you exec a command that never forks).

2.  In the .bb or .bbappend,

    a. to SRC_URI add "file://runit-init/<service-name>.run", where
       <recipe-extra-files-dir>/runit-init/<service-name>.run can be
       a symlink to e.g. an existing initscript in <recipe-extra-files-dir>.
    b. add RUNIT_SERVICES += "<servicename>[.once][.log]"
       (e.g. psplash.once).  The optional .once means that the script doesn't
       respawn on exit, and .log means that stdout and stderr are sent
       to syslog with <servicename> as prefix.
   c.  inherit runit-services

The result of 2. is to copy the <service-name>.run to
/etc/sv/service/<service-name>.run and to symlink
/var/service/<service-name>/run (creating dir if necessary),
to </etc/sv/service/<service-name>.run.  If with .once, then
/var/service/1-<service-name>/run is used instead.

# meta-runit-init

As an alternative to busybox-init or similar but larger sysvinit, or huge systemd, this layer allows the use of the busybox-equivalents of the 'runit' package (not in OE/Yocto; but busybox versions are) as the main init system. Assumes initramfs or a related layers (meta-earlyinit) have brought up essential filesystems like /proc, /sys, etc and made /var/service read-write with the 'runit' startup scripts on it. Currently considered alpha, although it does successfully bring pre-release cshored distro up to multi-user.

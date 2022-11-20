
# kernel-module-template

https://www.thegeekstuff.com/2013/07/write-linux-kernel-module/

https://wiki.archlinux.org/title/Compile_kernel_module#Build_environment

https://tldp.org/LDP/lkmpg/2.4/html/x208.htm

https://olegkutkov.me/2018/03/14/simple-linux-character-device-driver/

https://olegkutkov.me/2020/02/10/linux-block-device-driver/

https://linux-kernel-labs.github.io/refs/heads/master/labs/filesystems_part2.html#directory-inodes-operations

This gives a useful overview of how to iterate over the contents of a directory
from its inode.

---

Note for Docker testing:
 - `/dev/yeet` will not appear immediately after running `insmod yeet.ko`,
   however the yeet character device will appear at:

        `/sys/devices/virtual/`

   after it has been created it can be symlinked (hardlinked?)
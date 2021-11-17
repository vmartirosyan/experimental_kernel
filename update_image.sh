#!/bin/bash
cd source
make clean
make
cd ..
sudo umount /mnt2
sudo /sbin/losetup /dev/loop0 floppy_module.img
sudo mount /dev/loop0 /mnt2
sudo cp source/kernel.bin /mnt2/kernel
sudo cp mk_initrd_img/initrd.img /mnt2/initrd
sudo umount /dev/loop0
sudo /sbin/losetup -d /dev/loop0

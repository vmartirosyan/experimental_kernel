#!/bin/bash

#install grub on the floppy image

# boot device
dd if=/dev/zero of=floppy.img bs=512 count=2880

sudo mke2fs floppy.img

sudo mount floppy.img /mnt/floppy -o loop
sudo mkdir -p /mnt/floppy/boot/grub
sudo cp stage1 stage2 menu.lst grub.conf /mnt/floppy/boot/grub
sudo umount -f /mnt/floppy

# auxiliary floppy disk
dd if=/dev/zero of=aux_floppy.img bs=512 count=2
sudo mke2fs aux_floppy.img
dd if=stage1 of=aux_floppy.img bs=512 count=1
dd if=stage2 of=aux_floppy.img bs=512 seek=1

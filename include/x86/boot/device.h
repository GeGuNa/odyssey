/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BOOTDEV_H
#define BOOTDEV_H

struct boot_device {
	size_t biosdev;
	size_t partition;
	size_t sub_partition;
} __attribute__((packed));

#endif

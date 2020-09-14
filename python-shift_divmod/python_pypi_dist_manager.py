#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2020 Shlomi Fish <shlomif@cpan.org>
#
# Distributed under terms of the MIT license.

import sys

from pydistman import DistManager

try:
    cmd = sys.argv.pop(1)
except IndexError:
    cmd = 'build'

dist_name = "shift_divmod"

obj = DistManager(
    dist_name=dist_name,
    dist_version="0.4.0",
    project_name="ShiftDivMod",
    project_short_description=(
        "Implement faster divmod() for moduli with trailing 0 bits"
    ),
    release_date="2020-09-14",
    project_year="2020",
    aur_email="shlomif@cpan.org",
    project_email="shlomif@cpan.org",
    full_name="Shlomi Fish",
    github_username="shlomif",
)
obj.run_command(cmd=cmd, args=[])

/**
 * Seccomp Library test program
 *
 * Copyright (c) 2012 Red Hat <pmoore@redhat.com>
 * Author: Paul Moore <paul@paul-moore.com>
 */

/*
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License as
 * published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses>.
 */

#include <errno.h>
#include <unistd.h>

#include <seccomp.h>

#include "util.h"

int main(int argc, char *argv[])
{
	int rc;
	struct util_options opts;
	scmp_filter_ctx ctx = NULL;

	rc = util_getopt(argc, argv, &opts);
	if (rc < 0)
		goto out;

	ctx = seccomp_init(SCMP_ACT_KILL);
	if (ctx == NULL)
		return ENOMEM;

	/* NOTE: not strictly necessary since we get the native arch by default
	 *       but it serves as a good sanity check for the code and boosts
	 *       our code coverage numbers */
	rc = seccomp_arch_exist(ctx, seccomp_arch_native());
	if (rc != 0)
		goto out;

	rc = seccomp_arch_remove(ctx, SCMP_ARCH_NATIVE);
	if (rc != 0)
		goto out;

	/* NOTE: we are using a different approach to test for the native arch
	 *       to exercise slightly different code paths */
	rc = seccomp_arch_exist(ctx, 0);
	if (rc != -EEXIST)
		goto out;

	/* NOTE: more sanity/coverage tests (see above) */
	rc = seccomp_arch_add(ctx, SCMP_ARCH_NATIVE);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_NATIVE);
	if (rc != 0)
		goto out;

	rc = seccomp_arch_add(ctx, SCMP_ARCH_X86);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_X86_64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_X32);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_ARM);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_AARCH64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_LOONGARCH64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_MIPSEL);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_MIPSEL64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_MIPSEL64N32);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_PPC64LE);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_RISCV64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_add(ctx, SCMP_ARCH_SH);
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 1,
			      SCMP_A0(SCMP_CMP_EQ, STDIN_FILENO));
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,
			      SCMP_A0(SCMP_CMP_EQ, STDOUT_FILENO));
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,
			      SCMP_A0(SCMP_CMP_EQ, STDERR_FILENO));
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(socket), 0);
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(connect), 0);
	if (rc != 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(shutdown), 0);
	if (rc != 0)
		goto out;

	rc = util_filter_output(&opts, ctx);
	if (rc)
		goto out;

	/* not strictly necessary, but let's exercise the code paths */
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_X86);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_X86_64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_X32);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_ARM);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_AARCH64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_LOONGARCH64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_MIPSEL);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_MIPSEL64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_MIPSEL64N32);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_PPC64LE);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_RISCV64);
	if (rc != 0)
		goto out;
	rc = seccomp_arch_remove(ctx, SCMP_ARCH_SH);
	if (rc != 0)
		goto out;

out:
	seccomp_release(ctx);
	return (rc < 0 ? -rc : rc);
}

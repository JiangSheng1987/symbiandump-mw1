/*	$Id: utils.c,v 1.2 2000/08/14 16:26:08 amai Exp $	*/
/*	$NetBSD: utils.c,v 1.6 1997/02/26 14:40:51 cgd Exp $	*/

/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


/*-
 * Copyright (c) 1991, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgment:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if 0
static char sccsid[] = "@(#)utils.c	8.3 (Berkeley) 4/1/94";
static char rcsid[] = "$OpenBSD: utils.c,v 1.14 1999/05/06 17:19:47 millert Exp $";
#endif

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include "fts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "extern.h"
#ifdef __SYMBIAN32__
#ifdef __ARMCC__
// Turn off the 'extended constant initialiser used' warning
#pragma diag_remark 1294
#endif
#endif

int
copy_file(entp, dne)
	FTSENT *entp;
	int dne;
{
	static char buf[MAXBSIZE];
	struct stat to_stat, *fs;
	int ch, checkch, from_fd, rcount, rval, to_fd, wcount;
#ifdef VM_AND_BUFFER_CACHE_SYNCHRONIZED
	char *p;
#endif

	if ((from_fd = open(entp->fts_path, O_RDONLY, 0)) == -1) {
		printf("cp: ");
		warn("%s", entp->fts_path);
		return (1);
	}

	fs = entp->fts_statp;

	/*
	 * In -f (force) mode, we always unlink the destination first
	 * if it exists.  Note that -i and -f are mututally exclusive.
	 */
	if (!dne && fflag)
		(void)unlink(to.p_path);

	/*
	 * If the file exists and we're interactive, verify with the user.
	 * If the file DNE, set the mode to be the from file, minus setuid
	 * bits, modified by the umask; arguably wrong, but it makes copying
	 * executables work right and it's been that way forever.  (The
	 * other choice is 666 or'ed with the execute bits on the from file
	 * modified by the umask.)
	 */
	if (!dne && !fflag) {
		if (iflag) {
			(void)fprintf(stderr, "overwrite %s? ", to.p_path);
			checkch = ch = getchar();
			while (ch != '\n' && ch != EOF)
				ch = getchar();
			if (checkch != 'y' && checkch != 'Y') {
				(void)close(from_fd);
				return (0);
			}
		}
		to_fd = open(to.p_path, O_WRONLY | O_TRUNC, 0);
	} else
		to_fd = open(to.p_path, O_WRONLY | O_TRUNC | O_CREAT,
		    fs->st_mode & ~(S_ISTXT | S_ISUID | S_ISGID));

	if (to_fd == -1) {
		printf("cp: ");
		warn("%s", to.p_path);
		(void)close(from_fd);
		return (1);;
	}

	rval = 0;

	/*
	 * Mmap and write if less than 8M (the limit is so we don't totally
	 * trash memory on big files.  This is really a minor hack, but it
	 * wins some CPU back.
	 */
#ifdef VM_AND_BUFFER_CACHE_SYNCHRONIZED
	if (fs->st_size <= 8 * 1048576) {
		if ((p = mmap(NULL, (size_t)fs->st_size, PROT_READ,
		    0, from_fd, (off_t)0)) == MAP_FAILED) {
		    printf("cp: ");
			warn("mmap: %s", entp->fts_path);
			rval = 1;
		} else {
			if (write(to_fd, p, fs->st_size) != fs->st_size) {
				printf("cp: ");
				warn("%s", to.p_path);
				rval = 1;
			}
			/* Some systems don't unmap on close(2). */
			if (munmap(p, fs->st_size) < 0) {
				printf("cp: ");			
				warn("%s", entp->fts_path);
				rval = 1;
			}
		}
	} else
#endif
	{
		while ((rcount = read(from_fd, buf, MAXBSIZE)) > 0) {
			wcount = write(to_fd, buf, rcount);
			if (rcount != wcount || wcount == -1) {
				printf("cp: ");						
				warn("%s", to.p_path);
				rval = 1;
				break;
			}
		}
		if (rcount < 0) {
			printf("cp: ");			
			warn("%s", entp->fts_path);
			rval = 1;
		}
	}

	if (rval == 1) {
		(void)close(from_fd);
		(void)close(to_fd);
		return (1);
	}

	if (pflag && setfile(fs, to_fd))
		rval = 1;
	/*
	 * If the source was setuid or setgid, lose the bits unless the
	 * copy is owned by the same user and group.
	 */
#define	RETAINBITS \
	(S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
	else if (fs->st_mode & (S_ISUID | S_ISGID) && fs->st_uid == myuid)
		if (fstat(to_fd, &to_stat)) {
			printf("cp: ");			
			warn("%s", to.p_path);
			rval = 1;
		} else if (fs->st_gid == to_stat.st_gid &&
		    fchmod(to_fd, fs->st_mode & RETAINBITS & ~myumask)) {
		    printf("cp: ");			
			warn("%s", to.p_path);
			rval = 1;
		}
	(void)close(from_fd);
	(void)close(to_fd);

#ifndef __SYMBIAN32__
	if (close(to_fd)) {
		printf("cp: ");			
		warn("%s", to.p_path);
		rval = 1;
	}
#endif //__SYMBIAN32__
	return (rval);
}

int
copy_link(p, exists)
	FTSENT *p;
	int exists;
{
	int len;
	char link[MAXPATHLEN];

	if ((len = readlink(p->fts_path, link, sizeof(link)-1)) == -1) {
		printf("cp: ");			
		warn("readlink: %s", p->fts_path);
		return (1);
	}
	link[len] = '\0';
	if (exists && unlink(to.p_path)) {
		printf("cp: ");				
		warn("unlink: %s", to.p_path);
		return (1);
	}
	if (symlink(link, to.p_path)) {
		printf("cp: ");				
		warn("symlink: %s", link);
		return (1);
	}
	return (pflag ? setlink(p->fts_statp) : 0);
}

int
copy_fifo(from_stat, exists)
	struct stat *from_stat;
	int exists;
{
	if (exists && unlink(to.p_path)) {
		printf("cp: ");			
		warn("unlink: %s", to.p_path);
		return (1);
	}
	if (mkfifo(to.p_path, from_stat->st_mode)) {
		printf("cp: ");			
		warn("mkfifo: %s", to.p_path);
		return (1);
	}
	return (pflag ? setfile(from_stat, 0) : 0);
}

int
copy_special(from_stat, exists)
	struct stat *from_stat;
	int exists;
{
	if (exists && unlink(to.p_path)) {
		printf("cp: ");			
		warn("unlink: %s", to.p_path);
		return (1);
	}
#ifndef __SYMBIAN32__	
	if (mknod(to.p_path, from_stat->st_mode, from_stat->st_rdev)) {
#else	
	if (open(to.p_path, from_stat->st_mode)) {
#endif	
		printf("cp: ");			
		warn("open: %s", to.p_path);
		return (1);
	}
	return (pflag ? setfile(from_stat, 0) : 0);
}


int
setfile(fs, fd)
	struct stat *fs;
	int fd;
{
	static struct timeval tv[2];
	int rval;

	rval = 0;
	fs->st_mode &= S_ISTXT | S_ISUID | S_ISGID | S_IRWXU | S_IRWXG | S_IRWXO;

	TIMESPEC_TO_TIMEVAL(&tv[0], &fs->st_atimespec);
	TIMESPEC_TO_TIMEVAL(&tv[1], &fs->st_mtimespec);
	
	/*
	 * Changing the ownership probably won't succeed, unless we're root
	 * or POSIX_CHOWN_RESTRICTED is not set.  Set uid/gid before setting
	 * the mode; current BSD behavior is to remove all setuid bits on
	 * chown.  If chown fails, lose setuid/setgid bits.
	 */
#ifndef __SYMBIAN32__	 
	if (fd ? fchown(fd, fs->st_uid, fs->st_gid) :
	    chown(to.p_path, fs->st_uid, fs->st_gid)) {
		if (errno != EPERM) {
			warn("chown: %s", to.p_path);
			rval = 1;
		}
		fs->st_mode &= ~(S_ISTXT | S_ISUID | S_ISGID);
	}
#endif	
	if (fd ? fchmod(fd, fs->st_mode) : chmod(to.p_path, fs->st_mode)) {
		printf("cp: ");			
		warn("chown: %s", to.p_path);
		rval = 1;
	}

	/*
	 * XXX
	 * NFS doesn't support chflags; ignore errors unless there's reason
	 * to believe we're losing bits.  (Note, this still won't be right
	 * if the server supports flags and we were trying to *remove* flags
	 * on a file that we copied, i.e., that we didn't create.)
	 */
	errno = 0;
#ifndef __SYMBIAN32__	
	if (fd ? fchflags(fd, fs->st_flags) : chflags(to.p_path, fs->st_flags))
		if (errno != EOPNOTSUPP || fs->st_flags != 0) {
			printf("cp: ");			
			warn("chflags: %s", to.p_path);
			rval = 1;
		}
#endif		
#ifdef __SYMBIAN32__	
	if(fd>2)  //ignore stdin, stdout, stderr
		close(fd); //close the fd before updating the time stamp
#endif		
	if (utimes(to.p_path, tv) && errno!=EACCES) {
		printf("cp: ");
		warn("utimes: %s", to.p_path);
		rval = 1;
	}
	return (rval);
}


int
setlink(fs)
	register struct stat *fs;
{

	if (lchown(to.p_path, fs->st_uid, fs->st_gid)) {
		if (errno != EPERM) {
			printf("cp: ");					
			warn("lchown: %s", to.p_path);
			return (1);
		}
	}
	return (0);
}


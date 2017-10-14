/* Copyright (c) 2002, 2004 Joerg Wunsch
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in
 the documentation and/or other materials provided with the
 distribution.

 * Neither the name of the copyright holders nor the names of
 contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

/* Taken from avr-libc's lightweight allocator */

#include <string.h>
#include <stdlib.h>
#include <FreeRTOS.h>

/* Must have this many bytes between the stack and heap */
#define MALLOC_MARGIN 256
/* = 4 */
#define PTR_SIZE (sizeof(size_t))
/* Size of freelist linked list entry */
#define FL_SIZE (sizeof(FreeList_TypeDef))

/* Entry into the free list */
typedef struct __freelist {
	size_t sz;
	struct __freelist *nx;
} FreeList_TypeDef;

/* End of allocated region pointer */
char* _heapEnd;
/* End of memory pointer */
extern char _estack;
/* Free list of chunks not used */
static FreeList_TypeDef *freeList = NULL;

static void* _malloc(size_t len) {
	FreeList_TypeDef *fp1, *fp2;
	char *cp;
	size_t s, avail;
	/*
	 * Our minimum chunk size is the size of a pointer (plus the
	 * size of the "sz" field, but we don't need to account for
	 * this), otherwise we could not possibly fit a freelist entry
	 * into the chunk later.
	 */
	len = ((len / PTR_SIZE) + 1) * PTR_SIZE;
	/*
	 * First, walk the free list and try finding a chunk that
	 * would match exactly.  If we found one, we are done.  While
	 * walking, note down the size of the largest chunk we found
	 * that would still fit the request -- we need it for step 2.
	 */
	for (s = 0, fp1 = freeList, fp2 = NULL; fp1 != NULL; fp2 = fp1, fp1 = fp1->nx) {
		if (fp1->sz == len) {
			/*
			 * Found it.  Disconnect the chunk from the
			 * freelist, and return it.
			 */
			if (fp2)
				fp2->nx = fp1->nx;
			else
				freeList = fp1->nx;
			return &(fp1->nx);
		}
		if (fp1->sz > len) {
			if (s == 0 || fp1->sz < s)
				s = fp1->sz;
		}
	}
	/*
	 * Step 2: If we found a chunk on the freelist that would fit
	 * (but was too large), look it up again and use it, since it
	 * is our closest match now.  Since the freelist entry needs
	 * to be split into two entries then, watch out that the
	 * difference between the requested size and the size of the
	 * chunk found is large enough for another freelist entry; if
	 * not, just enlarge the request size to what we have found,
	 * and use the entire chunk.
	 */
	if (s) {
		if (s - len < FL_SIZE)
			len = s;
		for (fp1 = freeList, fp2 = NULL; fp1 != NULL; fp2 = fp1, fp1 = fp1->nx) {
			if (fp1->sz == s) {
				if (len == s) {
					/*
					 * Use entire chunk; same as
					 * above.
					 */
					if (fp2)
						fp2->nx = fp1->nx;
					else
						freeList = fp1->nx;
					return &(fp1->nx);
				}
				/*
				 * Split them up.  Note that we leave
				 * the first part as the new (smaller)
				 * freelist entry, and return the
				 * upper portion to the caller.  This
				 * saves us the work to fix up the
				 * freelist chain; we just need to
				 * fixup the size of the current
				 * entry, and note down the size of
				 * the new chunk before returning it
				 * to the caller.
				 */
				s -= len;
				cp = (char*)fp1 + s;
				fp2 = (FreeList_TypeDef*)cp;
				fp2->sz = len;
				fp1->sz = s - PTR_SIZE;
				return &(fp2->nx);
			}
		}
	}
	/*
	 * Step 3: If the request could not be satisfied from a
	 * freelist entry, just prepare a new chunk. This means we
	 * need to obtain more memory first. Do not check the end of
	 * stack value, as the OS might have that in a dynamic block.
	 */
	cp = &_estack - MALLOC_MARGIN;
	if (cp <= _heapEnd)
		/* Don't subtract unsigned from unsigned. */
		return NULL;
	avail = cp - _heapEnd;
	/* Allocate new chunk */
	if (avail >= len) {
		fp1 = (FreeList_TypeDef*)_heapEnd;
		_heapEnd += len + PTR_SIZE;
		fp1->sz = len;
		return &(fp1->nx);
	}
	return NULL;
}

static void _free(void *p) {
	FreeList_TypeDef *fp1, *fp2, *fpnew;
	char *cp1, *cp2, *cpnew;
	/* ISO C says free(NULL) must be a no-op */
	if (p == NULL)
		return;
	cpnew = p - PTR_SIZE;
	fpnew = (FreeList_TypeDef*)cpnew;
	fpnew->nx = NULL;
	/*
	 * Trivial case first: if there's no freelist yet, our entry
	 * will be the only one on it.  If this is the last entry, we
	 * can reduce heapEnd instead.
	 */
	if (freeList == NULL) {
		if ((char*)p + fpnew->sz == _heapEnd)
			_heapEnd = cpnew;
		else
			freeList = fpnew;
		return;
	}
	/*
	 * Now, find the position where our new entry belongs onto the
	 * freelist.  Try to aggregate the chunk with adjacent chunks
	 * if possible.
	 */
	for (fp1 = freeList, fp2 = NULL; fp1; fp2 = fp1, fp1 = fp1->nx)
		if (fp1 >= fpnew) {
			cp1 = (char*)fp1;
			fpnew->nx = fp1;
			if ((char*)&(fpnew->nx) + fpnew->sz == cp1) {
				/* upper chunk adjacent, assimilate it */
				fpnew->sz += fp1->sz + PTR_SIZE;
				fpnew->nx = fp1->nx;
			}
			if (fp2 == NULL) {
				/* new head of freelist */
				freeList = fpnew;
				return;
			}
			break;
		}
	/*
	 * Note that we get here either if we hit the "break" above,
	 * or if we fell off the end of the loop.  The latter means
	 * we've got a new topmost chunk.  Either way, try aggregating
	 * with the lower chunk if possible.
	 */
	fp2->nx = fpnew;
	cp2 = (char*)&(fp2->nx);
	if (cp2 + fp2->sz == cpnew) {
		/* lower chunk adjacent, merge */
		fp2->sz += fpnew->sz + PTR_SIZE;
		fp2->nx = fpnew->nx;
	}
	/*
	 * If there's a new topmost chunk, lower endOfMemory instead.
	 */
	for (fp1 = freeList, fp2 = NULL; fp1->nx != NULL; fp2 = fp1, fp1 = fp1->nx);
	cp2 = (char*)&(fp1->nx);
	if (cp2 + fp1->sz == _heapEnd) {
		if (fp2 == NULL)
			/* Freelist is empty now. */
			freeList = NULL;
		else
			fp2->nx = NULL;
		_heapEnd = cp2 - PTR_SIZE;
	}
}

/* free() proper uses critical sections to avoid clashes */
void free(void *ptr) {
	/* Get in line for changes. */
	_enterCritical();
	{
		_free(ptr);
	}
	_exitCritical();
}

/* malloc() proper uses critical sections to avoid clashes */
void* malloc(size_t len) {
	void *ret;
	/* Get in line for changes. */
	_enterCritical();
	{
		ret = _malloc(len);
	}
	_exitCritical();
	return ret;
}

void* realloc(void *ptr, size_t len) {
	_enterCritical();
	FreeList_TypeDef *fp1, *fp2, *fp3, *ofp3;
	char *cp, *cp1;
	void *memp;
	size_t s, incr;
	/* Trivial case, required by C standard. */
	if (ptr == NULL)
		memp = _malloc(len);
	else {
		cp1 = (char*)ptr;
		cp1 -= PTR_SIZE;
		fp1 = (FreeList_TypeDef*) cp1;
		cp = (char*)ptr + len; /* new next pointer */
		/* Pointer wrapped across top of RAM, fail. */
		/*if (cp < cp1)
			return NULL;*/
		fp2 = (FreeList_TypeDef*)(cp - PTR_SIZE);
		/*
		 * See whether we are growing or shrinking.  When shrinking,
		 * we split off a chunk for the released portion, and call
		 * free() on it.  Therefore, we can only shrink if the new
		 * size is at least sizeof(FreeList_TypeDef) smaller than the
		 * previous size.
		 */
		if (len <= fp1->sz) {
			/* The first test catches a possible unsigned int
			 * rollover condition. */
			if (fp1->sz <= FL_SIZE || len > fp1->sz - FL_SIZE)
				return ptr;
			fp2->sz = fp1->sz - len - PTR_SIZE;
			fp1->sz = len;
			_free(&(fp2->nx));
			_exitCritical();
			return ptr;
		}
		/*
		 * If we get here, we are growing.  First, see whether there
		 * is space in the free list on top of our current chunk.
		 */
		incr = len - fp1->sz;
		cp = (char*)ptr + fp1->sz;
		for (s = 0, ofp3 = NULL, fp3 = freeList; fp3; ofp3 = fp3, fp3 = fp3->nx) {
			if (fp3 == fp2 && fp3->sz >= incr) {
				/* found something that fits */
				if (incr <= fp3->sz + PTR_SIZE) {
					/* it just fits, so use it entirely */
					fp1->sz += fp3->sz + PTR_SIZE;
					if (ofp3)
						ofp3->nx = fp3->nx;
					else
						freeList = fp3->nx;
					_exitCritical();
					return ptr;
				}
				/* split off a new freelist entry */
				cp = (char*)ptr + len;
				fp2 = (FreeList_TypeDef*)(cp - PTR_SIZE);
				fp2->nx = fp3->nx;
				fp2->sz = fp3->sz - incr - PTR_SIZE;
				if (ofp3 != NULL)
					ofp3->nx = fp2;
				else
					freeList = fp2;
				fp1->sz = len;
				_exitCritical();
				return ptr;
			}
			/*
			 * Find the largest chunk on the freelist while
			 * walking it.
			 */
			if (fp3->sz > s)
				s = fp3->sz;
		}
		/*
		 * If we are the topmost chunk in memory, and there was no
		 * large enough chunk on the freelist that could be re-used
		 * (by a call to malloc() below), quickly extend the
		 * allocation area if possible, without need to copy the old
		 * data.
		 */
		if (_heapEnd == (char*)ptr + fp1->sz && len > s) {
			cp = (char*)ptr + len;
			cp1 = &_estack - MALLOC_MARGIN;
			if (cp < cp1) {
				_heapEnd = cp;
				fp1->sz = len;
				memp = ptr;
			} else
				memp = NULL;
		} else {
			/*
			 * Call malloc() for a new chunk, then copy over the data, and
			 * release the old region.
			 */
			memp = _malloc(len);
			if (memp != NULL) {
				memcpy(memp, ptr, fp1->sz);
				_free(ptr);
			}
		}
	}
	_exitCritical();
	return memp;
}

void* calloc(size_t num, size_t size) {
	/* Calculate size of memory. Does not check for overflow. */
	size_t bytes = num * size;
	void* mem = malloc(bytes);
	if(!mem) {
		/* malloc failed return NULL */
		return NULL;
	}
	/* Initialize Memory to 0 */
	memset(mem, 0, bytes);
	return mem;
}

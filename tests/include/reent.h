#ifndef REENT_HEADER
#define REENT_HEADER

extern struct _reent* const _global_impure_ptr __ATTRIBUTE_IMPURE_PTR__;
#define _GLOBAL_REENT _global_impure_ptr


//#define _REENT_EMERGENCY_SIZE 25
//#define _REENT_ASCTIME_SIZE 26
//
//#define _ATEXIT_SIZE 32 /* must be at least 32 to guarantee ANSI conformance */
//
///* needed by reentrant structure */
//struct __tm {
//	int __tm_sec;
//	int __tm_min;
//	int __tm_hour;
//	int __tm_mday;
//	int __tm_mon;
//	int __tm_year;
//	int __tm_wday;
//	int __tm_yday;
//	int __tm_isdst;
//};
//
//struct _rand48 {
//	unsigned short _seed[3];
//	unsigned short _mult[3];
//	unsigned short _add;
//#ifdef _REENT_SMALL
//	/* Put this in here as well, for good luck.  */
//	__extension__ unsigned long long _rand_next;
//#endif
//};
//
//struct _glue {
//	struct _glue* _next;
//	int _niobs;
//	__FILE* _iobs;
//};
//
//struct _atexit {
//	struct _atexit* _next; /* next in list */
//	int _ind;              /* next index in this table */
//	/* Some entries may already have been called, and will be NULL.  */
//	void (*_fns[_ATEXIT_SIZE])(void); /* the table itself */
//	struct _on_exit_args _on_exit_args;
//};
//
//typedef struct {
//	int __count;
//	union {
//		wint_t __wch;
//		unsigned char __wchb[4];
//	} __value; /* Value so far.  */
//} _mbstate_t;

struct _reent {
	int _errno; /* local copy of errno */

//	/* FILE is a big struct and may change over time.  To try to achieve binary
//	   compatibility with future versions, put stdin,stdout,stderr here.
//	   These are pointers into member __sf defined below.  */
//	__FILE *_stdin, *_stdout, *_stderr;
//
//	int _inc; /* used by tmpnam */
//	char _emergency[_REENT_EMERGENCY_SIZE];
//
//	/* TODO */
//	int _unspecified_locale_info; /* unused, reserved for locale stuff */
//	struct __locale_t* _locale;   /* per-thread locale */
//
//	int __sdidinit; /* 1 means stdio has been init'd */
//
//	void (*__cleanup)(struct _reent*);
//
//	/* used by mprec routines */
//	struct _Bigint* _result;
//	int _result_k;
//	struct _Bigint* _p5s;
//	struct _Bigint** _freelist;
//
//	/* used by some fp conversion routines */
//	int _cvtlen; /* should be size_t */
//	char* _cvtbuf;
//
//	union {
//		struct {
//			unsigned int _unused_rand;
//			char* _strtok_last;
//			char _asctime_buf[_REENT_ASCTIME_SIZE];
//			struct __tm _localtime_buf;
//			int _gamma_signgam;
//			__extension__ unsigned long long _rand_next;
//			struct _rand48 _r48;
//			_mbstate_t _mblen_state;
//			_mbstate_t _mbtowc_state;
//			_mbstate_t _wctomb_state;
//			char _l64a_buf[8];
//			char _signal_buf[_REENT_SIGNAL_SIZE];
//			int _getdate_err;
//			_mbstate_t _mbrlen_state;
//			_mbstate_t _mbrtowc_state;
//			_mbstate_t _mbsrtowcs_state;
//			_mbstate_t _wcrtomb_state;
//			_mbstate_t _wcsrtombs_state;
//			int _h_errno;
//		} _reent;
//		/* Two next two fields were once used by malloc.  They are no longer
//		   used. They are used to preserve the space used before so as to
//		   allow addition of new reent fields and keep binary compatibility.   */
//		struct {
//#define _N_LISTS 30
//			unsigned char* _nextf[_N_LISTS];
//			unsigned int _nmalloc[_N_LISTS];
//		} _unused;
//	} _new;
//
//#ifndef _REENT_GLOBAL_ATEXIT
//	/* atexit stuff */
//	struct _atexit* _atexit; /* points to head of LIFO stack */
//	struct _atexit _atexit0; /* one guaranteed table, required by ANSI */
//#endif
//
//	/* signal info */
//	void (**(_sig_func))(int);
//
//	/* These are here last so that __FILE can grow without changing the offsets
//	   of the above members (on the off chance that future binary compatibility
//	   would be broken otherwise).  */
//	struct _glue __sglue; /* root of glue chain */
//#ifndef _REENT_GLOBAL_STDIO_STREAMS
//	__FILE __sf[3]; /* first three file descriptors */
//#endif
};

#endif
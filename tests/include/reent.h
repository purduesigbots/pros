#ifndef REANT_HEADER_
#define REANT_HEADER_

extern __thread struct _reent* _REENT;

struct _reent 
{
	int _errno;
};

#endif  // !REANT_HEADER_

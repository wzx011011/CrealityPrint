#ifndef _CLOSEHOOK_1609725074904_H
#define _CLOSEHOOK_1609725074904_H

class CloseHook
{
public:
	virtual ~CloseHook() {}
	virtual void beforeClose() = 0;
};

#endif // _CLOSEHOOK_1609725074904_H
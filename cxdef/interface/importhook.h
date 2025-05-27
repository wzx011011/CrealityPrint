#ifndef _IMPORT_HOOK_1609725074904_H
#define _IMPORT_HOOK_1609725074904_H

class ImportHook
{
public:
	virtual ~ImportHook() {}
	virtual void importFiles() = 0;
	virtual void importText() = 0;
};

#endif // _IMPORT_HOOK_1609725074904_H
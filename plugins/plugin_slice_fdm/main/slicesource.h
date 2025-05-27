#ifndef _NULLSPACE_SLICESOURCE_1590261261108_H
#define _NULLSPACE_SLICESOURCE_1590261261108_H
#include <QtCore/QObject>
#include <QtCore/QList>

namespace us
{
	class ModelGroupInput;
	class GlobalSetting;
}

class SliceSource: public QObject
{
public:
	SliceSource(QObject* parent = nullptr);
	virtual ~SliceSource();

	bool needSlice();
	QList<const us::ModelGroupInput*> createGroupInputs(us::GlobalSetting* globset);
	QList<const us::ModelGroupInput*> createBeltGroupInputs(us::GlobalSetting* globset);
protected:
};

#endif // _NULLSPACE_SLICESOURCE_1590261261108_H

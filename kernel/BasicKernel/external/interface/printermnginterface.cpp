#include "printermnginterface.h"
#include "kernel/kernel.h"
#include "utils/printernetmng.h"

namespace creative_kernel
{
	void scanNetPrinter()
	{
		getKernel()->printerNetMng()->scanPrinter();
	}
}
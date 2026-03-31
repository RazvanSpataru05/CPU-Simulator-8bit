#include "ISAEntry.h"

const ISAEntry* GetISATable(int& outSize)
{
	outSize = sizeof(isaTable) / sizeof(ISAEntry);
	return isaTable;
}

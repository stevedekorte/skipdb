A Berkeley DB style database implemented with a skip list instead of a b-tree. It's small, portable and supports ACID transactions (with true durability).

The license is Revised BSD.

Example:

#include "SkipDB.h"

int main(void)
{
	Datum key;
	Datum value;
	int count;

	SkipDB *db = SkipDB_new();
	SkipDB_setPath_(db, "test.skipdb");
	SkipDB_open(db);
	
	// write
	
	SkipDB_beginTransaction(db);
	key = Datum_FromCString_("testKey");
	value = Datum_FromCString_("testValue");
	SkipDB_at_put_(db, key, value);
	SkipDB_commitTransaction(db);
	
	// read
	
	value = SkipDB_at_(db, key);
	
	// count
	
	count = SkipDB_count(db);

	// remove
	
	SkipDB_beginTransaction(db);
	SkipDB_removeAt_(db, key);
	SkipDB_commitTransaction(db);
	
	// there's also a cursor API
	// not shown in this example code

	SkipDB_close(db);
	
	return 0;
}

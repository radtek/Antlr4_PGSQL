
#include "BaseProtocol.h"
#include "PgSqlBuffer.h"

class PgSqlProtocol : public BaseProtocol {
public:
	bool CompleteFromDB(const char* data, const int len);
	void ParseFromDB();
	bool CompleteToDB(const char* data, const int len);
	void ParseToDB();

};
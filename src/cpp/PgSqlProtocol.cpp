
#include "PgSqlProtocol.h"
#include "PgSqlType.h"


bool PgSqlProtocol::CompleteFromDB(const char* data, const int len) {

	if (!Buffer_FromDB.get_B_complete())
	{
		cout << "CompleteFromDB   FALSE\n";
		Buffer_FromDB.Buffer_Write(data, len);
		return Buffer_FromDB.get_B_complete();

	}
	u_int f_from = 0;

	//cout<<data[f_from]<<endl;
	while (f_from < len) {
		if (data[f_from] == 0)
			f_from += To_int(data, f_from);
		else
			f_from += To_int(data, ++f_from);
		//cout<<f_from<<endl;

	}
	
	Buffer_FromDB.NewBuffer(f_from);
	Buffer_FromDB.Buffer_Write(data, len);
	if(Buffer_FromDB.get_B_complete()){
		PgSqlProtocol::ParseFromDB();
	}
	//return Buffer_FromDB.get_B_complete();

}

void  PgSqlProtocol::ParseFromDB() {
//	cout << "\n////////////////服务器发送数据////////////////\n";
	u_int len = Buffer_FromDB.get_B_length_need();
	u_int from = 0;
	char* data = Buffer_FromDB.Buffer_Read();
	while (from < len) {
		int type = data[from];
		switch (type) {
		// case 'R': {Authentication Type(data, from); Type.print(); break; }

		// case 'd': {CopyData Type(data, from); Type.print(); break; }
		// case 'c': {CopyDone Type(data, from); Type.print(); break; }
		// case 'K': {BackendKeyData Type(data, from); Type.print(); break; }
		// case '2': {BindComplete Type(data, from); Type.print(); break; }
		// case '3': {CloseComplete Type(data, from); Type.print(); break; }
		// case 'C': {CommandComplete Type(data, from); Type.print(); break; }
		// case 'G': {CopyInResponse Type(data, from); Type.print(); break; }
		// case 'H': {CopyOutResponse Type(data, from); Type.print(); break; }
		// case 'W': {CopyBothResponse Type(data, from); Type.print(); break; }
		// case 'D': {DataRow Type(data, from); Type.print(); break; }
		// case 'I': {EmptyQueryResponse Type(data, from); Type.print(); break; }
		// case 'V': {FunctionCallResponse Type(data, from); Type.print(); break; }
		// case 'n': {NoData Type(data, from); Type.print(); break; }
		// case 'N': {NoticeResponse Type(data, from); Type.print(); break; }
		// case 'A': {NotificationResponse Type(data, from); Type.print(); break; }
		// case 't': {ParameterDescription Type(data, from); Type.print(); break; }
		// case 'S': {ParameterStatus Type(data, from); Type.print(); break; }
		// case '1': {ParseComplete Type(data, from); Type.print(); break; }
		// case 's': {PortalSuspended Type(data, from); Type.print(); break; }
		// case 'Z': {ReadyForQuery Type(data, from); Type.print(); break; }
		// case 'T': {RowDescription Type(data, from); Type.print(); break; }

		default: 
			if (data[from] == 0) {
				//cout << "服务器发送未知类型------" << (u_int)data[from] << endl;;
				from += To_int(data, from);
			}
			else {
				//cout << "服务器发送未知类型------" <<(u_int) data[from] << endl;;
				from += To_int(data, from + 1) + 1;
			}
			break;
		}
	}
}

bool PgSqlProtocol::CompleteToDB(const char* data, const int len) {

	if (!Buffer_ToDB.get_B_complete())
	{
		cout << "CompleteToDB     FALSE\n";
		Buffer_ToDB.Buffer_Write(data, len);
		return Buffer_ToDB.get_B_complete();
	}
	u_int f_from = 0;
	const char *f_data = data;
	int length = 0;

	while (f_from < len) {
		if (data[f_from] == 0) {
			f_from += To_int(data, f_from);
		}
		else {
			f_from += To_int(data, ++f_from);
		}
	}

	Buffer_ToDB.NewBuffer(f_from);
	Buffer_ToDB.Buffer_Write(data, len);
	if( Buffer_ToDB.get_B_complete())
		PgSqlProtocol::ParseToDB();
	return Buffer_ToDB.get_B_complete();
}

void PgSqlProtocol::ParseToDB() {
	cout << "\n////////////////客户端发送数据////////////////\n";
	u_int len = Buffer_ToDB.get_B_length_need();
	u_int from = 0;
	char* data = Buffer_ToDB.Buffer_Read();
	while (from < len) {
		u_int type = data[from];
		switch (type) {
		case 0: {
			int temp = To_int(data, from);
			if (temp == 16) {
				CancelRequest Type(data, from); Type.print(); break;
			}
			else if (temp == 8) {
				SSLRequest Type(data, from); Type.print(); break;
			}
			else {
				cout << "start\n";
				StartupMessage Type(data, from); Type.print(); break;
			}
		}

		case 'X': {Terminate Type(data, from); Type.print(); break; }
		case 'Q': {Query Type(data, from); Type.print(); break; }
		case 'B': {Bind Type(data, from); Type.print(); break; }
		case 'd': {CopyData Type(data, from); Type.print(); break; }
		case 'c': {CopyDone Type(data, from); Type.print(); break; }
		case 'C': {Close Type(data, from); Type.print(); break; }
		case 'D': {Describle Type(data, from); Type.print(); break; }
		case 'E': {Execute Type(data, from); Type.print(); break; }
		case 'H': {Flush Type(data, from); Type.print(); break; }
		case 'F': {FunctionCall Type(data, from); Type.print(); break; }
		case 'P': {Parse Type(data, from); Type.print(); break; }
		case 'p': {PasswordMessage Type(data, from); Type.print(); break; }
		case 'S': {Sync Type(data, from); Type.print(); break; }

		default:
			if (data[from] == 0) {
				cout << "客户端发送未知类型------" << data[from] << endl;;
				from += To_int(data, from);

			}
			else {
				cout << "客户端发送未知类型------" << data[from] << endl;
				from += To_int(data, from + 1) + 1;
			}
			break;

		}
	}
}


#include "PgSqlstdafx.h"
#include "MySqlAntlr_Parser.h"


class PgSqlType {
public:
	char T_Type;
	int T_Length;
	PgSqlType(char * data, u_int& from);
	virtual void print() = 0;
};

typedef struct T_DATE_ROW {
	int Column_length;
	char *Data;
	struct T_DATE_ROW * next = NULL;
	void print(struct T_DATE_ROW *ctl, int count);

}T_Date_row;

typedef struct T_ROW_DESC {
	char *Column_name;
	int Table_OID;  //4 2 4 2 4 2为依次的Length
	short Column_index;
	int Type_OID;
	short Column_length;
	int Type_modifier;
	short	Format;

	void print(struct T_ROW_DESC  *ctl, int count);
}T_Row_desc;

typedef struct PARAMENTER {

	char* name = NULL;
	char* value = NULL;
	struct PARAMENTER *next = NULL;

}Parameter;



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//'Q'
class Query :private PgSqlType {
private:
	char * Simple_Query;
public:
	Query(char * data, u_int& from) :PgSqlType(data, from) {

		point_from(Simple_Query, data, from);
	}

	void print() {

		//if(T_Length==778){


		cout << "\nQuery\n";
		cout << "Length;" << T_Length << endl;
		cout << "Simple_Query;" << Simple_Query << endl;

		int i = 0;
		while (Simple_Query[i] != '\0') {
			if (Simple_Query[i] <= 0x7a && Simple_Query[i] >= 0x61)
				Simple_Query[i] -= 0x20;
			i++;
		}
		ANTLR_Parse(Simple_Query);
	//}
	}
};

//'B'
class Bind :private PgSqlType {
private:
	char *Portal;
	char *Statement;
	short Parmenter_formats;
	short *CTL1;
	short Parameter_values;
	T_Date_row * CTL2;
	short Result_formats;
public:
	Bind(char * data, u_int& from) :PgSqlType(data, from) {

		point_from(Portal, data, from);
		point_from(Statement, data, from);
		Parmenter_formats = To_short_from(data, from);
		short temp = 0;
		if (Parmenter_formats) {
			CTL1 = new short[Parmenter_formats];
			for (short temp = 0; temp < Parmenter_formats; temp++)
				CTL1[temp] = To_int_from(data, from);
		}

		Parameter_values = To_short_from(data, from);
		if (Parameter_values) {
			CTL2 = new T_Date_row[Parmenter_formats];
			for (short temp = 0; temp < Parameter_values; temp++) {
				CTL2[temp].Column_length = To_int_from(data, from);
				CTL2[temp].Data = data + from;
				from += CTL2[temp].Column_length;
			}
		}
		Result_formats = To_short_from(data, from);
	}

	void print() {
		cout << "Bind\n";
		cout << "Length;" << T_Length << endl;
		cout << "Portal;" << Portal << endl;
		cout << "Statement;" << Statement << endl;
		cout << "Parmenter_formats;" << Parmenter_formats << endl;

		for (short i = 0; i < Parmenter_formats; i++)
			cout << "formats:" << CTL1[i] << endl;

		CTL2->print(CTL2, Parmenter_formats);

		cout << "Result_formats:" << Result_formats << endl;
		cout << endl;
	}

	~Bind() {
		{
			if (Parmenter_formats) {
				delete[]CTL1;
			}
		}
		{	if (Parameter_values) {
			delete[]CTL2;
		}
		}
	}
};
//NULL
class CancelRequest :private PgSqlType {
private:
	int uid;
	int PID;
	int key;
public:
	CancelRequest(char * data, u_int& from) :PgSqlType(data, from) {
		from -= 5;
		T_Length = To_int_from(data, from);
		uid = To_int_from(data, from);
		PID = To_int_from(data, from);
		key = To_int_from(data, from);
	}
	void print() {
		cout << "CancelRequest\n";
		cout << "Length;" << T_Length << endl;
		cout << "uid;" << uid << endl;
		cout << "PID;" << PID << endl;
		cout << "key;" << key << endl;

		cout << endl;
	}
};

//'C'
class Close :private PgSqlType {
	char Bytel1;
	char * Protal;
public:
	Close(char * data, u_int& from) :PgSqlType(data, from) {
		//T_Type = data[from++];
		from -= 5;
		T_Length = To_int_from(data, from) + 1;
		Bytel1 = data[from++];
		point_from(Protal, data, from);
	}
	void print() {
		cout << "Simple_Query\n";
		cout << "Length;" << T_Length << endl;
		cout << "Bytel1;" << Bytel1 << endl;
		cout << "Protal;" << Protal << endl;

		cout << endl;
	}
};

//'D'
class Describle : private PgSqlType {
private:
	char tag;
	char * Portal;
public:
	Describle(char * data, u_int& from) :PgSqlType(data, from) {

		tag = data[from++];
		point_from(Portal, data, from);
	}

	void print() {
		cout << "Simple_Query\n";
		cout << "Length;" << T_Length << endl;
		cout << "tag;" << tag << endl;
		cout << "Portal;" << Portal << endl;
		cout << endl;
	}
};

//'E'
class Execute : private PgSqlType {
private:
	char * Protal;
	int Returns;
public:
	Execute(char * data, u_int& from) :PgSqlType(data, from) {


		point_from(Protal, data, from);
		Returns = To_int_from(data, from);
	}
	void print() {
		cout << "PgSqlType:Execute\n";
		cout << "Length;" << T_Length << endl;
		cout << "Portal;" << Protal << endl;
		cout << "Returns;" << Returns << endl;
		cout << endl;
	}
};

//'H'
class Flush :private PgSqlType {
public:
	Flush(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "Flush\n";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};


//'F'
class FunctionCall : private PgSqlType {
	int OID;
	short count1;
	short *CTL1 = NULL;
	short count2;
	T_Date_row *CTL2 = NULL;
	short RESULT;
public:
	FunctionCall(char * data, u_int& from) :PgSqlType(data, from) {

		OID = To_int_from(data, from);
		count1 = To_short_from(data, from);
		short temp;
		if (count1) {
			CTL1 = new short[count1];
			for (short temp = 0; temp < count1; temp++)
				CTL1[temp] = To_short_from(data, from);
		}

		count2 = To_short_from(data, from);

		if (count2) {
			CTL2 = new T_Date_row[count2];
			for (temp = 0; temp < count2; temp++) {
				CTL2[temp].Column_length = To_int_from(data, from);
				CTL2[temp].Data = data + from;
				from += CTL2[temp].Column_length;
			}
		}
		RESULT = To_short_from(data, from);
	}

	void print() {
		cout << "FunctionCall\n";
		cout << "Length;" << T_Length << endl;
		cout << "OID;" << OID << endl;

		cout << "count1;" << count1 << endl;
		for (int i = 0; i < count1; i++)
			cout << CTL1[i] << endl;

		cout << "count2;" << count2 << endl;
		CTL2->print(CTL2, count2);
		cout << "RESULT;" << RESULT << endl << endl;
	}

	~FunctionCall() {
		if (CTL2 != NULL)
			delete[]CTL2;
		if (CTL1 != NULL)
			delete[]CTL1;

	}
};

//'P'
class Parse :private PgSqlType {
private:
	char *Statement;
	char *Query;
	short Paraments;
	int *CTL;
public:
	Parse(char * data, u_int& from) :PgSqlType(data, from) {

		point_from(Statement, data, from);
		point_from(Query, data, from);
		Paraments = To_short_from(data, from);
		if (Paraments) {
			CTL = new int[Paraments];
			for (short i = 0; i < Paraments; i++)
				CTL[i] = To_int_from(data, from);
		}
	}

	void print() {
		cout << "Pars\n";
		cout << "Length;" << T_Length << endl;
		cout << "Statement;" << Statement << endl;
		cout << "Query;" << Query << endl;
		cout << "Paraments;" << Paraments << endl;
		int count_1 = Paraments;
		for (int i = 0; i < count_1; i++)
			cout << "Type_OID" << CTL[i] << endl;
		cout << endl;
	}
	~Parse()
	{
		if (Paraments)
			delete[] CTL;
	}
};


//'p'
class PasswordMessage :private PgSqlType {
private:
	char * Password;
public:
	PasswordMessage(char * data, u_int& from) :PgSqlType(data, from) {

		point_from(Password, data, from);
	}

	void print() {
		cout << "PasswordMessage\n";
		cout << "Length;" << T_Length << endl;
		cout << "Password;" << Password << endl;
		cout << endl;
	}
};

//'S'
class Sync :private PgSqlType {		//
public:
	Sync(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "Sync\n";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};

class SSLRequest :private PgSqlType {
	int SSL;
public:
	SSLRequest(char * data, u_int& from) :PgSqlType(data, from) {
		from -= 5;
		T_Length = To_int_from(data, from);
		SSL = To_int_from(data, from);

	}
	void print() {
		cout << "SSLRequest";
		cout << "Length;" << T_Length << endl;
		if (SSL != 0x12345679)
			cout << "error:  ";
		cout << "SSL" << SSL << endl;
	}
};


class StartupMessage : private PgSqlType {
private:
	Parameter *CTL = NULL;
public:
	StartupMessage(char * data, u_int& from) :PgSqlType(data, from) {

		from -= 5;
		u_int temp = from;
		T_Length = To_int_from(data, from);
		from += 4;
		CTL = new Parameter;
		Parameter *First = CTL;
		point_from(First->name, data, from);
		point_from(First->value, data, from);
		while (from < T_Length + temp - 1) {
			Parameter *Node = new Parameter;
			point_from(Node->name, data, from);
			point_from(Node->value, data, from);
			First->next = Node;
			First = First->next;
		}
		from++;
	}
	void print() {
		cout << "StartupMessage\n";
		cout << "Length;" << T_Length << endl;
		Parameter *First_1 = CTL;
		while (First_1 != NULL)
		{
			cout << "Parameter name:" << First_1->name << endl;;
			cout << "Parameter value:" << First_1->value << endl;;
			First_1 = First_1->next;
		}
		cout << endl;
	}
	~StartupMessage()
	{

		while (CTL != NULL) {
			Parameter *N = CTL;
			CTL = CTL->next;
			delete N;
		}
	}

};


class Terminate :private PgSqlType {//
public:
	Terminate(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "Terminate";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};





//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//'d'
class CopyData :private PgSqlType {			//
private:
	T_DATE_ROW ctl;
public:
	CopyData(char * data, u_int &from) :PgSqlType(data, from) {
		ctl.Column_length = T_Length - 5;
		ctl.Data = data + from;
		from += T_Length - 5;
	}
	void print() {
		cout << "CopyData\n";
		cout << "Length;" << T_Length << endl;

		ctl.print(&ctl, 1);

		cout << endl;
	}
};

//'c'
class CopyDone :private PgSqlType {
public:
	CopyDone(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "CopyDone\n";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//'R'
class Authentication : private PgSqlType {
private:
	int Authentication_type;
	int Salt_value = 0;
public:
	Authentication(char * data, u_int &from) :PgSqlType(data, from) {
		u_int temp = from - 5 + T_Length + 1;
		Authentication_type = To_int_from(data, from);
		Salt_value = 0;
		if (Authentication_type == 5 || Authentication_type == 8) {
			Salt_value = To_int_from(data, from);
			cout << "ERROR\n";
		}
		//from = temp;

	}
	void print() {
		cout << "Authentication\n";
		cout << "Length;" << T_Length << endl;
		cout << "Authentication_type:" << Authentication_type << endl;
		cout << "Salt_value:" << hex << (int)Salt_value << endl;
		cout << dec << endl;
	}

};

//'K'
class BackendKeyData : private PgSqlType {
private:
	int pid;
	int key;
public:
	BackendKeyData(char * data, u_int &from) :PgSqlType(data, from) {

		pid = To_int_from(data, from);
		key = To_int_from(data, from);

	}
	void print() {
		cout << "PgSqlType :BackendKeyData\n";
		cout << "Length;" << T_Length << endl;
		cout << "pid:" << pid << endl;
		cout << "key:" << hex << pid << endl;
		cout << endl;
		cout << dec << endl;
	}

};

//'2'
class BindComplete :private PgSqlType {
public:
	BindComplete(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "BindComplete\n";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};

//'3'
class CloseComplete :private PgSqlType {
public:
	CloseComplete(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "CloseComplete\n";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};

//'C'
class CommandComplete :private PgSqlType {			//
private:
	char * Tag;
public:
	CommandComplete(char * data, u_int &from) :PgSqlType(data, from) {

		point_from(Tag, data, from);
	}
	void print() {
		cout << "CommandComplete\n";
		cout << "Length;" << T_Length << endl;
		cout << "Tag:" << Tag << endl;
		cout << endl;
	}
};



//'G'
class CopyInResponse :private PgSqlType {
private:
	char Copy_type;
	short Count;
	short *p = NULL;
public:
	CopyInResponse(char * data, u_int& from) :PgSqlType(data, from) {

		Copy_type = data[from++];
		Count = To_short_from(data, from);
		if (Count)
		{
			p = new short[Count];
			for (short i = 0; i < Count; i++)
				p[i] = To_short_from(data, from);
		}

	}
	void print() {
		cout << "CopyInResponse\n";
		cout << "Length;" << T_Length << endl;
		cout << "Copy_type;" << Copy_type << endl;
		cout << "Count;" << Count << endl;
		if (Count) {
			for (short i = 0; i < Count; i++)
				cout << p[i] << endl;

		}
		cout << endl;
	}
	~CopyInResponse()
	{
		if (Count)
		{
			delete[]p;
		}
	}
};

//'H'
class CopyOutResponse :private PgSqlType {
private:
	char Copy_type;
	short Count;
	short *p = NULL;
public:
	CopyOutResponse(char * data, u_int& from) :PgSqlType(data, from) {

		Copy_type = data[from++];
		Count = To_short_from(data, from);
		if (Count)
		{
			p = new short[Count];
			for (short i = 0; i < Count; i++)
				p[i] = To_short_from(data, from);
		}

	}
	void print() {
		cout << "CopyOutResponse\n";
		cout << "Length;" << T_Length << endl;
		cout << "Copy_type;" << Copy_type << endl;
		cout << "Count;" << Count << endl;
		if (Count) {
			for (short i = 0; i < Count; i++)
				cout << p[i] << endl;

		}
		cout << endl;
	}
	~CopyOutResponse()
	{
		if (Count)
		{
			delete[]p;
		}
	}
};

//'W'
class CopyBothResponse :private PgSqlType {
private:
	char Copy_type;
	short Count;
	short *p = NULL;
public:
	CopyBothResponse(char * data, u_int& from) :PgSqlType(data, from) {

		Copy_type = data[from++];
		Count = To_short_from(data, from);
		if (Count)
		{
			p = new short[Count];
			for (short i = 0; i < Count; i++)
				p[i] = To_short_from(data, from);
		}

	}
	void print() {
		cout << "CopyBothResponse\n";
		cout << "Length;" << T_Length << endl;
		cout << "Copy_type;" << Copy_type << endl;
		cout << "Count;" << Count << endl;
		if (Count) {
			for (short i = 0; i < Count; i++)
				cout << p[i] << endl;

		}
		cout << endl;
	}
	~CopyBothResponse()
	{
		if (Count)
		{
			delete[]p;
		}
	}
};

//'D'
class DataRow :private PgSqlType {
private:
	short Field_count;
	T_Date_row *CTL;
public:
	DataRow(char * data, u_int &from) :PgSqlType(data, from) {
		CTL = NULL;
		Field_count = To_short_from(data, from);
		if (Field_count) {
			
			CTL = new T_Date_row[Field_count];
			for (short temp = 0; temp < Field_count; temp++)
			{
				CTL[temp].Column_length = To_int_from(data, from);
				if(CTL[temp].Column_length>0){
				CTL[temp].Data = data + from;
				from += CTL[temp].Column_length;}
			}
		}
		
	}

	void print() {
		cout << "Data_row\nLength:" << T_Length << endl;
		cout << "Field_count:" << Field_count << endl;
		CTL->print(CTL, Field_count);
		cout << endl;
	}

	~DataRow() {
		if (Field_count) {
			delete[]CTL;
		}
	}
};

//'I'
class EmptyQueryResponse :private PgSqlType {
public:
	EmptyQueryResponse(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "EmptyQueryResponse\n";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};

//'E'
class ErrorResponse : private PgSqlType {
private:
	char p;
	char * Portal;
public:
	ErrorResponse(char * data, u_int& from) :PgSqlType(data, from) {

		p = data[from++];
		if (p != 0)
			point_from(Portal, data, from);
	}

	void print() {
		cout << "Simple_Query\n";
		cout << "Length;" << T_Length << endl;
		cout << "p;" << p << endl;
		cout << "Portal;" << Portal << endl;
		cout << endl;
	}
};

//'V'
class FunctionCallResponse :private PgSqlType {
private:
	T_Date_row CTL;
public:
	FunctionCallResponse(char * data, u_int& from) :PgSqlType(data, from) {

		CTL.Column_length = To_int_from(data, from);
		if (CTL.Column_length != -1) {
			CTL.Data = data + from;
			from += CTL.Column_length;
		}
	}
	void print() {
		cout << "FunctionCallResponse\n";
		cout << "Length;" << T_Length << endl;
		CTL.print(&CTL, 1);
		cout << endl << endl;
	}

};

//'n'
class NoData :private PgSqlType {		//
public:
	NoData(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "NoData\n";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};

//'N'
class NoticeResponse :private PgSqlType {
private:
	char tag;
	char *value = NULL;
public:
	NoticeResponse(char * data, u_int& from) :PgSqlType(data, from) {

		char tag = data[from++];
		if (tag != 0) {
			point_from(value, data, from);
		}
	}
	void print() {
		cout << "NoticeResponse\n";
		cout << "Length;" << T_Length << endl;
		cout << "tag:" << tag << endl;
		if (tag != 0)
			cout << "value:" << value << endl;
		cout << endl;
	}
};

//'A'
class NotificationResponse :private PgSqlType {
private:
	int PID;
	char *name = NULL;
	char *value = NULL;
public:
	NotificationResponse(char * data, u_int& from) :PgSqlType(data, from) {

		PID = To_int_from(data, from);
		point_from(name, data, from);
		point_from(value, data, from);

	}
	void print() {
		cout << "NotificationResponse\n";
		cout << "Length;" << T_Length << endl;
		cout << "PID:" << PID << endl;
		cout << "name:" << name << endl;
		cout << "value:" << value << endl;
	}
};

//'t'
class ParameterDescription : private PgSqlType {
private:
	short count;
	int *CTL = NULL;
public:
	ParameterDescription(char * data, u_int& from) :PgSqlType(data, from) {

		count = To_short_from(data, from);

		if (count) {
			CTL = new int[count];
			for (short i = 0; i < count; i++)
				CTL[i] = To_int_from(data, from);
		}
	}

	void print() {
		cout << "Bind\n";
		cout << "Length;" << T_Length << endl;
		cout << "count;" << count << endl;
		for (short i = 0; i < count; i++)
			cout << CTL[i] << endl;
		cout << endl;
	}

	~ParameterDescription() {
		if (CTL != NULL)
			delete[]CTL;
	}
};

//'S'
class ParameterStatus :private PgSqlType {
private:
	char *name = NULL;
	char *value = NULL;
public:
	ParameterStatus(char * data, u_int& from) :PgSqlType(data, from) {

		point_from(name, data, from);
		point_from(value, data, from);

	}
	void print() {
		cout << "\nParameterStatus\n";
		cout << "Length:  " << T_Length << endl;
		cout << "name:  " << name << endl;
		cout << "value:  " << value << endl;
	}
};

//'1'
class ParseComplete :private PgSqlType {
public:
	ParseComplete(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "ParseComplete\n";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};

//'s'
class PortalSuspended :private PgSqlType {		//
public:
	PortalSuspended(char * data, u_int& from) :PgSqlType(data, from) {

	}
	void print() {
		cout << "PortalSuspended";
		cout << "Length;" << T_Length << endl;
		cout << endl;
	}
};

//'Z'
class ReadyForQuery :private PgSqlType {				//
private:
	char Staus;
public:
	ReadyForQuery(char * data, u_int &from) :PgSqlType(data, from) {

		Staus = data[from];
		from++;
	}
	void print() {
		cout << "ReadyForQuery\n";
		cout << "Length;" << T_Length << endl;
		cout << "Staus:" << Staus << endl;
		cout << endl;
	}
};

class RowDescription : private PgSqlType {
private:
	int Field_count;
	T_Row_desc  *CTL;
public:

	RowDescription(char * data, u_int &from) :PgSqlType(data, from) {


		Field_count = To_short_from(data, from);
		if (Field_count) {
			CTL = new T_Row_desc[Field_count];
			for (int temp = 0; temp < Field_count; temp++) {
				point_from(CTL[temp].Column_name, data, from);
				CTL[temp].Table_OID = To_int_from(data, from);
				CTL[temp].Column_index = To_short_from(data, from);
				CTL[temp].Type_OID = To_int_from(data, from);
				CTL[temp].Column_length = To_short_from(data, from);
				CTL[temp].Type_modifier = To_int_from(data, from);
				CTL[temp].Format = To_short_from(data, from);
			}
		}
	}
	void print() {
		cout << "ROW_DES";
		cout << "Length;" << T_Length << endl;
		cout << "Field_count:" << Field_count << endl;
		CTL->print(CTL, Field_count);
		cout << endl;
	}
	~RowDescription() {

		if (Field_count) {
			delete[]CTL;
		}
	}
};

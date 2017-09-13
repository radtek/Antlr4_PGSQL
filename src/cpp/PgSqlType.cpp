
#include "PgSqlType.h"
#include "PgSqlstdafx.h"

PgSqlType::PgSqlType(char * data, u_int& from) {
	T_Type = data[from++];
	T_Length = To_int_from(data, from) + 1;
}

void T_Date_row::print(struct T_DATE_ROW *ctl, int count) {
	int p = 0;
	for (p = 0; p < count; p++) {
		cout << "Column_length: " << ctl[p].Column_length << endl;
		if(ctl[p].Column_length>0){
		char temp = ctl[p].Data[Column_length];
		ctl[p].Data[Column_length] = '\0';
		cout << "Data: " << ctl[p].Data << endl;
		ctl[p].Data[Column_length] = temp;}
	}
}

void T_Row_desc::print(struct T_ROW_DESC  *ctl, int count) {
	int p = 0;
	for (p = 0; p < count; p++) {
		cout << "Column_name: " << ctl[p].Column_name << endl;
		cout << "Table_OID: " << ctl[p].Table_OID << endl;
		cout << "Column_index: " << ctl[p].Column_index << endl;
		cout << "Type_OID: " << ctl[p].Type_OID << endl;
		cout << "Column_length: " << ctl[p].Column_length << endl;
		cout << "Type_modifier: " << ctl[p].Type_modifier << endl;
		cout << "Format: " << ctl[p].Format << endl;
	}
}
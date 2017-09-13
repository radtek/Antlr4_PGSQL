#include <PgSqlstdafx.h>
class Buffer {
private:
	u_int B_length_count;//总长度
	u_int B_length_need ;//缓冲的data总长度
	u_int B_length_used;//已使用长度
	bool B_complete ;//判断缓冲区是否完整
	char * B_Buffer ;//缓冲区
public:

	Buffer(u_int a,u_int b,u_int c,bool d) {
		B_length_count = a;
		B_length_need = b;
		B_length_used = c;
		B_complete = d;
		B_Buffer=0;
	};

	u_int get_B_length_take();//还需要多长
	u_int get_B_length_need();
	bool get_B_complete();

	void NewBuffer(const int length_need);//判断+新建缓冲区

	void Buffer_Add(const int length_need);

	void Buffer_Write(const char *data,  const int length_write);//向缓冲区存入
	
	char* Buffer_Read();//缓冲区

	 ~Buffer() {
	 	free(B_Buffer);
	 }
};
;
extern Buffer Buffer_FromDB;//全局变量buffer
extern Buffer Buffer_ToDB;//全局变量buffer
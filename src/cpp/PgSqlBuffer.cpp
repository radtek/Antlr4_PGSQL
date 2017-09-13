#include "PgSqlBuffer.h"
#include  "PgSqlstdafx.h"
u_int Buffer::get_B_length_need() {
	return B_length_need;
}

u_int Buffer::get_B_length_take() {
	return B_length_need - B_length_used;
}

bool Buffer::get_B_complete() {
	return B_complete;
}

void Buffer::NewBuffer(const int length_need) {
	if (length_need > B_length_need) {//判断是否需要新建
		free(B_Buffer);
		B_Buffer = (char*)malloc(length_need + 1);
	}
	B_length_count = length_need + 1;
	B_Buffer[length_need] = '\0';//末尾归0
	B_length_need = length_need;//需要长度改变
	B_length_used = 0;//已使用清零
};

void Buffer::Buffer_Add(const int length_add) {
	B_length_count += length_add;
	int count1 = B_length_count;
	char* buffer1 = B_Buffer;
	B_Buffer = (char*)malloc(count1);
	while (count1--) {
		buffer1[count1] = B_Buffer[count1];
	}
	free(buffer1);
	B_length_need = count1;//需要长度改变
};

void Buffer::Buffer_Write(const char *data, const int length_write) {

	for (int i = 0; i < length_write; i++)
		B_Buffer[B_length_used++] = data[i];//在data的哪里开始存
	if (B_length_used == B_length_need)//判断完整
		B_complete = true;
	else if (B_length_used > B_length_need)
		cout << "memory over\n";
	else {
		B_complete = false;
	}
};

char* Buffer::Buffer_Read() {
	return B_Buffer;
};

Buffer Buffer_FromDB(0, 0, 0, true);//全局变量buffer
Buffer Buffer_ToDB(0, 0, 0, true);//全局变量buffer
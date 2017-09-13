#pragma once

class BaseProtocol {
public:
	/* 消息重组函数，负责将客户端发送给数据库的数据包重组成完整的消息。
	*  注：一个信令消息的长度可能达到几千字节以上。但一个IP包一般就一千五百字节左右。
	*  所以一个消息可能会分成多个数据包进行发送
	*  从网络上抓取的数据包，会先缓存起来，直到收到完整的消息再进行解析。
	*  参数：
	*       data:接收的数据内容,已经剥离TCP/IP头
	*       len：数据内容的长度
	*  返回值：
	true-重组完整的消息，false-无完整的消息.
	*/
	virtual bool CompleteToDB(const char* data, const int len) = 0;

	/*
	*   同上，重组数据库服务器发给客户端的消息
	*/
	virtual bool CompleteFromDB(const char* data, const int len) = 0;

	/*
	*   对重组完成的上行消息进行解析
	*/
	virtual void ParseToDB() = 0;

	/*
	*   对重组完成的上行消息进行解析
	*/
	virtual void ParseFromDB() = 0;
};
#ifndef BinaryData_h_
#define BinaryData_h_

class BinaryData
{
public:
	BinaryData(const int byte);
	void add(const void * value, const int byte);
	void add(const char * text);
	const char * get_buffer() const;

	/***
	@brief 対象のメモリデータが自分と同じ内容のデータから始まっているかどうかを判定する。
	@param 対象のメモリデータ
	@return 同じデータから始まっている...true, そうではない...false
	@detail (対象のメモリデータ量 >= 自分のデータ量)が前提であることに注意！
	*/
	bool is_same_beginning(void * target) const;



	~BinaryData();
private:
	char * data;
	int next_head;
};

#endif
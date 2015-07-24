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
	@brief �Ώۂ̃������f�[�^�������Ɠ������e�̃f�[�^����n�܂��Ă��邩�ǂ����𔻒肷��B
	@param �Ώۂ̃������f�[�^
	@return �����f�[�^����n�܂��Ă���...true, �����ł͂Ȃ�...false
	@detail (�Ώۂ̃������f�[�^�� >= �����̃f�[�^��)���O��ł��邱�Ƃɒ��ӁI
	*/
	bool is_same_beginning(void * target) const;



	~BinaryData();
private:
	char * data;
	int next_head;
};

#endif
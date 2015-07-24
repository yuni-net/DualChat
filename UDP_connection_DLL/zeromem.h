template <typename T>
void zeromem(T & variable)
{
	memset(&variable, 0, sizeof(T));
}

template <typename T>
void zeromem(T * pointer, const int byte)
{
	memset(pointer, 0, byte);
}
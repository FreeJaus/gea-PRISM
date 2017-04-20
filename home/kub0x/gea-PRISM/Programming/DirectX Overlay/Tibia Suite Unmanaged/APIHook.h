#pragma once

#define JMP_SZ 6

template<class T>
class APIHook final {

private:

	BYTE *pHook, *pOriginal;
	DWORD dwOld, dwNew;
	BYTE *bOriginal, *bHook;

	void Init();

public:

	APIHook(BYTE* _pOriginal, BYTE* _pHook);

	void Hook();
	T UnHook();

};

template<class T>
APIHook<T>::APIHook(BYTE* _pOriginal, BYTE* _pHook){
	pOriginal = _pOriginal;
	pHook = _pHook;
	bOriginal = new BYTE[JMP_SZ];
	bHook = new BYTE[JMP_SZ];
	Init();
}

template <class T>
void APIHook<T>::Init(){
	BYTE tempJMP[JMP_SZ] = { 0xE9, 0x90, 0x90, 0x90, 0x90, 0xC3 };
	memcpy(bHook, tempJMP, JMP_SZ);
	DWORD JMPSize = ((DWORD)pHook - (DWORD)pOriginal - 5);
	VirtualProtect((LPVOID)pOriginal, JMP_SZ, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy(bOriginal, pOriginal, JMP_SZ); //Original 6 Bytes
	memcpy(&bHook[1], &JMPSize, 4); //JMP Trampoline Payload
	memcpy(pOriginal, bHook, JMP_SZ);
	Hook();
}

template <class T>
void APIHook<T>::Hook(){
	memcpy((LPVOID)pOriginal, bHook, JMP_SZ);
}

template <class T>
T APIHook<T>::UnHook(){
	VirtualProtect((LPVOID)pOriginal, JMP_SZ, PAGE_EXECUTE_READWRITE, &dwOld);
	memcpy((LPVOID)pOriginal, bOriginal, JMP_SZ);
	DWORD dwNew = 0;
	VirtualProtect((LPVOID)pOriginal, JMP_SZ, dwOld, &dwNew);
	return reinterpret_cast<T>(pOriginal);
}
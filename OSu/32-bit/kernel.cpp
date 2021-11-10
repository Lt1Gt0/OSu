extern "C" void main(){
	*(char*)0xb8000 = 'a';
	return;
}
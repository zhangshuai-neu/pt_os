int main(){
	int * addr=0x00280000;
	*addr=0xff88;
	
	while(1){}
	return 0;
}

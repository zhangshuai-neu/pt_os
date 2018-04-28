int main(){
	int * addr=0x00180000;
	*addr=0xff88;
	
	while(1){}
	return 0;
}

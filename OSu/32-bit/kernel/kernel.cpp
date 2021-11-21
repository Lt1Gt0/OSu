#define VIDEO_ADDRESS 	0xb8000
#define MAX_ROWS 		25
#define MAX_COLUMNS 	80

extern "C" void main(void){
	
	int video_base_address = 0xb8000;
	int video_base_color_address = 0xb8001;
	char bruh[] = "bruh";

	int color_start = 4;
	for(int i = 0; i < 4; i++){
		int video_address = video_base_address + (i*2);
		int video_color_address = video_base_color_address + (i*2);
		*(char*)video_address = bruh[i];
		*(char*)video_color_address = color_start;
		color_start++;
	}		
	return;

}

void print_string(char* stringToPrint){
	return;
}
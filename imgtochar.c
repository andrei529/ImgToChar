#include <stdio.h>
#include <stdlib.h>
#include <IL/il.h>

#define displayInfo 1

int caracs;
const char chars[17] = {'M','N','D','8','0','Z','$','7','I','?','+','=','~',':',',','.',' '};
int vec[257];
int width;
int height;

void error(char * msg){
	printf("%s\n", msg);
	exit(1);
}

void scale(int newX, int newY){
	iluScale(newX, newY, 0);
}

unsigned int setup(char * filename){
	//Initialize and open the image
	ILboolean success;
	unsigned int imageID;
	
	ilInit();
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	
	success = ilLoadImage((ILstring)filename);
	
	if (!success) {
		ilDeleteImages(1, &imageID);
		error("Failed to open image!");
	}
	
	return imageID;
}

ILubyte ** RGBtoGray(){
	ILubyte * bytes = ilGetData();
	ILubyte ** image = (ILubyte**)malloc(sizeof(ILubyte*)*height);
	
	int i, j;
	for (i = 0; i < height; i++){
		image[i] = (ILubyte*)malloc(sizeof(ILubyte)*width);
		
		for (j = 0; j < width; j++){
			//printf( "%d,%d ->", i,j); 
			//printf( "(%d,", bytes[(i*width + j)*3 + 0]); 
			//printf( "%d,", bytes[(i*width + j)*3 + 1]);
			//printf( "%d) -> ", bytes[(i*width + j)*3 + 2]);
			image[i][j] = ((bytes[(i*width + j)*3])+(bytes[(i*width + j)*3+1])+(bytes[(i*width + j)*3+2]))/3;
			//image[i][j] = bytes[(i*width + j)*3];
			//printf("%d\n", image[i][j]);
		}
//		getchar();
//		printf("\n");
	}
	
	return image;
}

void setVec(){
	//caracs have the number of diferents chars will appear on the .txt, 17 is the max in the moment
	//in the future caracs will be a parameter, and will have a GUI too
	int i,j;
	
	for (i=1; i<=caracs; i++){
		int x = ((i-1)*(256./caracs));
		int y = ((i)*(256./caracs));
		int z = ((x/256.)*17);
		for (j=x;j<y;j++){
			vec[j] = chars[z];
		}
	}
}

void writeOut(ILubyte ** image){
//Just create the output file with the chars
	FILE * out = fopen("image.txt", "w");
	int i, j;
		
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			fprintf(out, "%c", vec[image[i][j]]);
			//printf("%c", vec[image[i][j]]);
			//printf("%d -> %c\n", image[i][j], vec[image[i][j]]);
		}
		fprintf(out, "\n");
		//printf("\n");
	}

	fclose(out);
}

void info(){
	printf("Width: %d, Height %d, Bytes per Pixel %d\n",
            ilGetInteger(IL_IMAGE_WIDTH),
            ilGetInteger(IL_IMAGE_HEIGHT),
            ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL));
	
	char *s;
	switch(ilGetInteger(IL_IMAGE_FORMAT)) {
		case IL_COLOR_INDEX     : s =  "IL_COLOR_INDEX"; break;    
		case IL_ALPHA           : s =  "IL_ALPHA"; break; 
		case IL_RGB             : s =  "IL_RGB"; break;    
		case IL_RGBA            : s =  "IL_RGBA"; break;    
		case IL_BGR             : s =  "IL_BGR"; break;    
		case IL_BGRA            : s =  "IL_BGRA"; break;    
		case IL_LUMINANCE       : s =  "IL_LUMINANCE"; break;    
		case IL_LUMINANCE_ALPHA	: s =  "IL_LUMINANCE_ALPHA"; break;
	}
	printf("Format %s\n", s);
	if (ilGetInteger(IL_IMAGE_FORMAT) != IL_RGB){
		printf("We do not tested this image format, but we will try to run...\n");
	}
	
	switch(ilGetInteger(IL_IMAGE_TYPE)) {
		case IL_BYTE           : s =  "IL_BYTE"; break;    
		case IL_UNSIGNED_BYTE  : s =  "IL_UNSIGNED_BYTE"; break;   
		case IL_SHORT          : s =  "IL_SHORT"; break;    
		case IL_UNSIGNED_SHORT : s =  "IL_UNSIGNED_SHORT"; break;    
		case IL_INT            : s =  "IL_INT"; break;    
		case IL_UNSIGNED_INT   : s =  "IL_UNSIGNED_INT"; break;    
		case IL_FLOAT          : s =  "IL_FLOAT"; break;    
		case IL_DOUBLE         : s =  "IL_DOUBLE"; break;
		case IL_HALF           : s =  "IL_HALF"; break;
	}
	printf("Data type: %s\n", s);
	if (ilGetInteger(IL_IMAGE_TYPE) != IL_UNSIGNED_BYTE){
		printf("We do not tested this data type, but we will try to run...\n");
	}
}

int main(int argc, char **argv){
	unsigned int imageID;
	char * filename;
	
	if (argc >= 2){
		filename = argv[1];
	}
	else{
		error("imgtochar <filename>");
	}
	
	if (argc >= 3){
		caracs = atoi(argv[2]);
		if (caracs < 2 || caracs > 17){
			error("Wrong number of chars, must to be more then 1 and less than 18");
		}
	}
	else{
		caracs = 17;
	}
	
	imageID = setup(filename);
	
	if (argc >= 5){
		scale(atoi(argv[2]), atoi(argv[3]));
	}
	
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	
	if (displayInfo){
		info();
	}
	
	ILubyte ** image = RGBtoGray();
	
	setVec();
	
	writeOut(image);
	
	printf("done.\n");
	return 0;
}

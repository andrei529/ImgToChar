#include <stdio.h>
#include <IL/il.h>

int main(int argc, char **argv){
	ILboolean success;
	unsigned int imageID;
	
	//Initialize and open the image
	ilInit();
	ilGenImages(1, &imageID);
	ilBindImage(imageID); 
	success = ilLoadImage((ILstring)"test.jpg");
	
	if (!success) {
		ilDeleteImages(1, &imageID);
		printf("Failed to open image!\n");
		return 0;
	}
	
	//display some info about the image
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

	//image is a vector with the image in grayscale 
	ILubyte * bytes = ilGetData();
	ILuint width, height;
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	
	int i, j;
	ILubyte image[height][width];
	
	for (i = 0; i < height; i++){
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
	
	//caracs have the number of diferents chars will appear on the .txt, 17 is the max in the moment
	//in the future caracs will be a parameter, and will have a GUI too
	int caracs = 17;
	char chars[17] = {'M','N','D','8','0','Z','$','7','I','?','+','=','~',':',',','.',' '};
	int vec[257];
	
	for (i=1; i<=caracs; i++){
		int x = ((i-1)*(256./caracs));
		int y = ((i)*(256./caracs));
		int z = ((x/256.)*17);
		for (j=x;j<y;j++){
			vec[j] = chars[z];
		}
	}
	
	//Just create the output file with the chars
	FILE * out = fopen("image.txt", "w");
		
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
	return 0;
}

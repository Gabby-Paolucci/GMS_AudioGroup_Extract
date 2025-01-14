/*
 * Game Maker Studio Audio Group Extractor
 *
 * Description:
 *     Extracts file from files such as "audiogroup1.dat", etc which merely
 *     are package files that hold files in it.
 *
 *     This was only tested on two "audiogroup1.dat" files. Will improve if
 *     this glitches out on other files.
 *
 * Synopsis:
 *     ./extract file
 *
 * Compilation:
 *     gcc --std=gnu89 -o extract extract.c
 *
 * Author:
 *     Clara Nguyen (@iDestyKK)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	char* outname="extract";
	//Check arguments
	if (argc < 2) {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	} else if (argc==3) {
		outname=argv[2];
	}

	char   *buf, fname[16];
	FILE   *fp;
	size_t s1, s2, p, n, rem, trk_num, ffloc;

	fp = fopen(argv[1], "rb");

	//Get size of the file
	fseek(fp, 0, SEEK_END);
	s1 = ftell(fp);
	fseek(fp, 0, SEEK_SET);


	//Read the entire file into a buffer. We'll be doing this all in memory.
	buf = malloc(s1 * sizeof(char));
	fread(buf, sizeof(char), s1, fp);
	fclose(fp);

	//Skip to the byte read in at 0x14.
	//That's where the first file is.
	trk_num = *(int*)&buf[0x10];
	ffloc   = *(int*)&buf[0x14];

	printf("Number of tracks: %d\n"    , trk_num);
	printf("First Track Addr: 0x%08x\n", ffloc  );

	for (p = ffloc, n = 0; p < s1; n++) {
		s2 = *(int*)&buf[p];
		p += 4;

		//Print out debug information and set file name.
		printf("File at 0x%08x (Size %d)... ", p, s2);
		
		//TODO: Figure out file formats by Magic Number
		sprintf(fname, "%s%03d.ogg", outname,n);

		//Extract the file
		fp = fopen(fname, "wb");
		fwrite(&buf[p], sizeof(char), s2, fp);
		fclose(fp);
		printf("Extracted!\n");

		//The file size must be at an address of a multiple of 4.
		rem = s2 % 4;
		if (rem == 0)
			rem = 4;
		p += s2 + 4 - rem;
	}

	//Have a nice day.
	free(buf);
	exit(0);
}

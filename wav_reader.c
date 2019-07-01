/**
 * WAVE format:
 * 44 byte header
 * Positions	Sample Value	Desc.
 * ---------	------------	-----
 *  1-4		"RIFF"		Marks file as riff file. Chars are 1 byte long
 *  5-8		File size int	Size of overall file -8bytes in bytes (32bit ints???)
 *  9-12	"WAVE"		File type header. For us, always equals "WAVE"
 *  13-16	"fmt"		Format chunk marker. Includes trailing null
 *  17-20	16		Length of format data as listed above
 *  21-22	1		Type of format (1 is PCM) - 2 byte integer
 *  23-24	2		Number of Channels - 2 byte integer
 *  25-28	44100		Sample Rate - 32 byte integer. Common vals are 44100 (CD), 48000 (DAT) (sam/sec)
 *  29-32	176400		(Sample Rate * BitsPerSample * Channels) / 8
 *  33-34	4		(BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo/ 16 bit mono4 -
 *  				16 bit stereo
 *  35-36	16		Bits per sample
 *  37-40	"data"		"data" chunk header. Marks the beginning of the data section
 *  41-44	File size (data)	Size of the data section.
 *
 *  Sample values are given above for a 16-bit stereo source
 *
 *  IMPORTANT: WAV format uses little-endian fmt to store bytes, so need to convert bytes to bit-endian in code
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "wav_reader.h"

void read_wave(WaveFile *p_wavefile, char *filename)
{
	FILE *fp = fopen(filename, "r"); // Double check the open type
	int i;

	/* Read 1st 4 chars into riff_marker. Apparently strings aren't backwards */
	for (i = 0; i < 4; i++)
	{
		p_wavefile->riff_marker[i] = getc(fp);
	}

	p_wavefile->riff_marker[4] = '\0';

	/* 5-8 go into file_size */
	for (i = 0; i < 4; i++)
	{
		p_wavefile->file_size[3 - i] = getc(fp);
	}

	/* 9-12 go into file_type_header */
	for (i = 0; i < 4; i++)
	{
		p_wavefile->file_type_header[i] = getc(fp);
	}
	p_wavefile->file_type_header[4] = '\0';

	fclose(fp);
}

int main()
{
	char filename[128] = "C:\\Users\\PC\\Documents\\Desktop_Dump_2_11_16\\DT\\Sun Traffic.wav";
	WaveFile wavefile = { 0 };

	long long file_size_calc = 0;
	int i;

	read_wave(&wavefile, filename);

	printf("RIFF marker: %s\n", wavefile.riff_marker);
	printf("file_size: %d %d %d %d\n", wavefile.file_size[0], wavefile.file_size[1], 
			wavefile.file_size[2], wavefile.file_size[3]);
	printf("File type header: %s\n", wavefile.file_type_header);

	for (i = 0; i < 3; i++)
	{
		file_size_calc += (wavefile.file_size[3 - i] << (i * 8));
	}

	printf("Calculated file size: %lld\n", file_size_calc);

	return 0;
}

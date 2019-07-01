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

void read_string(FILE *fp, char *output_location, int chars_to_read)
{
	int i;
	for (i = 0; i < chars_to_read; i++)
	{
		output_location[i] = getc(fp);
	}
	output_location[chars_to_read + 1] = '\0';
}

/* Tweak this to read in a little-endian number correctly */
void read_number(FILE *fp, int *output_location, long int *full_output_location, int ints_to_read)
{
	int i;
	*full_output_location = 0;
	for (i = 0; i < ints_to_read; i++)
	{
		output_location[ints_to_read - i - 1] = getc(fp);
		(*full_output_location) += (output_location[ints_to_read - i - 1] << (i * 8));
//		printf("Index: %d, Full: %ld\n", output_location[ints_to_read - i - 1], *full_output_location);
	}
}

void kill_junk(FILE *fp, int bytes_to_kill)
{
	int i = 0;
	int temp;

	for (i = 0; i < bytes_to_kill; i++)
	{
		temp = getc(fp);
	}

	printf("Killed %d bytes. Expected: %d\n", i, bytes_to_kill);
}

void kill_all_zeros(FILE *fp)
{
	int i = 0;
	int temp;

	do
	{
		temp = getc(fp);
		i++;
	} while('\0' == temp);

	printf("Killed %d chars\n", i);
}

void read_wave(WaveFile *p_wavefile, char *filename)
{
	FILE *fp = fopen(filename, "r"); // Double check the open type
	int i;
	read_string(fp, p_wavefile->riff_marker, 4);
	read_number(fp, p_wavefile->file_size_arr, &p_wavefile->file_size, 4);
	read_string(fp, p_wavefile->file_type_header, 4);
	read_string(fp, p_wavefile->format_chunk_marker, 4);
	read_number(fp, p_wavefile->format_data_length_arr, &p_wavefile->format_data_length, 4);
	/* Turn these into a "while NOT "fmt"" loop */
#if 1
	if (strcmp(p_wavefile->format_chunk_marker, "JUNK") == 0)
	{
		kill_junk(fp, p_wavefile->format_data_length);
		read_string(fp, p_wavefile->format_chunk_marker, 4);
		read_number(fp, p_wavefile->format_data_length_arr, &p_wavefile->format_data_length, 4);
	}
	if (strcmp(p_wavefile->format_chunk_marker, "bext") == 0)
	{
		kill_junk(fp, p_wavefile->format_data_length);
		read_string(fp, p_wavefile->format_chunk_marker, 4);
		read_number(fp, p_wavefile->format_data_length_arr, &p_wavefile->format_data_length, 4);
	}
#endif
#if 0	
	while (0 != strcmp(p_wavefile->format_chunk_marker, "fmt"))
	{
		kill_junk(fp, p_wavefile->format_data_length);
		read_string(fp, p_wavefile->format_chunk_marker, 4);
		read_number(fp, p_wavefile->format_data_length_arr, &p_wavefile->format_data_length, 4);
	}
#endif
	read_number(fp, p_wavefile->format_type_arr, &p_wavefile->format_type, 2);
	read_number(fp, p_wavefile->num_channels_arr, &p_wavefile->num_channels, 2);
	read_number(fp, p_wavefile->sample_rate_arr, &p_wavefile->sample_rate, 4);
	read_number(fp, p_wavefile->sample_math_arr, &p_wavefile->sample_math, 4);
	read_number(fp, p_wavefile->bitrate_math_arr, &p_wavefile->bitrate_math, 2);
	read_number(fp, p_wavefile->bits_per_sample_arr, &p_wavefile->bits_per_sample, 2);
	if (p_wavefile->format_data_length != 16)
		kill_junk(fp, p_wavefile->format_data_length - 16);
	read_string(fp, p_wavefile->data_chunk_header, 4);
	read_number(fp, p_wavefile->data_section_size_arr, &p_wavefile->data_section_size, 4);
	if (0 == strcmp(p_wavefile->data_chunk_header, "minf"))
	{
		kill_junk(fp, p_wavefile->data_section_size);
		read_string(fp, p_wavefile->data_chunk_header, 4);
		read_number(fp, p_wavefile->data_section_size_arr, &p_wavefile->data_section_size, 4);
	}
	if (0 == strcmp(p_wavefile->data_chunk_header, "elm1"))
	{
		kill_junk(fp, p_wavefile->data_section_size);
		read_string(fp, p_wavefile->data_chunk_header, 4);
		read_number(fp, p_wavefile->data_section_size_arr, &p_wavefile->data_section_size, 4);
	}
	/* Searching for the data header should also be a loop */
	/* Now we're ready to read the data! */

	fclose(fp);
}

void print_header(WaveFile wavefile)
{
	printf("RIFF marker: %s\n", wavefile.riff_marker);
	printf("File size: %ld\n", wavefile.file_size);
	printf("File type header: %s\n", wavefile.file_type_header);
	printf("Format chunk marker: %s\n", wavefile.format_chunk_marker);
	printf("Format data length: %ld\n", wavefile.format_data_length);
	printf("Format type: %ld\n", wavefile.format_type);
	printf("Number of channels: %ld\n", wavefile.num_channels);
	printf("Sample rate: %ld\n", wavefile.sample_rate);
	printf("Sample math: %ld\n", wavefile.sample_math);
	printf("Bitrate math: %ld\n", wavefile.bitrate_math);
	printf("Bits per sample: %ld\n", wavefile.bits_per_sample);
	printf("Data chunk header: %s\n", wavefile.data_chunk_header);
	printf("Data section size: %ld\n", wavefile.data_section_size);
}

int main()
{
	char filename[128] = "C:\\Users\\PC\\Documents\\Desktop_Dump_2_11_16\\DT\\Sun Traffic.wav";
	WaveFile wavefile = { 0 };

	long long file_size_calc = 0;
	int i;

	read_wave(&wavefile, filename);

	print_header(wavefile);

	return 0;
}

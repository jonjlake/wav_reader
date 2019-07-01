
typedef struct WaveFile
{
	char riff_marker[5];
	int file_size_arr[4];
	long int file_size;
	char file_type_header[5];
	char format_chunk_marker[5];
	int format_data_length_arr[4];
	long int format_data_length;
	int format_type_arr[2];
	long int format_type;
	int num_channels_arr[2];
	long int num_channels;
	int sample_rate_arr[4];
	long int sample_rate;
	int sample_math_arr[4]; // ???
	long int sample_math;
	int bitrate_math_arr[2]; // ???
	long int bitrate_math; // Change this to be block_align, or bytes_per_frame
	int bits_per_sample_arr[2];
	long int bits_per_sample; // Also BitDepth
	char data_chunk_header[5];
	int data_section_size_arr[4];
	long int data_section_size;
	/* Is sample data little-endian too? */
//	int **left_channel_samples;
//	int **right_channel_samples;
	/* Leave it open for a generic number of channels */
//	int **channel_samples;
	short **channel_samples;
	int num_frames;
} WaveFile;

void read_wave(WaveFile *p_wavefile, char *filename);

void destroy_wavearrays(WaveFile *p_wavefile);


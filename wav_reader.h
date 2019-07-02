
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
//	int sample_math_arr[4]; // ???
//	long int sample_math;
	int byterate_arr[4];
	long int byterate;
	int bitrate_math_arr[2]; // ???
	long int bitrate_math;
	int bits_per_sample_arr[2];
	long int bits_per_sample;
	char data_chunk_header[5];
	int data_section_size_arr[4];
	long int data_section_size;
} WaveFile;

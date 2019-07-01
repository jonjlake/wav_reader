
typedef struct WaveFile
{
	char riff_marker[5];
	int file_size[4];
	char file_type_header[5];
	char format_chunk_maker[4];
	int format_data_length[4];
	int format_type[2];
	int num_channels[2];
	int sample_rate[4];
	int sample_math[4]; // ???
	int bitrate_math[2]; // ???
	int bits_per_sample[2];
	char data_chunk_header[5];
	int data_section_size[4];
} WaveFile;

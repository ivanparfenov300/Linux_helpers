#include <lame/lame.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {

    char wav_file_name[50] = {0};
    printf("Enter path to wav file: ");
    fgets(wav_file_name, sizeof(wav_file_name), stdin);
    wav_file_name[strlen(wav_file_name) - 1] = '\0';

    // Specify wav and mp3 files for output
    if (argc != 3) {
        fprintf(stderr, "Format: %s <input.wav> <output.mp3>\n", argv[0]);
        return 1;
    }
    // Check file extensions
    const char *ext = strchr(argv[1], '.');
    if (!ext || strcasecmp(ext, ".wav") != 0) {
        fprintf(stderr, "Input file must have .wav extension\n");
        return 1;
    } else {
        printf("Extension check passed\n");
    }

    ext = strchr(argv[2], '.');
    if (!ext || strcasecmp(ext, ".mp3") != 0) {
        fprintf(stderr, "Output file must have .mp3 extension\n");
        return 1;
    } else {
        printf("Extension check passed\n");
    }

    SDL_AudioSpec spec;
    Uint8 *wav_data;
    Uint32 wav_size;

    // Initialize audio subsystem
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize audio system: %s\n", SDL_GetError());
        return 1;
    } else {
        printf("Initialization successful\n");
    }
    // Load file (specify full path)
    if (!SDL_LoadWAV(wav_file_name, &spec, &wav_data, &wav_size)) {
        fprintf(stderr, "Failed to load WAV file: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    // Display file properties
    else {
        printf("Loading successful\n");
        printf("Sample rate: %d Hz\n", spec.freq);
        printf("Channels: %d (%s)\n", spec.channels, spec.channels == 1 ? "Mono" : "Stereo");
    }
    // Initialize LAME encoder
    lame_t lame = lame_init();
    if (!lame) {
        fprintf(stderr, "LAME initialization error");
        SDL_FreeWAV(wav_data);
        SDL_Quit();
        return 1;
    } else {
        printf("LAME successfully initialized\n");
    }
    // Set sample rate
    lame_set_in_samplerate(lame, spec.freq);
    // Set number of channels
    lame_set_num_channels(lame, spec.channels);
    // Set quality
    
    // Set VBR quality and bitrate
    lame_set_VBR(lame, vbr_default);
    lame_set_VBR_quality(lame, 2);
    // Apply settings
    lame_init_params(lame);
    // Initialize parameters
    if (lame_init_params(lame) == 0) {
        fprintf(stderr, "Error initializing parameters\n");
        lame_close(lame);
        SDL_FreeWAV(wav_data);
        SDL_Quit();
        return 1;
    } else {
        printf("Parameters initialized successfully\n");
    }
    // Open MP3 file for writing
    FILE *mp3 = fopen(argv[2], "wb");
    if (!mp3) {
        fprintf(stderr, "Failed to create mp3 file\n");
        lame_close(lame);
        SDL_FreeWAV(wav_data);
        SDL_Quit();
        return 1;
    } else {
        printf("Successfully created mp3 file\n");
    }
    // Allocate memory for it
    int samples = wav_size / (spec.channels * sizeof(short));
    short *pcm = (short *)wav_data;
    unsigned char *mp3buf = malloc(BUFFER_SIZE);

    if (!mp3buf) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(mp3);
        lame_close(lame);
        SDL_FreeWAV(wav_data);
        SDL_Quit();
        return 1;
    } else {
        printf("Buffer allocated (%d bytes)\n", BUFFER_SIZE);
        printf("Starting conversion\n");
    }
    int total_bytes_written = 0;
    // Encode samples to MP3
    for (int processed = 0, to_process, bytes; processed < samples; processed += to_process) {
        to_process = (samples - processed) > 1152 ? 1152 : (samples - processed);
        bytes = (spec.channels == 1) ?
            lame_encode_buffer(lame, pcm + processed, NULL, to_process, mp3buf, BUFFER_SIZE) :
            lame_encode_buffer_interleaved(lame, pcm + processed * 2, to_process, mp3buf, BUFFER_SIZE);

        if (bytes > 0) {
            fwrite(mp3buf, 1, bytes, mp3);
            total_bytes_written += bytes;
        }

        if (processed % 11520 == 0 || processed + to_process >= samples) {
            float progress = (float)(processed + to_process) / samples * 100;
            int bar_width = 40;
            int pos = (int)(bar_width * progress / 100);

            printf("\r[");
            for (int i = 0; i < bar_width; i++) {
                if (i < pos) printf("=");
                else if (i == pos) printf(">");
                else printf(" ");
            }
            printf("] %.1f%%", progress);
            fflush(stdout);
        }
    }
    // Write encoded bytes to file
    int bytes = lame_encode_flush(lame, mp3buf, BUFFER_SIZE);
    if (bytes > 0) {
        fwrite(mp3buf, 1, bytes, mp3);
        total_bytes_written += bytes;
    }

    printf("Conversion complete\n");
    printf("MP3 bitrate: %d kbps\n", lame_get_brate(lame));
    printf("Output file size: %d bytes\n", total_bytes_written);
    // Display statistics
    float duration = (float)samples / spec.freq;
    float compression = (float)wav_size / total_bytes_written;
    printf("Duration: %.2f seconds\n", duration);
    printf("Compression ratio: %.1f:1\n", compression);
    // Free resources
    free(mp3buf);
    fclose(mp3);
    lame_close(lame);
    SDL_FreeWAV(wav_data);
    SDL_Quit();
    printf("Successfully converted\n");
    return 0;
}

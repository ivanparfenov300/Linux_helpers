
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef Uint8 u8;
typedef Uint32 u32;

int main(void) {

    const u8 DELAY = 100;

    char wav_file_name[50] = {0};
    printf("Enter path to wav file: ");
    fgets(wav_file_name, sizeof(wav_file_name), stdin);
    wav_file_name[strlen(wav_file_name) - 1] = '\0';

    // Initialize audio subsystem FIRST
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL2 initialization error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    printf("SDL2 successfully initialized.\n");

    SDL_AudioSpec spec;
    u8* audio_data;
    u32 audio_length;

    // Load WAV file
    if (SDL_LoadWAV(wav_file_name, &spec, &audio_data, &audio_length) == NULL) {
        fprintf(stderr, "Failed to load WAV file: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    printf("WAV file loaded successfully.\n");
    printf("Sample rate: %d Hz\n", spec.freq);
    printf("Channels: %d (%s)\n", spec.channels, spec.channels == 1 ? "Mono" : "Stereo");

    // Open audio device
    SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if (device == 0) {
        fprintf(stderr, "Failed to open audio device: %s\n", SDL_GetError());
        SDL_FreeWAV(audio_data);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Queue audio data
    SDL_QueueAudio(device, audio_data, audio_length);

    // Start playback (unpause)
    SDL_PauseAudioDevice(device, 0);

    printf("\nPlayback started... Press 'p' and ENTER to pause.\n"
           "'r' and ENTER to resume.\n"
           "'q' and ENTER to quit.\n\n");

    int running = 1;
    while (running && SDL_GetQueuedAudioSize(device) > 0) {
        int c = getchar();
        if (c == EOF) break;
        
        switch (c) {
            case 'p':
                SDL_PauseAudioDevice(device, 1);
                printf("Playback paused. Press 'r' to resume.\n");
                break;
            case 'r':
                SDL_PauseAudioDevice(device, 0);
                printf("Playback resumed. Press 'p' to pause.\n");
                break;
            case 'q':
                running = 0;
                printf("Stopping playback...\n");
                break;
            default:
                // Ignore other keys and newlines
                break;
        }
        SDL_Delay(DELAY);
    }

    // Clean up resources
    SDL_CloseAudioDevice(device);
    SDL_FreeWAV(audio_data);
    SDL_Quit();

    printf("Playback finished. Exiting program.\n");
    return EXIT_SUCCESS;
}

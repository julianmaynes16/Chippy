#include <SDL2/SDL.h>
#include <audio.h>
#include <SDL2/SDL_audio.h>
Audio::Audio(){
    SDL_AudioSpec desired;
	desired.freq = SAMPLE_RATE;
	desired.format = AUDIO_S16SYS;
	desired.channels = 1;
	desired.samples = 512;
	desired.callback = Audio::callback;
	desired.userdata = this;

	SDL_AudioSpec obtained;
	device = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desired, &obtained, 0);
}

void Audio::callback(void* context, uint8_t* stream, int bytes) {
	Audio* audio = (Audio*)context;
	int16_t* buffer = (int16_t*)stream;
	int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    int SAMPLE_RATE = 44100;
    const short VOLUME = 15000;
    const int PERIOD = SAMPLE_RATE / 440; // 100
    const int HALF_PERIOD = PERIOD / 2; // 50
	for (int i = 0; i < length; i++) {
        buffer[i] = ((i % PERIOD) < HALF_PERIOD) ? VOLUME : -VOLUME;
	}
}

//Audio:~Audio(){
//    SDL_CloseAudioDevice(device);
//}

void Audio::audio_play() {
	SDL_PauseAudioDevice(device, 0);
}

void Audio::audio_stop() {
	SDL_PauseAudioDevice(device, 0);
}
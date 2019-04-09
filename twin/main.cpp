#include <iostream>
#include <SDL.h>

#include "twin.hpp"

#define xcol(x) std::min(std::max(x, 0.0f), 1.0f)
#define frnd (float(rand()) / RAND_MAX)

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, &window, &renderer);

	SDL_Event evt;
	bool running = true;

	const int bw = 100;
	const int bh = 32;

	float pos[bw * bh * 2];
	float col[bw * bh * 3];
	twin::Twin posTween[bw * bh];
	twin::Twin colTween[bw * bh];
	for (int y = 0; y < bh; y++) {
		for (int x = 0; x < bw; x++) {
			int i = (x + y * bw), j = i * 2, k = i * 3;
			float rnd = frnd / float(bw);
			posTween[i] = twin::Twin({ &pos[j], &pos[j + 1] }, 3.0f + rnd * x)
				.key({ x * 5.0f + (320 - 2.5f * bw), -200.0f }, twin::funcs::OutElastic)
				.key({ x * 5.0f + (320 - 2.5f * bw), y * 5.0f + (240 - 2.5f * bh) });

			float fx = float(x) / bw;
			float fy = float(y) / bh;
			colTween[i] = twin::Twin({ &col[k], &col[k + 1], &col[k + 2] }, 3.0f + rnd * x, true)
				.key({ 0.0f, 0.0f, 0.0f }, 0.0f)
				.key({ fx, fy, 1.0f }, 0.7f)
				.key({ 0.0f, 0.0f, 0.0f }, 1.0f);
		}
	}

	const double ts = 1.0 / 60.0;
	double st = double(SDL_GetTicks()) / 1000.0;
	double accum = 0.0;

	while (running) {
		double ct = double(SDL_GetTicks()) / 1000.0;
		double delta = ct - st;
		st = ct;
		accum += delta;

		bool canRender = false;
		while (accum >= ts) {
			float dt = float(ts);

			for (int i = 0; i < bw*bh; i++) {
				posTween[i].update(dt);
				colTween[i].update(dt);
			}

			accum -= ts;
			canRender = true;
		}

		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) running = false;
		}

		if (canRender) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			for (int i = 0; i < bw*bh; i++) {
				int j = i * 3;
				SDL_SetRenderDrawColor(renderer, (col[j]*255), (col[j + 1] * 255), (col[j + 2] * 255), 255);
				SDL_Rect rec = { (pos[i * 2] - 2.0f), (pos[i * 2 + 1] - 2.0f), 4, 4 };
				SDL_RenderFillRect(renderer, &rec);
			}
					
			SDL_RenderPresent(renderer);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
/***  includes  ***/

#include <SDL3/SDL_render.h>
#define _POSIX_C_SOURCE 199309L
#include "CBopcodes.h"
#include "gameboy.h"
#include "opcodes.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/***  defines ***/

// resolution is 160 x 144
#define WIDTH 160
#define HEIGHT 144

#define MAX_EXECUTIONS 10

Gameboy gb;

uint8_t DMG_BIOS[0x100] = {0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB,
  0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E, 0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C,
  0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, 0x47, 0x11, 0x04,
  0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
  0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23,
  0x05, 0x20, 0xF9, 0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C,
  0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, 0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E,
  0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04, 0x1E, 0x02, 0x0E, 0x0C,
  0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2, 0x0E,
  0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64,
  0x20, 0x06, 0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42,
  0x15, 0x20, 0xD2, 0x05, 0x20, 0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04,
  0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17, 0x05, 0x20, 0xF5, 0x22, 0x23,
  0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73,
  0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00,
  0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
  0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C,
  0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, 0x21, 0x04, 0x01, 0x11, 0xA8, 0x00,
  0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20, 0xF5, 0x06, 0x19,
  0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
};

/***  file i/o ***/

int load_rom(const char *filename) {
  FILE *rom = fopen(filename, "rb");
  if (!rom) {
    perror("Failed to open ROM");
    return 0;
  }

  memset(&gb.mem[0], 0, 0x100);

  fseek(rom, 0, SEEK_END);
  long rom_size = ftell(rom);
  rewind(rom);

  printf("\n\nrom_size = %lu\n\n", rom_size);

  if (rom_size > (0x8000)) {
    fprintf(stderr, "ROM too large to fit in memory\n");
    fclose(rom);
    return 0;
  }

  printf("rom_size = %ld\n", rom_size);
  size_t bytes_read = fread(&gb.mem[0], 1, rom_size, rom);
  if (bytes_read != (size_t)rom_size) {
    fprintf(stderr, "Failed to read full ROM\n");
    fclose(rom);
    return 0;
  }

  fclose(rom);
  return 1;
}

/***	Rendering	 ***/
// void renderScanline(Gameboy *gb, u8 y) {
// }

/***  init  ***/

void gameboyInit(void) {
  memcpy(gb.bios, DMG_BIOS, sizeof(DMG_BIOS));
  gb.AF = 0;
  gb.BC = 0;
  gb.DE = 0;
  gb.HL = 0;
  gb.sp = 0;
  gb.pc = 0;
  gb.HALT_ON = 0;

	memset(gb.framebuf, 0, WIDTH * HEIGHT);
  memset(gb.mem, 0, sizeof(gb.mem));
  memcpy(gb.mem, gb.bios, sizeof(gb.bios));
  gb.biosComplete = 0;
}

int main(int argc, char **argv) {
    bool done = false;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("IDK man", WIDTH, HEIGHT, SDL_WINDOW_VULKAN);
    if (!window) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Couldn't create renderer: %s", SDL_GetError());
        return 1;
    }
		// SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  uint16_t executed[0x10000] = {0};
  gameboyInit();
  initOpcodeTable(&gb);
  initCBOpcodeTable(&gb);

  uint8_t opcode;
  uint16_t opcodeAddress;
  char Unimplemented[16] = "Not implemented";
  const char *printMessage;
  OpcodeEntry entry;
  int exitFlag = 0;
  int romLoaded;

  if (argc == 2) {
    char *filename = argv[1];
    if (filename) {
			if (!(romLoaded = load_rom(filename))) { return 1; }
      gb.pc = 0x100;
      gb.AF = 0x01B0;
      gb.BC = 0x0013;
      gb.DE = 0x00D8;
      gb.HL = 0x014D;
      gb.sp = 0xFFFE;
      gb.mem[0xFF05] = 0x00;
      gb.mem[0xFF06] = 0x00;
      gb.mem[0xFF07] = 0x00;
      gb.mem[0xFF10] = 0x80;
      gb.mem[0xFF11] = 0xBF;
      gb.mem[0xFF12] = 0xF3;
      gb.mem[0xFF14] = 0xBF;
      gb.mem[0xFF16] = 0x3F;
      gb.mem[0xFF17] = 0x00;
      gb.mem[0xFF19] = 0xBF;
      gb.mem[0xFF1A] = 0x07F;
      gb.mem[0xFF1B] = 0x0FF;
      gb.mem[0xFF1C] = 0x9F;
      gb.mem[0xFF1E] = 0xBF;
      gb.mem[0xFF20] = 0xFF;
      gb.mem[0xFF21] = 0x00;
      gb.mem[0xFF22] = 0x00;
      gb.mem[0xFF23] = 0xBF;
      gb.mem[0xFF24] = 0x77;
      gb.mem[0xFF25] = 0xF3;
      gb.mem[0xFF26] = 0xF1;//-GB, FO-SGB
      gb.mem[0xFF40] = 0x91;
      gb.mem[0xFF42] = 0x00;
      gb.mem[0xFF43] = 0x00;
      gb.mem[0xFF45] = 0x00;
      gb.mem[0xFF47] = 0xFC;
      gb.mem[0xFF48] = 0xFF;
      gb.mem[0xFF49] = 0xFF;
      gb.mem[0xFF4A] = 0x00;
      gb.mem[0xFF4B] = 0x00;
      gb.mem[0xFFFF] = 0x00;
      // gb.pc = 0x100;
      // gb.IME = 0;
      // printf("\nStarting rom:\n");
      // for (uint64_t i = 0x00; i < 2 * ROM_SIZE; i++) {
      //   printf("%02X ", gb.mem[i]);
      // }
      // printf("\n\n");
      // exitFlag = 0;
    }
  } else {
    // Boot rom is not quite loaded right for now.
    printf("No rom loaded.\nExecuting BIOS\n");
		gb.pc = 0;
    // return 0;
  }

  // i/o[0x44]
  // CMP 0x90
  // gb.io[0x0] = 0x90;
  uint64_t k = 0;

  while (!done) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				done = true;
			}
		}
    if (!gb.HALT_ON) {
      opcodeAddress = gb.pc;
      opcode = gb.mem[gb.pc++];

      if (opcode == 0xCB) {
        opcode = gb.mem[gb.pc++];
        entry = CBopcodeTable[opcode];
      } else {
        entry = opcodeTable[opcode];
      }

      if (!entry.mnemonic) exitFlag = 1;
      printMessage = (entry.mnemonic) ? entry.mnemonic : Unimplemented;

      if (executed[opcodeAddress]++ < 2 && strcmp("NOP", printMessage)) {
        printf(
          "0x%02X  |  %-15s    $%04X\n", opcode, printMessage, opcodeAddress);
      }
      if (exitFlag) break;
      entry.handler(&gb, &entry); // Executes instruction
      if (executed[opcodeAddress] > 2) { executed[opcodeAddress] = 2; }
      // executed[opcodeAddress] += 1;

			// for (int y = 0; y < HEIGHT; y++) {
			// 	renderScanline(&gb, y);
			// }

			// SDL_UpdateTexture(texture, NULL, gb.framebuf, WIDTH * sizeof(u32));
			// SDL_RenderClear(renderer);
			// SDL_RenderTexture(renderer, texture, NULL, NULL);
			// SDL_RenderPresent(renderer);
			// SDL_Delay(16); // ~60

    } else {
      if (k > 99999999) {
        printf("System is Halted\n");
        k = 0;
      } else {
        k++;
      }
    }
  }

	SDL_DestroyWindow(window);
	SDL_Quit();
  // printf("IO: \n");
  // for (int i=0; i<16; ++i) {
  //   u16 addr = (u16)(gb.io-gb.mem);
  //   printf("0x%02X:\t", addr + 8*i);
  //   for (int j=0; j<8; ++j)
  //     printf("%02X ", gb.io[8*i+j]);
  //   putchar('\n');
  // }
  return 0;
}

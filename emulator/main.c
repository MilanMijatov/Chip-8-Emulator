/*  Copyright (c) 2019, MilanMijatov
*/
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define PIXEL_SCALE 10
int op_zero(uint16_t);
int op_one(uint16_t);
int op_two(uint16_t);
int op_three(uint16_t);
int op_four(uint16_t);
int op_five(uint16_t);
int op_six(uint16_t);
int op_seven(uint16_t);
int op_eight(uint16_t);
int op_nine(uint16_t);
int op_ten(uint16_t);
int op_eleven(uint16_t);
int op_twelve(uint16_t);
int op_thirteen(uint16_t);
int op_fourteen(uint16_t);
int op_fifteen(uint16_t);

int start_SDL();
int main_loop();
int draw_scaled_pixel(int_fast16_t, int_fast16_t, uint_fast8_t);

const int (*p[16]) (uint16_t) = {op_zero, op_one, op_two, op_three, op_four, op_five, op_six, op_seven, op_eight, op_nine, op_ten, op_eleven, op_twelve, op_thirteen, op_fourteen, op_fifteen};

uint16_t pc;
uint8_t mem[4096] = {0xF0,0x90,0x90,0x90,0xF0,0x20,0x60,0x20,0x20,0x70,0xF0,0x10,0xF0,0x80,0xF0,0xF0,0x10,0xF0,0x10,0xF0,0x90,0x90,0xF0,0x10,0x10,0xF0,0x80,0xF0,0x10,0xF0,0xF0,0x80,0xF0,0x90,0xF0,0xF0,0x10,0x20,0x40,0x40,0xF0,0x90,0xF0,0x90,0xF0,0xF0,0x90,0xF0,0x10,0xF0,0xF0,0x90,0xF0,0x90,0x90,0xE0,0x90,0xE0,0x90,0xE0,0xF0,0x80,0x80,0x80,0xF0,0xE0,0x90,0x90,0x90,0xE0,0xF0,0x80,0xF0,0x80,0xF0,0xF0,0x80,0xF0,0x80,0x80};
/* define all 0x1000 1 byte memory locations */
uint8_t scrn[32][64];
uint8_t v[16];      /* define all 0x10 8-bit data registers */
uint16_t I;         /* define the address register, 16 bits wide */
uint16_t stack[16];
uint16_t *stackptr = stack;
uint32_t timer;
uint32_t starttime;
uint_fast8_t k[16];

/* begin SDL2 declations */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
int quit = 0;
SDL_Event event;

int main(int argc, char *argv[])
{
    int result, i;

    //FILE *ptr;
    //unsigned char buffer[1000] = {-1};
    //ptr = fopen("pong.rom","rb");  // r for read, b for binary
    int a = open(argv[1] ,O_RDONLY);
    int num = read(a,mem+0x200,1000);
    close(a);
    //fclose(ptr);
    //size_t num = fread(buffer,sizeof(buffer),1,ptr);
    
/*     pc = 0x200;
    pc += 10;
    printf("%u\n",mem[pc]); */

    start_SDL();

	SDL_DestroyWindow(window);
	SDL_Quit();
}

int start_SDL()
{
    if(SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf("SDL could not initialize! SDL_Error: %s/n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(!window)
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		else
		{
			screenSurface = SDL_GetWindowSurface( window );
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00));
			main_loop();
		}
	}
}

int main_loop()
{
    int i,b,d,c,result;
    for(pc = 0x200, i = 0; /*i < 52 /* i < 0x200 + num */ !quit; i += 2, pc += 2)
    { /* interpretor lol */
        printf("PC:%d\t", pc);
        b = mem[pc];
        //printf("%u\t",b >> 4);
        d = mem[pc + 1];
        c = (b << 8 ) | (d & 0xff);

        result = (*p[b >> 4]) (c);

        while( SDL_PollEvent( &event ) != 0 )
        {
            //User requests quit
            if( event.type == SDL_QUIT )
            {
                quit = 1;
            }
            else if( event.type == SDL_KEYDOWN )
            {
                //Select surfaces based on key press
                switch( event.key.keysym.sym )
                {
                    case SDLK_1:
                        k[1] = 1;
                        break;

                    case SDLK_4:
                        k[4] = 1;
                        break;

                    case SDLK_c:
                        k[12] = 1;
                        break;

                    case SDLK_d:
                        k[13] = 1;
                    break;
                }
            }

        }
        SDL_Delay(5);
        //SDL_WaitEvent(&event);
    
/*         while (event.type != SDL_MOUSEBUTTONDOWN)
        {
            SDL_WaitEvent(&event);
            if( event.type == SDL_QUIT )
            {
                quit = 1;
            }
            while( SDL_PollEvent( &event ) != 0 )
            {
                //User requests quit
                if( event.type == SDL_QUIT )
                {
                    quit = 1;
                }
            }
        }  */
    }
    printf("\n");
    for(i = 0; i < 32; i++)
    {
        for(result = 0; result < 64; result++)
                printf("%u ", scrn[i][result]);
        printf("\n");
    }
}

int update_timer()
{

}

int draw_scaled_pixel(int_fast16_t x, int_fast16_t y, uint_fast8_t c)
{
    SDL_Rect PIXELRect;
    PIXELRect.h = PIXEL_SCALE;
    PIXELRect.w = PIXEL_SCALE;
    PIXELRect.x = x * PIXEL_SCALE;
    PIXELRect.y = y * PIXEL_SCALE;
    if(c)
        SDL_FillRect(screenSurface, &PIXELRect, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF));
    else
        SDL_FillRect(screenSurface, &PIXELRect, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00));

    SDL_UpdateWindowSurface(window);
}

int op_zero(uint16_t a)
{
    a = a & 0x00FF;
    if(a == 0xEE)
    {
        stackptr--;
        pc = *stackptr;
        printf("Return to %d\n", pc);
    }
    else if((a & 0x00F0) == 0xE0)
    {
        /* for(a = 0; a < 32; a++)
            memset(scrn[a], 0, sizeof scrn[a]); */
        memset(scrn, 0, sizeof scrn);
        SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00));
    }
    else
        EXIT_FAILURE;
}

int op_one(uint16_t a)
{
    pc = a & 0x0FFF;
    pc -= 2;
    return 0;
}

int op_two(uint16_t a)
{
    *stackptr = pc;
    stackptr++;
    pc = a & 0x0FFF;
    printf("PC(%d) added to stack, PC is now %d\n",*(stackptr-1), a & 0x0FFF);
    pc -= 2;
    return 0;
}

int op_three(uint16_t a)
{
    uint8_t n1 = v[(a & 0x0F00) >> 8];
    uint8_t n2 = a & 0x00FF;
    if(n1 == n2)
    {
        printf("%u == %u\n",v[(a & 0x0F00) >> 8],a & 0x00FF);
        pc += 2;
        printf("pc is now %d\n",pc);
    }
    else
        printf("%u != %u\n",v[(a & 0x0F00) >> 8],a & 0x00FF);
    return 0;
}

int op_four(uint16_t a)
{
    uint8_t n1 = v[(a & 0x0F00) >> 8];
    uint8_t n2 = a & 0x00FF;
    if(n1 != n2)
    {
        printf("%u != %u\n",v[(a & 0x0F00) >> 8],a & 0x00FF);
        pc += 2;
        printf("pc is now %d\n",pc);
    }
    else
        printf("%u == %u\n",v[(a & 0x0F00) >> 8],a & 0x00FF);
    return 0;
}

int op_five(uint16_t a)
{
    uint8_t n1 = v[(a & 0x0F00) >> 8];
    uint8_t n2 = v[(a & 0x00F0) >> 4];
    if(n1 == n2)
        pc += 2;
    return 0;
}

int op_six(uint16_t a)
{
    v[(a & 0x0F00) >> 8] = a & 0x00FF;
    //printf("Register %d\n", a);
    printf("Register %d = %d\n",(a & 0x0F00) >> 8, a & 0x00FF);
    //printf("Register %d = %d\n",(a & 0x0F00) >> 8, v[(a & 0x0F00) >> 8]);
    return 0;
}

int op_seven(uint16_t a)
{
    v[(a & 0x0F00) >> 8] += a & 0x00FF;
    printf("v%d: %d + %d = %d\n", (a & 0x0F00) >> 8, v[(a & 0x0F00) >> 8] - a & 0x00FF, a & 0x00FF, v[(a & 0x0F00) >> 8]);
    return 0;
}

int op_eight(uint16_t a)
{
    switch (a & 0x000F)
    {
        case 0x0:
            v[(a & 0x0F00) >> 8] = v[(a & 0x00F0) >> 4];
            break;
        case 0x1:
            v[(a & 0x0F00) >> 8] |= v[(a & 0x00F0) >> 4];
            break;
        case 0x2:
            printf("v%u(%u) is now ",(a & 0x0F00) >> 8 ,v[(a & 0x0F00) >> 8]);
            v[(a & 0x0F00) >> 8] &= v[(a & 0x00F0) >> 4];
            printf("%u after being ANDed by %u", v[(a & 0x0F00) >> 8], v[(a & 0x00F0) >> 4]);
            break;
        case 0x3:
            v[(a & 0x0F00) >> 8] ^= v[(a & 0x00F0) >> 4];
            break;
        case 0x4:
/*             v[(a & 0x0F00) >> 8] += v[(a & 0x00F0) >> 4];
            uint16_t w = 0;
            w += v[(a & 0x00F0) >> 4];
            w += v[(a & 0x0F00) >> 8];
            if(v[(a & 0x0F00) >> 8] != w)
            {
                v[0xF] = 1;
                v[(a & 0x0F00) >> 8] = w & 0x00FF;
            }
            v[0xF] = 0; */

            if(v[(a & 0x0F00) >> 8] + v[(a & 0x00F0) >> 4] > 255)
                v[0xF] = 1;
            else
                v[0xF] = 0;

            v[(a & 0x0F00) >> 8] += v[(a & 0x00F0) >> 4];

            break;
        case 0x5:
            if(v[(a & 0x0F00) >> 8] > v[(a & 0x00F0) >> 4])
                v[0xF] = 1;
            else
                v[0xF] = 0;
            v[(a & 0x0F00) >> 8] -= v[(a & 0x00F0) >> 4];
            break;
        case 0x6:
            v[0xF] = v[(a & 0x00F0) >> 4] & 0b00000001;
            v[(a & 0x0F00) >> 8] = v[(a & 0x00F0) >> 4] >> 1;
            break;
        case 0x7:
            if(v[(a & 0x0F00) >> 8] < v[(a & 0x00F0) >> 4])
                v[0xF] = 1;
            else
                v[0xF] = 0;
            v[(a & 0x0F00) >> 8] = v[(a & 0x00F0) >> 4] - v[(a & 0x0F00) >> 8];
            break;
        case 0xE:
            v[0xF] = (v[(a & 0x00F0) >> 4] & 0b10000000) >> 7;
            v[(a & 0x0F00) >> 8] = v[(a & 0x00F0) >> 4] << 1;
            break;
    
        default:
            EXIT_FAILURE;
    }
    return 0;
}

int op_nine(uint16_t a)
{
    uint8_t n1 = v[(a & 0x0F00) >> 8];
    uint8_t n2 = v[(a & 0x00F0) >> 4];
    if(n1 != n2)
        pc += 2;
    return 0;
}

int op_ten(uint16_t a)
{
    I = a & 0x0FFF;
    printf("Register I = %d\n", I);
    return 0;
}

int op_eleven(uint16_t a)
{
    pc = (a & 0x0FFF) + v[0];
    pc -= 2;
    return 0;
}

int op_twelve(uint16_t a)
{
    srand(time(NULL));
    int r = rand() % 256;
    v[(a & 0x0F00) >> 8] = r & (a & 0x00FF);
    return 0;
}

int op_thirteen(uint16_t a)
{
    printf("Display sprites\n");
    int i, k;
    uint8_t x, y, b;
    x = (a & 0x0F00) >> 8;
    y = (a & 0x00F0) >> 4;
    x = v[x] % 64;
    y = v[y] % 32;

    uint8_t ab[8] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};
    
    for(i = 0, a = a & 0x000F; i < a; i++, y++)
        for(k = 0; k < 8; k++)
            if(mem[I + i] & ab[k])
            {
                b = scrn[y][x + k];
                scrn[y][x + k] = scrn[y][x + k] ^ 1;
                if(b == 1 && scrn[y][x + k] != 1)
                    v[0xF] = 1;
                draw_scaled_pixel(x + k, y, scrn[y][x + k]);
            }

    return 0;
}

int op_fourteen(uint16_t a)
{
    switch(a & 0x00FF)
    {
        case 0x9E:
            if(k[v[(a & 0xF00) >> 8]])
            {
                pc += 2;
                k[v[(a & 0xF00) >> 8]] = 0;
            }
            break;
        case 0xA1:
            if(k[v[(a & 0xF00) >> 8]])
            {
                pc += 2;
                k[v[(a & 0xF00) >> 8]] = 0;
            }
            break;
    }
    return 0;
}

int op_fifteen(uint16_t a)
{
    int i, k;
    uint32_t j;
    uint8_t b;
    switch (a & 0x00FF)
    {
        case 0x07:
            j = SDL_GetTicks() - starttime;
            if(!timer)
            {
                v[(a & 0x0F00) >> 8] = 0;
            }
            else if(j > timer)
            {
                v[(a & 0x0F00) >> 8] = 0;
                timer = 0;
            }
            else
            {
                v[(a & 0x0F00) >> 8] = timer - j;
            }
            printf("Read the delay timer: %d\n", v[(a & 0x0F00) >> 8]);
            break;
        case 0x0A:
            /* code */
            break;
        case 0x15:
            timer = v[(a & 0x0F00) >> 8] * 16;
            starttime = SDL_GetTicks();
            printf("Write to the delay time: %d\n", v[(a & 0x0F00) >> 8]);
            break;
        case 0x18:
            /* code */
            break;
        case 0x1E:
            I += v[(a & 0x0F00) >> 8];
            break;
        case 0x29:
            I = v[(a & 0x0F00) >> 8] * 5;
            printf("I = %d\n", v[(a & 0x0F00) >> 8] * 5);
            break;
        case 0x33:
            b = v[(a & 0x0F00) >>  8];
            mem[I] = b /100;
            mem[I + 1] = (b % 100)/10;
            mem[I + 2] = b % 10;
            printf("Allocated %u, %u, %u\n", mem[I], mem[I+1], mem[I+2], I, I+1, I+2);
            break;
        case 0x55:
            for(i = 0, k = (a & 0x0F00) >> 8; i <= k; i++)
                mem[I+i] = v[i];
            break;
        case 0x65:
            for(i = 0, k = (a & 0x0F00) >> 8; i <= k; i++)
                v[i] = mem[I+i];
            printf("0x65\n");
            break;
        default:
            EXIT_FAILURE;
    }
    return 0;
}
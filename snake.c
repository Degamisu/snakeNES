#include <stdlib.h>
#include <string.h>
#include "neslib.h"

const char PALETTE[32] = {
  0x0F, 0x16, 0x0F, 0x26, 0x0F, 0x02, 0x30, 0x20,
  0x0F, 0x15, 0x27, 0x05, 0x0F, 0x0D, 0x30, 0x25,
  0x0F, 0x27, 0x30, 0x16, 0x0F, 0x0F, 0x17, 0x3A,
  0x0F, 0x30, 0x2A, 0x0F, 0x0F, 0x30, 0x0F, 0x30
};

#define SNAKE_MAX_LENGTH 256
#define SNAKE_SPRITE 48

struct Snake {
    int x[SNAKE_MAX_LENGTH];
    int y[SNAKE_MAX_LENGTH];
    int length;
    int direction;
};

struct Snake snake;

void drawSnake() {
    int i;
    // Clear all sprites
    oam_clear();
    
    // Draw the snake head sprite at the head position
    oam_spr(snake.x[0] * 8, snake.y[0] * 8, SNAKE_SPRITE, 0, 0);
    
    // Draw the rest of the snake body using a different sprite (1A)
    for (i = 1; i < snake.length; i++) {
        oam_spr(snake.x[i] * 8, snake.y[i] * 8, 0x1A, 1, 1);  // Use sprite 1A for the body
    }
}


void handleInput() {
    if (pad_trigger(0)) {
        if (pad_poll(0) & PAD_LEFT && snake.direction != 0) {
            snake.direction = 2;  // Left
        } else if (pad_poll(0) & PAD_RIGHT && snake.direction != 2) {
            snake.direction = 0;  // Right
        } else if (pad_poll(0) & PAD_UP && snake.direction != 1) {
            snake.direction = 3;  // Up
        } else if (pad_poll(0) & PAD_DOWN && snake.direction != 3) {
            snake.direction = 1;  // Down
        }
    }
}

void moveSnake() {
    int i;

    // Move the body
    for (i = snake.length - 1; i > 0; i--) {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    // Move the head based on the direction
    switch (snake.direction) {
        case 0:  // Right
            snake.x[0]++;
            break;
        case 1:  // Down
            snake.y[0]++;
            break;
        case 2:  // Left
            snake.x[0]--;
            break;
        case 3:  // Up
            snake.y[0]--;
            break;
    }

    // Wrap around the screen
    snake.x[0] = (snake.x[0] + 32) % 32;
    snake.y[0] = (snake.y[0] + 30) % 30;
}

int checkCollision() {
    int i;

    // Check for collision with the snake's own body
    for (i = 1; i < snake.length; i++) {
        if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
            return 1;  // Collision detected
        }
    }

    return 0;  // No collision
}

void setupGraphics() {
    // Clear sprites
    oam_clear();

    // Set palette colors
    pal_all(PALETTE);

    // Enable sprite rendering
    ppu_on_spr();

    // Enable rendering
    ppu_on_all();

    // Wait for the PPU to stabilize
    delay(2);
}


void main(void) {

    setupGraphics();

    snake.x[0] = 10;
    snake.y[0] = 10;
    snake.length = 1;
    snake.direction = 0;

    while (1) {
        // Update PPU
        ppu_wait_nmi();

        handleInput();
        moveSnake();

        if (checkCollision()) {
            // Game over logic (you can implement your own)
            // For now, just reset the game
            snake.x[0] = 10;
            snake.y[0] = 10;
            snake.length = 1;
            snake.direction = 0;
        }

        drawSnake();
    }
}

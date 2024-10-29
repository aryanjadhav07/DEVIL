#include <graphics.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cstdio> // For sprintf
#include <windows.h> // For full-screen support

void exitmessage(void);
void lost(void);

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator with current time
    int gd = DETECT, gm, no = 0, area, maxx, maxy, chance = 0;
    int ch, x = 25, y = 25, xdir = 1, ydir = 1, x1 = 20, y1, x1dir = 0, area1;
    void *buff, *cuff;

    // Initialize graphics in full screen
    initwindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), "Full Screen Game");

    maxx = getmaxx();
    maxy = getmaxy();

    // Original devil design
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, RED);
    circle(50, 50, 30);  // Devil's head (circular)
    floodfill(50, 50, WHITE);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    circle(42, 45, 5);  // Left eye
    floodfill(42, 45, BLACK);
    circle(58, 45, 5);  // Right eye
    floodfill(58, 45, BLACK);
    setcolor(WHITE);
    arc(50, 55, 0, 180, 10);  // Smiling mouth
    setfillstyle(SOLID_FILL, RED);
    floodfill(50, 55, WHITE);  // Fill the mouth color

    // Draw the horns
    setcolor(BLACK);
    line(30, 30, 40, 10);  // Left horn
    line(40, 10, 50, 30);
    line(70, 30, 60, 10);  // Right horn
    line(60, 10, 50, 30);

    // Store devil's image
    area = imagesize(25, 20, 75, 80);
    buff = malloc(area);
    getimage(25, 20, 75, 80, buff);
    putimage(25, 20, buff, XOR_PUT); // Erase devil

    y1 = maxy / 2;

    // Create a larger and more realistic gun shape
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    
    // Draw gun barrel
    int gunBarrelWidth = 120;
    int gunBarrelHeight = 50;
    rectangle(10, y1 - gunBarrelHeight / 2, 10 + gunBarrelWidth, y1 + gunBarrelHeight / 2);
    floodfill(15, y1, WHITE);

    // Draw gun handle
    int gunHandleWidth = 40;
    int gunHandleHeight = 50;
    rectangle(10, y1 + gunBarrelHeight / 2 - 10, 10 + gunHandleWidth, y1 + gunBarrelHeight / 2 + gunHandleHeight);
    floodfill(15, y1 + gunBarrelHeight / 2, WHITE);

    // Save the gun image to memory for easy redraw
    area1 = imagesize(10, y1 - gunBarrelHeight / 2, 10 + gunBarrelWidth, y1 + gunBarrelHeight / 2 + gunHandleHeight);
    cuff = malloc(area1);
    getimage(10, y1 - gunBarrelHeight / 2, 10 + gunBarrelWidth, y1 + gunBarrelHeight / 2 + gunHandleHeight, cuff);
    putimage(10, y1 - gunBarrelHeight / 2, cuff, XOR_PUT); // Erase gun

    // Set viewport and text styles
    setcolor(14);
    rectangle(0, 0, maxx, maxy);
    setviewport(1, 1, maxx - 1, maxy - 1, 1);
    settextstyle(1, 0, 3);
    settextjustify(1, 1);
    delay(4000);
    setcolor(WHITE);

    // Display random pixels
    while (no < 100) {
        putpixel(rand() % maxx, rand() % maxy, rand() % 16); // Use rand() to generate random pixels
        no++;
    }
    outtextxy(maxx / 2, 30, "...the world is coming to an end...");
    outtextxy(maxx / 2, 80, "...the devil is going to destroy the universe...");
    outtextxy(maxx / 2, 130, "...destroy the devil before it destroys us...");
    outtextxy(maxx / 2, 180, "...ALL THE BEST...");
    delay(2000);
    outtextxy(maxx / 2, 270, "use '6' to shoot the devil");
    outtextxy(maxx / 2, 310, "use arrow keys to move gun vertically");
    outtextxy(maxx / 2, 360, "You have only 10 bullets");
    outtextxy(maxx / 2, 450, "press any key to continue....");

    while (!kbhit()); // Wait for input
    getch(); // Consume the key press

    do {
        clearviewport();
        // Display bullets left using outtextxy
        char bulletsText[30];
        sprintf(bulletsText, "Bullets Left: %d", 10 - chance);
        outtextxy(130, 30, bulletsText); // Display the bullet count at position (10, 10)

        no = 0;
        while (no < 100) {
            putpixel(rand() % maxx, rand() % maxy, rand() % 16);
            no++;
        }

        if (kbhit()) {
            ch = getch(); // Use getch() to read a key
            if (ch == '6') { // Shoot bullet
                x1dir = 1;
                chance++;
            }
            if (ch == 72 && x1dir == 0) // Up arrow key
                y1 -= 20; // Move gun up
            if (ch == 80 && x1dir == 0) // Down arrow key
                y1 += 20; // Move gun down
            if (ch == 27) // Escape key
                exitmessage();
        }

        // Draw the bullet (increased speed)
        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        int bulletWidth = 30;  // Wider bullet
        int bulletHeight = 10;  // Taller bullet
        fillellipse(x1 + bulletWidth / 2, y1, bulletWidth, bulletHeight); // Larger bullet
        
        putimage(10, y1 - gunBarrelHeight / 2, cuff, XOR_PUT); // Draw gun
        delay(7); // Reduced delay for increased speed
        putimage(10, y1 - gunBarrelHeight / 2, cuff, XOR_PUT); // Erase gun
        x1 += 30 * x1dir; // Increased bullet speed (changed from 20 to 30)
        putimage(x, y, buff, XOR_PUT); // Draw devil
        delay(30);
        putimage(x, y, buff, XOR_PUT); // Erase devil
        x += (xdir * (rand() % 30));
        y += (ydir * (rand() % 20));

        // Keep devil within screen
        if (x >= maxx - 101) {
            xdir *= -1;
            x = maxx - 101;
        }
        if (x <= 175 && xdir == 1) x = 175;
        if (x <= 175 && xdir == -1) xdir *= -1;
        if (y >= maxy - 101) {
            ydir *= -1;
            y = maxy - 101;
        }
        if (y <= 30) {
            ydir *= -1;
            y = 30;
        }

        if (x1 >= maxx - 3 || x <= 1) {
            x1 = 20;
            x1dir = 0;
        }
        if (y1 >= maxy - 50) // Keep gun within screen (adjust for larger gun)
            y1 = maxy - 51;
        if (y1 <= 50) y1 = 51;

        // Successful hit
        if (x1 <= x + 60 && x1 >= x + 12 && y1 <= y + 60 && y1 >= y + 10) {
            outtextxy(maxx / 2, maxy / 2, "...YOU SAVED THE WORLD!..."); 
            delay(5000);
            exit(0);
        }
    } while (chance <= 10);

    if (chance == 11)
        lost();

    closegraph();
    return 0;
}

void exitmessage() {
    char cho;
    outtextxy(getmaxx() / 2, getmaxy() / 2, "EXIT : Y or N...");
    delay(100);

    while (!kbhit()); // Wait for input
    cho = getch(); // Read the input character

    if (cho == 'y' || cho == 'Y')
        exit(0);
    else if (cho == 'n' || cho == 'N') {
        clearviewport();
        return;
    }
}

void lost() {
    outtextxy(getmaxx() / 2, getmaxy() / 2 - 50, "Your bullets are over");
    outtextxy(getmaxx() / 2, getmaxy() / 2, "The devil destroys the world");
}

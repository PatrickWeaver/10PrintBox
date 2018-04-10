/*  The 10 Print algorithm, implemented on an Arduino with native TV output
 *  See https://10print.org
 *  Remixed by Patrick Weaver from a Arduino TVOut version written by Danny Walker, 2018
 *  Licensed as CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/
 *  http://patrickweaver.net
 *  http://danny.makesthings.work
 */

#include <TVout.h> // Include the TV output library.

// If you don't have the TVout Library installed already, install it through Sketch -> Include Library -> Manage Libraries.

TVout TV; // Declare the TVout object, and call it TV for convenience.
int gridSize = 8; // The size of the tiles. Feel free to play with this and see what happens! It's best to keep it to a common factor of 128 and 96, so 4, 8, 16 etc.
int xRes;
int yRes;

void setup()  {
  TV.begin(NTSC);	// Start the TV output. Change NTSC to PAL if you're in not USA/any other NTSC region.
  xRes = TV.hres() / gridSize;
  yRes = TV.vres() / gridSize;
  Serial.begin(9600);
   for(int y=0;y<TV.vres();y+=gridSize){ // Loop over the horizontal rows of the tile grid.
    for(int x=0;x<TV.hres();x+= gridSize){ // For each row, loop over each individual tile horizontally.
      drawGridSquare(x,y,gridSize, 0);
    }
  }
}

void loop() {

  int xRand = random(0, xRes) * gridSize;
  int yRand = random(0, yRes) * gridSize;
  drawGridSquare(xRand, yRand, gridSize, 10);
}

void drawGridSquare(int x,int y, int gridSize, int delayTime) {

  int threshold = analogRead(A0); // Read the potentiometer to determine the threshold to be used.
  int randomNum = random(0,1023); // Generate a random number from 0-1023

  if(randomNum > threshold){
    // Clear one grid square
    TV.draw_rect(x,y,gridSize-1,gridSize-1,0,0);
    // Draw a forward slash
    TV.draw_line(x, y+gridSize-1, x+gridSize-1, y,1);
  } else {
    // Clear one grid square
    TV.draw_rect(x,y,gridSize-1,gridSize-1,0,0);
    // Draw a backward slash
    TV.draw_line(x,y,x+gridSize, y+gridSize,1);
  }
  TV.delay(delayTime);
}

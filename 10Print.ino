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
int gridSizes[] = {1, 4, 8, 16, 32};
int gridSize = 8; // Default size is 8
int delayTimes[] = {0, 2, 5, 10, 25, 75, 200, 500, 1000, 5000, 86400000};
int delayTime;
int hRes;
int vRes;
int xRes;
int yRes;

void setup()  {
  TV.begin(NTSC);	// Start the TV output. Change NTSC to PAL if you're in not USA/any other NTSC region.
  hRes = TV.hres();
  vRes = TV.vres();
  Serial.begin(9600);

  for(int y=0;y<TV.vres();y+=gridSize){ // Loop over the horizontal rows of the tile grid.
    for(int x=0;x<TV.hres();x+= gridSize){ // For each row, loop over each individual tile horizontally.
      drawGridSquare(x,y,gridSize);
    }
  }

}

void loop() {
  int sizeRead = analogRead(A1); // Read the potentiometer to get grid size
  int gridSizeArrayDivisor = (1023 / (sizeof(gridSizes) / sizeof(gridSizes[0]))) + 1;
  gridSize = gridSizes[sizeRead / gridSizeArrayDivisor];

  int delayRead = analogRead(A2);
  int delayTimeArrayDivisor = (1023 / (sizeof(delayTimes) / sizeof(delayTimes[0]))) + 1;
  delayTime = delayTimes[delayRead / delayTimeArrayDivisor];


  xRes = hRes / gridSize;
  yRes = vRes / gridSize;
  int xRand = random(0, xRes) * gridSize;
  int yRand = random(0, yRes) * gridSize;

  drawGridSquare(xRand, yRand, gridSize);

  delayIfSame(delayTime, delayTimes, delayTimeArrayDivisor);
}

void drawGridSquare(int x,int y, int gridSize) {

  int directionRead = analogRead(A0); // Read the potentiometer to get direction probability
  int randomNum = random(0,1023); // Generate a random number from 0-1023

  if(randomNum > directionRead){
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
}

int delayIfSame(int delayTime, int *delayTimes, int delayTimeArrayDivisor) {
  if (delayTime > 100) {
    for (int i = 0; i < delayTime; i+= 100) {
      int newDelayRead = analogRead(A2);
      int newDelayTime = delayTimes[newDelayRead / delayTimeArrayDivisor];
      if (newDelayTime == delayTime) {
        TV.delay(100);
      } else {
        break;
      }
    }
  } else {
    TV.delay(delayTime);
  }

}

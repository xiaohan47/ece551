#include <stdio.h>
#include <stdlib.h>
//I've provided "min" and "max" functions in
//case they are useful to you
int min (int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}
int max (int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

//Declare your rectangle structure here!
struct _rectangle{
	int x;
	int y;
	int width;
	int height;
};
typedef struct _rectangle rectangle;

rectangle canonicalize(rectangle r) {
  //WRITE THIS FUNCTION
  if(r.width<0){
	  r.width = 0-r.width;
          r.x = r.x-r.width;
  }
  if(r.height<0){
	  r.height = 0-r.height;
	  r.y = r.y-r.height;
  }
  else{
	  return r;
  }
  return r;
}
rectangle intersection(rectangle r1, rectangle r2) {
  //WRITE THIS FUNCTION
  if (r1.width<0||r1.height<0){
	  r1=canonicalize(r1);
  }
  if (r2.width<0||r2.height<0){
	  r2=canonicalize(r2);
  }
  //canonicalize rectangle
  int r1_topx,r1_topy,r2_topx,r2_topy;
  r1_topx = r1.x+r1.width;
  r1_topy = r1.y+r1.height;
  r2_topx = r2.x+r2.width;
  r2_topy = r2.y+r2.height;
  //sign rectangle top right
  rectangle result_r;
  if (r1.y>r2_topy||r2.y>r1_topy||r1_topx<r2.x||r2_topx<r1.x){
	  result_r.x=max(r1.x,r2.x);
	  result_r.y=max(r1.y,r2.y);
	  result_r.width=0;
	  result_r.height=0;
	  return (result_r);
  }
  //no intersection condition
  if (r1.y==r2_topy||r2.y==r1_topy){
	  result_r.x=max(r1.x,r2.x);
	  result_r.y=max(r1.y,r2.y);
	  result_r.width=min(r1_topx,r2_topx)-result_r.x;
	  result_r.height=0;
          return (result_r);
  }
  if (r1_topx==r2.x||r2_topx==r1.x){
	  result_r.x=max(r1.x,r2.x);
	  result_r.y=max(r1.y,r2.y);
	  result_r.width=0;
	  result_r.height=min(r1_topy,r2_topy)-result_r.y;
          return (result_r);
  }
//one side intersection but no overlap
  else{ 
  result_r.x = max(r1.x,r2.x);
  result_r.y = max(r1.y,r2.y);
  result_r.width = min(r1_topx,r2_topx)-result_r.x;
  result_r.height=min(r1_topy,r2_topy)-result_r.y;
  return result_r;
  }
}

//You should not need to modify any code below this line
void printRectangle(rectangle r) {
  r = canonicalize(r);
  if (r.width == 0 && r.height == 0) {
    printf("<empty>\n");
  }
  else {
    printf("(%d,%d) to (%d,%d)\n", r.x, r.y, 
	                           r.x + r.width, r.y + r.height);
  }
}

int main (void) {
  rectangle r1;
  rectangle r2;
  rectangle r3;
  rectangle r4;

  r1.x = 2;
  r1.y = 3;
  r1.width = 5;
  r1.height = 6;
  printf("r1 is ");
  printRectangle(r1);

  r2.x = 4;
  r2.y = 5;
  r2.width = -5;
  r2.height = -7;
  printf("r2 is ");
  printRectangle(r2);
  
  r3.x = -2;
  r3.y = 7;
  r3.width = 7;
  r3.height = -10;
  printf("r3 is ");
  printRectangle(r3);

  r4.x = 0;
  r4.y = 7;
  r4.width = -4;
  r4.height = 2;
  printf("r4 is ");
  printRectangle(r4);

  //test everything with r1
  rectangle i = intersection(r1,r1);
  printf("intersection(r1,r1): ");
  printRectangle(i);

  i = intersection(r1,r2);
  printf("intersection(r1,r2): ");
  printRectangle(i);
  
  i = intersection(r1,r3);
  printf("intersection(r1,r3): ");
  printRectangle(i);

  i = intersection(r1,r4);
  printf("intersection(r1,r4): ");
  printRectangle(i);

  //test everything with r2
  i = intersection(r2,r1);
  printf("intersection(r2,r1): ");
  printRectangle(i);

  i = intersection(r2,r2);
  printf("intersection(r2,r2): ");
  printRectangle(i);
  
  i = intersection(r2,r3);
  printf("intersection(r2,r3): ");
  printRectangle(i);

  i = intersection(r2,r4);
  printf("intersection(r2,r4): ");
  printRectangle(i);

  //test everything with r3
  i = intersection(r3,r1);
  printf("intersection(r3,r1): ");
  printRectangle(i);

  i = intersection(r3,r2);
  printf("intersection(r3,r2): ");
  printRectangle(i);
  
  i = intersection(r3,r3);
  printf("intersection(r3,r3): ");
  printRectangle(i);

  i = intersection(r3,r4);
  printf("intersection(r3,r4): ");
  printRectangle(i);

  //test everything with r4
  i = intersection(r4,r1);
  printf("intersection(r4,r1): ");
  printRectangle(i);

  i = intersection(r4,r2);
  printf("intersection(r4,r2): ");
  printRectangle(i);
  
  i = intersection(r4,r3);
  printf("intersection(r4,r3): ");
  printRectangle(i);

  i = intersection(r4,r4);
  printf("intersection(r4,r4): ");
  printRectangle(i);


  return EXIT_SUCCESS;

}

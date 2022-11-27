#include <stdio.h>

struct Box {
  
  int id;
  int weight;
  int length;
  int width;
  int height;
    
} exampleBox = {
    1, 25, 100, 200, 400
};

void printInt(int x) {
    printf("The value of this box value is: %i\n", x);
}

int main() {
    struct Box* boxPtr = &exampleBox;
    
    printInt(boxPtr->height);
    printInt(boxPtr->width);
    printInt(boxPtr->length);
    printInt(boxPtr->weight);
    printInt(boxPtr->id);

    return 0;
}
#include <stdio.h>
#include <stdbool.h>

// Raster CLI
// Author: Jethro Kyle Sempio

const int HEIGHT = 30;
const int WIDTH = 30;

struct FrameBuffer{
    bool buffer[HEIGHT][WIDTH];
};

struct Point {
    int x;
    int y;
};

struct Triangle {
    struct Point point1;
    struct Point point2;
    struct Point point3;
};

bool isOnPoint(struct Point *point, int x, int y){
    if(point->x == x && point->y == y){
        return true;
    }
    return false;
}

struct Point getRawCoordinates(int *x, int *y) {
    struct Point rawPoint;
    int halfWidth = WIDTH / 2;
    int halfHeight = HEIGHT / 2;
    rawPoint.x = halfWidth + *x;
    rawPoint.y = halfHeight - *y;
    return rawPoint;
}

void drawLineUpwardAngle(struct FrameBuffer *buffer, struct Point *pointA, struct Point *pointB){
    struct Point rawPoint;
    int x_gap = (pointB->x - pointA->x) - 2;
    int y_gap = (pointA->y - pointB->y) - 2;
    int pixels_per_line = y_gap / x_gap;
    int x_counter = pixels_per_line;
    int curr_x = pointA->x;
    int curr_y = pointA->y;
    
    rawPoint = getRawCoordinates(&pointA->x, &pointA->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;
    rawPoint = getRawCoordinates(&pointB->x, &pointB->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;

    for(int y = 0; y < y_gap; y++){
        curr_y++;
        while(x_counter > 0 && curr_x < pointB->x){
            curr_x++;
            rawPoint = getRawCoordinates(&curr_x, &curr_y);
            buffer->buffer[rawPoint.y][rawPoint.x] = true;
            x_counter--;
        }
        x_counter = pixels_per_line;
    }
}

void drawLineDownwardAngle(struct FrameBuffer *buffer, struct Point *pointA, struct Point *pointB){
    struct Point rawPoint;
    int x_gap = (pointB->x - pointA->x) - 1;
    int y_gap = (pointA->y - pointB->y) - 1;
    int pixels_per_line = y_gap / x_gap;
    int x_counter = pixels_per_line;
    int curr_x = pointA->x;
    int curr_y = pointA->y;

    rawPoint = getRawCoordinates(&pointA->x, &pointA->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;
    rawPoint = getRawCoordinates(&pointB->x, &pointB->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;

    for(int y = 0; y < y_gap; y++){
        curr_y--;
        while(x_counter > 0){
            if(curr_x < pointB->x){
                curr_x++;
            }
            rawPoint = getRawCoordinates(&curr_x, &curr_y);
            buffer->buffer[rawPoint.y][rawPoint.x] = true;
            x_counter--;
        }
        x_counter = pixels_per_line;
    }
}

void drawLineVertical(struct FrameBuffer *buffer, struct Point *pointA, struct Point *pointB){
    struct Point rawPoint;
    
    rawPoint = getRawCoordinates(&pointA->x, &pointA->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;
    rawPoint = getRawCoordinates(&pointB->x, &pointB->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;

    int curr_y = pointA->x - 1;

    while(curr_y > pointB->y){
        rawPoint = getRawCoordinates(&curr_y, &pointA->y);
        buffer->buffer[rawPoint.y][rawPoint.x] = true;
        curr_y--;
    }
}

void drawLineHorizontal(struct FrameBuffer *buffer, struct Point *pointA, struct Point *pointB){
    struct Point rawPoint;
    
    rawPoint = getRawCoordinates(&pointA->x, &pointA->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;
    rawPoint = getRawCoordinates(&pointB->x, &pointB->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;

    int curr_x = pointA->x + 1;

    while(curr_x < pointB->x){
        rawPoint = getRawCoordinates(&curr_x, &pointA->y);
        buffer->buffer[rawPoint.y][rawPoint.x] = true;
        curr_x++;
    }
}

void drawLine(struct FrameBuffer *buffer, struct Point *pointA, struct Point *pointB){

    bool isUpward;
    bool isSwap;

    struct Point rawPoint;
    struct Point derivedPoint;

    if((pointA->x == pointB->x) && (pointA->y == pointB->y)){
        rawPoint = getRawCoordinates(&pointA->x, &pointA->y);
        buffer->buffer[rawPoint.y][rawPoint.x] = true;
    }
    else if((pointA->x == pointB->x) && (pointA->y != pointB->y)){
        // loop here
        if(pointA->y > pointB->y){
            drawLineHorizontal(buffer, pointA, pointB);
        }
        else {
            drawLineHorizontal(buffer, pointB, pointA);
        }
    }
    else if((pointA->x != pointB->x) && (pointA->y == pointB->y)){
        // loop here
        if(pointA->x > pointB->x){
            drawLineVertical(buffer, pointA, pointB);
        }
        else {
            drawLineVertical(buffer, pointB, pointA);
        }
    }
    else {
        if(pointA->y > pointB->y){
            isUpward = false;
        }
        else {
            isUpward = true;
        }

        if(pointA->x < pointB->x){
            isSwap = false;
        }
        else {
            isSwap = true;
        }

        if(isUpward){
            if(!isSwap){
                drawLineUpwardAngle(buffer, pointA, pointB);
            }
            else {
                drawLineUpwardAngle(buffer, pointB, pointA);
            }
        }
        else {
            if(!isSwap){
                drawLineDownwardAngle(buffer, pointA, pointB);
            }
            else {
                drawLineDownwardAngle(buffer, pointB, pointA);
            }
        }
    }
}

void drawTriangle(struct FrameBuffer *buffer ,struct Triangle tri){
    
    int x_gap = (tri.point2.x - tri.point1.x) - 2;
    int y_gap = (tri.point2.y - tri.point1.y) - 2;
    int pixel_per_line = 0;
    
    int last_x = 0;
    int counter_x = 0;

    if(x_gap < 0){
        x_gap = x_gap * -1;
    }

    if(y_gap < 0){
        y_gap = y_gap * -1;
    }

    pixel_per_line = x_gap / y_gap;
    printf("%d, %d, %d", x_gap, y_gap, pixel_per_line);

    for(int y=0; y < HEIGHT; y++){
        
        counter_x = pixel_per_line;
        
        for(int x=0; x < WIDTH; x++){
            if(isOnPoint(&tri.point1, x, y) || isOnPoint(&tri.point2, x, y) || isOnPoint(&tri.point3, x, y)){
                buffer->buffer[y][x] = true;
            }
            else {
                struct Point *point2 = &tri.point2;
                if(counter_x > 0){
                    if(y < point2->y){
                        if(last_x < point2->x){
                            buffer->buffer[y][last_x] = true;
                            counter_x--;
                            last_x++;
                        }
                        else {
                            counter_x = 0;
                        }
                        
                    }
                    else {
                        buffer->buffer[y][last_x] = false;
                    }
                    buffer->buffer[y][last_x] = false;
                }
            }
        }
    }
}

void clearBuffer(struct FrameBuffer *buffer){
    for(int y=0; y < HEIGHT; y++){
        for(int x=0; x < WIDTH; x++){
            buffer->buffer[y][x] = false;
        }
    }
}

void drawBuffer(struct FrameBuffer *buffer){
    for(int y=0; y < HEIGHT; y++){
        for(int x=0; x < WIDTH; x++){
            if(buffer->buffer[y][x]){
                printf("█");
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main(){
    struct FrameBuffer buffer;
    struct Point pointA, pointB;

    clearBuffer(&buffer);

    //pointA.x = 0;
    //pointA.y = 0;

    //pointB.x = 6;
    //pointB.y = -6;
    
    //drawLine(&buffer, &pointA, &pointB);

    pointA.x = 2;
    pointA.y = 2;

    pointB.x = 8;
    pointB.y = -8;
    
    drawLine(&buffer, &pointA, &pointB);

    drawBuffer(&buffer);
    return 0;
}
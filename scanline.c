#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

// Raster CLI
// Author: Jethro Kyle Sempio

const int HEIGHT = 22;
const int WIDTH = 40;

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
    int x_gap = (pointB->x - pointA->x) - 1;
    int y_gap = (pointB->y - pointA->y) - 1;
    
    int pixels_per_line = 0;
    bool is_x_mode = true;
    bool is_x_zero_gap_mode = false;
    bool is_y_zero_gap_mode = false;

    if(x_gap == 0){
        is_x_zero_gap_mode = true;
    }
    else if(y_gap == 0){
        is_y_zero_gap_mode = true;
    }
    else if(x_gap > y_gap){
        pixels_per_line = x_gap / y_gap;
    }
    else {
        pixels_per_line = y_gap / x_gap;
        is_x_mode = false;
    }
    
    int y_median = 0;
    int remainder = 0;
    
    if(remainder > 0){
        y_median = y_gap / 2 - remainder;
    }
    else {
        y_median = -1;
    }

    int counter = pixels_per_line;
     
    int curr_x = pointA->x;
    int curr_y = pointA->y;

    rawPoint = getRawCoordinates(&pointA->x, &pointA->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;
    rawPoint = getRawCoordinates(&pointB->x, &pointB->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;

    bool isMedianMode = false;
    if(is_x_zero_gap_mode){
        while(curr_y < pointB->y){
            rawPoint = getRawCoordinates(&curr_x, &curr_y);
            buffer->buffer[rawPoint.y][rawPoint.x] = true;
            curr_y++;
        }
    }
    else if(is_y_zero_gap_mode){
         while(curr_x < pointB->x){
            rawPoint = getRawCoordinates(&curr_x, &curr_y);
            buffer->buffer[rawPoint.y][rawPoint.x] = true;
            curr_x++;
        }
    }
    else if(is_x_mode){
        for(int y = 0; y < y_gap; y++){
            curr_y++;

            if(y == y_median){
                isMedianMode = true;
            }
            else if(isMedianMode && remainder == 0){
                isMedianMode = false;
            }

            if(!isMedianMode){
                while(counter > 0){
                    if(curr_x < pointB->x){
                        curr_x++;
                    }
                    rawPoint = getRawCoordinates(&curr_x, &curr_y);
                    buffer->buffer[rawPoint.y][rawPoint.x] = true;
                    counter--;
                }
                counter = pixels_per_line;
            }
            else {
                rawPoint = getRawCoordinates(&curr_x, &curr_y);
                buffer->buffer[rawPoint.y][rawPoint.x] = true;
                remainder--;
            }
            
        }
    }
    else {
        for(int x = 0; x < x_gap; x++){
            curr_x++;
            while(counter > 0){
                if(curr_y < pointB->y){
                    curr_y++;
                }
                rawPoint = getRawCoordinates(&curr_x, &curr_y);
                buffer->buffer[rawPoint.y][rawPoint.x] = true;
                counter--;
            }
            counter = pixels_per_line;
        }
    }
}

void drawLineDownwardAngle(struct FrameBuffer *buffer, struct Point *pointA, struct Point *pointB){
    struct Point rawPoint;
    int x_gap = (pointB->x - pointA->x) - 1;
    int y_gap = (pointA->y - pointB->y) - 1;
    
    int pixels_per_line = 0;
    bool is_x_mode = true;
    bool is_x_zero_gap_mode = false;
    bool is_y_zero_gap_mode = false;

    if(x_gap == 0){
        is_x_zero_gap_mode = true;
    }
    else if(y_gap == 0){
        is_y_zero_gap_mode = true;
    }
    else if(x_gap > y_gap){
        pixels_per_line = x_gap / y_gap;
    }
    else {
        pixels_per_line = y_gap / x_gap;
        is_x_mode = false;
    }
    
    int y_median = 0;
    int remainder = 0;
    
    if(remainder > 0){
        y_median = y_gap / 2 - remainder;
    }
    else {
        y_median = -1;
    }

    int counter = pixels_per_line;
     
    int curr_x = pointA->x;
    int curr_y = pointA->y;

    rawPoint = getRawCoordinates(&pointA->x, &pointA->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;
    rawPoint = getRawCoordinates(&pointB->x, &pointB->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;

    bool isMedianMode = false;
    if(is_x_zero_gap_mode){
        for(int y = 0; y < y_gap; y++){
            curr_y++;
            rawPoint = getRawCoordinates(&curr_x, &curr_y);
            buffer->buffer[rawPoint.y][rawPoint.x] = true;
        }
    }
    else if(is_y_zero_gap_mode){
        for(int x = 0; x < x_gap; x++){
            curr_x++;
            rawPoint = getRawCoordinates(&curr_x, &curr_y);
            buffer->buffer[rawPoint.y][rawPoint.x] = true;
        }
    }
    if(is_x_mode){
        for(int y = 0; y < y_gap; y++){
            curr_y--;

            if(y == y_median){
                isMedianMode = true;
            }
            else if(isMedianMode && remainder == 0){
                isMedianMode = false;
            }

            if(!isMedianMode){
                while(counter > 0){
                    if(curr_x < pointB->x){
                        curr_x++;
                    }
                    rawPoint = getRawCoordinates(&curr_x, &curr_y);
                    buffer->buffer[rawPoint.y][rawPoint.x] = true;
                    counter--;
                }
                counter = pixels_per_line;
            }
            else {
                rawPoint = getRawCoordinates(&curr_x, &curr_y);
                buffer->buffer[rawPoint.y][rawPoint.x] = true;
                remainder--;
            }
            
        }
    }
    else {
        for(int x = 0; x < x_gap; x++){
            curr_x++;
            while(counter > 0){
                if(curr_y > pointB->y){
                    curr_y--;
                }
                rawPoint = getRawCoordinates(&curr_x, &curr_y);
                buffer->buffer[rawPoint.y][rawPoint.x] = true;
                counter--;
            }
            counter = pixels_per_line;
        }
    }
}

void drawLineVertical(struct FrameBuffer *buffer, struct Point *pointA, struct Point *pointB){
    struct Point rawPoint;
    
    rawPoint = getRawCoordinates(&pointA->x, &pointA->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;
    rawPoint = getRawCoordinates(&pointB->x, &pointB->y);
    buffer->buffer[rawPoint.y][rawPoint.x] = true;

    int curr_y = pointA->y - 1;

    while(curr_y > pointB->y){
        rawPoint = getRawCoordinates(&pointA->x, &curr_y);
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
            drawLineVertical(buffer, pointA, pointB);
        }
        else {
            drawLineVertical(buffer, pointB, pointA);
        }
    }
    else if((pointA->x != pointB->x) && (pointA->y == pointB->y)){
        // loop here
        if(pointA->x < pointB->x){
            drawLineHorizontal(buffer, pointA, pointB);
        }
        else {
            drawLineHorizontal(buffer, pointB, pointA);
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
                drawLineDownwardAngle(buffer, pointB, pointA);
            }
        }
        else {
            if(!isSwap){
                drawLineDownwardAngle(buffer, pointA, pointB);
            }
            else {
                drawLineUpwardAngle(buffer, pointB, pointA);
            }
        }
    }
}

void drawTriangle(struct FrameBuffer *buffer ,struct Triangle *tri){
    drawLine(buffer, &tri->point1, &tri->point2);
    drawLine(buffer, &tri->point3, &tri->point2);
    drawLine(buffer, &tri->point3, &tri->point1);
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

// Demo
int main(){
    struct FrameBuffer buffer;    
    struct Triangle tr1;

    int varY = 9;
    int varX = 10;
    int varX2 = -15;
    int varX3 = 0;

    bool polarity = true;
    while(true){
        clearBuffer(&buffer);

        tr1.point1.x = 2;
        tr1.point1.y = varX3;

        tr1.point3.x = varX2;
        tr1.point3.y = 5;

        tr1.point2.x = varX;
        tr1.point2.y = varY;

        if(varY == -4){
            polarity = false;
        }
        else if(varY == 9){
            polarity = true;
        }

        if(polarity){
            varY--;
            varX--;
            varX2++;
            varX3--;
        }
        else {
            varY++;
            varX++;
            varX2--;
            varX3++;
        }

        drawTriangle(&buffer, &tr1);
        drawBuffer(&buffer);
        printf("\n");
        printf("Raster CLI--------------------------- \n");
        printf("Point 1: (%d, %d) \n", tr1.point1.x, tr1.point1.y);
        printf("Point 2: (%d, %d) \n", tr1.point2.x, tr1.point2.y);
        printf("Point 3: (%d, %d) \n", tr1.point3.x, tr1.point3.y);
        usleep(70 * 1000);
        system("clear");
    }

    return 0;
}
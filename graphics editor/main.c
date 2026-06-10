#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 40
#define MAX_SHAPES 50

typedef enum { LINE, RECTANGLE, TRIANGLE, CIRCLE } ShapeType;

typedef struct {
    int id;
    ShapeType type;
    int p1[2], p2[2], p3[2]; 
    int radius;              
    int active;              
} Shape;

// Global Variables
char canvas[ROWS][COLS];
Shape shapes[MAX_SHAPES];

// Function Prototypes
void init_canvas();
void display_canvas();
void redraw_canvas();
void add_shape();
void delete_shape();
void modify_shape();

// Drawing Algorithms
void draw_line(int x0, int y0, int x1, int y1);
void draw_rectangle(int x1, int y1, int x2, int y2);
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void draw_circle(int xc, int yc, int r);

int main() {
    int choice;
    init_canvas();

    for (int i = 0; i < MAX_SHAPES; i++) {
        shapes[i].active = 0;
        shapes[i].id = i + 1;
    }

    while (1) {
        display_canvas();
        printf("\n=== 2D GRAPHICS EDITOR MENU ===\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Clear Canvas\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf(" %d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1: add_shape(); break;
            case 2: delete_shape(); break;
            case 3: modify_shape(); break;
            case 4: 
                init_canvas(); 
                for(int i=0; i<MAX_SHAPES; i++) shapes[i].active = 0; 
                break;
            case 5: printf("Exiting editor. Goodbye!\n"); exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

void init_canvas() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            canvas[i][j] = '_';
        }
    }
}

void display_canvas() {
    printf("\033[H\033[2J");
    fflush(stdout);

    printf("   ");
    for (int j = 0; j < COLS; j++) printf("%d", j % 10);
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("%2d ", i);
        for (int j = 0; j < COLS; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
    
    printf("\nActive Objects:\n");
    for (int i = 0; i < MAX_SHAPES; i++) {
        if (shapes[i].active) {
            printf("ID %d: ", shapes[i].id);
            if (shapes[i].type == LINE) printf("Line\n");
            else if (shapes[i].type == RECTANGLE) printf("Rectangle\n");
            else if (shapes[i].type == TRIANGLE) printf("Triangle\n");
            else if (shapes[i].type == CIRCLE) printf("Circle\n");
        }
    }
}

void redraw_canvas() {
    init_canvas();
    for (int i = 0; i < MAX_SHAPES; i++) {
        if (shapes[i].active) {
            switch (shapes[i].type) {
                case LINE:
                    draw_line(shapes[i].p1[0], shapes[i].p1[1], shapes[i].p2[0], shapes[i].p2[1]);
                    break;
                case RECTANGLE:
                    draw_rectangle(shapes[i].p1[0], shapes[i].p1[1], shapes[i].p2[0], shapes[i].p2[1]);
                    break;
                case TRIANGLE:
                    draw_triangle(shapes[i].p1[0], shapes[i].p1[1], shapes[i].p2[0], shapes[i].p2[1], shapes[i].p3[0], shapes[i].p3[1]);
                    break;
                case CIRCLE:
                    draw_circle(shapes[i].p1[0], shapes[i].p1[1], shapes[i].radius);
                    break;
            }
        }
    }
}

void plot(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        canvas[y][x] = '*';
    }
}

void draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        plot(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_rectangle(int x1, int y1, int x2, int y2) {
    draw_line(x1, y1, x2, y1);
    draw_line(x2, y1, x2, y2);
    draw_line(x2, y2, x1, y2);
    draw_line(x1, y2, x1, y1);
}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}

void draw_circle(int xc, int yc, int r) {
    int cx = 0;
    int cy = r;
    int d = 3 - 2 * r;
    
    while (cy >= cx) {
        plot(xc + cx, yc + cy); plot(xc - cx, yc + cy);
        plot(xc + cx, yc - cy); plot(xc - cx, yc - cy);
        plot(xc + cy, yc + cx); plot(xc - cy, yc + cx);
        plot(xc + cy, yc - cx); plot(xc - cy, yc - cx);
        cx++;
        if (d > 0) {
            cy--;
            d = d + 4 * (cx - cy) + 10;
        } else {
            d = d + 4 * cx + 6;
        }
    }
}

void add_shape() {
    // Clear out old data to make sure the selected shape renders completely on its own
    for (int i = 0; i < MAX_SHAPES; i++) {
        shapes[i].active = 0;
    }

    int target_index = 0; 
    Shape s;
    s.id = target_index + 1;
    s.active = 1;

    printf("\nSelect Shape to Add:\n1. Line\n2. Rectangle\n3. Triangle\n4. Circle\nChoice: ");
    int type;
    if (scanf(" %d", &type) != 1) return;

    if (type == 1) {
        s.type = LINE;
        printf("Enter start X (0-39) and Y (0-19): "); scanf(" %d %d", &s.p1[0], &s.p1[1]);
        printf("Enter end X (0-39) and Y (0-19): "); scanf(" %d %d", &s.p2[0], &s.p2[1]);
    } else if (type == 2) {
        s.type = RECTANGLE;
        printf("Enter Top-Left X (0-39) and Y (0-19): "); scanf(" %d %d", &s.p1[0], &s.p1[1]);
        printf("Enter Bottom-Right X (0-39) and Y (0-19): "); scanf(" %d %d", &s.p2[0], &s.p2[1]);
    } else if (type == 3) {
        s.type = TRIANGLE;
        printf("Enter Point 1 X (0-39) and Y (0-19): "); scanf(" %d %d", &s.p1[0], &s.p1[1]);
        printf("Enter Point 2 X (0-39) and Y (0-19): "); scanf(" %d %d", &s.p2[0], &s.p2[1]);
        printf("Enter Point 3 X (0-39) and Y (0-19): "); scanf(" %d %d", &s.p3[0], &s.p3[1]);
    } else if (type == 4) {
        s.type = CIRCLE;
        printf("Enter Center X (0-39) and Y (0-19): "); scanf(" %d %d", &s.p1[0], &s.p1[1]);
        printf("Enter Radius: "); scanf(" %d", &s.radius);
    } else {
        printf("Invalid Shape!\n");
        return;
    }

    shapes[target_index] = s;
    redraw_canvas();
}

void delete_shape() {
    int id, found = 0;
    printf("Enter Object ID to delete: ");
    if (scanf(" %d", &id) != 1) return;

    for (int i = 0; i < MAX_SHAPES; i++) {
        if (shapes[i].id == id && shapes[i].active) {
            shapes[i].active = 0;
            found = 1;
            break;
        }
    }

    if (found) {
        printf("Object %d deleted successfully.\n", id);
        redraw_canvas();
    } else {
        printf("Object ID not found!\n");
    }
}

void modify_shape() {
    int id, found = 0;
    printf("Enter Object ID to modify: ");
    if (scanf(" %d", &id) != 1) return;

    for (int i = 0; i < MAX_SHAPES; i++) {
        if (shapes[i].id == id && shapes[i].active) {
            found = 1;
            printf("Modifying Object ID %d...\n", id);
            if (shapes[i].type == LINE) {
                printf("Enter new start X and Y: "); scanf(" %d %d", &shapes[i].p1[0], &shapes[i].p1[1]);
                printf("Enter new end X and Y: "); scanf(" %d %d", &shapes[i].p2[0], &shapes[i].p2[1]);
            } else if (shapes[i].type == RECTANGLE) {
                printf("Enter new Top-Left X and Y: "); scanf(" %d %d", &shapes[i].p1[0], &shapes[i].p1[1]);
                printf("Enter new Bottom-Right X and Y: "); scanf(" %d %d", &shapes[i].p2[0], &shapes[i].p2[1]);
            } else if (shapes[i].type == TRIANGLE) {
                printf("Enter new Point 1 X and Y: "); scanf(" %d %d", &shapes[i].p1[0], &shapes[i].p1[1]);
                printf("Enter new Point 2 X and Y: "); scanf(" %d %d", &shapes[i].p2[0], &shapes[i].p2[1]);
                printf("Enter new Point 3 X and Y: "); scanf(" %d %d", &shapes[i].p3[0], &shapes[i].p3[1]);
            } else if (shapes[i].type == CIRCLE) {
                printf("Enter new Center X and Y: "); scanf(" %d %d", &shapes[i].p1[0], &shapes[i].p1[1]);
                printf("Enter new Radius: "); scanf(" %d", &shapes[i].radius);
            }
            break;
        }
    }

    if (found) {
        redraw_canvas();
    } else {
        printf("Object ID not found!\n");
    }
}
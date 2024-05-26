#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <GL/glut.h>

float cameraX = 0.5f;
float cameraY = 1.0f;
float cameraZ = 3.0f; //camera 

bool doorOpen = false;
float doorAngle = 0.0f;

bool windowsOpen = false;
float windowOpenOffset = 0.0f;

float buildingHalfWidth = 0.75f;//animation
float buildingHalfLength = 0.9f;
float movementSpeed = 0.01f;
float rotationSpeed = 5.0f;
enum Side { FRONT, RIGHT, BACK, LEFT };
Side currentSide = BACK;

float bicyclePosX = -0.5f;
float bicyclePosZ = 1.2f;
float bicycleAngle = 0.0f;
float rightWheelAngle = 0.0f;
float leftWheelAngle = 0.0f;
bool animateBicycle = false;
bool moveForward = false;
bool moveBackward = false;
bool rotateRightWheel = false;
bool rotateLeftWheel = false;


void drawSmallWindow(float xPos, float yPos, float width, float height, float spacing, bool isOpen);
void drawDoor();
void drawBuilding();
void drawRoad();
void drawBicycle();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void drawBuilding() {
    glBegin(GL_QUADS);



    glColor3f(0.7f, 0.5f, 0.3f); // Brown color for walls

    // Front wall (ground floor)
    glVertex3f(-0.4f, -0.5f, -0.5f);
    glVertex3f(0.4f, -0.5f, -0.5f);
    glVertex3f(0.4f, 0.0f, -0.5f);
    glVertex3f(-0.4f, 0.0f, -0.5f);



    // Back 
    glVertex3f(-0.4f, -0.5f, 0.5f);
    glVertex3f(0.4f, -0.5f, 0.5f);
    glVertex3f(0.4f, 0.0f, 0.5f);
    glVertex3f(-0.4f, 0.0f, 0.5f);

    // Left 
    glVertex3f(-0.4f, -0.5f, -0.5f);
    glVertex3f(-0.4f, -0.5f, 0.5f);
    glVertex3f(-0.4f, 0.0f, 0.5f);
    glVertex3f(-0.4f, 0.0f, -0.5f);

    // Right 
    glVertex3f(0.4f, -0.5f, -0.5f);
    glVertex3f(0.4f, -0.5f, 0.5f);
    glVertex3f(0.4f, 0.0f, 0.5f);
    glVertex3f(0.4f, 0.0f, -0.5f);

    // Front wall (second floor)
    glVertex3f(-0.4f, 0.0f, -0.5f);
    glVertex3f(0.4f, 0.0f, -0.5f);
    glVertex3f(0.4f, 0.4f, -0.5f);
    glVertex3f(-0.4f, 0.4f, -0.5f);

    // Back wall 
    glVertex3f(-0.4f, 0.0f, 0.5f);
    glVertex3f(0.4f, 0.0f, 0.5f);
    glVertex3f(0.4f, 0.4f, 0.5f);
    glVertex3f(-0.4f, 0.4f, 0.5f);

    // Left
    glVertex3f(-0.4f, 0.0f, -0.5f);
    glVertex3f(-0.4f, 0.0f, 0.5f);
    glVertex3f(-0.4f, 0.4f, 0.5f);
    glVertex3f(-0.4f, 0.4f, -0.5f);

    // Right 
    glVertex3f(0.4f, 0.0f, -0.5f);
    glVertex3f(0.4f, 0.0f, 0.5f);
    glVertex3f(0.4f, 0.4f, 0.5f);
    glVertex3f(0.4f, 0.4f, -0.5f);


    glEnd();

    // Draw the 3D roof
    glBegin(GL_TRIANGLES);
    glColor3f(0.2f, 0.1f, 0.1f); // Dark brown color for roof

    float roofHeight = 0.2f;

    // Front face of the roof
    glVertex3f(-0.4f, 0.4f, -0.5f);
    glVertex3f(0.4f, 0.4f, -0.5f);
    glVertex3f(0.0f, 0.4f + roofHeight, 0.0f);

    // Back 
    glVertex3f(-0.4f, 0.4f, 0.5f);
    glVertex3f(0.4f, 0.4f, 0.5f);
    glVertex3f(0.0f, 0.4f + roofHeight, 0.0f);

    // Left 
    glVertex3f(-0.4f, 0.4f, -0.5f);
    glVertex3f(-0.4f, 0.4f, 0.5f);
    glVertex3f(0.0f, 0.4f + roofHeight, 0.0f);

    // Right 
    glVertex3f(0.4f, 0.4f, -0.5f);
    glVertex3f(0.4f, 0.4f, 0.5f);
    glVertex3f(0.0f, 0.4f + roofHeight, 0.0f);
    glEnd();

    // Draw windows
    glColor3f(0.7f, 0.9f, 1.0f); // Light  color for windows

    // Draw smaller windows replacing each original window
    float windowSizeX = 0.05f; // Reduced width of smaller window
    float windowSizeY = 0.05f; // Reduced height of smaller window
    float windowSpacing = 0.01f; // Spacing between smaller windows

    // Window 1 (top-left)
    drawSmallWindow(-0.3f, 0.25f, windowSizeX, windowSizeY, windowSpacing, windowsOpen);

    // Window 2 (top-right)
    drawSmallWindow(0.15f, 0.25f, windowSizeX, windowSizeY, windowSpacing, windowsOpen);

    // Window 3 (bottom-left)
    drawSmallWindow(-0.3f, -0.25f, windowSizeX, windowSizeY, windowSpacing, windowsOpen);

    // Window 4 (bottom-right)
    drawSmallWindow(0.15f, -0.25f, windowSizeX, windowSizeY, windowSpacing, windowsOpen);

    // Draw door
    drawDoor();
}

void drawSmallWindow(float xPos, float yPos, float width, float height, float spacing, bool isOpen)
{
    // Calculate the offset based on window animation state
    float offset = isOpen ? spacing : 0.0f;

    glBegin(GL_QUADS);
    glVertex3f(xPos, yPos, 0.501f);
    glVertex3f(xPos + width, yPos, 0.501f);
    glVertex3f(xPos + width, yPos - height, 0.501f);
    glVertex3f(xPos, yPos - height, 0.501f);
    glEnd();

    // Draw three more small windows closely next to the first one with offset
    glBegin(GL_QUADS);
    glVertex3f(xPos + width + offset, yPos, 0.501f);
    glVertex3f(xPos + 2 * width + offset, yPos, 0.501f);
    glVertex3f(xPos + 2 * width + offset, yPos - height, 0.501f);
    glVertex3f(xPos + width + offset, yPos - height, 0.501f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(xPos, yPos - height - offset, 0.501f);
    glVertex3f(xPos + width, yPos - height - offset, 0.501f);
    glVertex3f(xPos + width, yPos - 2 * height - offset, 0.501f);
    glVertex3f(xPos, yPos - 2 * height - offset, 0.501f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(xPos + width + offset, yPos - height - offset, 0.501f);
    glVertex3f(xPos + 2 * width + offset, yPos - height - offset, 0.501f);
    glVertex3f(xPos + 2 * width + offset, yPos - 2 * height - offset, 0.501f);
    glVertex3f(xPos + width + offset, yPos - 2 * height - offset, 0.501f);
    glEnd();
}

void drawDoor()
{
    float doorHeight = 0.3f;
    float doorThickness = 0.1f; // Initial thickness of the door

    // Calculate the current door thickness based on the scale
    float currentDoorThickness = doorThickness * 1;

    // Draw the door and line in one rendering block
    glPushMatrix(); // Save the current matrix
    glTranslatef(0.0f, -0.5f, 0.501f); // Position the door at the bottom of the building
    glRotatef(doorAngle, -0.0f, 1.0f, 0.0f); // Rotate the door around the y-axis
    if (doorOpen) {
        glColor3f(0.3f, 0.3f, 0.3f);
    }
    else {
        glColor3f(0.2f, 0.1f, 0.1f); // Dark brown color for door
    }
    glBegin(GL_QUADS);
    glVertex3f(-currentDoorThickness, 0.0f, 0.0f); // Bottom-left corner
    glVertex3f(currentDoorThickness, 0.0f, 0.0f);  // Bottom-right corner
    glVertex3f(currentDoorThickness, doorHeight, 0.0f);   // Top-right corner (adjusted height)
    glVertex3f(-currentDoorThickness, doorHeight, 0.0f);  // Top-left corner (adjusted height)
    glEnd();

    // Draw a line where the door stopped rotating
    // Position the line at the center of the door's final position
    float linePosition = currentDoorThickness * std::cos(doorAngle * 3.14159 / 180.0f);

    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the line
    glBegin(GL_LINES);
    glVertex3f(linePosition, 0.0f, 0.0f); // Start of the line
    glVertex3f(linePosition, doorHeight, 0.0f); // End of the line
    glEnd();

    glPopMatrix(); // Restore the previous matrix (undo transformations)
}



void drawRoad()
{
    // Draw a simple grey road extending beneath the entire scene
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f); // Grey color for the road
    glVertex3f(-2.0f, -0.6f, 2.0f); // Bottom-left (extends below the building)
    glVertex3f(2.0f, -0.6f, 2.0f); // Bottom-right (extends below the building)
    glVertex3f(1.0f, -0.6f, -2.0f); // Top-right (extends beyond the back of the scene)
    glVertex3f(-1.0f, -0.6f, -2.0f); // Top-left (extends beyond the back of the scene)
    glEnd();
}



void drawWheel(float radius, float width, int segments) {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -width / 2.0f);
    glPopMatrix();

    // Draw spokes
    glBegin(GL_LINES);
    for (int i = 0; i < segments; ++i) {
        float theta = (i * 2.0f * 3.14f) / segments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(x, y, -width / 2.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(x, y, width / 2.0f);
    }
    glEnd();
}

void drawFrame() {
    glColor3f(0.5f, 0.5f, 0.5f); // Grey color for the bicycle frame

    // Draw main body (frame)
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.05f);  // Rear axle
    glVertex3f(0.3f, 0.0f, 0.05f);  // Front axle
    glVertex3f(0.25f, 0.15f, 0.05f); // Front top of frame
    glVertex3f(-0.05f, 0.15f, 0.05f); // Rear top of frame
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, -0.05f);  // Rear axle
    glVertex3f(0.3f, 0.0f, -0.05f);  // Front axle
    glVertex3f(0.25f, 0.15f, -0.05f); // Front top of frame
    glVertex3f(-0.05f, 0.15f, -0.05f); // Rear top of frame
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, -0.05f);  // Rear axle
    glVertex3f(0.0f, 0.0f, 0.05f);  // Rear axle
    glVertex3f(-0.05f, 0.15f, 0.05f); // Rear top of frame
    glVertex3f(-0.05f, 0.15f, -0.05f); // Rear top of frame
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.3f, 0.0f, -0.05f);  // Front axle
    glVertex3f(0.3f, 0.0f, 0.05f);  // Front axle
    glVertex3f(0.25f, 0.15f, 0.05f); // Front top of frame
    glVertex3f(0.25f, 0.15f, -0.05f); // Front top of frame
    glEnd();
}

void drawHandlebars() {
    glColor3f(0.5f, 0.0f, 0.0f); // Dark grey color for handlebars
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(0.25f, 0.15f, 0.05f); // Front top of frame (handlebar attachment)
    glVertex3f(0.35f, 0.2f, 0.05f);  // Handlebar end
    glVertex3f(0.25f, 0.15f, -0.05f); // Front top of frame (handlebar attachment)
    glVertex3f(0.35f, 0.2f, -0.05f);  // Handlebar end
    glEnd();
}

void drawSeat() {
    glColor3f(0.5f, 0.0f, 0.0f); // Grey color for the seat
    glBegin(GL_QUADS);
    glVertex3f(-0.05f, 0.15f, 0.05f); // Rear top of frame (seat attachment)
    glVertex3f(0.0f, 0.15f, 0.05f);  // Rear axle
    glVertex3f(0.0f, 0.2f, 0.05f);   // Seat top
    glVertex3f(-0.05f, 0.2f, 0.05f); // Seat top
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.05f, 0.15f, -0.05f); // Rear top of frame (seat attachment)
    glVertex3f(0.0f, 0.15f, -0.05f);  // Rear axle
    glVertex3f(0.0f, 0.2f, -0.05f);   // Seat top
    glVertex3f(-0.05f, 0.2f, -0.05f); // Seat top
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.05f, 0.15f, -0.05f); // Rear top of frame (seat attachment)
    glVertex3f(-0.05f, 0.15f, 0.05f);  // Rear axle
    glVertex3f(-0.05f, 0.2f, 0.05f);   // Seat top
    glVertex3f(-0.05f, 0.2f, -0.05f);  // Seat top
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.15f, -0.05f); // Rear axle
    glVertex3f(0.0f, 0.15f, 0.05f);  // Rear axle
    glVertex3f(0.0f, 0.2f, 0.05f);   // Seat top
    glVertex3f(0.0f, 0.2f, -0.05f);  // Seat top
    glEnd();
}

void drawBicycle() {
    glPushMatrix();
    glTranslatef(bicyclePosX, -0.4f, bicyclePosZ); // Position the bicycle
    glRotatef(bicycleAngle * (180.0f / 3.14f), 0.0f, 1.0f, 0.0f); // Rotate bicycle to face the direction of movement

    // Update right wheel rotation if 'r' key is held down
    if (rotateRightWheel) {
        rightWheelAngle += 5.0f; // Adjust rotation speed as needed
    }

    // Update left wheel rotation if 'l' key is held down
    if (rotateLeftWheel) {
        leftWheelAngle += 5.0f; // Adjust rotation speed as needed
    }

    // Draw rear wheel (left wheel)
    glPushMatrix();
    glRotatef(leftWheelAngle, 0.0f, 0.0f, 1.0f); // Rotate left wheel

    glColor3f(0.1f, 0.1f, 0.1f); // Dark grey color for wheels
    drawWheel(0.1f, 0.02f, 8); // Outer radius, thickness, number of spokes

    glPopMatrix(); // Restore left wheel matrix

    // Draw front wheel (right wheel)
    glPushMatrix();
    glTranslatef(0.3f, 0.0f, 0.0f); // Position at front axle
    glRotatef(rightWheelAngle, 0.0f, 0.0f, 1.0f); // Rotate right wheel

    drawWheel(0.1f, 0.02f, 8); // Outer radius, thickness, number of spokes

    glPopMatrix(); // Restore right wheel matrix

    // Draw bicycle frame and other components
    drawFrame();
    drawHandlebars();
    drawSeat();

    glPopMatrix(); // Restore the transformation matrix
}


void updateBicyclePosition()
{
    if (animateBicycle)
    {
        switch (currentSide)
        {
        case FRONT:
            bicyclePosX -= movementSpeed;
            if (bicyclePosX <= -buildingHalfWidth)
            {
                bicyclePosX = -buildingHalfWidth;
                currentSide = RIGHT;
            }
            break;

        case RIGHT:
            bicyclePosZ += movementSpeed;
            if (bicyclePosZ >= buildingHalfLength)
            {
                bicyclePosZ = buildingHalfLength;
                currentSide = BACK;
            }
            break;

        case BACK:
            bicyclePosX += movementSpeed;
            if (bicyclePosX >= buildingHalfWidth)
            {
                bicyclePosX = buildingHalfWidth;
                currentSide = LEFT;
            }
            break;

        case LEFT:
            bicyclePosZ -= movementSpeed;
            if (bicyclePosZ <= -buildingHalfWidth)
            {
                bicyclePosZ = -buildingHalfLength;
                currentSide = FRONT;
            }
            break;
        }

        // Rotate the wheels as the bicycle moves
        rightWheelAngle += rotationSpeed;
        if (rightWheelAngle > 360.0f)
        {
            rightWheelAngle -= 360.0f;
        }

        leftWheelAngle += rotationSpeed;
        if (leftWheelAngle > 360.0f)
        {
            leftWheelAngle -= 360.0f;
        }

        // Rotate the bicycle at the corners
        switch (currentSide)
        {
        case FRONT:
            bicycleAngle = 160.0f;
            break;
        case RIGHT:

            bicycleAngle = 300.0f;
            break;
        case BACK:
            bicycleAngle = 0.0f;
            break;
        case LEFT:
            bicycleAngle = -80.0f;
            break;
        }
    }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        // Pressed 'o' to open the door
        doorOpen = true;
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        // Pressed 'c' to close the door
        doorOpen = false;
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        // Toggle window animation state
        windowsOpen = !windowsOpen;
    }
    else if (key == GLFW_KEY_F) {
        // Move forward
        moveForward = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    else if (key == GLFW_KEY_B) {
        // Move backward
        moveBackward = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    else if (key == GLFW_KEY_R)
    {
        // Rotate right wheel continuously when 'r' is held down
        rotateRightWheel = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    else if (key == GLFW_KEY_L)
    {
        // Rotate left wheel continuously when 'l' is held down
        rotateLeftWheel = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        cameraZ += 0.1f;
    }
    else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        cameraZ -= 0.1f;
    }
    else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        cameraX += 0.1f;
    }
    else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        cameraX -= 0.1f;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Start the bicycle animation on left mouse button press
        animateBicycle = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        // Stop the bicycle animation on left mouse button release
        animateBicycle = false;
    }
}


int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Building Scene", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    // Set the mouse button callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering

    glClearColor(0.4f, 0.8f, 0.6f, 1.0f); // Set green background color

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -3.0f); // Move back to view the scene

        // Handle door animation
        // Handle door animation
        if (doorOpen)
        {
            // Open the door (rotate around the y-axis)
            doorAngle += 2.0f;
            if (doorAngle > 40.0f)
                doorAngle = 40.0f;
        }
        else
        {
            // Close the door (back to its original position)
            doorAngle -= 2.0f;
            if (doorAngle < 0.0f)
                doorAngle = 0.0f;
        }



        // Handle window animation
        if (windowsOpen)
        {
            // Animate window opening
            if (windowOpenOffset < 0.1f)
            {
                windowOpenOffset += 0.009f; // Adjust animation speed as needed
            }
        }
        else
        {
            // Animate window closing
            if (windowOpenOffset > 0.0f)
            {
                windowOpenOffset -= 0.0001f; // Adjust animation speed as needed
            }
        }

        // Handle continuous movement based on key state
        if (moveForward) {
            bicyclePosX += 0.02f; // Increment position along x-axis
            if (bicyclePosX > 0.9f) {
                bicyclePosX = 0.9f; // Restrict to road end
            }
        }
        if (moveBackward) {
            bicyclePosX -= 0.02f; // Decrement position along x-axis
            if (bicyclePosX < -0.9f) {
                bicyclePosX = -0.9f; // Restrict to road start
            }
        }

        // Handle continuous wheel rotation based on rotation angles

        gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // Draw the building
        drawBuilding();
        drawRoad();
        drawBicycle();
        updateBicyclePosition();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
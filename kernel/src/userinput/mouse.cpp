#include <userinput/mouse.h>

uint8_t MousePointer[] = {
    0b10000000, 0b00000000, 
    0b11000000, 0b00000000,
    0b11100000, 0b00000000,
    0b11110000, 0b00000000,
    0b11111000, 0b00000000,
    0b11111100, 0b00000000,
    0b11111110, 0b00000000,
    0b11111111, 0b00000000,
    0b11111111, 0b00000000, 
    0b11111111, 0b11000000,
    0b11111100, 0b00000000,
    0b11001100, 0b00000000,
    0b10000110, 0b00000000,
    0b00000110, 0b00000000,
    0b00000011, 0b00000000,
    0b00000011, 0b00000000,
};

void MouseWait()
{
    uint64_t timeout = 100000;
    while (timeout--) {
        //Check if mouse is avaliable to be read
        if ((inb(0x64) & 0b10) == 0) 
            return;
    }
}

void MouseWaitInput()
{
    uint64_t timeout = 100000;
    while (timeout--) {
        //Check if mouse is avaliable to be read
        if (inb(0x64) & 0b1)
            return;
    }
}

void MouseWrite(uint8_t value)
{
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t MouseRead()
{
    MouseWaitInput();
    return inb(0x60);
}

/* The PS2 Mouse works by sending information packets 
 * The first byte of the packet includes mouse information like
 * overflows, signs and button clicks
 * The second and third byte of the packet is the X and Y movement
 * of the mouse respectively
 */

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
Point MousePosition;
Point MousePositionOld;

void HandlePS2Mouse(uint8_t data)
{
    ProcessMousePacket();
    static bool skip = true;
    if (skip) {
        skip = false;
        return;
    }

    switch (MouseCycle) {
    case 0: // x-y overflow, sign, button click
        //The 5th bit of the data will always be set to 1
        //In the case it is not, the packet is misaligned then break
        if ((data & 0b00001000) == 0) break;
        MousePacket[0] = data;
        MouseCycle++;
        break;
    case 1: // X movement
        MousePacket[1] = data;
        MouseCycle++;
        break;
    case 2: // Y movement
        MousePacket[2] = data;
        MousePacketReady = true;
        MouseCycle = 0;
        break;
    }
}

void ProcessMousePacket()
{
    if (!MousePacketReady)
        return;

    bool xNegative, yNegative, xOverflow, yOverflow;

    xNegative = (MousePacket[0] & PS2XSign);
    yNegative = (MousePacket[0] & PS2YSign);
    xOverflow = (MousePacket[0] & PS2XOverflow);
    yOverflow = (MousePacket[0] & PS2YOverflow);

    if (!xNegative) {
        MousePosition.X += MousePacket[1];
        if (xOverflow) 
            MousePosition.X += 255;
    } else {
        MousePacket[1] = 256 - MousePacket[1];
        MousePosition.X -= MousePacket[1];
        if (xOverflow) 
            MousePosition.X -= 255;
    }

    if (!yNegative) {
        MousePosition.Y -= MousePacket[2];
        if (yOverflow)
            MousePosition.Y -= 255;
    } else {
        MousePacket[2] = 256 - MousePacket[2];
        MousePosition.Y += MousePacket[2];
        if(yOverflow)
            MousePosition.Y += 255;
    }

    if (MousePosition.X < 0) 
        MousePosition.X = 0;

    if (MousePosition.X > GlobalRenderer->TargetFrameBuffer->Width-1) 
        MousePosition.X = GlobalRenderer->TargetFrameBuffer->Width-1;

    if (MousePosition.Y < 0) 
        MousePosition.Y = 0;

    if (MousePosition.Y > GlobalRenderer->TargetFrameBuffer->Height-1) 
        MousePosition.Y = GlobalRenderer->TargetFrameBuffer->Height-1;
    
    GlobalRenderer->ClearMouseCursor(MousePointer, MousePositionOld);
    GlobalRenderer->DrawOverlayMouseCursor(MousePointer, MousePosition, 0xffffffff);
    
    /* No current opperations for mouse buttons so leave blank for now */
    // if (MousePacket[0] & PS2LeftButton) {}
    
    // if (MousePacket[0] & PS2MiddleButton) {} 
    
    // if (MousePacket[0] & PS2RightButton) {}
    
    MousePacketReady = false;
    MousePositionOld = MousePosition;
}

void InitPS2Mouse()
{
    outb(0x64, 0xA8); //enabling the auxiliary device - mouse

    MouseWait();
    outb(0x64, 0x20); //tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
    uint8_t status = inb(0x60);
    status |= 0b10;
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status); // setting the correct bit is the "compaq" status byte

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
}

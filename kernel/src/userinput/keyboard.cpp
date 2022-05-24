#include <userinput/keyboard.h>

bool isLeftShiftPressed;
bool isRightShiftPressed;
bool isNextSpecial = false;

void HandlerKeyboardSpecial(uint8_t scancode)
{
	auto charsizeY = GlobalRenderer.PSF1_Font->psf1_Header->charsize;
	auto charsizeX = charsizeY >> 1;
	switch (scancode) {
	case CursorUp:
		if (GlobalRenderer.CursorPosition.Y - charsizeY >= 0)
			GlobalRenderer.CursorPosition.Y -= charsizeY;
		break;

	case CursorRight:
		if (GlobalRenderer.CursorPosition.X < GlobalRenderer.TargetFrameBuffer->Width - charsizeX)
			GlobalRenderer.CursorPosition.X += charsizeX;
		break;

	case CursorLeft:
		if (GlobalRenderer.CursorPosition.X - charsizeX >= 0)
			GlobalRenderer.CursorPosition.X -= charsizeX;
		break;

	case CursorDown:
		if (GlobalRenderer.CursorPosition.Y < GlobalRenderer.TargetFrameBuffer->Height - charsizeY)
			GlobalRenderer.CursorPosition.Y += charsizeY;
		break;
	}

	isNextSpecial = false;
}

void HandlerKeyboard(uint8_t scancode)
{
	if (isNextSpecial)
	{
		HandlerKeyboardSpecial(scancode);
		return;
	}

	switch (scancode) {
	case LeftShift:
		isLeftShiftPressed = true;
		return;
	case LeftShift + 0x80:
		isLeftShiftPressed = false;
		return;
	case RightShift:
		isRightShiftPressed = true;
		return;
	case RightShift + 0x80:
		isRightShiftPressed = false;
		return;
	case Enter:
		GlobalRenderer.Next();
		return;
	case SpaceBar:
		GlobalRenderer.PutChar(' ');
		return;
	case BackSpace:
		GlobalRenderer.ClearChar();
		return;
	case Special:
		isNextSpecial = true;
		return;
	}

  char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);

  if (ascii != 0)
    GlobalRenderer.PutChar(ascii);
}
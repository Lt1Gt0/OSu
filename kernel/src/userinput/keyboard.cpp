#include <userinput/keyboard.h>
#include <renderer.h>

bool isLeftShiftPressed;
bool isRightShiftPressed;
bool isNextSpecial = false;

void HandlerKeyboardSpecial(uint8 scancode)
{
	using namespace QWERTYKeyboard;

	auto charsizeY = GlobalRenderer.mPSF1Font->psf1Header->charsize;
	auto charsizeX = charsizeY >> 1;

	switch (scancode) {
		case CursorUp:
		{
			if (GlobalRenderer.mCursorPosition.Y - charsizeY >= 0)
				GlobalRenderer.mCursorPosition.Y -= charsizeY;
				
			break;
		}
		case CursorRight:
		{
			if (GlobalRenderer.mCursorPosition.X < GlobalRenderer.mTargetFrameBuffer->Width - charsizeX)
				GlobalRenderer.mCursorPosition.X += charsizeX;

			break;
		}
		case CursorLeft:
		{
			if (GlobalRenderer.mCursorPosition.X - charsizeX >= 0)
				GlobalRenderer.mCursorPosition.X -= charsizeX;

			break;
		}
		case CursorDown:
		{
			if (GlobalRenderer.mCursorPosition.Y < GlobalRenderer.mTargetFrameBuffer->Height - charsizeY)
				GlobalRenderer.mCursorPosition.Y += charsizeY;

			break;
		}
	}

	isNextSpecial = false;
}

void HandlerKeyboard(uint8 scancode)
{
	using namespace QWERTYKeyboard;

	if (isNextSpecial) {
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

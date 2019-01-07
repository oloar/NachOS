#include "frameprovider.h"
#include "system.h"

FrameProvider::FrameProvider() {
	bitmap = new BitMap(NumPhysPages);
	bzero(bitmap);
}

FrameProvider::~FrameProvider() {
	delete bitmap;
}

int FrameProvider::GetEmptyFrame() {
	int frameidx = bitmap.Find();
	// Verification that there is a free frame.
	ASSERT(frameidx != -1);
	return frameidx;
}
void FrameProvider::ReleaseFrame(int frameidx) {
	// Verification that frameidx is indeed a frame index
	ASSERT(frameidx >= 0 && frameidx < NumPhysPages);
	return bitmap.Clear(frameidx);
}
int FrameProvider::NumAvailFrame() {
	return bitmap.NumClear();
}

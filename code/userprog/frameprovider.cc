#include "frameprovider.h"
#include "system.h"

FrameProvider::FrameProvider() {
	bitmap = new BitMap(NumPhysPages);
	bzero(machine->mainMemory, MemorySize);
}

FrameProvider::~FrameProvider() {
	delete bitmap;
}

int FrameProvider::GetEmptyFrame() {
	int frameidx = bitmap->Find();
	// Verification that there is a free frame.
	ASSERT(frameidx != -1);
	return frameidx;
}

int FrameProvider::GetRandomEmptyFrame() {
	int mapindex = 0,
		freeindex = 0,
		randomindex,
		nbfree;
	int *freeFrames;
	nbfree = bitmap->NumClear();
	ASSERT(nbfree > 0);

	/* If only one free bit no need to look for it */
	if (nbfree == 1)
		return GetEmptyFrame();

	freeFrames = (int *) malloc(nbfree * sizeof(int));

	/* nbfree lower or equal to bitmap's size */
	while (freeindex < nbfree) {
		if (! bitmap->Test(mapindex)) {
			freeFrames[freeindex] = mapindex;
			freeindex++;
		}
		mapindex++;
	}

	randomindex = freeFrames[rand() % nbfree];
	bitmap->Mark(randomindex);

	free(freeFrames);

	return randomindex;
}

void FrameProvider::ReleaseFrame(int frameidx) {
	// Verification that frameidx is indeed a frame index
	ASSERT(frameidx >= 0 && frameidx < NumPhysPages);
	return bitmap->Clear(frameidx);
}
int FrameProvider::NumAvailFrame() {
	return bitmap->NumClear();
}

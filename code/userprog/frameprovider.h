#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H
#include "bitmap.h"

class FrameProvider {
	public:
		FrameProvider();
		~FrameProvider();

		int GetEmptyFrame();
		int GetRandomEmptyFrame();
		void ReleaseFrame(int frameidx);
		int NumAvailFrame();
	private:
		BitMap *bitmap;
};
#endif /* FRAMEPROVIDER_H */
